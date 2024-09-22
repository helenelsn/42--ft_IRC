/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:49 by Helene            #+#    #+#             */
/*   Updated: 2024/09/22 23:12:33 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

/* -------------------------- CONSTRUCTORS, DESTRUCTOR ------------------------------- */

Server::Server(std::string const& port, std::string const& password)
: _port(port), _password(password)
{
    // ?
}

Server::~Server()
{
    close(_server_socket);
}

/* -------------------------- INIT SERVER ------------------------------- */

void    Server::InitServer(void)
{
    // struct  sockaddr_in toAdd;
    // memset(&toAdd, 0, sizeof(toAdd));
    // toAdd.sin_family = AF_INET;
    // toAdd.sin_port = htons(atoi((this->_port).c_str()));
    // toAdd.sin_addr.s_addr = INADDR_ANY; // represents “any” IP address, meaning the socket will be bound to all available network interfaces on the host.
    // _server_socket = socket(PF_INET, SOCK_STREAM, 0);

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // localhost by default
    if (getaddrinfo(NULL, (this->_port).c_str(), &hints, &res))
        ;// throw exception
    _server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    
    std::cout << "Connection socket created" << std::endl;

    int en = 1;
    // The setsockopt() API allows the application to reuse the local address when the server is restarted before the required wait time expires.
	if(setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on server socket"));
	
    // The fcntl() API sets the socket to be nonblocking
    if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on server socket"));
    
    // The bind() API gets a unique name for the socket.
    // if (bind(_server_socket, (struct sockaddr *)&toAdd, sizeof(toAdd)))
    //         close(_server_socket);// throw an exception
    if (bind(_server_socket, res->ai_addr, res->ai_addrlen))
            close(_server_socket);// throw an exception
    
    // makes the socker passive
    if (listen(_server_socket, BACKLOG))
        close(_server_socket);// throw an exception
    
    
    AddToPoll(_server_socket, POLL_IN);

    freeaddrinfo(res);

    std::cout << "Connection socket listening on port " << this->_port << std::endl;
}


/* -------------------------- MAIN LOOP ------------------------------- */

/* 
POLLIN : there is data to read
POLLOUT : Writing is now possible
POLLHUP : Hang up. When reading from a stream socket, this event merely indicates that the peer
    closed its end of the channel.  Subsequent reads from the channel will return 0 (end of file) 
    only after all outstanding data in the channel has been consumed.
POLLERR : Error condition

* poll : 
* The accept() and recv() APIs are completed when the EWOULDBLOCK is returned.

*/
void    Server::RunServer()
{ 
    printf("in RunServer()\n");
    while (!serverShutdown)
    {
        // The poll() API allows the process to wait for an event to occur and to wake up the process when the event occurs.
        if (poll(&this->_sockets[0], _sockets.size(), -1) == -1)
        {
            int errNum = errno;
            if (errNum != EINTR) // EINTR : A signal occurred before any requested event
                throw(std::runtime_error("poll() failed"));
                // throw IOException(poll, errNum); ?
            break;
        }
        
        // printf("\'while(serverShutdown)\', serverShutdown = %i\n", serverShutdown);
        
        //for (poll_it it = _sockets.begin(); it != _sockets.end(); it++)
        for (size_t i = 0; i < _sockets.size(); i++)
        {            
            //printf("in the for(pollfd) loop\n");
            // if, if, if,... ou if, else if, else if, ... ? Peut avoir (revents & POLL_IN) && (revents & POLL_OUT) ?
            if (_sockets[i].revents & POLL_ERR)
                ;
            else if (_sockets[i].revents & POLL_IN) // data is ready to recv() on this socket.
            {
                // printf("POLL_IN event, fd = %d\n", _sockets[i].fd);
                if (_sockets[i].fd == _server_socket) // server side
                    AcceptClientConnection();
                else
                    ReadData(_sockets[i].fd); // client side
            }
            else if (_sockets[i].revents & POLL_OUT) // we can send() data to this socket without blocking.
                ;
            else if (_sockets[i].revents & POLLHUP)
                ;
        }
    }
    
    
    //shutdown(_server_socket)
    // Fermer tous les fd ici, ou le faire dans le destructeur ?
}



/* -------------------------- GETTERS, SETTERS ------------------------------- */

// quel type renvoyer ? pointeur, iterateur, ... ?
Client              *Server::getClient(int fd)
{
    // printf("DEBUG : list of current clients in map : \n");
    // for (clients_it it = this->_clients.begin(); it != this->_clients.end(); it++)
    //     printf("\tclient : fd = %d, client address = %p\n", it->first, &it->second);
    
    clients_it cli = this->_clients.find(fd);
    if (cli == _clients.end())
        return NULL;
    return &(cli->second);
}

/* -------------------------- ADD, REMOVE ------------------------------- */

void    Server::AddClient(int fd)
{
    Client cli(fd);
    this->_clients.insert(std::pair<int, Client>(fd, cli));
    // this->_clients.insert(std::map<int, Client>::value_type(fd, cli)); // ca met la _sockFd du client inséré à 0, alors qu'est à 4 dans cli, pourquoi ? 
    // this->_clients[fd] = cli; // pourquoi donne une erreur de compilation ?
}

void    Server::RemoveClient(int fd)
{
    clients_it it = _clients.find(fd);
    if (it == _clients.end())
        ;// error msg
    _clients.erase(it);
    // _clients.erase(fd);
}


void    Server::AddToPoll(int fd, int events)
{
    pollfd newPoll;

    newPoll.fd = fd;
    newPoll.events = events;
    newPoll.revents = 0;
    (this->_sockets).push_back(newPoll);
}

void    Server::RemoveFromPoll(int fd)
{
    for (poll_it it = _sockets.begin(); it != _sockets.end(); it++)
    {
        // printf("DEBUG : removeFromPoll, *it (ie fd) = %d\n", it->fd);
        
        if (it->fd == fd)
        {
            _sockets.erase(it); // vérifier que fait pas de la merde
            // close(fd);
            break;
        }
    }
}

void    Server::RemoveClientFromAll(Client *client)
{
    int client_fd = client->getSockFd();
    this->RemoveClient(client_fd);
    this->RemoveFromPoll(client_fd);
    close(client_fd); // verifier retour de close
}


/* -------------------------- EVENTS ------------------------------- */

// todo : gerer les commandes PASS, NICK et USER. 
// Ou alors crée deja la socket serveur-client, puis voit la première commande entrée ? 
void    Server::AcceptClientConnection(void)
{
    struct sockaddr_in  client_addr;
    socklen_t           addr_size = sizeof(client_addr);

    // printf("DEBUG : in AcceptClientConnection()\n");
    int newClient = accept(_server_socket, (struct sockaddr *)&client_addr, &addr_size);
    if (newClient == -1)
        ;
    
    // la rend non bloquante, car accept() est une fonction bloquante 
    if (fcntl(newClient, F_SETFL, O_NONBLOCK))
    {
        perror("fcntl : ");
        close(newClient);
    }
    
    AddToPoll(newClient, POLL_IN | POLL_OUT);
    AddClient(newClient);
    
    printf("New connection! Socket fd: %d, client fd: %d\n", _server_socket, newClient);
}


// avec nc, deux manieres d envoier de la data : avec ctrl+d et la touche enter. pour continuer a lire jusqu a avoir la commande complete, recv() jusqu a lire un '\n\r', ou EOF 
void    Server::ReadData(int fd)
{
    Client *client = getClient(fd); 
    if (!client)
        ; // fd pas dans la liste des server-clients sockets sur ecoute
    // printf("in ReadData(), fd = %d\n", client->getSockFd());

    char        buffer[BUFSIZ];
    std::string msg;
    memset(&buffer, 0, BUFSIZ);

    int bytes_read = recv(client->getSockFd(), buffer, BUFSIZ, 0);
    if (bytes_read == -1)
    {
        RemoveClientFromAll(client);
        ; // throw exception
    }
    if (!bytes_read) // EOF, ie closed connection on the other side (shutdown)
    {
        // que faire dans le cas ou a juste envoyé un buffer vide ?
        
        printf("DEBUG : in ReadData(), recv() returned 0\n");
        RemoveClientFromAll(client);
        // close(client->getSockFd());
        // RemoveFromPoll(client->getSockFd());
        // RemoveClient(client->getSockFd());
    }
    else
    {
        // while (bytes_read > 0) // si jamais BUFSIZ n etait pas assez grand
        // {
        //     msg += buffer;
        //     memset(&buffer, 0, BUFSIZ);
        //     bytes_read = recv(client->getSockFd(), buffer, BUFSIZ, 0);
        // }
        // if (bytes_read == -1)
        //     ; // throw exception
        msg += buffer;
        
        
        client->writeToReadBuffer(buffer);
        size_t pos = (client->getReadBuffer()).find_first_of("\r\n");
        if (pos == std::string::npos)
            return ;
                
        
        printf("[Server] Data received from %d : --- %s --- \n", client->getSockFd(), client->getReadBuffer().c_str());
        
        send(client->getSockFd(),client->getReadBuffer().c_str(), client->getReadBuffer().size(), 0);
        client->clearReadBuffer();
    }
}

