/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 18:05:03 by Helene            #+#    #+#             */
/*   Updated: 2024/10/03 15:16:30 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

/*
Must implement invite-only channels, as well as channels requiring a password to join 
Also need to check for user's limit (ie is there space left in that channel for the new user)
*/
void    cmdJoin(CommandContext &ctx)
{
    printf("in cmdJoin()\n");
    // ctx._server._log(DEBUG, "in Join, writeBuffer before adding msg = " + ctx._client.getWriteBuffer());
    // ctx._client.addToWriteBuffer(ERR_PASSWDMISMATCH(ctx._client.getNickname()));
    // ctx._server._log(DEBUG, "in Join, writeBuffer after adding msg = " + ctx._client.getWriteBuffer());

    std::vector<std::string> params = ctx._parameters;
    if (params.empty())
    {
        ctx._client.addToWriteBuffer(ERR_NEEDMOREPARAMS(ctx._client.getNickname(), ctx.getCommand()));
        return ;
    }
    
    
}