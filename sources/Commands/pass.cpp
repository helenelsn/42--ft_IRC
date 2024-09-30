/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:46:03 by Helene            #+#    #+#             */
/*   Updated: 2024/09/30 13:51:49 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

/*
The PASS command is used to set a ‘connection password’.
If set, the password must be set before any attempt to register the connection is made.
This requires that clients send a PASS command before sending the NICK / USER combination.

The password supplied must match the one defined in the server configuration.
It is possible to send multiple PASS commands before registering but only the last one sent 
is used for verification and it may not be changed once the client has been registered.

If the password supplied does not match the password expected by the server,
then the server SHOULD send ERR_PASSWDMISMATCH (464) and MAY then close the connection with ERROR. 
Servers MUST send at least one of these two messages.

Numeric replies:
    ERR_NEEDMOREPARAMS (461)
    ERR_ALREADYREGISTERED (462)
    ERR_PASSWDMISMATCH (464)

-> When getting the PASS command, needs to know :
    - if the client is already registered, or is currently registering (has already sent the NICK command)
    
*/
void    cmdPass(CommandContext &ctx)
{
    printf("in cmdPass()\n");

    // tester si check d'abord si a assez de parametres, ou si est deja registered

    int cState = ctx._client.getState();
    int unreg = Unregistered;
    if ((ctx._client.getState() & Unregistered) != Unregistered)
    {
        // ERR_ALREADYREGISTERED
        ctx._client.addToWriteBuffer(ERR_ALREADYREGISTERED(ctx._client.getNick()));
        return ;
    }

    if (ctx._parameters.empty())
    {
        // ERR_NEEDMOREPARAMS
        ctx._client.addToWriteBuffer(ERR_NEEDMOREPARAMS(ctx._client.getNick(), ctx._command));
        return ;
    }

    std::vector<std::string> params = ctx._parameters;
    std::string passwd = params[params.size() - 1];
    // juste çàa ou doit faire du parsing en plus pour tej (par ex) les potentiels ' ' avant et apres ?
    if (passwd != ctx._server.getPasswd())
    {
        // ERR_PASSWDMISMATCH
        ctx._client.addToWriteBuffer(ERR_PASSWDMISMATCH(ctx._client.getNick()));
        return ;
    }   

    // garder en memoire le password supplied ?
}
