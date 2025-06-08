#ifndef SOCKETCHAT_CHAT_SERVER_HPP
#define SOCKETCHAT_CHAT_SERVER_HPP

#ifdef SOCKETCHAT_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <algorithm>
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
	void bindSocket();
	void listenConnections();
	void acceptConnections();
	void connectionAccepter();
	void disconnectClient(const int connection);

	//  chat methods
	void inputMessage();
	void sendMessage(const std::string &message);
	void receiveMessage(const int connection);
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
	std::vector<int> connections_;

	//  server socket data
	unsigned short port_;
	unsigned long address_;
	int sockFileDescriptor_;
	sockaddr_in serverSocketAddress_;

	//  instance data
	std::string user_;
	std::vector<std::string> messages_;
	std::mutex mutex_;
};

#endif //  #ifndef SOCKETCHAT_CHAT_SERVER_HPP
