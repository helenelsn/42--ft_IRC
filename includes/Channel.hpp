/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 15:10:41 by Helene            #+#    #+#             */
/*   Updated: 2024/09/23 22:14:16 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Create a hash table to keep track of all irc channels

#include "../includes/irc.hpp"
#include "../includes/Client.hpp"

class Channel
{
    public :
        typedef std::vector<Client> clients;
        typedef clients::iterator   clients_it;

    private :
        std::string _name;
        std::string _topic;
        std::string _currentlySetModes;
        std::vector<Client> _clients; // ? // std::vector<std::string, Client> pout <nick, client>
        // creation time 
        // amount of users in the channel (if zero, channel is deleted)
        // channel users, and their privileges
        // channel modes

    // protected :

    public :
};