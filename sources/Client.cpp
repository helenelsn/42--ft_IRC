/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:46 by Helene            #+#    #+#             */
/*   Updated: 2024/09/30 13:50:04 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(int fd, Server *server)
: _sockFd(fd), _state(Unregistered), _server(server)
{
    printf("Client constructor, _server address : %p\n", &(*(this->_server)));
}

Client::~Client()
{
    //close(_sockFd);
}

bool Client::operator!=(Client const& other)
{
    return !(*this == other);
}

Client::Client(Client const& other)
{
    *this = other;
}

Client& Client::operator=(Client const& other) 
{
    if (this == &other) // operator== a coder  
        return *this;
    
    this->_server = other._server;
    this->_sockFd = other._sockFd;
    this->_state = other._state;
    this->_modes = other._modes;
    
    this->_hostname = other._hostname;
    this->_nickname = other._nickname;
    this->_realname = other._realname;
    this->_username = other._username;
    
    this->_readBuffer = other._readBuffer;
    this->_writeBuffer = other._writeBuffer;
    
    return *this;
}

bool Client::operator==(Client const& other)
{
    return (this->_nickname == other._nickname); // autre chose ?
}

// ou juste int& getState(), renvoie une reference au state que peut direct modifier, et pas besoin de setState() ?
int             Client::getState(void) const
{
    return _state;
}

void            Client::setState(int newState)
{
    _state = newState;
}

Server& Client::getServer()
{
    return *(this->_server); // vérifier
}

int Client::getSockFd(void)
{
    return this->_sockFd;
}

std::string Client::getNick(void) const
{
    return this->_nickname;
}

std::string& Client::getReadBuffer(void)
{
    return this->_readBuffer;
}

void    Client::addToReadBuffer(std::string const &data)
{
    _readBuffer += data; 
}

void    Client::clearReadBuffer(void)
{
    _readBuffer.clear();
}

void    Client::resetReadBuffer(std::string const& newBuffer)
{
    _readBuffer = newBuffer;
}

void            Client::addToWriteBuffer(std::string const& data)
{
    _writeBuffer += data;
}

std::string&    Client::getWriteBuffer(void)
{
    return _writeBuffer;
}

void            Client::clearWriteBuffer(void)
{
    _writeBuffer.clear();
}

