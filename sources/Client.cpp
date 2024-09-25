/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:46 by Helene            #+#    #+#             */
/*   Updated: 2024/09/25 21:35:19 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(int fd)
: _sockFd(fd)
{
    
}

Client::~Client()
{
    //close(_sockFd);
}

Client Client::operator==(Client const& other) 
{
    if (this == &other) // a verif 
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

int Client::getSockFd(void)
{
    return this->_sockFd;
}

std::string Client::getReadBuffer(void)
{
    return this->_readBuffer;
}

void    Client::writeToReadBuffer(std::string data)
{
    _readBuffer += data; 
}

void    Client::clearReadBuffer(void)
{
    _readBuffer.clear();
}

void    Client::rewriteBuffer(std::string const& newBuffer)
{
    _readBuffer = newBuffer;
}
