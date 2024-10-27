/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:46:42 by Helene            #+#    #+#             */
/*   Updated: 2024/10/27 18:50:35 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"
#include "CommandContext.hpp"
#include "NumericalReplies.hpp"

# define DEFAULT_KICK_REASON "vote rn"

class Channel;

void    cmdPass(CommandContext &ctx);
void    cmdUser(CommandContext &ctx);
void    cmdNick(CommandContext &ctx);
void    cmdMotd(CommandContext &ctx);
void    cmdPing(CommandContext &ctx);
void    cmdQuit(CommandContext &ctx);
void    cmdJoin(CommandContext &ctx);
void    cmdPart(CommandContext &ctx);
void	cmdPrivmsg(CommandContext &ctx);


/* ---------- Utils ---------- */

std::string getPrefix(Client &client, Channel &channel);
bool        checkChanMask(std::string const& chanName);