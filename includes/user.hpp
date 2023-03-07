/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:15:51 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:15:52 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef USER_HPP
 #define USER_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>

#include "channel.hpp"
#include "server.hpp"
#include "CommanHandler.hpp"

class Channel;
class Server;
class CommanHandler;

class User {
	public:
		User(pollfd &client, char* host, char* service, Server* server);

		User&					operator=(const User &src);
		~User();

		void					setFd(int new_fd);
		int 					getFd() const;
		void 					setNick(std::string nick); // set in lowercase / convert
		std::string				getNick() const;
		void					setUsername(std::string username);
		std::string				getUsername();
		void					setRealname(std::string realname);
		std::string				getRealname() const;
		void					setHost(char* host);
		char**					getHost();
		void					setClient(pollfd &client); // stored client info on user profile
		void					setService(char* service);

		void					setRegistered();
		bool					isRegistered();

		time_t					getTime(); // returns actual time of call (used for Ping)
		time_t					getCreationTime(); // returns time of creation of user

		// Execution of Commands
		std::string 			getPrefix();
		void					reply(const char * reply_msg);
		void					execute_join_cmd(User* user, std::string& cmd_name, std::vector<std::string> args);
		void					execute_kick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args);
		void 					execute_invite_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args);
		void 					execute_quit_cmd(const std::string& cmd_name, std::vector<std::string> args);
		void					execute_ping_cmd(const std::string& cmd_name, std::vector<std::string> args);
		void					execute_nick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args);
		void 					execute_pass_cmd(User *usr, const std::string& cmd_name, std::vector<std::string> args);
		void					execute_user_cmd(User* user, std::vector<std::string> args);

		void 					execute_ban_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args);
		void					execute_mode_cmd(std::string cmd_name, std::vector<std::string> cmd_args);
		void					execute_privmsg_cmd(std::string cmd_name, User* user, std::vector<std::string> cmd_args);
		void					execute_notice_cmd(std::string cmd_name, User* user, std::vector<std::string> args);
		void					execute_part_cmd(User* user, std::vector<std::string> args);

		std::vector<Channel *>	getChannels();
		Channel*				get_channel_if_in(const std::string& channel_name);
		Channel*				findUserinChannel(std::string name);
		void					deleteChannel(const std::string channel);

		void					delete_Channel(Channel* channel);

		void 					replyLight(const char* reply_msg);
		void 					replyLightAll(const char* reply_msg, Channel* activeChannel);
		
		std::string				replyMessage;
		Server*					_server;


	// all reply message replies: 

	const char* RPL_WELCOME(std::string nick);
	const char* RPL_JOIN(std::string nick, std::string channel);
	const char* RPL_NAMREPLY(std::string channelname, std::string userlist);
	const char* RPL_PRIVMSG(std::string user, std::string receiver,  std::string msg);
	const char* RPL_NOTICE(std::string user, std::string receiver,  std::string msg);
	const char* RPL_YOUREOPER(std::string nick, std::string channelname);
	const char* RPL_INVITING(std::string Inviter, std::string toInvite, std::string channelname);
	const char* ERR_TOOMANYCHANNELS(std::string nick, std::string channel_name);
	const char* ERR_NONICKNAMEGIVEN(std::string nick);
	const char* ERR_USERONCHANNEL(std::string user, std::string channelname);
	const char* ERR_NICKNAMEINUSE(std::string nick);
	const char* ERR_UNKOWNMODE(std::string mode);
	const char* ERR_NOTONCHANNEL(std::string channel);
	const char* ERR_CHANOPRIVSNEEDED(std::string nick, std::string channel);
	const char* ERR_UNKNOWNCOMMAND(std::string cmd_name);


	const char* ERR_CHANNELISFULL(std::string nick, std::string channelname);
	const char* ERR_BANNEDFROMCHAN(std::string channelname, std::string nick);
	const char* ERR_CHATBANNED(std::string channelname, std::string nick);
	const char* ERR_NOTREGISTERED(std::string nickname);
	const char* ERR_NEEDMOREPARAMS(std::string user, std::string cmd);
	const char* ERR_ALREADYREGISTRED(std::string nickname);
	const char* ERR_NOSUCHCHANNEL(std::string nick, std::string channel);
	const char* ERR_NOSUCHNICK(std::string target);
	const char* ERR_NOSUCHSERVER(std::string servername);
	const char* ERR_USERNOTINCHANNEL(std::string target, std::string channel_name);

	std::vector<Channel *>		_channelList; // list of channels the user is part of for cross-exchanging data
	std::string					_nick;

	private:
		int						_fd;
		char*					_host;
		char*					_service;
		int						_userState;
		bool					_isRegistered;
		bool					_sentPassword;
		time_t					_creationTime;
		std::string				_username;
		std::string				_realname;
		struct pollfd &			_client;

};

#endif
