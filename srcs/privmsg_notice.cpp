/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg_notice.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:17:15 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:17:16 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/user.hpp"
#include "../includes/server.hpp"
#include "../includes/channel.hpp"
#include "../includes/CommanHandler.hpp"

// PRIVMSG <receiver> <text to be sent>
void User::execute_privmsg_cmd(std::string cmd_name, User* user, std::vector<std::string> args)
{
	std::cout << "Reached execute_privmsg_cmd: Start" << std::endl;

	if (args[0].empty() || args.size() < 2 || args[1].empty())
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), "PRIVMSG"));
		return;
	}
	if (args[0].at(0) == '#')
	{
		std::cout << "#channel PRIVMSG checking..." << std::endl;
		std::string	channel_name = args[0];
		Channel *destination_channel = NULL;
		std::vector<Channel *>::iterator iter;
		for (iter = this->_server->_channelList.begin(); iter != this->_channelList.end(); iter++)
		{
			if ((*iter)->getName() == channel_name)
			{
				destination_channel = *iter;
				break ;
			}
		}
		if (destination_channel == NULL)
		{
			std::cout << "#channel PRIVMSG checking... NOSUCHCHANNEL" << std::endl;
			user->reply(ERR_NOSUCHCHANNEL(user->getNick(), channel_name));
			return ;
		}
		else if (destination_channel->ifBanned(this->_nick))
		{
			std::cout << "#channel PRIVMSG checking... CHATBANNED" << std::endl;
			user->reply(ERR_CHATBANNED(user->getNick(), channel_name));
			return ;
		}
	}
	std::string receiver = args[0];
	std::string msg;
	std::vector<std::string>::iterator start = args.begin();
	start++;
	std::vector<std::string>::iterator end = args.end();
	// read in message args
	while(start != end)
	{
		msg.append(*start);
		msg.append(" ");
		start++;
	}
	// check if msg starts with ':'
	if (msg[0] == ':')
		msg = msg.substr(1);

	if (receiver[0] == '#') // #: send to all in channel, here: check if channel exists
	{
		std::string channel_name = receiver.substr(0);
		Channel* channel = user->get_channel_if_in(channel_name); // is calling user in that channel he wants to send to?
		if (channel == NULL)
		{
			user->reply(ERR_NOSUCHCHANNEL(user->getNick(), channel_name));
			return ;
		}
		channel->notify_others(cmd_name, msg, user);
		return ;
	}
	// receiver not on server
	User* target_user = user->_server->findByNick(receiver);
	if (target_user == NULL)
	{
		user->reply(ERR_NOSUCHNICK(receiver));
		return;
	}
	// actually sending the priv msg (can make one write/send function out of it for other uses)
	// Response: :sender_nick!sender_user@server PRIVMSG receiver_nick :message
	// std::string priv_msg = RPL_PRIVMSG(user->getPrefix(), receiver, msg);
	std::string priv_msg = RPL_PRIVMSG(user->_nick + "!" + user->_username + "@" + user->_server->_serverName, receiver, msg);
	std::string msg_buf = priv_msg + "\r\n";
	if (send(target_user->_fd, msg_buf.c_str(), msg_buf.length(), 0) < 0)
		std::cerr << "Error while sending message to client." << std::endl;
	// priv_msg = RPL_PRIVMSG(user->getPrefix(), receiver, msg);
	// msg_buf = priv_msg + "\r\n";
	// if (send(this->_fd, msg_buf.c_str(), msg_buf.length(), 0) < 0)
	// 	std::cerr << "Error while sending message to client." << std::endl;
}

// NOTICE <receiver> <text to be sent>
void User::execute_notice_cmd(std::string cmd_name, User* user, std::vector<std::string> args)
{
	std::cout << "Reached execute_notice_cmd: Start" << std::endl;

	if (args[0].empty() || args.size() < 2 || args[1].empty())
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), "NOTICE"));
		return;
	}
	std::string receiver = args[0];
	std::string msg;
	std::vector<std::string>::iterator start = args.begin();
	start++;
	std::vector<std::string>::iterator end = args.end();
	// read in message args
	while(start != end)
	{
		msg.append(*start);
		msg.append(" ");
		start++;
	}
	// check if msg starts with ':'
	if (msg[0] == ':')
		msg = msg.substr(1);

	if (receiver[0] == '#') // #: send to all in channel, here: check if channel exists
	{
		std::string channel_name = receiver.substr(0);
		Channel* channel = user->get_channel_if_in(channel_name); // is calling user in that channel he wants to send to?
		if (channel == NULL)
		{
			user->reply(ERR_NOSUCHCHANNEL(user->getNick(), channel_name));
			return ;
		}
		channel->notify_others(cmd_name, msg, user);
		return ;
	}
	// receiver not on server
	User* target_user = user->_server->findByNick(receiver);
	if (target_user == NULL)
	{
		user->reply(ERR_NOSUCHNICK(receiver));
		return;
	}
	// actually sending the priv msg (can make one write/send function out of it for other uses)
	std::string priv_msg = RPL_NOTICE(user->_nick, receiver, msg);
	std::string msg_buf = priv_msg + "\r\n";
	// std::cout << "NOTICE REPLY: " << priv_msg << std::endl;
	if (send(target_user->_fd, msg_buf.c_str(), msg_buf.length(), 0) < 0)
		std::cerr << "Error while sending message to client." << std::endl;
}