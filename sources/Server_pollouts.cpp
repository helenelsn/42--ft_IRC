/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_pollouts.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:35:43 by hlesny            #+#    #+#             */
/*   Updated: 2024/10/03 15:36:03 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void    Server::SendWriteBuffer(int fd)
{
    Client  *client;
    
    client = this->getClient(fd);
    if (client == NULL) // client does not exist 
        ; // which exception

    if (client->getWriteBuffer().empty())
        return ;
    
    int bytes_sent = send(fd, client->getWriteBuffer().c_str(), client->getWriteBuffer().size(), 0);

    if (bytes_sent == -1)
        ;
    else
    {
        std::stringstream ss;
        ss << client->getSockFd();
        _logger.log(DEBUG, "<Client " + ss.str() + "><SEND> " + client->getWriteBuffer());
    }
    
    client->clearWriteBuffer(); 

    // check if client is disconnected ? or is there another place to do it ?
}