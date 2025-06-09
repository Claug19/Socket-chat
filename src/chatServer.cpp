#include "chatServer.hpp"

#define SERVERPORT 8080
#define SERVERADDRESS "192.168.25.164"
#define EMPTYLNS 50
#define MAXCONNECTIONS 5
#define MAXBUFFERSIZE 1024

//  public
//  socket methods
void ChatInstance::startSocket() {
	//  clean display
	clearScreen();
	std::cout << "SOCKET BASED CHAT\n\n";

	//  session setup
	setPort();
	setAddress();
	setUser();

	//  server socket setup
	createSocket();
	bindSocket();
	listenConnections();
	acceptConnections();

	//  instance
	showInstance();
}

void ChatInstance::closeSocket() {
	for (auto connection : connections_)
		close(connection);
	close(sockFileDescriptor_);
	exitFlag_ = true;
}

//  chat methods
void ChatInstance::setPort() {
	std::cout << "Custom port (leave blank for default): ";

	std::string portString;
	std::getline(std::cin, portString);
	if (portString.empty()) {
		port_ = SERVERPORT;
		return;
	}

	port_ = std::stoi(portString);
}

void ChatInstance::setAddress() {
	std::cout << "Custom address (leave blank for default): ";

	std::string addressString;
	std::getline(std::cin, addressString);
	if (addressString.empty()) {
		convertAddress(SERVERADDRESS);
		return;
	}

	convertAddress(addressString);
}

void ChatInstance::setUser() {
	std::cout << "Set user: ";

	while (user_.empty())
		std::cin >> user_;
}

void ChatInstance::showInstance() {
	//  display instance interface
	if (exitFlag_)
		return;

	display();
	inputMessage();
	showInstance();
}

//  private
//  socket methods
void ChatInstance::createSocket() {
	sysLog(INFO, "Server socket setup..");

	sockFileDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFileDescriptor_ < 0) {
		quickSysLog(ERROR, "Socket creation failed!");
		closeSocket();
		exit(EXIT_FAILURE);
	}
}

void ChatInstance::bindSocket() {
	serverSocketAddress_.sin_family = AF_INET;
	serverSocketAddress_.sin_port = htons(port_);
	serverSocketAddress_.sin_addr.s_addr = address_;

	auto result = bind(sockFileDescriptor_, (struct sockaddr*)&serverSocketAddress_, sizeof(serverSocketAddress_));
	if (result < 0) {
		quickSysLog(ERROR, "Bind failed!");
		closeSocket();
		exit(EXIT_FAILURE);
	}

	sysLog(INFO, "Socket initialized successfully!");
	sysLog(INFO,
	   	std::string("Address: ") + inet_ntoa(serverSocketAddress_.sin_addr) +
    	"\tPort: " + std::to_string(ntohs(serverSocketAddress_.sin_port))
	);
	sysLog(INFO, "User \'" + user_ + "\' logged in");
}

void ChatInstance::listenConnections() {
	auto result = listen(sockFileDescriptor_, MAXCONNECTIONS);
	if (result < 0) {
		quickSysLog(ERROR, "Listen failed!");
		closeSocket();
		exit(EXIT_FAILURE);
	}
}

void ChatInstance::acceptConnections() {
	//  starts a thread that accepts future connections
	std::thread accepterThread(&ChatInstance::connectionAccepter, this);
	accepterThread.detach();
}

void ChatInstance::connectionAccepter() {
	int clientFileDescriptor = accept(sockFileDescriptor_, nullptr, nullptr);

	//  causes the thread to terminate early without executing further
	if (exitFlag_)
		return;

	if (clientFileDescriptor < 0) {
		quickSysLog(ERROR, "Connection failed!");
	}
	else {
		sysLogAndUpdate(INFO, "Connection accepted! File descriptor: " + std::to_string(clientFileDescriptor));

		connections_.push_back(clientFileDescriptor);

		//  starts a listener thread for each connection
		std::thread connectionThread(&ChatInstance::receiveMessage, this, clientFileDescriptor);
		connectionThread.detach();
	}
	connectionAccepter();
}

void ChatInstance::disconnectClient(const int connection) {
	sysLogAndUpdate(INFO, "Socket disconnected. File Descriptor: " + std::to_string(connection));

	close(connection);
	connections_.erase(
	    std::remove(connections_.begin(), connections_.end(), connection),
	    connections_.end()
	);
}

//  chat message
void ChatInstance::inputMessage() {
	std::cout << "MESSAGE:\n";

	std::string message;
	std::getline(std::cin, message);
	if (message.empty())
		return;

	//  check if the input is a command
	if (message[0] == '/') {
		if (runCommand(message))
			return;
		sysLog(WARNING, "Invalid message or command!");
		return;
	}

	log(NONE, addUserToMessage(message));
	if (!connections_.empty())
		sendMessage(addUserToMessage(message));
}

void ChatInstance::sendMessage(const std::string &message) {
	for (auto connection : connections_)
		send(connection, message.c_str(), MAXBUFFERSIZE, 0);
}

void ChatInstance::receiveMessage(const int connection) {
	char buffer[MAXBUFFERSIZE] = { 0 };
	auto result = recv(connection, buffer, MAXBUFFERSIZE, 0);

	//  causes the thread to terminate early without executing further
	if (exitFlag_)
		return;

	if (result <= 0) {
		disconnectClient(connection);
		return;
	}

	mutex_.lock();
	logAndUpdate(NONE, buffer);
	mutex_.unlock();

	sendMessage(buffer);
	receiveMessage(connection);
}

bool ChatInstance::runCommand(const std::string &message) {
	//  executes slash commands
	if (message == "/exit") {
		closeSocket();
		clearScreen();
		return true;
	}
	return false;
}

//  utility
void ChatInstance::display() {
	//  clear screen, display messages
	clearScreen();
	std::cout << "CHAT:\n\n";
	for (auto message : messages_)
		std::cout << message << std::endl;

	//  empty space between message stack and input
	for (auto emptyLn = 0u; emptyLn < (EMPTYLNS - messages_.size()); emptyLn++)
		std::cout << std::endl;
}

void ChatInstance::updateDisplay() {
	//  ANSI up one line to keep input prompt
	std::cout << "\x1b[1A";

	//  ANSI up and delete until last message
	for (auto upLine = 0u; upLine < (EMPTYLNS - messages_.size() + 1); upLine++)
		std::cout << "\x1b[1A" << "\x1b[2K";

	//  output new message
	std::cout << "\r" << messages_.back() << std::endl;

	//  create new lines until original position
	for (auto emptyLn = 0u; emptyLn < (EMPTYLNS - messages_.size() + 1); emptyLn++)
		std::cout << std::endl;
}

void ChatInstance::clearScreen() {
#ifdef WINDOWS
	std::system("cls");
#else
	std::system ("clear");  //  Assume POSIX
#endif
}

void ChatInstance::convertAddress(const std::string &address) {
	if (inet_pton(AF_INET, address.c_str(), &address_) <= 0) {
		quickSysLog(ERROR, "Invalid address/ Address not supported!");
		exit(EXIT_FAILURE);
	}
}

std::string ChatInstance::addUserToMessage(const std::string& message) {
	return std::string("<" + user_ + "> : " + message);
}

std::string ChatInstance::addSystemToMessage(const std::string& message) {
	return std::string("[System] : " + message);
}

//  loggers
void ChatInstance::log(const ELogType type, const std::string& message) {
	messages_.push_back(logger_->log(type, message));
}

void ChatInstance::sysLog(const ELogType type, const std::string& message) {
	log(type, addSystemToMessage(message));
}

void ChatInstance::logAndUpdate(const ELogType type, const std::string& message) {
	messages_.push_back(logger_->log(type, message));
	updateDisplay();
}

void ChatInstance::sysLogAndUpdate(const ELogType type, const std::string& message) {
	logAndUpdate(type, addSystemToMessage(message));
}

void ChatInstance::quickLog(const ELogType type, const std::string& message) {
	std::cout << logger_->log(type, message) << std::endl;
}

void ChatInstance::quickSysLog(const ELogType type, const std::string& message) {
	quickLog(type, addSystemToMessage(message));
}


int main() {
	std::unique_ptr<ChatInstance> server = std::make_unique<ChatInstance>();
	server->startSocket();
	return 0;
}
