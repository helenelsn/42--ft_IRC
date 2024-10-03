/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepompid <hepompid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:46:42 by Helene            #+#    #+#             */
/*   Updated: 2024/10/03 19:19:01 by hepompid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"
#include "CommandContext.hpp"
#include "NumericalReplies.hpp"

void    cmdPass(CommandContext &ctx);
void    cmdUser(CommandContext &ctx);
void    cmdNick(CommandContext &ctx);
void    cmdJoin(CommandContext &ctx);
void    cmdMotd(CommandContext &ctx);
void    cmdPing(CommandContext &ctx);
void    cmdQuit(CommandContext &ctx);
void	cmdPrivmsg(CommandContext &ctx);