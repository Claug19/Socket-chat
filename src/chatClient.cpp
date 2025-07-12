#include "chatClient.hpp"

//  public
//  socket methods
void ChatClient::startSocket() {
	//  clean display
	clearScreen();
	std::cout << "SOCKET BASED CHAT\n\n";

	//  session setup
	setPort();
	setAddress();
	setUser();

	//  client socket setup
	createSocket();
	connectSocket();
	connectionAccepter();

	//  instace
	showInstance();
}

void ChatClient::closeSocket() {
	close(sockFileDescriptor_);
	exitFlag_ = true;
}

//  chat methods
void ChatClient::setPort() {
	std::cout << "Custom port (leave blank for default): ";

	std::string portString;
	std::getline(std::cin, portString);

	if (portString.empty()) {
		serverPort_ = config_->getAsInt("SERVERPORT");
		return;
	}

	if (!isInteger(portString)) {
		std::cout << "Invalid port provided. Retrying..\n";
		setPort();
		return;
	}

	serverPort_ = std::stoi(portString);
}

void ChatClient::setAddress() {
	std::cout << "Custom address (leave blank for default): ";

	std::string addressString;
	std::getline(std::cin, addressString);

	if (addressString.empty()) {
		convertAddress(config_->get("SERVERADDRESS"));
		return;
	}

	if (!isAddress(addressString)) {
		std::cout << "Invalid address provided. Retrying..\n";
		setAddress();
		return;
	}

	convertAddress(addressString);
}

void ChatClient::setUser() {
	std::cout << "Set user: ";

	while (user_.empty())
		std::cin >> user_;
}

void ChatClient::showInstance() {
	//  display instance interface
	if (exitFlag_)
		return;

	display();
	inputMessage();
	showInstance();
}


//  private
//  socket methods
void ChatClient::createSocket() {
	sysLog(ELogType::INFO, "Client socket setup..");

	sockFileDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFileDescriptor_ < 0) {
		quickSysLog(ELogType::INFO, "Socket creation failed!");
		closeSocket();
		exit(EXIT_FAILURE);
	}
}

void ChatClient::connectSocket() {
	serverSocketAddress_.sin_family = AF_INET;
	serverSocketAddress_.sin_port = htons(serverPort_);
	serverSocketAddress_.sin_addr.s_addr = serverAddress_;

	if (connect(sockFileDescriptor_, (struct sockaddr *)&serverSocketAddress_, sizeof(serverSocketAddress_)) < 0) {
		quickSysLog(ELogType::INFO, "Connection failed!");
		closeSocket();
		exit(EXIT_FAILURE);
	};

	sysLog(ELogType::INFO, "Socket initialized successfully!");
	sysLog(ELogType::INFO,
	    std::string("Address: ") + inet_ntoa(serverSocketAddress_.sin_addr) +
	    "\tPort: " + std::to_string(ntohs(serverSocketAddress_.sin_port))
	);

	const std::string userLogin = addSystemToMessage("User \'" + user_ + "\' logged in");
	log(ELogType::INFO, userLogin);
	sendMessage(userLogin);
}

void ChatClient::connectionAccepter() {
	//  starts a listener thread for the connection
	std::thread connectionThread(&ChatClient::receiveMessage, this);
	connectionThread.detach();
}

void ChatClient::disconnectSocket() {
	closeSocket();
	clearScreen();
	quickSysLog(ELogType::INFO, "Server socket disconnected.");
	exit(EXIT_SUCCESS);
}

//  chat message
void ChatClient::inputMessage() {
	std::cout << "MESSAGE:\n";

	std::string message;
	std::getline(std::cin, message);
	if (message.empty())
		return;

	if (message[0] == '/') {
		if (runCommand(message))
			return;
		sysLog(ELogType::INFO, "Invalid message or command");
		return;
	}

	sendMessage(addUserToMessage(message));
}

void ChatClient::sendMessage(const std::string &message) {
	send(sockFileDescriptor_, message.c_str(), config_->getAsInt("MAXBUFFERSIZE"), 0);
}

void ChatClient::receiveMessage() {
	char buffer[config_->getAsInt("MAXBUFFERSIZE")] = { 0 };
	auto result = recv(sockFileDescriptor_, buffer, config_->getAsInt("MAXBUFFERSIZE"), 0);

	//  causes the thread to terminate early without executing further
	if (exitFlag_)
		return;

	if (result <= 0) {
		disconnectSocket();
		return;
	}

	mutex_.lock();
	logAndUpdate(NONE, buffer);
	mutex_.unlock();

	receiveMessage();
}

bool ChatClient::runCommand(const std::string &message) {
	//  executes slash commands
	if (message == "/exit") {
		sendMessage(addSystemToMessage("User \'" + user_ + "\' logged out"));
		closeSocket();
		clearScreen();
		return true;
	}
	return false;
}

//  utility
void ChatClient::display() {
	//  clear screen, display messages
	clearScreen();
	std::cout << "CHAT:\n\n";
	for (auto message : messages_) {
		std::cout << message << std::endl;
	}

	//  empty space between message stack and input
	for (auto emptyLn = 0u; emptyLn < (config_->getAsInt("EMPTYLNS") - messages_.size()); emptyLn++) {
		std::cout << std::endl;
	}
}

void ChatClient::updateDisplay() {
	//  ANSI up one line to keep input prompt
	std::cout << "\x1b[1A";

	//  ANSI up and delete until last message
	for (auto upLine = 0u; upLine < (config_->getAsInt("EMPTYLNS") - messages_.size() + 1); upLine++)
		std::cout << "\x1b[1A" << "\x1b[2K";

	//  output new message
	std::cout << "\r" << messages_.back() << std::endl;

	//  create new lines until original position
	for (auto emptyLn = 0u; emptyLn < (config_->getAsInt("EMPTYLNS") - messages_.size() + 1); emptyLn++)
		std::cout << std::endl;
}

void ChatClient::clearScreen() {
#ifdef WINDOWS
	std::system("cls");
#else
	std::system ("clear");  //  Assume POSIX
#endif
}

bool ChatClient::isInteger(const std::string& input) {
	for (auto character : input) {
		if (!std::isdigit(character))
			return false;
	}
	return true;
}

bool ChatClient::isAddress(const std::string& input) {
	for (auto character : input) {
		if (!(std::isdigit(character) || character == '.'))
			return false;
	}
	return true;
}

void ChatClient::convertAddress(const std::string &address) {
	if (inet_pton(AF_INET, address.c_str(), &serverAddress_) <= 0) {
		quickSysLog(ELogType::INFO, "Invalid address/ Address not supported!");
		exit(EXIT_FAILURE);
	}
}

std::string ChatClient::addUserToMessage(const std::string& message) {
	return std::string("<" + user_ + "> : " + message);
}

std::string ChatClient::addSystemToMessage(const std::string& message) {
	return std::string("[System] : " + message);
}

//  loggers
void ChatClient::log(const ELogType type, const std::string& message) {
	messages_.push_back(logger_->log(type, message));
}

void ChatClient::sysLog(const ELogType type, const std::string& message) {
	log(type, addSystemToMessage(message));
}

void ChatClient::logAndUpdate(const ELogType type, const std::string& message) {
	messages_.push_back(logger_->log(type, message));
	updateDisplay();
}

void ChatClient::sysLogAndUpdate(const ELogType type, const std::string& message) {
	logAndUpdate(type, addSystemToMessage(message));
}

void ChatClient::quickLog(const ELogType type, const std::string& message) {
	std::cout << logger_->log(type, message) << std::endl;
}

void ChatClient::quickSysLog(const ELogType type, const std::string& message) {
	quickLog(type, addSystemToMessage(message));
}

// getters
unsigned short ChatClient::getPort() {
	return serverPort_;
}

unsigned long ChatClient::getAddress() {
	return serverAddress_;
}

std::string ChatClient::getUser() {
	return user_;
}

int main() {
	std::unique_ptr<ChatClient> client = std::make_unique<ChatClient>();
	client->startSocket();
	return 0;
}
