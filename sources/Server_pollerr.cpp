/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_pollerr.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 13:26:30 by Helene            #+#    #+#             */
/*   Updated: 2024/10/04 15:05:13 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void    Server::HandlePollErr(int fd)
{
    int errNum = errno;
    std::string reason = std::strerror(errNum);
    if (fd == this->_server_socket)
    {
        this->_log(ERROR, "Error on server socket : " + reason);
        serverShutdown = true;
        return ;
    }
    std::stringstream ss;
    ss << fd;
    this->_log(ERROR, "Error on socket " + ss.str() + ": " + std::string(std::strerror(errNum)));
    
    DisconnectClient(getClient(fd), reason); 
    RemoveClient(getClient(fd)); // remove client from server here or after ?
}