/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_pollouts.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:35:43 by hlesny            #+#    #+#             */
/*   Updated: 2024/10/04 15:14:53 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void    Server::SendWriteBuffer(int fd)
{
    Client  *client;

    // check if client disconnected (in case of not removed yet (is it possible ?))
    
    
    client = this->getClient(fd);
    if (client == NULL) // client does not exist 
        return;

    std::string toSend = client->getWriteBuffer();
    if (toSend.empty() || toSend.find(CRLF) == std::string::npos)
        return ;
    
    int bytes_sent = send(fd, toSend.c_str(), toSend.size(), 0);

    if (bytes_sent == -1)
    {
        std::perror("send() :");
        return ;   
    }
    else
    {
        std::stringstream ss;
        ss << client->getSockFd();
        _logger.log(DEBUG, "<Client " + ss.str() + "><SEND> " + toSend);
    }
    client->clearWriteBuffer();

    // check if client is disconnected ? or is there another place to do it ?
    if (client->checkState(Disconnected))
        RemoveClient(client);    
}