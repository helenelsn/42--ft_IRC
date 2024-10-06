/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 15:26:00 by Helene            #+#    #+#             */
/*   Updated: 2024/10/06 21:13:17 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

/* 
Parameters: <channel>{,<channel>} [<reason>]

This message may be sent from a server to a client to notify the client that someone 
has been removed from a channel. 
In this case, the message <source> will be the client who is being removed, 
and <channel> will be the channel which that client has been removed from.
*/
void    cmdPart(CommandContext &ctx)
{
    if (ctx._parameters.empty())
    {
        ctx._client.addToWriteBuffer(ERR_NEEDMOREPARAMS(ctx._client.getNickname(), ctx._command));
        return ;
    } 
    
    Channel channel;
    std::string channelName;
    std::stringstream ss(ctx._parameters[0]);
    std::string reason = (ctx._parameters.size() >= 2) ? ctx._parameters[1] : "";
    
    ss << ','; // ca marche pour la suite ça ?
    getline(ss, channelName, ','); // car ss n'est d'office pas vide
    do
    {
        channelName = channelName.substr(1); // removes the '#' or '&' caracter
        if (!ctx._server.channelExists(channelName))
            ctx._client.addToWriteBuffer(ERR_NOSUCHCHANNEL(ctx._client.getNickname(), channelName)); // add the '#' before the channel's name ?
        else
        {
            channel = ctx._server.getChannel(channelName);
            ss << ctx._client.getUserID() << " PART #" << channelName << " :" << reason << CRLF;
            channel.sendToAll(ctx._client, ss.str());
        }
    }
    while (getline(ss, channelName, ',') && !ss.eof());
    
}