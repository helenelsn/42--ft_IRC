/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:55:46 by Helene            #+#    #+#             */
/*   Updated: 2024/09/30 13:34:04 by hlesny           ###   ########.fr       */
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

Nicknames are non-empty strings with the following restrictions:

    They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), 
        question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
    They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
    They MUST NOT start with a character listed as a channel type (# OR &), channel membership prefix (@, % OR +), or prefix listed in the IRCv3 multi-prefix Extension.
    They SHOULD NOT contain any dot character ('.', 0x2E).

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