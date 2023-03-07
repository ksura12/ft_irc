/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aionescu <aionescu@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:09:38 by aionescu          #+#    #+#             */
/*   Updated: 2023/03/05 15:09:39 by aionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"
#include "../includes/server.hpp"
#include "../includes/channel.hpp"
#include "../includes/CommanHandler.hpp"

/*        KICK        */
// format: KICK <channel> <user> [<comment>]
void User::execute_kick_cmd(User *user, const std::string &cmd_name, std::vector<std::string> args)
{
	if (args.size() < 2)
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name));
		return;
	}

	const std::string channel_name = args[0];
	std::string target = args[1];
	std::string reason = "No Reason";
	if (args.size() > 2)
	{
		reason = "";
		long unsigned int	args_index = 2;
		while (args_index < args.size())
		{
			reason.append(args[args_index]);
			if (args_index < args.size() - 1)
				reason.push_back(' ');
			args_index++;
		}
	}

	// Find Channel in Server Channel List and return it's Channel Obj Pointer
	if (user->_server->findChannel(channel_name) == NULL)
	{
		user->replyLight("Channel does not exist");
		return;
	}
	Channel *foundChannel = user->_server->findChannel(channel_name);

	// Find if user that wants to kick someone is in same channel
	// Find First User in Channel::UsersinChannelList or in User::_channelList
	// Compare channels

	// We know have Channel Pointer and First User Pointer

	// Find First User by User Obj Pointer in Channel::UserList and return 1 if True, 0 if False
	if (foundChannel->isUserinChannelbyp(user) == NULL)
	{
		user->reply(ERR_NOTONCHANNEL(channel_name));
		return;
	}

	// Find First User in Channel Operator List
	if (foundChannel->is_User_Operator(user) == 0)
	{
		std::cout << "user is not operator\n";
		// reply("to First User that he cannot Kick second User");
		user->reply(ERR_CHANOPRIVSNEEDED(user->getNick(), channel_name));
		return;
	}
	//  Find Second User by Nick in Channel and return a pointer to Second User Object
	if (foundChannel->isUserinChannelbys(target) == NULL)
	{
		std::cout << "user is not found on channel\n";
		// reply("to First User that he cannot Kick second User");
		user->reply(ERR_USERNOTINCHANNEL(user->getNick(), channel_name));
		return;
	}
	User *secondUser = foundChannel->isUserinChannelbys(target);

	// We know the Channel Pointer, Pointer for the First User and Pointer for Second User
	// nick!user@server KICK #channel target_nick :the reason string
	std::string toSend = ":" + user->_nick + "!" + user->_username + "@" + user->_server->_serverName + " KICK " + channel_name + " " + secondUser->getNick() + " " + reason;
	this->replyLightAll(toSend.c_str(), foundChannel);

	foundChannel->deleteUser(target); // instead of deleteUser => use killUser()
	secondUser->delete_Channel(foundChannel);
}