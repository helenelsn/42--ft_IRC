/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 18:05:03 by Helene            #+#    #+#             */
/*   Updated: 2024/10/07 23:16:44 by Helene           ###   ########.fr       */
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
    return prefix;
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
    for (std::map<std::string, Client*>::iterator it = channel.getAllMembers().begin(); it != channel.getAllMembers().end(); it++)
    {
        prefix = getPrefix(client, channel);
        if (it != channel.getAllMembers().begin() && *(it->second) != client)
            ss << " "; // sépare chaque client par un espace
        ss << prefix << it->first;
    }
    ss << CRLF;
    client.addToWriteBuffer(ss.str());
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
    Channel *channel = ctx._server.getChannel(channelName);
    if (!channel)
    {
        Channel newChannel(channelName, &ctx._client);
        ctx._server.addChannel(newChannel, channelName);
        joinRpl(ctx._client, newChannel);
        return ;
    }
    
    if (channel->isInvited(ctx._client.getNickname()))
        channel->addInvitedUser(&ctx._client);
    else if (channel->isFull())
        ctx._client.addToWriteBuffer(ERR_CHANNELISFULL(ctx._client.getNickname(), channelName));
    else if (channel->getInviteOnlyMode() && channel->isInvited(ctx._client.getNickname()))
        ctx._client.addToWriteBuffer(ERR_INVITEONLYCHAN(ctx._client.getNickname(), channelName));
    else if (channel->getPasswordMode() && (key.empty() || key != channel->getPassword()))
        ctx._client.addToWriteBuffer(ERR_BADCHANNELKEY(ctx._client.getNickname(), channelName));
    else
    {
        joinRpl(ctx._client, *channel);

        // std::string prefix = ctx._client.getUserID();
        // std::stringstream ss;
        // ss << prefix << " JOIN " << channelName ;
        // for (std::map<std::string, Client>::iterator it = channel->getAllMembers().begin(); it != channel->getAllMembers().end(); it++)
        //     it->second.addToWriteBuffer(ss.str());
            
        channel->addMember(&ctx._client);
        channel->sendToAll(ctx._client, ctx._client.getUserID() + " JOIN " + channel->getName() + CRLF);
    }
}

void    parseParameters(std::vector<std::string> &params, std::vector<std::string> &channels,
        std::vector<std::string> &keys)
{
    // les channels et keys sont séparées par un ','
    // ex : JOIN #foo,&bar fubar 

    std::stringstream ss(params[0]);
    std::string buffer;

    // ss << ',';
    while (getline(ss, buffer, ','))
        channels.push_back(buffer);
    
    if (params.size() >= 2)
    {
        ss.str(params[1]);
        // ss << ',';
        while (getline(ss, buffer, ','))
            keys.push_back(buffer);
    }

}


bool    checkChanMask(std::string const& chanName)
{
    return (!chanName.empty() && chanName[0] == '#' && chanName.size() <= 50
        && (chanName.find_first_of("\r\n\b:, ") == std::string::npos)); // need to implement &channelName as well as #channelName ?
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
        if (!checkChanMask(channels[i]))
        {
            ctx._client.addToWriteBuffer(ERR_BADCHANMASK(channels[i]));
            if (j < keys.size())
                j++;
            continue;
        }
        if (j < keys.size())
        {
            joinChannel(ctx, channels[i], keys[j]); // checker si une erreur pour l'un des channels fait que l'on arrete de checker pour les suivants
            j++;   
        }
        else
            joinChannel(ctx, channels[i], "");
    }
}