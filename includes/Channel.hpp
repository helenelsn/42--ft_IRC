/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 15:10:41 by Helene            #+#    #+#             */
/*   Updated: 2024/09/26 21:52:04 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Create a hash table to keep track of all irc channels -> could also just be a vector ? and would be in Server class

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
        std::vector<Client> _clients; // ou std::map<std::string, Client> pour <nick, client> (ou <client_fd, Client>)
        
        // creation time 
        // amount of users in the channel (if zero, channel is deleted)
        // channel users, and their privileges
        // channel modes

    public :
        // constructeurs etc 
        
};