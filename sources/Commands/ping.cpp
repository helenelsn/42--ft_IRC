/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:35:37 by Helene            #+#    #+#             */
/*   Updated: 2024/10/03 14:33:51 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

/*
Command: PING
  Parameters: <token>
When receiving a PING message, clients or servers must reply to it with a PONG message with the same <token> value.

RECV <127.0.0.1> 'PING localhost'
SEND <127.0.0.1> 'PONG :localhost'

*/
void    cmdPing(CommandContext &ctx)
{
    std::vector<std::string> params = ctx._parameters;
    if (params.empty())
    {
        ctx._client.addToWriteBuffer(ERR_NEEDMOREPARAMS(ctx._client.getNickname(), ctx._command));
        return ;
    }
    ctx._client.addToWriteBuffer("PONG :" + params[0] + CRLF);
}