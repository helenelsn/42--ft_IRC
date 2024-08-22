/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:56:29 by Helene            #+#    #+#             */
/*   Updated: 2024/08/22 17:38:16 by Helene           ###   ########.fr       */
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


#include "Client.hpp"

# define    BACKLOG 10 // nombre max de demandes de connexions dans la file d'attente


/* Contains a vector of clients to keep track of them and manage their requests. */
class Server
{
    private :
        std::string         _port;
        std::string         _password;
        int                 _server_socket;
        std::vector<Client> _clients;
        
        std::vector<int>    _clients_sockets; // plus utile avec la structure de dessous
        std::vector<struct pollfd> _sockets;
        
        int                 _client_fd_test;
        
        

    public :  
        Server(std::string const& port, std::string const& password); // Lier la socket à une adresse et un port de la machine locale, puis écouter pour détecter des demandes de connexion via la socket.
        ~Server(); // Fermer toutes les sockets.
        void                InitServer(); // creates server socket and binds it to the given port
        void                RunServer(); // listen() on server socket, and makes looping calls to poll()
        void                AddClient();
        void                RemoveClient();
        void                AddToPoll(int fd, int events); 
        void                RemoveFromPoll(int fd); // removes corresponding fd from pollfds' vector and closes it, decrements poll_size
        void                AcceptClientConnection();
        void                ReadData(int fd);
};

void    Server::AddClient()
{
    
}

void    Server::RemoveClient()
{
    
}


void    Server::RemoveFromPoll(int fd)
{
    for (std::vector<struct pollfd>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
    {
        if (it->fd == fd)
        {
            _sockets.erase(it); // vérifier que fait pas de la merde
            close(fd);
            //break;
        }
    }
}

void    Server::AddToPoll(int fd, int events)
{
    pollfd newPoll;

    newPoll.fd = fd;
    newPoll.events = events;
    newPoll.revents = 0;
    (this->_sockets).push_back(newPoll);
}

void    Server::InitServer(void)
{
    struct  sockaddr_in toAdd;
    memset(&toAdd, 0, sizeof(toAdd));
    toAdd.sin_family = AF_INET;
    toAdd.sin_port = htons(atoi((this->_port).c_str()));
    toAdd.sin_addr.s_addr = INADDR_ANY; // represents “any” IP address, meaning the socket will be bound to all available network interfaces on the host.
    
    // cree la socket
    _server_socket = socket(PF_INET, SOCK_STREAM, 0);
    std::cout << "Connection socket created" << std::endl;

    int en = 1;
	if(setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on server socket"));
	
    // rend la socket non bloquante
    if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on server socket"));
    
    // binds the server socket to the given port
    if (bind(_server_socket, (struct sockaddr *)&toAdd, sizeof(toAdd)))
            ;// throw an exception
    
    // makes the socker passive
    if (listen(_server_socket, BACKLOG))
        ;// throw an exception
    
    AddToPoll(_server_socket, POLLIN); // rajouter d'autres flags ?

    std::cout << "Connection socket listening on port 4242" << std::endl;
}

void    Server::AcceptClientConnection(void)
{
    // struct sockaddr_storage client_addr; 
    // socklen_t addr_size;

    // addr_size = sizeof(client_addr);
    // _client_fd_test = accept(_server_socket, (struct sockaddr *)&client_addr, &addr_size);
    // if (_client_fd_test == -1) {
    //     fprintf(stderr, "accept: %s\n", strerror(errno));
    //     // launch exception;
    // }
    // printf("New connection! Socket fd: %d, client fd: %d\n", _server_socket, _client_fd_test);

    struct sockaddr_in  client_addr;
    socklen_t           addr_size = sizeof(client_addr);
    int newClient = accept(_server_socket, (struct sockaddr *)&client_addr, &addr_size);
    if (newClient == -1)
        ;
    
    // la rend non bloquante, car accept() est une fonction bloquante 
    if (fcntl(newClient, F_SETFL, O_NONBLOCK))
        ; 
    
    AddToPoll(newClient, POLL_IN);
    AddClient();
    
}

void    Server::ReadData(int fd)
{
    
}

void    Server::RunServer()
{
    while (1) // à modifier : doit gérer les signaux 
    {
        // appels à poll(), effectue les lectures/écritures en conséquence
        if (poll(&_sockets[0], _sockets.size(), -1))
            throw(std::runtime_error("poll() failed"));
        
        for (std::vector<struct pollfd>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
        {
            // if, if, if,... ou if, else if, else if, ... ? Peut avoir (revents & POLL_IN) && (revents & POLL_OUT) ?
            if (it->revents & POLL_ERR)
                ;
            if (it->revents & POLL_IN)
            {
                if (it->fd == _server_socket)
                    AcceptClientConnection();
                else
                    ReadData(it->fd);
            }
            if (it->revents & POLL_OUT)
                ;
        }
    }

    // Fermer tous les fd ici, ou le faire dans le destructeur ?
}

Server::Server(std::string const& port, std::string const& password)
: _port(port), _password(password)
{
    try {
        InitServer(); // crée une socket liée au port passé en argument, et écoute pour de nouvelles connections client
    }
    catch (const std::exception &e) {
        
    }





    
    // !!!! à modifier : l'appel à accept() bloque l’exécution en attendant une demande de connexion : le programme tourne dans le vide
    AcceptClientConnection();

    // on recoit un message via la socket client
    char buffer[BUFSIZ];
    int bytes_read = 1;
    while (bytes_read >= 0) {
        printf("Reading client socket %d\n", _client_fd_test);
        bytes_read = recv(_client_fd_test, buffer, BUFSIZ, 0);
        if (bytes_read == 0) {
            printf("Client socket %d: closed connection.\n", _client_fd_test);
            break ;
        }
        else if (bytes_read == -1) {
            fprintf(stderr, "recv error: %s\n", strerror(errno));
            break ;
        }
        else {
            // Si on a bien reçu un message, on va l'imprimer
            // puis renvoyer un message au client
            char *msg = "Got your message.";
            int msg_len = strlen(msg);
            int bytes_sent;

            buffer[bytes_read] = '\0';
            printf("Message received from client socket %d: \"%s\"\n", _client_fd_test, buffer);
                        
            bytes_sent = send(_client_fd_test, msg, msg_len, 0);
            if (bytes_sent == -1) {
                fprintf(stderr, "send error: %s\n", strerror(errno));
            }
            else if (bytes_sent == msg_len) {
                printf("Sent full message to client socket %d: \"%s\"\n", _client_fd_test, msg);
            }
            else {
                printf("Sent partial message to client socket %d: %d bytes sent.\n", _client_fd_test, bytes_sent);
            }
        }
    }
    
}

Server::~Server()
{
    close(_client_fd_test);
    close(_server_socket);
}


/*
Functionalities that might have to be implemented :
The server pings its clients periodically in order to identify timed out clients. 
*/