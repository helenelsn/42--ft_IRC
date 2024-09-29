/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:56:29 by Helene            #+#    #+#             */
/*   Updated: 2024/09/27 18:17:51 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once // tester si suffit pour les inclusions multiples

#include "irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Logger.hpp"
#include "CommandsHandler.hpp"

# define    BACKLOG 10 // nombre max de demandes de connexions dans la file d'attente
// -> voir comment la gérer

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
        pollfds             _newSockets; // utile ? // sockets from new connections, to add tp _sockets at the end of the for() loop
        clients             _clients;
        channels            _channels;
        Logger              _logger;
        CommandsHandler     _commandsHandler; 
        
    public :  
        Server(std::string const& port, std::string const& password);
        ~Server(); 
        void                InitServer(); // creates server socket, binds it to the given port, listen() on server socket
        void                RunServer(); // looping calls to poll()
        
        Client              *getClient(int fd);
        void                AddClient(int fd);
        void                AddToPoll(int fd, int events);
        void                RemoveClient(Client *client);
        void                AcceptClientConnection();
        void                ReadData(int fd);

        void                ProcessBuffer(Client* &client);
        void                ProcessCommand(std::string const& line, Client* &client);
        void                ParseLine(std::string line, CommandContext &ctx);

        void                RestartServer();
        void                ShutdownServer();
};

/*
Functionalities that might have to be implemented :
The server pings its clients periodically in order to identify timed out clients. 
*/
