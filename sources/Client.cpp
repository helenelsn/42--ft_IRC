/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:46 by Helene            #+#    #+#             */
/*   Updated: 2024/09/28 20:47:52 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(int fd, Server *server)
: _sockFd(fd), _state(disconnected & unregistered), _server(server)
{
    printf("Client constructor, _server address : %p\n", &(*(this->_server)));
}

Client::~Client()
{
    //close(_sockFd);
}

Client::Client(Client const& other)
{
    *this = other;
}

Client& Client::operator=(Client const& other) 
{
    if (this == &other) // operator== a coder  
        return *this;
    
    this->_sockFd = other._sockFd;
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

