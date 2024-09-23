/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:56:26 by Helene            #+#    #+#             */
/*   Updated: 2024/09/23 22:11:14 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once // tester si suffit pour les inclusions multiples, 

// #include <iostream>
// #include <vector>
// #include <sys/socket.h> //-> for socket()
// #include <sys/types.h> //-> for socket()
// #include <netinet/in.h> //-> for sockaddr_in
// #include <fcntl.h> //-> for fcntl()
// #include <unistd.h> //-> for close()
// #include <arpa/inet.h> //-> for inet_ntoa()
// #include <poll.h> //-> for poll()
// #include <csignal> //-> for signal()

// #include <netdb.h> // for struct addrinfo

// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <errno.h>

#include "../includes/irc.hpp"

/* Contains all informations about a client inside a server. 
Read incoming data on the associated socket and parse the messages. */
// A uniquement besoin d'une socket qui se connecte a un serveur distant
class Client 
{
    private :
        int         _sockFd;
        std::string _username;
        std::string _hostname;
        std::string _nickname;
        std::string _realname;
        std::string _readBuffer; // data sent by client
        std::string _writeBuffer; // data to be sent to client
        std::string _modes; // user's currently set modes
        
        // int state; logged in, registered, disconnected, ?
        

    public :
        Client(int fd);
        ~Client();
        Client(Client const& other) {*this = other;} // private ?
        Client      operator==(Client const& other); // private ? 
        int         getSockFd(void);
        std::string getReadBuffer(void);
        void        writeToReadBuffer(std::string data);
        void        clearReadBuffer(void); 
        
        void        connect_to_s(std::string const& port);
        void        send_msg();
        void        receive_msg();
        
};
