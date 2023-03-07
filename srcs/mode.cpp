/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aionescu <aionescu@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:05:25 by aionescu          #+#    #+#             */
/*   Updated: 2023/03/05 15:05:28 by aionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"
#include "../includes/server.hpp"
#include "../includes/channel.hpp"
#include "../includes/CommanHandler.hpp"

/*
*  The MODE command is a dual-purpose command in IRC.  It allows both
   usernames and channels to have their mode changed.  The rationale for
   this choice is that one day nicknames will be obsolete and the
   equivalent property will be the channel.
* format: MODE <channelname> {[+|-]o} [<user>]
* o - give/take channel operator privileges;
*/
void User::execute_mode_cmd(std::string cmd_name, std::vector<std::string> cmd_args)
{
	// parsing
	if (cmd_args.size() < 3)
	// if (cmd_args.size() != 3)
	{
		if (cmd_args.size() == 1 && cmd_args[0].at(0) == '#')
		{
			std::string	channel_name = cmd_args[0];
			Channel *channel_ptr = this->_server->findChannel(cmd_args[0]);
			if (channel_ptr == NULL)
			{
				this->reply(ERR_NOSUCHCHANNEL(this->getNick(), channel_name));
				return;
			}
			// check if user is on channel
			channel_ptr = this->get_channel_if_in(channel_name);
			if (channel_ptr == NULL)
			{
				this->reply(ERR_NOTONCHANNEL(channel_name));
				return;
			}
			// check if executing user is operator
			if (channel_ptr->ifOperator(this->getNick()) == 0)
			{
				this->reply(ERR_CHANOPRIVSNEEDED(this->getNick(), channel_name));
				return;
			}
			else
			{
				this->reply(RPL_YOUREOPER(this->getNick(), channel_name));
				return;
			}
		}
		else
			this->reply(ERR_NEEDMOREPARAMS(this->getNick(), cmd_name));
		return;
	}
	if (cmd_args[0].at(0) != '#')
	{
		// 472     ERR_UNKNOWNMODE

		//std::cerr << "format: /mode +o nickname" << std::endl;
		replyLight("MODE :wrong format, use: /mode +/-o <nickname> or +/-i while inside a channel");
		return;
	}
	// getting parameters
	const std::string channel_name = cmd_args[0]; // with or without # ?
	std::string mode = cmd_args[1];
	std::cout << mode << std::endl;
	// std::string target = cmd_args[2];
	// std::cout << target << std::endl;
	//checking for ban mode: template: /mode #channel +b *!*@hostmask
	//example /mode #channel +b *!*@127.0.0.1
	//127.0.0.1 => user->_host
	if (mode.size() != 2)
	{
		this->reply(ERR_UNKOWNMODE(mode));
		return;
	}
	if (mode.at(1) == 'b' && (mode.at(0) == '+' || mode.at(0) == '-'))
	{
		execute_ban_cmd(this, cmd_name, cmd_args);
		return ;
	}
	// check if mode exists: only +/- o implemented
	if ((mode.at(1) != 'o' /*&& mode.at(1) != 'i'*/) && (mode.at(0) != '+' || mode.at(0) != '-'))
	{
		this->reply(ERR_UNKOWNMODE(mode));
		return;
	}
	// check if channel exists
	Channel *temp = this->_server->findChannel(channel_name);
	if (temp == NULL)
	{
		this->reply(ERR_NOSUCHCHANNEL(this->getNick(), channel_name));
		return;
	}
	// check if user is on channel
	Channel *channel = this->get_channel_if_in(channel_name);
	if (channel == NULL)
	{
		this->reply(ERR_NOTONCHANNEL(channel_name));
		return;
	}
	// check if executing user is operator
	if (channel->ifOperator(this->getNick()) == 0)
	{
		this->reply(ERR_CHANOPRIVSNEEDED(this->getNick(), channel_name));
		return;
	}
	// actual changing of the mode
	// if (mode.at(0) == '+' && mode.at(1) == 'i')
	// {
	// 	channel->setInviteStatus(true);
	// 	return;
	// }
	// else if (mode.at(0) == '-' && mode.at(1) == 'i')
	// {
	// 	channel->setInviteStatus(false);
	// 	return;
	// }
	std::string target = cmd_args[2];
	std::cout << target << std::endl;
	User* target_user = channel->isUserinChannelbys(target);
	if (mode.at(0) == '+' && mode.at(1) == 'o')
	{
		// nickname!username@servername MODE #channelname +o :targetnick
		channel->setOperator(target_user);
		target_user->reply(RPL_YOUREOPER(target_user->getNick(), channel->getName()));

		std::string reply_construct = ":" + this->getNick() + "!" + this->getUsername() + "@" + this->_server->_serverName + " MODE " + channel->getName() + " +o :" + target_user->getNick();
		this->replyLightAll(reply_construct.c_str(), channel);
		return;
	}
	else if (mode.at(0) == '-' && mode.at(1) == 'o')
	{
		channel->deleteOperator(target_user);
		std::string userNameOp = this->getNick() + " is not an operator anymore";
		target_user->replyLightAll(userNameOp.c_str(), channel);
		std::string allUserList = channel->findAllUsers();
		std::string reply_construct = ":" + this->getNick() + "!" + this->getUsername() + "@" + this->_server->_serverName + " MODE " + channel->getName() + " -o :" + target_user->getNick();
		this->replyLightAll(reply_construct.c_str(), channel);
		return;
	}
}