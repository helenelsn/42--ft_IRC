/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:23:46 by Helene            #+#    #+#             */
/*   Updated: 2024/10/02 12:36:56 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

void    Server::sendMotd(Client &client)
{
    std::ifstream motdFile;
    std::string line;

    // Va pas l'ouvrir à chaque commande MOTD, si ? Plutot le stocker dans le serveur 
    motdFile.open("motd.txt");
    if (!motdFile.is_open())
    {
        // Error : could not open Motd file (missing or bad access rights)
        return ;
    }

    client.addToWriteBuffer(RPL_MOTDSTART(std::string(SERVER_NAME)));
    while (std::getline(motdFile, line)) // By default, the delimiter is \n (newline).
        client.addToWriteBuffer(RPL_MOTD(line));
    client.addToWriteBuffer(RPL_ENFODMOTD);
    
    motdFile.close();
}

void    cmdMotd(CommandContext &ctx)
{
    // arguments checks etc
    
    ctx._server.sendMotd(ctx._client);
}