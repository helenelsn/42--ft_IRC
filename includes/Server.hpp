/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:56:29 by Helene            #+#    #+#             */
/*   Updated: 2024/10/05 13:43:23 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once // tester si suffit pour les inclusions multiples

#include "irc.hpp"
#include "Client.hpp"
#include "Channel_LN.hpp"
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
        pollfds             _disconnectedClients; 
        clients             _clients;
        channels            _channels;
        Logger              _logger;
        CommandsHandler     _commandsHandler; 
        std::string         _creationDate;
        
        Server(Server const& other);
        Server& operator=(Server const& other);
        void                setCreationDate(void);
        
    public :  
        Server(std::string const& port, std::string const& password);
        ~Server(); 
        void                InitServer(); // creates server socket, binds it to the given port, listen() on server socket
        void                RunServer(); // looping calls to poll()
        
        std::string         getPasswd(void) { return _password; }
        Client              *getClient(int fd);
        
        void                tryLogin(Client &client);
        void                sendMotd(Client &client);
        std::string         getCreationDate(void);
        
        // POLLIN        
        void                AcceptClientConnection();
        void                AddClient(int fd);
        
        void                ReadData(int fd);
        void                ProcessBuffer(Client* &client);
        void                ProcessCommand(std::string const& line, Client* &client);
        void                ParseLine(std::string line, CommandContext &ctx);

        // POLLOUT
        void                SendWriteBuffer(int fd);

        // POLLERR
        void                HandlePollErr(int fd);

        void                DisconnectClient(Client *client, std::string const& reason);
        // void                InformOfDisconnect(Client &client, std::string const& departureReason = DEPARTURE_REASON);
        void                InformOthers(Client &client, std::string const& source,  std::string const& msg);
        void                RemoveClient(Client *client);
        void                RemoveSocket(int client_fd);
        void                RestartServer();
        void                ShutdownServer();

        void                _log(e_logLevel level, std::string const& msg) { _logger.log(level, msg); }

        // USEFUL FOR COMMANDS PROCESSING
        bool                NickAlreadyUsed(std::string const& newNick);
};

/*
Functionalities that might have to be implemented :
The server pings its clients periodically in order to identify timed out clients. 
*/
