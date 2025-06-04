#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

std::mutex mtx;

class ChatInstance {
public:
	//  socket methods
	void startSocket();
	void closeSocket();

	//  chat methods
	void setPort();
	void setAddress();
	void setUser();
	void showInstance();
private:
	//  socket methods
	void createSocket();
	void connectSocket();
	void connectionAccepter();
	void disconnectSocket();

	//  chat methods
	void inputMessage();
	void sendMessage(const std::string &message);
	void receiveMessage();
	bool runCommand(const std::string &command);

	//  utility
	void display();
	void updateDisplay();
	void clearScreen();
	void convertAddress(const std::string &address);

	// loggers
	void log(const std::string& message);
	void sys_log(const std::string& message);

	//  flags
	bool exitFlag_ = false;
	bool instanceLoaded_ = false;

	//  client socket data
	int sockFileDescriptor_;

	//  server socket data
	unsigned short serverPort_;
	unsigned long serverAddress_;
	sockaddr_in serverSocketAddress_;

	//  instance data
	std::string user_;
	std::vector<std::string> messages_;
};
