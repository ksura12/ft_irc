/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass_ping.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:17:05 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:17:06 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/user.hpp"
#include "../includes/server.hpp"
#include "../includes/channel.hpp"
#include "../includes/CommanHandler.hpp"

// Use of the extended prefix (['!' <user> ] ['@' <host> ]) must
// not be used in server to server communications and is only
// intended for server to client messages in order to provide
// clients with more useful information about who a message is
// from without the need for additional queries.
// -> calling start in server leading to here.
std::string User::getPrefix()
{
	char* buf = this->_host;
	std::string hostname;
	hostname.assign(buf);
	std::string return_msg = this->getNick();

	/* :<nickname>!<username>@host CMD <target> \r\n */
	if (this->_username.empty() == false) {
		return_msg += "!" + this->_username;
		if (hostname.empty() == false)
			return_msg += "@" + hostname;
	}
	return(return_msg);
 }

// format: PING <server1> [<server2>] 
// 		the User pings the server1, if there would be a server2 specified, 
//		server1 would need to forward the Ping command to server2
void User::execute_ping_cmd(const std::string& cmd_name, std::vector<std::string> args)
{
	(void) cmd_name;
	this->reply(("PONG " + args[0]).c_str()); // server replies to client PONG 
}

// format: PASS <password>
void User::execute_pass_cmd(User* user, const std::string& cmd_name, std::vector<std::string> args)
{
	if (args.size() == 0)
	{
		user->reply(ERR_NEEDMOREPARAMS(user->getNick(), cmd_name));
		return ;
	}
	if (args.size() > 1)
	{
		user->replyLight("pass :too many parameters");
		return ;
	}
	if (user->isRegistered() == true)
	{
		user->reply(ERR_ALREADYREGISTRED(user->getNick()));
		return ;
	}
	std::string _newPassword = args[0];
	if (user->_server->getPass() == _newPassword)
	{
		this->setRegistered();
		user->replyLight("pass :successfully registered");
		return ;
	}
	else
		user->replyLight("pass :wrong password");
	return ;
}