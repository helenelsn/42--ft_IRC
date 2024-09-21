/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:49 by Helene            #+#    #+#             */
/*   Updated: 2024/09/21 15:36:56 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

/* -------------------------- GETTERS, SETTERS ------------------------------- */

// quel type renvoyer ? pointeur, iterateur, ... ?
Client              *Server::getClient(int fd)
{
    // printf("DEBUG : list of current clients in map : \n");
    // for (clients_it it = this->_clients.begin(); it != this->_clients.end(); it++)
    //     printf("\tclient : fd = %d, client address = %p\n", it->first, &it->second);
    
    clients_it cli = this->_clients.find(fd);
    printf("DEBUG : in getClient, fd = %d, foundClient.fd = %d\n", fd, cli->first);
    if (cli == _clients.end())
        return NULL;
    return &(cli->second);
}

/* -------------------------- ADD, REMOVE ------------------------------- */

void    Server::AddClient(int fd)
{
    Client cli(fd);
    this->_clients.insert(std::map<int, Client>::value_type(fd, cli));
    // this->_clients[fd] = cli; // pourquoi donne une erreur de compilation ?
}

void    Server::RemoveClient(int fd)
{
    
}


void    Server::RemoveFromPoll(int fd)
{
    for (poll_it it = _sockets.begin(); it != _sockets.end(); it++)
    {
        // printf("DEBUG : removeFromPoll, *it (ie fd) = %d\n", it->fd);
        
        if (it->fd == fd)
        {
            _sockets.erase(it); // vérifier que fait pas de la merde
            close(fd);
            break;
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

/* -------------------------- INIT ------------------------------- */

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
    // The setsockopt() API allows the application to reuse the local address when the server is restarted before the required wait time expires.
	if(setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on server socket"));
	
    // The fcntl() API sets the socket to be nonblocking
    if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on server socket"));
    
    // The bind() API gets a unique name for the socket.
    if (bind(_server_socket, (struct sockaddr *)&toAdd, sizeof(toAdd)))
            close(_server_socket);// throw an exception
    
    // makes the socker passive
    if (listen(_server_socket, BACKLOG))
        close(_server_socket);// throw an exception
    
    AddToPoll(_server_socket, POLLIN);

    std::cout << "Connection socket listening on port " << this->_port << std::endl;
}



/* -------------------------- EVENTS ------------------------------- */


// todo : gerer les commandes PASS, NICK et USER 
void    Server::AcceptClientConnection(void)
{
    struct sockaddr_in  client_addr;
    socklen_t           addr_size = sizeof(client_addr);

    int newClient = accept(_server_socket, (struct sockaddr *)&client_addr, &addr_size);
    if (newClient == -1)
        ;
    
    // la rend non bloquante, car accept() est une fonction bloquante 
    if (fcntl(newClient, F_SETFL, O_NONBLOCK))
    {
        perror("fcntl : ");
        close(newClient);
    }
        
    AddToPoll(newClient, POLL_IN); // POLL_IN | POLL_OUT ?
    AddClient(newClient);
    
    printf("New connection! Socket fd: %d, client fd: %d\n", _server_socket, newClient);
}


// avec nc, deux manieres d envoier de la data : avec ctrl+d et la touche enter. pour continuer a lire jusqu a avoir la commande complete, recv() jusqu a lire un '\n\r', ou EOF 
void    Server::ReadData(int fd)
{
    Client *client = getClient(fd); //todo : renvoie NULL ici alors que devrait pas, du coup segfault juste apres
    // printf("client from getClient() is NULL : %d\n", client == NULL ? 1 : 0);
    printf("in ReadData(), fd = %d\n", client->getSockFd());

    char        buffer[BUFSIZ];
    std::string msg;
    memset(&buffer, 0, BUFSIZ);

    int bytes_read = recv(client->getSockFd(), buffer, BUFSIZ, 0);
    if (bytes_read == -1)
        ; // throw exception
    if (!bytes_read) // EOF, ie closed connection on the other side (shutdown)
    {
        // que faire dans le cas ou a juste envoyé un buffer vide ?
        close(client->getSockFd());
        RemoveFromPoll(client->getSockFd());
        RemoveClient(client->getSockFd());
    }
    else
    {
        /* while (bytes_read > 0) // si jamais BUFSIZ n etait pas assez grand
        {
            msg += buffer;
            memset(&buffer, 0, BUFSIZ);
            bytes_read = recv(client->getSockFd(), buffer, BUFSIZ, 0);
        }
        if (bytes_read == -1)
            ; // throw exception */
        
        // parse data contained in msg
        
        msg += buffer;

        
        printf("[Server] Data received from %d : --- %s --- \n", client->getSockFd(), buffer);

        // send back to client for now, to check data integrity via sockets communication
        
        send(client->getSockFd(), msg.c_str(), msg.size(), 0);
    }
}



/* -------------------------- MAIN METHOD ------------------------------- */

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
                //printf("POLL_IN event, fd = %d\n", it->fd);
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


/* -------------------------- CONSTRUCTORS, DESTRUCTOR ------------------------------- */

Server::Server(std::string const& port, std::string const& password)
: _port(port), _password(password)
{

    //AcceptClientConnection();

/*
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
    */
    
}

Server::~Server()
{
    close(_server_socket);
}
