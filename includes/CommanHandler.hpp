/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommanHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:14:33 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:14:41 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "user.hpp"

class User;
class Message;
class Server;

class CommanHandler
{
	public:
		CommanHandler();
		CommanHandler(const CommanHandler& obj);
		CommanHandler& operator=(const CommanHandler& obj);
		~CommanHandler();

		void start(User* user, const std::string& message);

};

#endif