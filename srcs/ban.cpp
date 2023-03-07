/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ban.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aionescu <aionescu@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:05:47 by aionescu          #+#    #+#             */
/*   Updated: 2023/03/05 15:05:49 by aionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"
#include "../includes/server.hpp"
#include "../includes/channel.hpp"
#include "../includes/CommanHandler.hpp"

/*        BAN        */
// HexChat automatically interprets the mask in the most appropriate way
// format entered by user: /BAN mask
// format sent by HexChat: MODE #channel +b (nick OR interpreted_mask)
 void User::execute_ban_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	(void) cmd_name;
	if (args.size() < 3)
	{
		return;
	}

	const std::string	channel_name = args[0];
	std::string			ban_flag = args[1];
	std::string			target_nick = args[2];
	for (long unsigned int i = 0; i < target_nick.size(); i++)
	{
		if (target_nick.at(i) == '*' || target_nick.at(i) == '!')
			target_nick.erase(i);
	}
	std::string			mask_user_and_server = "!*@*";

	// Find Channel in Server Channel List and return it's Channel Obj Pointer
	if (user->_server->findChannel(channel_name) == NULL)
	{
		user->reply(ERR_NOSUCHCHANNEL(user->getNick(), channel_name));
		return ;
	}
	Channel* foundChannel = user->_server->findChannel(channel_name);

	//  Find First User by User Obj Pointer in Channel::UserList and return 1 if True, 0 if False
	if (foundChannel->isUserinChannelbyp(user) == NULL)
	{
		this->reply(ERR_NOTONCHANNEL(channel_name));
		return ;
	}

	// Find First User in Channel Operator List
	if (foundChannel->is_User_Operator(user) == 0)
	{
		user->reply(ERR_CHANOPRIVSNEEDED(user->getNick(), channel_name));
		return ;
	}

	if (ban_flag == "+b")
	{
		if (foundChannel->ifBanned(target_nick))
		{
			// :darkArmy.IRC4Fun.net 697 banner_nick #channel target_nick!*@* b :Channel ban list already contains target_nick!*@*
			std::string response = ":" + this->_server->_serverName + " 697 " + this->_nick + " " + foundChannel->getName() + " " + target_nick + mask_user_and_server + " b " + ":Channel ban list already contains " + target_nick + mask_user_and_server;
			this->replyLight(response.c_str());
		}
		else
		{
			foundChannel->addBanned(target_nick);
			// :banner_nick!banner_user@server MODE #channel +b :target_nick!*@*
			std::string response = ":" + this->_nick + "!" + this->_username + "@" + this->_server->_serverName + " MODE " + foundChannel->getName() + " +b :" + target_nick + mask_user_and_server;
			this->replyLightAll(response.c_str(), foundChannel);
		}
	}
	else // ban_flag is "-b"
	{
		if (foundChannel->ifBanned(target_nick))
		{
			foundChannel->removeBanned(target_nick);
			// :banner_nick!banner_user@server MODE #channel -b :target_nick!*@*
			std::string response = ":" + this->_nick + "!" + this->_username + "@" + this->_server->_serverName + " MODE " + foundChannel->getName() + " -b :" + target_nick + mask_user_and_server;
			this->replyLightAll(response.c_str(), foundChannel);
		}
		else
		{
			// :darkArmy.IRC4Fun.net 698 banner_nick #channel target_nick!*@* b :Channel ban list does not contain target_nick!*@*
			std::string response = ":" + this->_server->_serverName + " 698 " + this->_nick + " " + foundChannel->getName() + " " + target_nick + mask_user_and_server + " b " + ":Channel ban list does not contain " + target_nick + mask_user_and_server;
			this->replyLight(response.c_str());
		}
	}
}