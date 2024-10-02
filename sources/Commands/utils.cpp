/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:58:41 by Helene            #+#    #+#             */
/*   Updated: 2024/10/02 14:16:18 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

/*
If the password supplied does not match the password expected by the server,
then the server SHOULD send ERR_PASSWDMISMATCH (464) and MAY then close the connection with ERROR. 
Servers MUST send at least one of these two messages.
*/
void    Server::tryLogin(Client &client)
{
    // check for passwd validity
    if (this->getPasswd() != client.getPassword())
    {
        client.addToWriteBuffer(ERR_PASSWDMISMATCH(client.getNickname()));
        // Peut décider de fermer la connection avec ERROR
        return ;
    }
    client.addToWriteBuffer(RPL_WELCOME(client.getNickname(), client.getUsername(), client.getHostname()));
    client.addToWriteBuffer(RPL_YOURHOST(std::string(SERVER_NAME)));
    client.addToWriteBuffer(RPL_CREATED(this->getCreationDate()));
    client.addToWriteBuffer(RPL_MYINFO(std::string(SERVER_NAME), std::string(USER_MODES), std::string(CHANNEL_MODES)));
    this->sendMotd(client);
    
    // else : registering completed
    /* 
    RPL_WELCOME
    RPL_YOURHOST
    RPL_CREATED
    RPL_MYINFO
    RPL_MOTDSTART // first line of MOTD
    RPL_MOTD // MOTD line by line
    ...
    RPL_MOTD // last line of MOTD
    RPL_ENDOFMOTD
    */
}