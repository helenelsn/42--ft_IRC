/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:56:29 by Helene            #+#    #+#             */
/*   Updated: 2024/09/22 19:56:28 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once // tester si suffit pour les inclusions multiples, 

#include <iostream>
#include <vector>
#include <unistd.h> // for close()
#include <netdb.h> // for struct addrinfo
#include <fcntl.h> // for fcntl()
#include <poll.h> // for poll()
#include <sys/socket.h> // for socket()
#include <sys/types.h> // for socket()

#include <sys/select.h> // for select()
#include <sys/time.h> // for select()

#include <netinet/in.h> // for sockaddr_in
#include <arpa/inet.h> // for inet_ntoa()
#include <csignal> // for signal()

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <map>

#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"


// class Client;

# define    BACKLOG 10 // nombre max de demandes de connexions dans la file d'attente
# define    BUFFERSIZE 3

extern bool serverShutdown;

class Server
{
    public :
        typedef std::vector<struct pollfd> pollfds;
        typedef pollfds::iterator   poll_it;
        typedef std::map<int, Client> clients;
        typedef clients::iterator clients_it;
        typedef std::map<std::string, Channel> channels;
        typedef channels::iterator channels_it;
        
    private :
        std::string         _port;
        std::string         _password;
        int                 _server_socket;
        pollfds             _sockets; // server-clients sockets 
        clients             _clients; // std::vector<Client> _clients ? 
        channels            _channels;
        void                RemoveClient(int fd);
        void                RemoveFromPoll(int fd); // removes corresponding fd from pollfds' vector and closes it, decrements poll_size
        
    public :  
        Server(std::string const& port, std::string const& password);
        ~Server(); 
        void                InitServer(); // creates server socket and binds it to the given port
        void                RunServer(); // listen() on server socket, and makes looping calls to poll()
        Client              *getClient(int fd);
        void                AddClient(int fd);
        void                AddToPoll(int fd, int events);
        void                RemoveClientFromAll(Client *client);
        void                AcceptClientConnection();
        void                ReadData(int fd);
        void                Parser(int fd, std::string data) { (void)fd; (void)data; }
};

/*
Functionalities that might have to be implemented :
The server pings its clients periodically in order to identify timed out clients. 
*/




