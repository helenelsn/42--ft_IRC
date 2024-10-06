/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 18:05:03 by Helene            #+#    #+#             */
/*   Updated: 2024/10/06 19:17:23 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

static std::string  getPrefix(Client &client, Channel &channel)
{
    std::string prefix;
    if (channel.isFounder(client.getNickname()))
        prefix = "~";
    else if (channel.isOperator(client.getNickname()))
        prefix = "@";
    else // halfops are not implemented here
        prefix = ""; 
}

static void    joinRpl(Client &client, Channel &channel)
{
    // A JOIN message with the client as the message <source> and the channel they have 
    //  joined as the first parameter of the message.
    client.addToWriteBuffer(client.getUserID() + " JOIN " + channel.getName() + CRLF);
    
    if (!channel.getTopic().empty())
        client.addToWriteBuffer(RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic()));
    
    std::stringstream ss;
    std::string prefix;
    ss << RPL_NAMREPLY(client.getNickname(), "=", channel.getName());
    for (std::map<std::string, Client>::iterator it = channel.getAllMembers().begin(); it != channel.getAllMembers().end(); it++)
    {
        prefix = getPrefix(client, channel);
        if (it != channel.getAllMembers().begin())
            ss << " ";
        ss << prefix << client.getNickname();
    }
    ss << CRLF;
    client.addToWriteBuffer(RPL_ENDOFNAMES(client.getNickname(), channel.getName()));
}

/*
Check : Does the channel exist 
        -> If not, creates it. the client is now the channel operator
        -> If it does :
            Is it an invite only channel
            Does this channel require a password
            Is the channel's user's limit reached yet ?
*/
void    joinChannel(CommandContext &ctx, std::string const& channelName, std::string const& key)
{
    Channel channel; 
    try {
        channel = ctx._server.getChannel(channelName); // peut bien modifier le channel via cette variable?
    }
    catch (std::exception &e) { // channel does not exist -> create it
        Channel newChannel(channelName, ctx._client);
        ctx._server.addChannel(newChannel, channelName);
        joinRpl(ctx._client, newChannel);
        return ;
    }
    
    if (channel.isInvited(ctx._client.getNickname()))
        channel.addInvitedUser(ctx._client);
    else if (channel.isFull())
        ctx._client.addToWriteBuffer(ERR_CHANNELISFULL(ctx._client.getNickname(), channelName));
    else if (channel.getInviteOnlyMode() && channel.isInvited(ctx._client.getNickname()))
        ctx._client.addToWriteBuffer(ERR_INVITEONLYCHAN(ctx._client.getNickname(), channelName));
    else if (channel.getPasswordMode() && (key.empty() || key != channel.getPassword()))
        ctx._client.addToWriteBuffer(ERR_BADCHANNELKEY(ctx._client.getNickname(), channelName));
    else
    {
        channel.addMember(ctx._client);
        joinRpl(ctx._client, channel);
        channel.sendToAll(ctx._client, ctx._client.getUserID() + " JOIN " + channel.getName());
    }
}

void    parseParameters(std::vector<std::string> &params, std::vector<std::string> &channels,
        std::vector<std::string> &keys)
{
    for (size_t i = 0; i < params.size(); i++)
    {
        if (params[i][0] == '#') // '&' as well ?
            channels.push_back(params[i]);
        else
            keys.push_back(params[i]);
    }
}

void    cmdJoin(CommandContext &ctx)
{
    std::vector<std::string> params = ctx._parameters;
    if (params.empty() || params.size() < 1)
    {
        ctx._client.addToWriteBuffer(ERR_NEEDMOREPARAMS(ctx._client.getNickname(), ctx.getCommand()));
        return ;
    }

    std::vector<std::string> channels;
    std::vector<std::string> keys;
    parseParameters(params, channels, keys);
    size_t j = 0;
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (j < keys.size())
        {
            joinChannel(ctx, channels[i], keys[j]); // checker si une erreur pour l'un des channels fait que l'on arrete de checker pour les suivants
            j++;   
        }
        else
            joinChannel(ctx, channels[i], "");
    }
}