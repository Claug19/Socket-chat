<div id="readme-top"/>

# SOCKET CHAT

The current project is a C++ implementation of a TCP socket based chat.
It cotains a server instance and a client instance.
In order to create a session for the chat, the server must run at all times since client-client connexion is not possible.

### TABLE OF CONTESTS

- [Build and run](#build-and-run)
- [Utilization](#utilization)
- [Implementation](#implementation)
- [Testing](#testing)
- [Documentation](#documentation)
- [Further improvements](#further-improvements)

## **Build and run**

<div style="color: red"><b>IMPORTANT:</b></div>

Ensure you have GCC/make installed on your system: `sudo apt-get install build-essential`

**Server:**

- Build: `make buildServer`
- Build & Run: `make runServer`
- Run with GDB (will cause display issue): `make gdbServer`

**Client:**

- Build: `make buildClient`
- Build & Run: `make runClient`
- Run with GDB (will cause display issue): `make gdbClient`

## **Utilization**

- Run the server and client instances
- You will be prompted to add a socket PORT (optional), ADDRESS (optional) and User
- A chat session will be displayed showing all recieved messages after the login process
- You can either send messages or use a built in command by starting your command with '/'.
- Commands: `/exit - closes the current instance (all instances if run on server)`

## **Implementation**
(to be added)

## **Testing**
(to be added)

## **Documentation**

- [opengroup.org syssocket.h 1997](https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html)
- [opengroup.org syssocket.h 2004](https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/socket.h.html)
- [IETF RFC9293 TCP](https://datatracker.ietf.org/doc/html/rfc9293)

## **Further improvements**
- <code>(DONE)</code> create a configuration file to setup default ports and number of connections.
- <code>(DONE)</code> create logger class to save outputs of instances.
- make client socket reusable
- make server take the IP address of the current machine (hardcoded at the moment)
- project built on Linux socket so it needs a safe port to Windows

<p align="right"><a href="#readme-top">(back to top)</a></p>
