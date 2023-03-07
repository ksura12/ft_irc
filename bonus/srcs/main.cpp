/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksura <ksura@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 14:19:06 by ksura             #+#    #+#             */
/*   Updated: 2023/03/05 14:19:07 by ksura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <iostream> /* std::cout, std::cerr, std::endl */
#include <string> /* std::string */
#include <cstdlib> /* exit */
#include <cstdio> /* perror */
#include <string>
#include <cstring> /* memcpy, memset */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>

const unsigned int MAX_LINE = 1024; //buffersize
int PORT = 55000;
const char *HOST = "127.0.0.1";
int sockfd;

void        irc_connect();
void        irc_disconnect();
void        irc_identify();
void        s2u(const char* msg);
void            bot_functions(const std::string &buffer);
std::string    irc_parse(std::string buffer);
void           input_handler(char **argv);

int main(int argc, char **argv)
{
    (void) argv;
    if (argc != 2)
    {
        std::cout << "Please enter the right amount of parameters: ./bot <portnumber>" << std::endl;
        return -1;
    }
    PORT = atoi(argv[1]);
    irc_connect();
    irc_identify();
    for(;;) {
        char buffer[MAX_LINE+1] = {0};
        if(recv(sockfd, buffer, MAX_LINE*sizeof(char), 0)<0) {
            perror("recv()");
            irc_disconnect();
            exit(1);
        }
        std::cout << buffer << std::endl;

        std::string parsed = irc_parse(buffer);
        bot_functions(parsed);
    }
    irc_disconnect();
}

void irc_disconnect() {
close(sockfd);
}

void irc_connect() 
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("socket()");
        irc_disconnect();
        exit(1);
    }
    hostent *hp = gethostbyname(HOST);
    if(!hp) {
        std::cerr << "gethostbyname()" << std::endl;
        irc_disconnect();
        exit(1);
    }
    sockaddr_in sin;
    memset((char*)&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    memcpy((char*)&sin.sin_addr, hp->h_addr, hp->h_length);
    sin.sin_port = htons(PORT);
    memset(&(sin.sin_zero), 0, 8*sizeof(char));
    if(connect(sockfd, (sockaddr*) &sin, sizeof(sin))==-1) {
        perror("connect()");
        irc_disconnect();
        exit(1);
    }
}

void s2u(const char* msg)
{
    send(sockfd, msg, strlen(msg), 0);
}


void irc_identify()
{
s2u("pass pass\r\n");  
s2u("NICK RoBot\r\n"); // Nick
s2u("user KevinsBot\r\n"); // User
s2u("JOIN #botchannel\r\n"); // enter channel
s2u("/PRIVMSG #test :Hello! This is RoBot. How are you?\r\n"); 
}

std::string irc_parse(std::string buffer) {
    if(buffer.find("\r\n") == buffer.length()-2)
        buffer.erase(buffer.length()-2);
    return buffer;
}

void bot_functions(const std::string &buffer) {
    if(buffer.find("Hello") != std::string::npos) {
        s2u("PRIVMSG #botchannel :Hello! How are you?\r\n");
    }
    if(buffer.find("JOIN") != std::string::npos) {
        s2u("PRIVMSG #botchannel :Hello! This a JokeBot. Can I tell you a joke? Please answer with: joke\n Otherwise you could try entering: art\r\n");
    }
    if(buffer.find("joke") != std::string::npos) {
        int joke = rand() % 10;
        switch (joke)
        {
        case 1:
            s2u("PRIVMSG #botchannel :Why do programmers prefer dark mode? Because light attracts bugs.\r\n");
            break;
        case 2:
            s2u("PRIVMSG #botchannel :Why do programmers always mix up Halloween and Christmas? Because Oct 31 equals Dec 25.\r\n");
            break;
        case 3:
            s2u("PRIVMSG #botchannel :Why do programmers always mix up Halloween and Christmas? Because Octal 31 equals Decimal 25.\r\n");
            break;
        case 4:
            s2u("PRIVMSG #botchannel :Why did the programmer quit his job? He didn't get arrays.\r\n");
            break;
        case 5:
            s2u("PRIVMSG #botchannel :Why did the programmer go broke? He used up all his cache.\r\n");
            break;
        case 6:
            s2u("PRIVMSG #botchannel : Why do programmers prefer dark chocolate? Because it's bitter and complex, just like their code.\r\n");
            break;
        case 7:
            s2u("PRIVMSG #botchannel :Why did the programmer go to art school? To learn how to make better frames.\r\n");
            break;
        case 8:
            s2u("PRIVMSG #botchannel :Why did the programmer go to the beach? To surf the web.\r\n");
            break;
        case 9:
            s2u("PRIVMSG #botchannel :Why did the programmer cross the road? To get to the other site.\r\n");
            break;
        default:
            s2u("PRIVMSG #botchannel :Why do Java developers wear glasses? Because they can't C#!\r\n");
            break;
        }
    }
    if (buffer.find("art") != std::string::npos)
    {
        s2u("PRIVMSG #botchannel :OK, Maybe you want to see some art? Just joking, i have no other functions, take it or not\r\n");
    }
    
}

