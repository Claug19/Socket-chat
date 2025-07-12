#ifndef SOCKETCHAT_CHAT_CLIENT_HPP
#define SOCKETCHAT_CHAT_CLIENT_HPP

#ifdef SOCKETCHAT_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "utils/getConfig.hpp"
#include "utils/logger.hpp"

class ChatClient {
public:
	ChatClient() :
		logger_(utils::Logger::getLogger(std::string("Client"))),
		config_(utils::Config::getConfig())
	{}

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
	bool isInteger(const std::string& input);
	bool isAddress(const std::string& input);
	void convertAddress(const std::string &address);
	std::string addUserToMessage(const std::string& message);
	std::string addSystemToMessage(const std::string& message);

	// loggers
	void log(const ELogType type, const std::string& message);
	void sysLog(const ELogType type, const std::string& message);
	void logAndUpdate(const ELogType type, const std::string& message);
	void sysLogAndUpdate(const ELogType type, const std::string& message);
	void quickLog(const ELogType type, const std::string& message);
	void quickSysLog(const ELogType type, const std::string& message);

	// getters
	unsigned short getPort();
	unsigned long getAddress();
	std::string getUser();

	//  flags
	bool exitFlag_ = false;

	//  client socket data
	int sockFileDescriptor_;

	//  server socket data
	unsigned short serverPort_;
	unsigned long serverAddress_;
	sockaddr_in serverSocketAddress_;

	//  instance data
	std::string user_;
	std::vector<std::string> messages_;
	std::mutex mutex_;
	utils::LoggerPtr logger_;
	utils::ConfigPtr config_;
};

#endif //  #ifndef SOCKETCHAT_CHAT_CLIENT_HPP
