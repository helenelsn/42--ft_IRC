/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandsHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:51:07 by Helene            #+#    #+#             */
/*   Updated: 2024/09/30 13:25:10 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CommandsHandler.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

CommandContext::CommandContext(std::string const& prefix, std::string const& cmd, std::vector<std::string> const& parameters, Client &client)
: _prefix(prefix), _command(cmd), _parameters(parameters), _client(client), _server(client.getServer())
{
    // add the server as class attribute as well, or will be another way to have access to it ? 
    // can access it through the client, but easier if is a class attribute as well (or not ?)
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
    _commands["JOIN"] = &cmdJoin;
    _commands["PASS"] = &cmdPass;
    _commands["USER"] = &cmdUser;
    _commands["NICK"] = &cmdNick;
}

void    CommandsHandler::execCommand(std::string const& name)
{
    (void) name;
}
