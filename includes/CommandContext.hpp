/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:48:43 by Helene            #+#    #+#             */
/*   Updated: 2024/09/28 18:07:01 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"
#include "Client.hpp"

class   CommandContext
{
    private :
        std::string                 _prefix; // utile ?
        std::string                 _command;
        std::vector<std::string>    _parameters;
        Client&                     _client;
        Server&                     _server;
    // quoi d'autre ? Serveur ? Channel, ou est deja stocké dans client ou serveur ? dans client normalement
    
    public :
        CommandContext(Client &client);
        CommandContext(std::string const& prefix, std::string const& cmd, std::vector<std::string> const& parameters, Client &client);
        ~CommandContext() {}
        void            fillCommand(std::string const& prefix, std::string const& cmd, std::vector<std::string> const& parameters);
        void            setCommand(std::string const& name);
        std::string     getCommand(void) const;
};
