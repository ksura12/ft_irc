/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommanHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:16:34 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:16:35 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/CommanHandler.hpp"

/*
* getline(): gets a line until delimitor;
* message: the message read in by the servers file descriptor
* client needs to be initizalied with the fd of the server
*/
void CommanHandler::start(User* user, const std::string& message) // client = user
{
	std::cout << "Reached the CommanHandler Function: Start" << std::endl;
	std::string line;
	std::stringstream s_stream(message);
	std::string cmd_name;

	while (std::getline(s_stream, line))
	{
		if (line[line.length() -1] == '\r') // irc messages are terminated with \r carriage return 
			line = line.substr(0, (line.length() - 1)); //*/ line.find('\r'));
		else // for netcat connection
			line = line.substr(0, (line.length())); //*/ line.find('\r'));
		cmd_name = line.substr(0, line.find(32)); // 32 == space
		std::cout << "cmd_name: " << cmd_name << std::endl;
		try
		{
			std::vector<std::string> cmd_args;
			std::string buffer = line.substr(cmd_name.length(), line.length());
			std::cout << "buffer string contents: " << buffer << std::endl;
			std::stringstream s_stream_args(buffer); // end of cmd_name till eol
			std::string output;

			while (s_stream_args >> output) // automatically splits at " "
			{
				std::cout << "output: " << output << std::endl;
				cmd_args.push_back(output);
			}
			if (cmd_name.compare("JOIN") == 0)
				user->execute_join_cmd(user, cmd_name, cmd_args);
			else if (cmd_name.compare("KICK") == 0)
				user->execute_kick_cmd(user, cmd_name, cmd_args);
			else if (cmd_name.compare("NICK") == 0)
				user->execute_nick_cmd(user, cmd_name, cmd_args);
			else if (cmd_name.compare("PASS") == 0 || cmd_name.compare("pass") == 0)
				user->execute_pass_cmd(user, cmd_name, cmd_args);
			else if (cmd_name.compare("privmsg") == 0)
				user->replyLight("privmsg :Only Enabled from Hexchat GUI");
			else if (cmd_name.compare("PRIVMSG") == 0) 
				user->execute_privmsg_cmd(cmd_name, user, cmd_args);
			else if (cmd_name.compare("QUIT") == 0)
				user->execute_quit_cmd(cmd_name, cmd_args);
			else if (cmd_name.compare("PART") == 0)
				user->execute_part_cmd(user, cmd_args);
			else if (cmd_name.compare("NOTICE") == 0)
				user->execute_notice_cmd(cmd_name, user, cmd_args);
			// else if (cmd_name.compare("INVITE") == 0) // NOT REQUIRED FOR EVALS -- to be tested
			// {
			// 	user->execute_invite_cmd(user, cmd_name, cmd_args);
			// }
			// else if (cmd_name.compare("BAN") == 0) // NOT REQUIRED FOR EVALS
			// {
			// 	user->execute_ban_cmd(user, cmd_name, cmd_args);
			// }
			else if (cmd_name.compare("MODE") == 0)
				user->execute_mode_cmd(cmd_name, cmd_args);
			else if (cmd_name.compare("user") == 0) // Work in progress
			{
				std::cout << "Reached the CommanHandler Function: in else if" << std::endl;
				user->execute_user_cmd(user, cmd_args);
			}


		}
		catch (const std::out_of_range& e)
		{
			//user->reply(user->ERR_UNKNOWNCOMMAND(cmd_name));
			std::cout << "Server caught uncommand command. Check last input" << std::endl;
		}
	}
}
