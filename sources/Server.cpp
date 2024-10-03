/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:49 by Helene            #+#    #+#             */
/*   Updated: 2024/10/03 17:18:36 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

/* -------------------------- CONSTRUCTORS, DESTRUCTOR ------------------------------- */

void    Server::setCreationDate(void)
{
    time_t time = std::time(0);
    this->_creationDate = std::string(ctime(&time));
}

Server::Server(std::string const& port, std::string const& password)
: _port(port), _password(password)
{
    this->setCreationDate();
    
    this->_logger.log(INFO, "Server created");
    std::stringstream ss;
    ss << &(*this);
    this->_logger.log(INFO, "Server address : " + ss.str());
}

Server::~Server()
{
    // close(_server_socket);
    // ou le fait direct dans shutdownServer() ?
}

Server::Server(Server const& other)
{
    *this = other;
}

Server& Server::operator=(Server const& other)
{
    (void)other; // ou se fait chier a tt code ?
    return *this;
}




/* -------------------------- INIT SERVER ------------------------------- */

void    Server::InitServer(void)
{
    
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // localhost by default
    if (getaddrinfo(NULL, (this->_port).c_str(), &hints, &res))
        throw(std::runtime_error("getaddrinfo() call failed"));
    _server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    
    this->_logger.log(INFO, "Connection socket created");

    int en = 1;
    // The setsockopt() API allows the application to reuse the local address when the server is restarted before the required wait time expires.
	if(setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on server socket"));
	
    // The fcntl() API sets the socket to be nonblocking
    if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on server socket"));
    
    // The bind() API gets a unique name for the socket.
    if (bind(_server_socket, res->ai_addr, res->ai_addrlen))
    {
        close(_server_socket);
        throw(std::runtime_error("failed to bind server socket"));
    }
    
    // makes the socker passive
    if (listen(_server_socket, BACKLOG))
    {
        close(_server_socket);
        throw(std::runtime_error("call to listen() on socket server failed"));
    }
    
    pollfd newPoll;

    newPoll.fd = _server_socket;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    (this->_sockets).push_back(newPoll);
    
    freeaddrinfo(res);

    // debug
    std::stringstream ss;
    ss << _server_socket;
    this->_logger.log(INFO, "Server listening on port " + this->_port + ", with socketFd " + ss.str());
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
    this->_logger.log(DEBUG, "In RunServer()");
    while (!serverShutdown)
    {
        // The poll() API allows the process to wait for an event to occur and to wake up the process when the event occurs.
        if (poll(&this->_sockets[0], _sockets.size(), -1) == -1)
        {
            int errNum = errno;
            if (errNum != EINTR) // EINTR : A signal occurred before any requested event
                throw(std::runtime_error("poll() failed"));
            break;
        }
                
        for (size_t i = 0; i < _sockets.size() && !serverShutdown; i++)
        {            
            if (_sockets[i].revents & POLLIN) // data is ready to recv() on this socket.
            {
                if (_sockets[i].fd == _server_socket)
                    AcceptClientConnection();
                else
                    ReadData(_sockets[i].fd);
            }
            else if (_sockets[i].revents & POLLOUT) // we can send() data to this socket without blocking.
                SendWriteBuffer(_sockets[i].fd);
            else if (_sockets[i].revents & POLL_ERR)
            {
                ; // a coder
            }
            // else if (_sockets[i].revents & POLLHUP) 
                // ;
        }
    }
    
    // ShutdownServer();
}



/* -------------------------- GETTERS, SETTERS ------------------------------- */

// quel type renvoyer ? pointeur, iterateur, ... ?
Client              *Server::getClient(int fd)
{
    clients_it cli = this->_clients.find(fd);
    if (cli == _clients.end())
        return NULL;
    return &(cli->second);
}

std::string     Server::getCreationDate(void)
{
    return _creationDate;
}

/* -------------------------- ADD, REMOVE CLIENT ------------------------------- */

void    Server::AddClient(int fd)
{
    pollfd newClient;

    newClient.fd = fd;
    newClient.events = POLLIN | POLLOUT;
    newClient.revents = 0;
    (this->_sockets).push_back(newClient);
    
    Client cli(fd, this);
    // this->_clients[fd] = cli; // erreur de compilation liée aux constructeurs, pourquoi ?
    
    this->_clients.insert(std::pair<int, Client>(fd, cli));

}

void    Server::RemoveClient(Client *client)
{
    int client_fd = client->getSockFd();
    clients_it it = _clients.find(client_fd);
    
    if (it == _clients.end())
        return ; // client does not exist
    _clients.erase(it);

    for (poll_it it = _sockets.begin(); it != _sockets.end(); it++)
    {        
        if (it->fd == client_fd)
        {
            _sockets.erase(it);
            break;
        }
    }
    if (close(client_fd) == -1)
        std::perror("close() :");
}




/* -------------------------- COMMANDS' ADDITIONAL METHODS ------------------------------- */

bool    Server::NickAlreadyUsed(std::string const& newNick)
{
    for (clients_it it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if (it->second.getNickname() == newNick) // check si it->seconf.getNickname().empty() ?
            return true;
    }
    return false;
}



/* -------------------------- Restart and Shutdown ------------------------------- */

// à coder
void    Server::RestartServer() 
{
    this->_logger.log(INFO, "Restarting server");
}


void    Server::ShutdownServer() 
{
    this->_logger.log(INFO, "Shutting down server");
    
    for (poll_it it = _sockets.begin(); it != _sockets.end(); it++)
        close(it->fd);

    // delete clients (autre chose a faire que juste close les sockets ?)

    // delete channels -> is there anything to do about that here ?
    
}