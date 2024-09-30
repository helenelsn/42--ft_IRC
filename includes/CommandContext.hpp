/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandContext.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:48:43 by Helene            #+#    #+#             */
/*   Updated: 2024/09/29 18:33:45 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"
// #include "Client.hpp"
// #include "Server.hpp"

class Server;
class Client; 

class   CommandContext
{
    // public :
    //     std::vector<std::string>    parametersVec;

    public :
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
        void                            fillCommand(std::string const& prefix, std::string const& cmd, std::vector<std::string> const& parameters);
        void                            setCommand(std::string const& name);
        std::string                     getCommand(void) const;
        // std::vector<std::string>&   getParams(void) {return _parameters;} // copie ou reference ? aura besoin de les modifier via getParams ?
        // int                         getState(void) {return _client.Client::getState(); }
        // void                        setState(int newState) { _client.Client::setState(newState); }
};
