/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:49 by Helene            #+#    #+#             */
/*   Updated: 2024/09/29 15:48:52 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

/* -------------------------- CONSTRUCTORS, DESTRUCTOR ------------------------------- */

Server::Server(std::string const& port, std::string const& password)
: _port(port), _password(password)
{
    this->_logger.log(INFO, "Server created");
    std::stringstream ss;
    ss << &(*this);
    this->_logger.log(INFO, "Server address : " + ss.str());
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
            close(_server_socket);
            // throw an exception
    
    // makes the socker passive
    if (listen(_server_socket, BACKLOG))
        close(_server_socket);
        // throw an exception
    
    pollfd newPoll;

    newPoll.fd = _server_socket;
    newPoll.events = POLL_IN;
    newPoll.revents = 0;
    (this->_sockets).push_back(newPoll);
    
    // AddToPoll(_server_socket, POLL_IN);

    freeaddrinfo(res);

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
                // throw IOException(poll, errNum); ?
            break;
        }
                
        //for (poll_it it = _sockets.begin(); it != _sockets.end() && !serverShutdown; it++) // ca couille avec les iterateurs, regarder pourquoi 
        for (size_t i = 0; i < _sockets.size() && !serverShutdown; i++)
        {            
            if (_sockets[i].revents & POLL_ERR)
                ;
            else if (_sockets[i].revents & POLL_IN) // data is ready to recv() on this socket.
            {
                if (_sockets[i].fd == _server_socket)
                    AcceptClientConnection();
                else
                    ReadData(_sockets[i].fd);
            }
            else if (_sockets[i].revents & POLL_OUT) // we can send() data to this socket without blocking.
                ; // send() le writeBuffer du client associé à la socket 
            else if (_sockets[i].revents & POLLHUP) 
                ;
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

/* -------------------------- ADD, REMOVE ------------------------------- */


void    Server::AddClient(int fd)
{
    pollfd newClient;

    newClient.fd = fd;
    newClient.events = POLL_IN | POLL_OUT;
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
        ;// error msg
    _clients.erase(it);

    for (poll_it it = _sockets.begin(); it != _sockets.end(); it++)
    {        
        if (it->fd == client_fd)
        {
            _sockets.erase(it);
            break;
        }
    }
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
    {
        int errNum = errno;
        this->_logger.log(INFO, std::string("Could not accept connection : ") + std::strerror(errNum));
    }
    
    // la rend non bloquante, car accept() est une fonction bloquante 
    if (fcntl(newClient, F_SETFL, O_NONBLOCK))
    {
        perror("fcntl : ");
        close(newClient);
    }
    
    // AddToPoll(newClient, POLL_IN | POLL_OUT);
    AddClient(newClient);
    
    std::stringstream ss;
    ss << newClient;
    this->_logger.log(INFO, "New connection! Client-server socket : " + ss.str());
}


void    Server::ReadData(int fd)
{
    Client *client = getClient(fd); 
    if (!client)
        ; // fd pas dans la liste des server-clients sockets sur ecoute

    char        buffer[BUFSIZ];
    std::string msg;
    memset(&buffer, 0, BUFSIZ);

    int bytes_read = recv(client->getSockFd(), buffer, BUFSIZ, 0);
    if (bytes_read == -1)
    {
        std::stringstream ss;
        ss << client->getSockFd();
        this->_logger.log(INFO, "Client " + ss.str() + " : recv() failed");
        RemoveClient(client); // ?
        ; // throw exception ?
    }
    else if (!bytes_read) // EOF, ie closed connection on the other side
    {
        std::stringstream ss;
        ss << client->getSockFd();
        this->_logger.log(DEBUG, "Client " + ss.str() + " disconnected"); 
        RemoveClient(client);
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
        client->addToReadBuffer(std::string(&buffer[0], &buffer[bytes_read]));
        
        // Gestion Ctrl+D
        size_t pos = (client->getReadBuffer()).find("\r\n");
        if (pos == std::string::npos)
            return ;
        
        std::stringstream ss;
        ss << client->getSockFd();
        this->_logger.log(DEBUG, "[Server] Data received from " + ss.str() + " : ---" + client->getReadBuffer() + "---");
        //send(client->getSockFd(),client->getReadBuffer().c_str(), client->getReadBuffer().size(), 0);

        ProcessBuffer(client); // ProcessBuffer : Client or Server method ? Dans tous les cas, a besoin d'avoir acces au client         
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

void    Server::ParseLine(std::string line, CommandContext &ctx)
{
    // std::string temp;
    std::string prefix;
    std::string command;
    std::vector<std::string> parameters;

    std::string::iterator begin = line.begin();
    std::string::iterator it;

    // if prefix
    if (line[0] == ':')
    {
        it = std::find(begin, line.end(), ' ');
        prefix = std::string(begin, it);
        begin = it;
        if (it != line.end()) 
            begin++;
    }

    // find command
    it = std::find(begin, line.end(), ' ');
    command = std::string(begin, it);
    // update iterator
    begin = it;
    if (it != line.end())
        begin++;
    
    this->_logger.log(DEBUG, "prefix = " + prefix);
    this->_logger.log(DEBUG, "command = " + command);
    

    // put parameters in vector of strings
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

    ctx.fillCommand(prefix, command, parameters);
    
    // logger
    for (size_t i = 0; i < parameters.size(); i++)
    {
        std::stringstream ss;
        ss << i;
        this->_logger.log(DEBUG, "parameter " + ss.str() + " = " + parameters[i]);
    }
}

// a faire dans CommandsHandler directement plutôt, non ?
void Server::ProcessCommand(std::string const& line, Client* &client) // Client* &client ?? verifier syntaxe, et si comprend bien ce qu'a écrit 
{
    CommandContext ctx(*client); // est ce que le client pourra ensuite bien etre modifié via CommandContext ? Ou faut il passer un pointeur ? Pas encore tres au clair sur l'utilisation references/pointeurs 

    // extract prefix, command and command parameters from the line sent by client. update command's context ctx
    ParseLine(line, ctx);
    
    // enlever les ' ' en trop, ou considère que ça en fait une commande inconnue si il y en a ? -> tester le comportement avec irssi et un serveur existant
    std::string cmd = ctx.getCommand();
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);
    // peut utiliser std::transform directement sur ctx.cmd, suffit juste de renvoyer une référence pour getCommand(), et non pas une copie de cmd
    ctx.setCommand(cmd);
    
    std::map<std::string, CommandExecutor>::iterator it = _commandsHandler._commands.find(cmd);
    if (it == _commandsHandler._commands.end())
        ; // unknown command
    
    this->_logger.log(DEBUG, "Processing command <" + cmd + ">");
    if (it->second)
        it->second(ctx);
    
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
void    Server::ProcessBuffer(Client* &client) 
{
    std::string::iterator it;
    std::string updatedBuffer;
    size_t pos;
    
    pos = client->getReadBuffer().find(CRLF); 
    while (pos != std::string::npos)
    {
        it = client->getReadBuffer().begin();
        std::string test = std::string(it, it + pos);

        this->_logger.log(DEBUG, "current parsed command : " + std::string(it, it + pos));
        
        ProcessCommand(std::string(it, it + pos), client);
        // ParseLine(std::string(it, it + pos));
        
        updatedBuffer = client->getReadBuffer().substr(pos + 2);
        client->resetReadBuffer(updatedBuffer);
        pos = client->getReadBuffer().find(CRLF);
    }
}




// à coder
void    Server::RestartServer() 
{
    this->_logger.log(INFO, "Restarting server");
}


void    Server::ShutdownServer() 
{
    for (poll_it it = _sockets.begin(); it != _sockets.end(); it++)
        close(it->fd);

    // delete clients (autre chose a faire que juste close les sockets ?)

    // delete channels -> is there anything to do about that here ?
    
    this->_logger.log(INFO, "Shutting down server");
}