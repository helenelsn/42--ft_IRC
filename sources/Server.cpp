/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:49 by Helene            #+#    #+#             */
/*   Updated: 2024/09/25 22:20:36 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

/* -------------------------- CONSTRUCTORS, DESTRUCTOR ------------------------------- */

Server::Server(std::string const& port, std::string const& password)
: _port(port), _password(password)
{
    this->_logger.log(INFO, "Server created");
}

Server::~Server()
{
    // close(_server_socket);
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
    
    this->_logger.log(INFO, "Connection socket created");

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

    this->_logger.log(INFO, "Server listening on port " + this->_port + ", with socketFd " + std::to_string(_server_socket));
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
                // throw IOException(poll, errNum); ?
            break;
        }
                
        //for (poll_it it = _sockets.begin(); it != _sockets.end() && !serverShutdown; it++) // ca couille avec les iterateurs, regarder pourquoi 
        for (size_t i = 0; i < _sockets.size() && !serverShutdown; i++)
        {            
            // Peut avoir (revents & POLL_IN) && (revents & POLL_OUT) ? Tout doit passer par le serveur, peut partir du principe que doit etre sequentiel ? 
            if (_sockets[i].revents & POLL_ERR)
                ;
            else if (_sockets[i].revents & POLL_IN) // data is ready to recv() on this socket.
            {
                if (_sockets[i].fd == _server_socket) // server side
                    AcceptClientConnection();
                else
                    ReadData(_sockets[i].fd); // client side
            }
            else if (_sockets[i].revents & POLL_OUT) // we can send() data to this socket without blocking.
                ; // send() le writeBuffer du client associé à la socket 
            else if (_sockets[i].revents & POLLHUP) 
                ;
        }
    }
    
    // Fermer tous les fd 
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


/* -------------------------- POLL REVENTS ------------------------------- */

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
    
    this->_logger.log(INFO, "New connection! Client-server socket : " + std::to_string(newClient));
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
        this->_logger.log(INFO, "Client " + std::to_string(client->getSockFd()) + " exited the server");
        RemoveClientFromAll(client);
        ; // throw exception
    }
    if (!bytes_read) // EOF, ie closed connection on the other side (shutdown)
    {
        this->_logger.log(DEBUG, "in ReadData(), recv() returned 0");
        RemoveClientFromAll(client);
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
        
        // check limite des 512 caractères ou balec ?
        client->writeToReadBuffer(std::string(&buffer[0], &buffer[bytes_read]));
        
        // Gestion Ctrl+D
        size_t pos = (client->getReadBuffer()).find_first_of("\r\n");
        if (pos == std::string::npos)
            return ;
                
        // this->_logger.log(DEBUG, "[Server] Data received from " + std::to_string(client->getSockFd()) + " : ---" + client->getReadBuffer() + "---");

        // renvoie pour l'instant le message dans son intégralité à l'envoyeur, pour vérifier l'intégrité
        send(client->getSockFd(),client->getReadBuffer().c_str(), client->getReadBuffer().size(), 0);



        ParseBuffer(client); // ParseBuffer : Client or Server method ? Dans tous les cas, a besoin d'avoir acces au client 

        //client->clearReadBuffer();
        
    }
}



/*  A message contains at least two parts: the command and the command parameters. There may be at most 15 parameters. 
    The command and the parameters are all separated by a single ASCII space character. 
    
    Some messages also include a prefix before the command and the command parameters. 
    The presence of a prefix is indicated with a single leading colon character. 
    The prefix is used to indicate the origin of the message. 
    For example, when a user sends a message to a channel, the server will forward that message to all the users in the channel,
    and will include a prefix to specify the user that sent that message originally.

    When the last parameter is prefixed with a colon character,
    the value of that parameter will be the remainder of the message (including space characters).
    ex -> PRIVMSG rory :Hey Rory...
*/

void    Server::ParseCommand(std::string line)
{
    std::string temp;
    std::string prefix;
    std::string command;
    std::vector<std::string> parameters;

    std::string::iterator begin = line.begin();
    std::string::iterator it;

    
    if (line[0] == ':')
    {
        it = std::find(begin, line.end(), ' ');
        prefix = std::string(begin, it);
        begin = it;
        if (it != line.end()) 
            begin++;
    }

    it = std::find(begin, line.end(), ' ');
    command = std::string(begin, it);

    begin = it;
    if (it != line.end())
        begin++;
    
    this->_logger.log(DEBUG, "prefix = " + prefix);
    this->_logger.log(DEBUG, "command = " + command);
    
    while (it != line.end())
    {
        if ((*begin) == ':')
        {
            parameters.push_back(std::string(begin, line.end()));
            break;
        }
        it = std::find(begin, line.end(), ' ');
        parameters.push_back(std::string(begin, it));
        begin = it;
        if (it != line.end())
            begin++;
    }
    
    for (size_t i = 0; i < parameters.size(); i++)
        this->_logger.log(DEBUG, "parameter " + std::to_string(i) + " = " + parameters[i]);

}


/* 
si la socket etait bloquée, peut avoir écrit plusieurs messages à la suite qui doivent etre traités en meme temps
 -> split selon les "\r\n"
 -> s'il reste des trucs après le dernier CRLF, les laisser dans le ReadBuffer du client (il ne restera donc à ce jour
    dedans plus que le début de message ne finissant pas encore par un CRLF, le reste aura été éffacé)
->  pour chaque message reçu, le parser et éxecuter la commande correspondante 
 */
/*
Reflechir a la structure du code : Où faire le parsing ? 
    -> méthode serveur : doit alors écrire une méthode Client qui permette de modifier son ReadBuffer
    -> méthode client : pour chaque commande extraite, devra repasser par le Serveur pour l'executer (avoir un pointeur vers le Serveur pour chaque client ?)
*/
void    Server::ParseBuffer(Client* &client) 
{
    std::string::iterator it;
    std::string updatedBuffer;
    size_t pos;
    
    pos = client->getReadBuffer().find_first_of(CRLF); 
    while (pos != std::string::npos)
    {
        it = client->getReadBuffer().begin();
        ParseCommand(std::string(it, it + pos));
        
        updatedBuffer = client->getReadBuffer().substr(pos + 1);
        client->rewriteBuffer(updatedBuffer);
        pos = client->getReadBuffer().find_first_of(CRLF);
    }
}