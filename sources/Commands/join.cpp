/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 18:05:03 by Helene            #+#    #+#             */
/*   Updated: 2024/09/30 18:07:16 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

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