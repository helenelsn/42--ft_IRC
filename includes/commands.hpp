/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:46:42 by Helene            #+#    #+#             */
/*   Updated: 2024/09/28 16:50:11 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "irc.hpp"
#include "CommandContext.hpp"

void    cmdPass(CommandContext &ctx);
void    cmdUser(CommandContext &ctx);
void    cmdNick(CommandContext &ctx);
void    cmdJoin(CommandContext &ctx);
