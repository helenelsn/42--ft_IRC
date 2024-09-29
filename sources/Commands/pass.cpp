/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:46:03 by Helene            #+#    #+#             */
/*   Updated: 2024/09/28 18:06:10 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"

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
*/
void    cmdPass(CommandContext &ctx)
{
    printf("in cmdPass()\n");

}
