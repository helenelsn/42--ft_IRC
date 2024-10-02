/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:55:46 by Helene            #+#    #+#             */
/*   Updated: 2024/10/02 11:38:38 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

static bool validChar(char c)
{
    return (c == '[' || c == ']' || c == '{' || c == '}' 
        || c == '\\' || c == '|');
}

/*
Is alphabetical
Is not a digit
Is a valid special char
Is not any of the forbidden char
*/
static bool validFirstChar(char c)
{
    return (std::isalpha(c) || validChar(c));
}

/*
Servers MUST allow at least all alphanumerical characters, square and curly brackets ([]{}), 
backslashes (\), and pipe (|) characters in nicknames, and MAY disallow digits as the first character. 
Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands, including:
    no leading # character or other character advertized in CHANTYPES
    no leading colon (:)
    no ASCII space
*/
static bool    validNick(std::string const& s)
{
    std::string::const_iterator it = s.begin();

    if (s.size() > 9)
        return false;
    
    if (!std::isalpha(*it) && !validChar(*it)) // !(std::isalpha(c) || validChar(c))
        return false;
    it++;
    for (it; it != s.end(); it++)
    {
        if (! (std::isalnum(*it) || validChar(*it))) // suffit, ou rajouter des checks d'autres caractères ?
            return false;
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
// si donne un nick invalide, ne pas addState(nick), peut encore rentrer PASS si n a pas encore rentre USER
void    cmdNick(CommandContext &ctx)
{
    ctx._client.setState(Registering);

    if (ctx._parameters.empty())
    {
        ctx._client.addToWriteBuffer(ERR_NONICKNAMEGIVEN(ctx._client.getNickname())); 
        return ;
    }
    
    std::string nickname = ctx._parameters[0];
    if (!validNick(nickname))
    {
        ctx._client.addToWriteBuffer(ERR_ERRONEUSNICKNAME(ctx._client.getNickname(), nickname));
        return ;
    }
    if (ctx._server.NickAlreadyUsed(nickname))
    {
        ctx._client.addToWriteBuffer(ERR_NICKNAMEINUSE(ctx._client.getNickname(), nickname)); // verifier les params a donner a la macro, pas sure que 'client' soit le nick
        return ;
    }
    
    // nick does not already exist on this server, and is valid
    // conséquences :
    // update les channels dans lesquelles est le client ? 
    // envoyer un msg a tous les clients connectés au serveru pour les prévenir du chgt de nick
    
    std::string oldNick = ctx._client.getNickname();
    
    // if client not already registered (ie had not provided a nickname before)
    if ((ctx._client.getState() & Nick) != Nick)
    {
        oldNick = nickname;
        ctx._client.addState(Nick);
    }
    else
    {
        // unregister previous nickname -> Server Rpl 
    }
    ctx._client.setNickname(nickname);
        
    
    // RPL_NICK 
    /* The NICK message may be sent from the server to clients to acknowledge their NICK command was successful,
    and to inform other clients about the change of nickname. In these cases, the <source> of the message 
    will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.
    */
    
    // oldNickname changed his nickname to newNickname

    ctx._client.addState(Nick);
    if ((ctx._client.getState() & Registering) == Registering) // a rentre NICK et USER
    {
        // check for passwd validity
        // if invalid, send ERR_INVALIDPASSWD or sthg
        // else, what ? -> send MOTD, etc
    }
}