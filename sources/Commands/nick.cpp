/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:55:46 by Helene            #+#    #+#             */
/*   Updated: 2024/09/30 12:40:56 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

static bool    isValid(std::string const& s)
{
    std::string::const_iterator it;

    for (it = s.begin(); it != s.end(); it++)
    {
        
    }
    
    return true;
}

/*
Fonctions check a coder :
    - si le nick n'est pas deja utilisé dans le network
    - si le nick contient bien uniquement des caracteres valides
*/
void    cmdNick(CommandContext &ctx)
{
    ctx._client.setState(Registering); // quand set le state ? ie checker si nick dispo avant ? a tester avec irssi

    if (ctx._server.NickAlreadyUsed(ctx._parameters[0]))
    {
        ctx._client.addToWriteBuffer(ERR_NICKNAMEINUSE(ctx._client.getNick(), ctx._parameters[0])); // verifier les params a donner a la macro, pas sure que 'client' soit le nick
        return ;
    }
    
    if (!isValid(ctx._parameters[0]))
    {
        ctx._client.addToWriteBuffer(ERR_ERRONEUSNICKNAME(ctx._client.getNick(), ctx._parameters[0]));
        return ;
    }
        
}