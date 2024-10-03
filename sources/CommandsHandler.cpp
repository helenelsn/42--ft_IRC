/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandsHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:51:07 by Helene            #+#    #+#             */
/*   Updated: 2024/10/03 17:10:27 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CommandsHandler.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

CommandContext::CommandContext(std::string const& prefix, std::string const& cmd, std::vector<std::string> const& parameters, Client &client)
: _prefix(prefix), _command(cmd), _parameters(parameters), _client(client), _server(client.getServer())
{
    
}

CommandContext::CommandContext(Client &client)
: _client(client), _server(client.getServer())
{
    
}

void    CommandContext::fillCommand(std::string const& prefix, std::string const& cmd, std::vector<std::string> const& parameters)
{
    _prefix = prefix;
    _command = cmd;
    _parameters = parameters;
}

void    CommandContext::setCommand(std::string const& name)
{
    _command = name;
}

std::string     CommandContext::getCommand(void) const
{
    return _command;
}

CommandsHandler::CommandsHandler()
{
    _commands["CAP"] = NULL;
    _commands["PASS"] = &cmdPass;
    _commands["USER"] = &cmdUser;
    _commands["NICK"] = &cmdNick;
    _commands["MOTD"] = &cmdMotd;
    _commands["QUIT"] = &cmdQuit;
    _commands["PING"] = &cmdPing;
    _commands["JOIN"] = &cmdJoin;
}

void    CommandsHandler::execCommand(std::string const& name)
{
    (void) name;
}
