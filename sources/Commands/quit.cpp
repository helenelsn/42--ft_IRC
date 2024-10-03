/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:36:02 by Helene            #+#    #+#             */
/*   Updated: 2024/10/03 19:15:54 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

/*
When connections are terminated by a client-sent QUIT command, 
servers SHOULD prepend <reason> with the ASCII string "Quit: " when sending QUIT messages 
to other clients, to represent that this user terminated the connection themselves. 
This applies even if <reason> is empty, in which case the reason sent to other clients 
SHOULD be just this "Quit: " string.
*/
void    cmdQuit(CommandContext &ctx)
{
    std::string reason = ctx._parameters.empty() ? "" : ctx._parameters[0];
    ctx._client.addToWriteBuffer("QUIT :" + reason + CRLF);
    // sthg else ?
    
    // car peut pas le supprimer tout de suite, doit pouvoir envoyer le rpl serveur au client (ou l envoie pas au clinet qui se casse ?)
    ctx._client.setState(Disconnected); 
    
    // ctx._server.RemoveClient(&ctx._client);
}