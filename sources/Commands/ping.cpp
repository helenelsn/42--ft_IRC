/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:35:37 by Helene            #+#    #+#             */
/*   Updated: 2024/10/02 12:36:39 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

/*
RECV <127.0.0.1> 'PING localhost'
SEND <127.0.0.1> 'PONG :localhost'

*/
void    cmdPing(CommandContext &ctx)
{
    
}