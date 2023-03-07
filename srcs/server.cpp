/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:17:44 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:17:45 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <errno.h>
#include <ctime>
#include "../includes/server.hpp"


		/*  Constructors and Deconstructors  */
//===================================================//

bool Server::_active = true;

Server::Server(std::string serverPass, int port): _port(port), _pass(serverPass)
{
	_serverName = "OurServer";
	std::cout << "Server Object Created" << std::endl;
}

Server::~Server()
{
	std::cout << "Deconstructor Called" << std::endl;
}

		/*   Getters and Setters    */
//===================================================//

void	Server::setPass(std::string pass) {
	this->_pass = pass;
}

std::string	Server::getPass() const {
	return (this->_pass);
}

void	Server::addChannel(Channel *newChannel)
{
	this->_channelList.push_back(newChannel);
}

			/*    Checkers     */
//===================================================//

User*	Server::findByFd(int clientFd) {
	std::vector<User*>::iterator itr;
	for (itr=this->_userList.begin(); itr != this->_userList.end(); itr++) {
		if (clientFd == (*itr)->getFd())
			return (*itr);
	}
	std::cerr << "User FD not found" << std::endl;
	return (NULL);
}

User*	Server::findByNick(std::string nick) {
	std::vector<User*>::iterator itr;
	for (itr=this->_userList.begin(); itr != this->_userList.end(); itr++) {
		if (nick == (*itr)->getNick())
			return (*itr);
	}
	std::cerr << "User nickname not found" << std::endl;
	return (NULL);
}

User*	Server::findByUser(std::string user) {
	std::vector<User*>::iterator itr;
	for (itr=this->_userList.begin(); itr != this->_userList.end(); itr++) {
		if (user == (*itr)->getUsername())
			return (*itr);
	}
	std::cerr << "User username not found" << std::endl;
	return (NULL);
}

Channel*	Server::findChannel(std::string name) {
	std::vector<Channel*>::iterator itr;
	for (itr=this->_channelList.begin(); itr != this->_channelList.end(); itr++) {
		if (name == (*itr)->getName())
			return (*itr);
	}
	std::cerr << "Channel not found by name" << std::endl;
	return (NULL);
}


			/*   Server Actions  */
//==============================================//

void	Server::killUser(User* user)
{
	std::vector<Channel*> activeChannelList = user->getChannels();
	std::vector<Channel*>::iterator start = activeChannelList.begin();
	std::vector<Channel*>::iterator end = activeChannelList.end();
	if (user->getChannels().size() > 1)
	{
		while (start != end)
		{
			(*start)->deleteUser(user->getNick()); // delete user from userloglist in channel
			start++;
		}
	}
	else if (user->getChannels().size() != 0)
	{
		(*start)->deleteUser(user->getNick());
		//start++;
	}
	std::vector<User*>::iterator start_U = _userList.begin();
	std::vector<User*>::iterator end_U = _userList.end();
	if (_userList.size() > 0)
	{
		while (start_U != end_U)
		{
			if ((*start_U)->getNick() == user->getNick()) // delete user from server
			{
				_userList.erase(start_U);
				std::cout << "erasing user" << std::endl;
				break ;
			}
			start_U++;
		}
	}
	delete user;
}

			/*    Socket Actions    */
//===================================================//

/*
* setsocktopt: sets certain options when using the associated socket
* SOL_SOCKET: indicates we are setting options at socket level
* SO_REUSEADDR: allows the socket to be bound to an address that is already in use
* **************************************
* fcntl(): manipulates filedescriptors;
* O_NONBLOCK: Do not block an open, a read, or a write on the file (do not wait for terminal input)
*/
int Server::createServer(void)
{
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)
	{
		std::cerr << "Can't create a socket!" << std::endl;
		return (-1);
	}
	
	int opt = 1;
	if (setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		std::cerr << "Error during setting socket options\n";
	
	if (fcntl(listening, F_SETFL, O_NONBLOCK) == -1)
		std::cerr << "Error during setting socket to NON_BLOCKING\n";
	
	struct sockaddr_in hint = {};
	// setting everything in the struct hint to zero. prevents certain segfaults
	bzero((char*) &hint, sizeof(hint));

	hint.sin_family = AF_INET; // IPv4 only
	hint.sin_addr.s_addr = htonl(INADDR_ANY); // puts host IP
	// inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	hint.sin_port = htons(this->_port); // convert host bytes to network bytes
	if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1) // binding socket to host IP
	{
		std::cerr << "Can't bind to IP/Port!" << std::endl;
		std::cerr << errno << std::endl;
		return (-2);
	}
	if (listen(listening, SOMAXCONN) == -1) // socket waiting for connection
	{
		std::cerr << "Can't listen!" << std::endl;
		return (-3);
	}
	//ft_log("Server listening");
	this->fd_server = listening;
	return (1);
}

/* Reads input from an fd, returns message if received successfully. */
int Server::readInput(int client_no)
{
	char buf[4096];
	memset(buf, 0, 4096);
	int bytesRecv = recv(this->clients[client_no].fd, buf, 4096, 0);
	if (bytesRecv == -1)
	{
		std::cerr << "There was a connection issue!" << std::endl;
		return (-1);
	}
	else if (bytesRecv == 0)
	{
		if (close(this->clients[client_no].fd) == -1)
			std::cerr << "close!" << std::endl;
		std::cerr << "The Client disconnected!" << std::endl;
		this->clients[client_no].fd *= -1; // it will be ignored in future
		return (-1);
	}
	else // data received from client
	{
		// Display message
		std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
	}
	this->msg = std::string(buf, 0, bytesRecv);
	return (1);
}

/*
* pollfd{fd,events,revents}: struct defines the relevant socket, what kind of event to wait for, and
* the type of event to respond with;
***********************************************
* POLLIN: read in data (from client);
* poll(): waits for action (I/O) on a filedescriptor in a set of filedescriptors
* returns -1 if faied, 0 if timed out or num of fd's connected;
* POLLHUP: Connection disconnected
*/
int Server::acceptCall()
{
	for (int i = 0; i < 1024; i++)
	{
		if ((this->clients[i].revents & POLLIN) == POLLIN) // fd is ready for reading
		{
			std::cout << "\n ----- New message incoming from fd: " << this->clients[i].fd << std::endl;
			if (this->clients[i].fd == this->fd_server) // request for new connection
			{
				std::cout << "New Connection " << std::endl;
				sockaddr_in user;
				socklen_t userSize = sizeof(user);
				char host[NI_MAXHOST];
				char service[NI_MAXSERV];
				memset(host, 0, NI_MAXHOST);
				memset(service, 0, NI_MAXSERV);
				int userSocket = accept(this->fd_server, (sockaddr *)&user, &userSize);
				if (userSocket == -1)
				{
					std::cerr << "Problem with client connecting!" << std::endl;
					return (-1);
				}

				// Next 10 lines deal with client info:
				int result = getnameinfo((sockaddr *)&user, userSize, host, NI_MAXHOST, service, NI_MAXSERV, 0);
				if (result)
				{
					std::cout << host << " connected on " << service << std::endl;
				}
				else
				{
					inet_ntop(AF_INET, &user.sin_addr, host, NI_MAXHOST);
					std::cout << host << " connected on " << ntohs(user.sin_port) << std::endl;
				}
				// ... until here

				int j = 0;
				for (; j < 1024; j++)
					if (clients[j].fd == -1)
						break;
				if (j < 1024)
				{
					clients[j].fd = userSocket;
					clients[j].events = POLLIN;
					clients[j].revents = 0;
						std::cout << "New connection data:\n fd_server: " << this->fd_server << std::endl;
						std::cout << "handler: " << this->handler << std::endl;
						std::cout << "msg: " << this->msg << std::endl;
						std::cout << "pass: " << this->_pass << std::endl;
						std::cout << "port: " << this->_port << std::endl;
						std::cout << "host: " << host << std::endl;
						std::cout << "service: " << service << std::endl;
						std::cout << "*********************************************************************************" << std::endl;
						User	*newUser = new User(clients[j], host, service, this);
						//std::cout << "Reached debugging point 1" << std::endl;
						this->_userList.push_back(newUser);
					//}
				}
				else
				{
					std::cout << "Server Capacity for Clients is Full" << std::endl;
					close(userSocket);
				}
			}
			else // data from an existing connection, recieve it
			{
				User* foundUser = this->findByFd(this->clients[i].fd);
				if (this->readInput(i) == -1)
					return (-1);
				else
				{
					std::cout << "Reached Accept Call Function, in  else if: Start" << std::endl;
					handler->start(foundUser, msg);

				}
			}
		}
	}
	return (1);
}

void	Server::initClient()
{
	for (int i = 0; i < 1024; i++)
	{
		this->clients[i].fd = -1;
		this->clients[i].events = 0;
		this->clients[i].revents = 0;
	}
}

bool	Server::free_everything() {
	std::cout << "This function will free and kill all users quit" << std::endl;
	// kill all users, then all channels will be killed
	std::vector<User *>::iterator	iterU = this->_userList.begin();
	while (!this->_userList.empty())
	{
		this->killUser(*iterU);
		//iterU++;
	}
	std::cout << "free_everything(): All users killed. Server ready to shut down." << std::endl;
	// server is killed outside;
	return (false);
}

void	Server::pollLoop()
{
	bool _activeLoop = true;
	while (_activeLoop)
	{
		switch (poll(this->clients, 1024, 10000))
		{
		case 0:
			break;
		case -1:
			break;
		default:
			this->acceptCall();
			break;
		}
		if (_active == false)
			_activeLoop = this->free_everything();
	}
}
