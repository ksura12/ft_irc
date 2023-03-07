/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responses.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aionescu <aionescu@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 15:06:05 by aionescu          #+#    #+#             */
/*   Updated: 2023/03/05 15:06:06 by aionescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/user.hpp"

// Use this template to write all reply messages: 
// const char* User::RPL_OR_/ERR_<NAME>(std::string INPUT1, std::string INPUT2, ...)
// {replyMessage = "..." + INPUT1 + ...; return (replyMessage.c_str());}

// # define RPL_WELCOME(nick)              ("001 " #nick " welcome to ft_irc")
const char* User::RPL_WELCOME(std::string nick)
	{replyMessage = "001 " + nick + " welcome to ft_irc"; return (replyMessage.c_str());}

// # define RPL_NAMREPLY(channel, userlist)                 ("353 " #channel " :" #userlist)
const char* User::RPL_NAMREPLY(std::string channelname, std::string userlist)
	{replyMessage = "353 " + channelname + " :" + userlist; return (replyMessage.c_str());}

// # define RPL_JOIN(nick, channel)                        (":" #nick " JOIN :" #channel)
const char* User::RPL_JOIN(std::string nick, std::string channel)
	{(void) nick; replyMessage = "JOIN :" + channel; return (replyMessage.c_str());}

//# define ERR_NOSUCHNICK(target, channel_name)			("401 " #target  " " #channel_name " :No such nick/channel")
const char* User::ERR_NOSUCHNICK(std::string target)
	{replyMessage = "401 " + target + " :No such nick/channel"; return (replyMessage.c_str());}

//# define ERR_NOSUCHSERVER(servername)	("402 " "<server name> :No such server")
const char* User::ERR_NOSUCHSERVER(std::string servername)
	{replyMessage = "402 " + servername + " :No such server"; return (replyMessage.c_str());}

// # define ERR_NOSUCHCHANNEL	(:darkArmy.IRC4Fun.net 403 nick #channel :No such channel)
const char* User::ERR_NOSUCHCHANNEL(std::string nick, std::string channel)
	{replyMessage = "403 " + nick + " " + channel + " :No such channel"; return (replyMessage.c_str());}

// # define ERR_TOOMANYCHANNELS(nick, channel_name)         ("405 " #nick " " #channel_name ": maximum of channels reached")
const char* User::ERR_TOOMANYCHANNELS(std::string nick, std::string channel_name)
	{replyMessage = "405 " + nick + " " + channel_name + ": maximum of channels reached"; return (replyMessage.c_str());}

// 421  ERR_UNKNOWNCOMMAND "<command> :Unknown command"
const char* User::ERR_UNKNOWNCOMMAND(std::string cmd_name)
	{replyMessage = "421 " + cmd_name + " " + ":Unknown command"; return (replyMessage.c_str());}

// # define ERR_NONICKNAMEGIVEN(nick)	                ("431 " #nick " :No nickname given")
const char* User::ERR_NONICKNAMEGIVEN(std::string nick)
	{replyMessage = "431 " + nick + " :No nickname given"; return (replyMessage.c_str());}


// # define ERR_NICKNAMEINUSE(nick)		            ("433 " #nick  " :Nickname is already in use")
const char* User::ERR_NICKNAMEINUSE(std::string nick)
	{replyMessage = "433 " + nick + " :Nickname is already in use"; return (replyMessage.c_str());}

// # define ERR_USERNOTINCHANNEL(target, channel_name)	("441 " #target " " #channel_name " :They aren't on that channel")
const char* User::ERR_USERNOTINCHANNEL(std::string target, std::string channel_name)
	{replyMessage = "441 " + target + " " + channel_name + " :They aren't on that channel"; return (replyMessage.c_str());}

//       442     ERR_NOTONCHANNEL "<channel> :You're not on that channel"
const char* User::ERR_NOTONCHANNEL(std::string channel)
	{replyMessage = "442 " + channel + " :You're not on that channel"; return (replyMessage.c_str());}

// # define ERR_USERONCHANNEL(user, channel)		("443 " #user " " #channel " :is already on channel")
const char* User::ERR_USERONCHANNEL(std::string user, std::string channelname)
	{replyMessage = "443 " + user + " " + channelname + " :is already on channel"; return (replyMessage.c_str());}

// # define ERR_NOTREGISTERED(_nick)		            ("451 " #_nick " :You have not registered")
const char* User::ERR_NOTREGISTERED(std::string nickname)
	{replyMessage = "451 " + nickname + " :You have not registered"; return (replyMessage.c_str());}


// # define ERR_NEEDMOREPARAMS(nick, cmd)	           ("461 " #nick " " #cmd " :not enough parameters")
const char* User::ERR_NEEDMOREPARAMS(std::string user, std::string cmd)
	{replyMessage = "461 " + user + " " + cmd + " :not enough parameters"; return (replyMessage.c_str());}

//# define ERR_ALREADYREGISTRED(nick)                  ("462 " #nick " :You may not reregister")
const char* User::ERR_ALREADYREGISTRED(std::string nickname)
	{replyMessage = "462 " + nickname + " :You may not reregister"; return (replyMessage.c_str());}


// # define ERR_CHANNELISFULL(nick, channel)		("471 " #nick " " #channel " channel is full")
const char* User::ERR_CHANNELISFULL(std::string nick, std::string channelname)
	{replyMessage = "471 " + nick + " " + channelname + " channel is full"; return (replyMessage.c_str());}

// <char> :is unknown mode char to me"
const char* User::ERR_UNKOWNMODE(std::string mode)
	{replyMessage = "472 " + mode + ":is unknown mode char to me"; return (replyMessage.c_str());}

// # define ERR_BANNEDFROMCHAN(channel)	        ("474" #channel " :Cannot join channel (+b)")
const char* User::ERR_BANNEDFROMCHAN(std::string channelname, std::string nick)
	{replyMessage = "474 " + nick + " " + channelname + " :Cannot join channel (you're banned)"; return (replyMessage.c_str());}

// :odyssey.IRC4Fun.net 404 nickname #channel :You cannot send messages to this channel whilst banned.
const char* User::ERR_CHATBANNED(std::string channelname, std::string nick)
	{replyMessage = "001 " + channelname + " " + nick + " - You cannot send messages to this channel whilst banned."; return (replyMessage.c_str());}

// #define RPL_PRIVMSG(user, target, message)		":" + source + " PRIVMSG " + target + " :" + message
const char* User::RPL_PRIVMSG(std::string user, std::string receiver,  std::string msg)
	{replyMessage = ":" + user + " PRIVMSG " + receiver + " :" + msg; return (replyMessage.c_str());}

// #define RPL_NOTICE(user, target, message)		":" + source + " NOTICE " + target + " :" + message
const char* User::RPL_NOTICE(std::string user, std::string receiver,  std::string msg)
	{replyMessage = ":" + user + "!" + this->_username + "@" + this->_server->_serverName + " NOTICE " + receiver + " :" + msg; return (replyMessage.c_str());}

// # define ERR_CHANOPRIVSNEEDED(nick, channel)	("482 " #nick " " #channel " :You're not channel operator")
const char* User::ERR_CHANOPRIVSNEEDED(std::string nick, std::string channel)
	{replyMessage = "482 " + nick + " " + channel + " - You're not a channel operator"; return (replyMessage.c_str());}


// # define RPL_YOUREOPER			381         (":You are now an IRC operator")
const char* User::RPL_YOUREOPER(std::string nick, std::string channelname)
	{replyMessage = "381 " + nick + " " + channelname + " - You are a channel operator"; return (replyMessage.c_str());}

// # define RPL_INVITING			341         ("::<inviter> INVITING <invitee> <channel>")
const char* User::RPL_INVITING(std::string Inviter, std::string toInvite, std::string channelname)
	{replyMessage = ":" + Inviter + "INVITING" + toInvite + channelname; return (replyMessage.c_str());}