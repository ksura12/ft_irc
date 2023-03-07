/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_nick.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:17:53 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:17:54 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/user.hpp"
#include "../includes/server.hpp"
#include "../includes/channel.hpp"
#include "../includes/CommanHandler.hpp"


/*
* format: USER <username> <hostname> <servername> <realname>
* ***********************************************************
* used in communication between servers to indicate new user
* arriving on IRC, since only after both USER and NICK have been
* received from a client does a user become registered.
*/
void User::execute_user_cmd(User* user, std::vector<std::string> args)
{
	//std::cout << "Reached in the execute_user_cmd Function: Start" << std::endl;
	if (args.size() == 0)
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getUsername(), "User"));
		return ;
	}
	std::string username = args[0];
	//std::cout << "Reached in the execute_user_cmd Function: args[0]: " << args[0] << std::endl;

	user->setUsername(username);
	//std::cout << "Reached in the execute_user_cmd Function: if SetUser: " << getUsername() << "\n" << std::endl;
	std::cout << getUsername() << std::endl;
	std::string toSend = "USER Changed: " + username;
		// std::string toSend = ":" + user->getUsername() + " USER " + username;
	// user->replyLight(toSend.c_str());
	// user->setNick(nickname);
	//Notify all Users in All channels in which the User is in about the username
	std::vector<Channel*>::iterator itr;
	this->replyLight(toSend.c_str());
	for (itr = this->_channelList.begin(); itr != this->_channelList.end(); itr++) {
		Channel* foundChannel = (*itr);
		this->replyLightAll(toSend.c_str(), foundChannel);
	}
	
	std::cout << "The username is set to: " << this->getUsername() << std::endl;
}

/* 			NICK			*/
void User::execute_nick_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	(void) cmd_name;
	if (args.size() == 0)
	{
		user->reply(ERR_NONICKNAMEGIVEN(user->getNick()));
		return ;
	}
	std::string nickname = args[0];
	if (user->_server->findByNick(nickname) != NULL)
	{
		user->reply(ERR_NICKNAMEINUSE((user->getNick())));
		return ;
	}
	if (this->getNick() == "")
	{
		user->setNick(nickname);
		user->reply(RPL_WELCOME(nickname));
		user->replyLight("\nYou can use following commands on server:\n  /pass <server_password>\n  /join #<channelname>\n  /nick <newnickname>\n\nand following commands in a channel:\n  /part\n  /quit\n  /privmsg <nickname>\n\nand following commands as an operator:\n  /mode -o OR mode +o <nickname>\n  /kick <nickname>\n");
		return ;
	}
	std::string toSend = ":" + user->getNick() + " NICK " + nickname;
	user->replyLight(toSend.c_str());
	user->setNick(nickname);
	user->setUsername(nickname);

	//Notify all Users in All channels in which the User is in about the nicknamechange
	std::vector<Channel*>::iterator itr;
	for (itr = this->_channelList.begin(); itr != this->_channelList.end(); itr++) {
		Channel* foundChannel = (*itr);
		this->replyLightAll(toSend.c_str(), foundChannel);
	}
}