/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 22:58:57 by Helene            #+#    #+#             */
/*   Updated: 2024/09/25 15:22:49 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Parser.hpp"

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


// peut avoir plusieurs "\r\n" dans un meme message ? 
struct s_command   Parser::parseMessage(std::string message) const
{
    // tej le "\r\n" de fin
    std::stringstream sstream(message.substr(0, message.size() - 2)); //std::string(&message[0], &message[message.size() - 3]));
    std::string temp;
    std::vector<std::string> msgElems;
    t_command command;

    //std::getline(sstream, temp, ' '); // reads characters until it encounters the specified delimiter (or '\n' if not specified), or the end of the stream.
    

    // printf("DEBUG : in parseMessage(), initial msg = %s\n", message.c_str());
    // ?
    // std::stringstream ss(message);
    // ss << "DEBUG : in parseMessage(), initial message = ";
    // std::cout << ss.str() << std::endl;
    
}