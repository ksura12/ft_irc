/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aionescu <aionescu@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:09:09 by aionescu          #+#    #+#             */
/*   Updated: 2023/03/05 15:09:10 by aionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"
#include "../includes/server.hpp"
#include "../includes/channel.hpp"
#include "../includes/CommanHandler.hpp"

/*        JOIN        */
void User::execute_join_cmd(User* user, std::string& cmd_name, std::vector<std::string> args)
{
	(void) user;
	(void) cmd_name;
	if (this->isRegistered() == false) {
		this->reply(ERR_NOTREGISTERED(this->getNick()));
		return ;
	}
	if (args.size() != 1)
		{
			replyLight("join :only one channel name allowed as parameter");
			return ;
		}
	if (args[0].empty())
		{
			replyLight("join :no channel name detected");
			return ;
		}
	if(this->_server->findChannel(args[0]) == NULL)
	{
		if (args[0].at(0) != '#')
		{
			replyLight("Wrong input for channel name, try with #");
			return ;
		}

		Channel* newChannel = new Channel(args[0]);
		this->_server->addChannel(newChannel);
		newChannel->_server = this->_server;
		newChannel->addCreator(this);
		newChannel->addUser(this);
		this->reply(RPL_JOIN(this->getNick(), newChannel->getName()));
		this->_channelList.push_back(newChannel);

		//this part is for making client recognize operators: / has to be sent on login
		std::string allUserList = newChannel->findAllUsers();
		std::string reply_construct = ":" + this->_server->_serverName + " 353 " + this->getNick() + " = " + newChannel->getName() + " :" + allUserList;
		this->replyLight(reply_construct.c_str());
		std::string reply_endOfNamesList = ":" + this->_server->_serverName + " 366 " + this->getNick() + " " + newChannel->getName() + " :End of /NAMES list.";
		this->replyLight(reply_endOfNamesList.c_str());
		std::string newCNotice = "New channel " + newChannel->getName() + " created by you. You are operator.";
		this->replyLight(newCNotice.c_str());

		newChannel->_countClients++;
	}
	else 
	{
		Channel* foundChannel = this->_server->findChannel(args[0]);
		if (foundChannel->ifBanned(this) == true || foundChannel->ifBanned(this->_nick) == true)
			this->reply(ERR_BANNEDFROMCHAN(args[0], _nick));
		else if(foundChannel->getInviteStatus() && !foundChannel->ifInvited(this))
			this->reply("Channel is in invite only mode");
		else
		{
			// maximum of 20 channels per connection
			if (foundChannel->getClientCount() >= foundChannel->getMaxClients())
			{
				this->reply(ERR_CHANNELISFULL(this->getNick(), args[0]));
				return ;
			}
			if (this->_channelList.size() >= 20)
			{
				this->reply(ERR_TOOMANYCHANNELS(this->getNick(), args[0]));
				return ;
			}
			if (foundChannel->ifJoined(this->getNick()) == 1) {
				this->reply(ERR_USERONCHANNEL(this->getNick(), foundChannel->getName()));
				return ;
			}
			foundChannel->addUser(this);
			this->_channelList.push_back(foundChannel);
			// std::string joinReply = ":" + this->getPrefix() + " JOIN " + foundChannel->getName();
			std::string joinReply = ":" + this->_nick + "!" + this->_username + "@" + this->_server->_serverName + " JOIN " + foundChannel->getName();
			this->replyLightAll(joinReply.c_str(), foundChannel); //sending the join rpl to all users in channel
			std::string allUserList = foundChannel->findAllUsers();
			std::string reply_construct = ":" + this->_server->_serverName + " 353 " + this->getNick() + " = " + foundChannel->getName() + " :" + allUserList;
			this->replyLightAll(reply_construct.c_str(), foundChannel);
			std::string newCNotice = "Joined channel " + foundChannel->getName() + ". You are not an operator.";
			this->replyLight(newCNotice.c_str());
			foundChannel->_countClients++;
		}
	}
}

// format: INVITE <nickname> <channel>
void User::execute_invite_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	// check if user is operator of the channel
		if (args.size() < 2 )
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name));
		return ;
	}
	const std::string channel_name = args[1];
	std::string toInvite = args[0];
	if (user->_server->findChannel(channel_name) == NULL)
	{
		user->replyLight("Channel does not exist");
		return ;
	}
	Channel* foundChannel = user->_server->findChannel(channel_name);
	// Find If Inviting User is in Channel: 1 if True, 0 if False
	if (foundChannel->isUserinChannelbyp(this) == NULL)
	{
		user->reply(ERR_NOTONCHANNEL(channel_name));
		return ;
	}
	//Find Inviter in Channel Operator List
	if (foundChannel->is_User_Operator(this) == 0)
	{
		std::cout << "user is not operator\n";
		user->reply(ERR_CHANOPRIVSNEEDED(user->getNick(), channel_name));
		return ;
	}
	// Find if toInivte User exists
	if (_server->findByNick(toInvite) == NULL)
		return ;
	//  Find toInvite User in Channel , return if he is already in
	if (foundChannel->isUserinChannelbys(toInvite))
	{
		return ;
	}
	if (foundChannel->getInviteStatus() == false)
	{
		user->replyLight("Channel Mode is not invite only");
		return ;
	}
	User* invitedUser = _server->findByNick(toInvite);
	// sending message to the Inviter and the toInvite as reply from server to accepting the command
	user->replyLight(RPL_INVITING(this->getNick(), toInvite, channel_name));
	invitedUser->replyLight(RPL_INVITING(this->getNick(), toInvite, channel_name));
	// adding the toInvite to the _inviteList of the channel
	foundChannel->addInvited(invitedUser);
}