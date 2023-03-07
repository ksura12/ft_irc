/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:14:53 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:14:54 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
 #define SERVER_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <poll.h>
#include <sstream>
#include <signal.h>
#include <vector>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "user.hpp"
#include "CommanHandler.hpp"

class Channel;
class User;
class Message;
class CommanHandler;

class Server {
	public:
		Server();
		~Server();
		Server(std::string pass, int port);

		int							createServer();
		void						initClient();
		void						pollLoop();
		void						pingClient();

		static bool 				_active;

		void						setPass(std::string pass);
		std::string					getPass() const;

		int							fd_server;
		struct pollfd 				clients[1024];
		std::string					msg;
		CommanHandler*				handler;
		

		void						killUser(User * user);
		Channel*					findChannel(std::string name); // finds a channel by name

		void						addChannel(Channel* newChannel);

		User*						findByFd(int clientFd); // finds user by FD
		User*						findByNick(std::string nick); // finds user by nickname
		User*						findByUser(std::string user); // finds user by username
		int							readInput(int client_no);
		int							acceptCall();
		bool						free_everything();

		std::string					_serverName;
		std::vector<Channel*> 		_channelList;

	private:
		int							_port;
		std::string					_pass;
		std::vector<User*> 			_userList;

		std::vector<std::string>	parseIncomingMsg(std::string buffer);
};

#endif