/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 22:58:57 by Helene            #+#    #+#             */
/*   Updated: 2024/09/23 23:46:24 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

// class Parser, avec un tableau de pointeurs sur fonctions, chaque fonction gérant une commande

/*  A message contains at least two parts: the command and the command parameters. There may be at most 15 parameters. 
    The command and the parameters are all separated by a single ASCII space character. 
    
    Some messages also include a prefix before the command and the command parameters. 
    The presence of a prefix is indicated with a single leading colon character. 
    The prefix is used to indicate the origin of the message. 
    For example, when a user sends a message to a channel, the server will forward that message to all the users in the channel,
    and will include a prefix to specify the user that sent that message originally.

    When the last parameter is prefixed with a colon character,
    the value of that parameter will be the remainder of the message (including space characters).
    ex -> PRIVMSG rory :Hey Rory...
*/

typedef struct  s_command {
    std::string prefix;
    std::string command;
    std::string parameters;
}       t_command;



class Parser // executes the command as well ? Or just formats the command in a more usable way ? ie shound it be a class or function ?
{
    public :
        t_command   parseCommand(std::string message) const;

    private :
        
};

// tout remplir, puis check apres si a un prefix avec prefix.empty() ?
struct s_command   Parser::parseCommand(std::string message) const
{
    std::stringstream sstream(message);
    t_command command;

    // skip le(s) espaces du début 
    // si a un ':', le mot suivant est le prefix
    // sinon, le mot suivant est la commande

    // syntaxe : [<' ' (?)><:><prefix><' '><command><' '><parameters (up to 15)>]
    
    size_t pos = message.find_first_not_of(' ');
    if (pos == std::string::npos)
        ; // ?
    
}