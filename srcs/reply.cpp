/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:17:21 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:17:22 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/user.hpp"
#include "../includes/server.hpp"
#include "../includes/channel.hpp"
#include "../includes/CommanHandler.hpp"

/*        REPLY        */
void User::reply(/*const std::string& reply_msg*/ const char* reply_msg)
{
	std::string str_reply;
	str_reply.assign(reply_msg);

	std::string sendMessage = ":" + getPrefix() + " " + str_reply + "\r\n";
	// write(":" + getPrefix() + " " + reply_msg);
	send(_fd, sendMessage.c_str(), sendMessage.size(), 0);
}

/* replyLight is similar to reply(), but does not add a prefix, 
and allows sending custom messages back to client */
void User::replyLight(const char* reply_msg)
{
	std::string str_reply;
	str_reply.assign(reply_msg);

	// std::cout << "char* accepted in function replyLight: " << str_reply << std::endl;
	std::string sendMessage = str_reply + "\r\n";
	send(_fd, sendMessage.c_str(), sendMessage.size(), 0);
}

/* replyLight is similar to reply(), but does not add a prefix, 
and allows sending custom messages back to client */
void User::replyLightAll(const char* reply_msg, Channel* activeChannel)
{
	std::vector<User *>::iterator itr;
	std::vector<User *>::iterator start = activeChannel->_userLogList.begin();
	std::vector<User *>::iterator end = activeChannel->_userLogList.end();
	std::string str_reply;
	str_reply.assign(reply_msg);

	std::string sendMessage = str_reply + "\r\n";

	for (itr = start; itr != end; itr++) {
		send((*itr)->getFd(), sendMessage.c_str(), sendMessage.size(), 0);
	}
}
