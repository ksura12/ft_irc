/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_quit.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aionescu <aionescu@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:07:29 by aionescu          #+#    #+#             */
/*   Updated: 2023/03/05 15:07:30 by aionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"
#include "../includes/server.hpp"
#include "../includes/channel.hpp"
#include "../includes/CommanHandler.hpp"

/* format: PART <channel> *( "," <channel> ) [ <Part Message> ]
	 ***********************************************************
	The PART command causes the user sending the message to be removed
   from the list of active members for all given channels listed in the
   parameter string.  If a "Part Message" is given, this will be sent
   instead of the default message, the nickname.  This request is always
   granted by the server.
*/
void User::execute_part_cmd(User* user, std::vector<std::string> args)
{
	std::string part_msg;
	__SIZE_TYPE__ i = 0;

	// creating the right parting msg
	// if no msg is given as parameter, the nickname is used for it
		if (args.size() < 1)
		{
			user->replyLight("No params provided");
			return ;
		}
		// if one msg is given, it is used instead of nickname
		if (args.size() >= 2)
		{
			while (++i < args.size())
			{
				part_msg += " " + args[i];
			}
			if (part_msg.length() == 0)
				part_msg = "Leaving!";
		}
		else
		{
			replyLight("Please use format: '/part' without parameters");

			return;
		}
		if (this->_server->findChannel(args[0]) == NULL) {
			replyLight("part :Channel not found");
			return;
		}
		Channel* foundChannel = this->_server->findChannel(args[0]);
		if (foundChannel != NULL)
		{
			if (this->get_channel_if_in(foundChannel->getName()) != NULL)
			{
				// notifying others users about the parting
				// :mynick!mynick@myhost.com PART #mychannel :Leaving
				std::string reply_construct = ":" + this->getNick() + "!" + this->getUsername() + "@" + this->_server->_serverName + " PART " + foundChannel->getName() + part_msg;
				this->replyLightAll(reply_construct.c_str(), this->get_channel_if_in(foundChannel->getName()));

				//erasing User from the channellist and operator list
				foundChannel->partUser(getNick());

				// delete Channel from the Users Channellist
				this->delete_Channel(foundChannel);
			}
			else
			{
				this->reply(ERR_NOTONCHANNEL(foundChannel->getName()));
				return;
			}
		}
		else
		{
			this->reply(ERR_NOSUCHCHANNEL(this->getNick(), foundChannel->getName()));
			return;
		}
}

// format: QUIT (:)[<quitmsg>] OK NEED TESTING
/* A client session is ended with a quit message.  The server must close
   the connection to a client which sends a QUIT message. If a "Quit
   Message" is given, this will be sent instead of the default message,
   the nickname.
   If, for some other reason, a client connection is closed without  the
   client  issuing  a  QUIT  command  (e.g.  client  dies and EOF occurs
   on socket), the server is required to fill in the quit  message  with
   some sort  of  message  reflecting the nature of the event which
   caused it to happen.
*/
void User::execute_quit_cmd(const std::string& cmd_name, std::vector<std::string> args)
{
	(void) cmd_name;
	// send quit msg
	std::vector<Channel *> channel = getChannels();
	for(unsigned long i = 0;  i < getChannels().size(); i++)
	{
		std::string reason = "Quit: Leaving";
		if (args.size() == 0)
		{
			channel[i]->notify_others("QUIT", reason, this);
		}
		else if (args.size() > 0)
		{
			reason = "Quit: ";
			long unsigned int	args_index = 0;

			args[0].erase(0, 1);
			while (args_index < args.size())
			{
				reason.append(args[args_index]);
				if (args_index < args.size() - 1)
					reason.push_back(' ');
				args_index++;
			}
			channel[i]->notify_others("QUIT", reason, this);
		}
	}
	this->_server->killUser(this); //deletes User from all Channels
}