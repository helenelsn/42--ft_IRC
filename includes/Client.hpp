/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:56:26 by Helene            #+#    #+#             */
/*   Updated: 2024/09/29 15:43:24 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once // tester si suffit pour les inclusions multiples 

#include "../includes/irc.hpp"

class Server; // enough ?

typedef enum 
{
    unregistered = 1,
    registering = 2,
    registered = 4,
    disconnected = 8, // ?
    connected = 16 // ?
}       e_state; // values = powers of 2s to be able to perform & and | operations 

/* Contains all informations about a client inside a server. 
Read incoming data on the associated socket and parse the messages. */
// A uniquement besoin d'une socket qui se connecte a un serveur distant
class Client 
{
    private :
        int         _sockFd;
        std::string _username;
        std::string _hostname;
        std::string _nickname;
        std::string _realname;
        std::string _readBuffer; // data sent by client
        std::string _writeBuffer; // data to be sent to client
        std::string _modes; // user's currently set modes
        int         _state;
        Server      *_server;

        // un client a la liste des channels dans lesquels il est
        
        // int state; logged in, registered, disconnected, ?
        

    public :
        Client(int fd, Server *server);
        ~Client();
        Client(Client const& other);
        Client&          operator=(Client const& other);
        bool            operator==(Client const& other);
        int             getSockFd(void);
        Server&         getServer();
        std::string     getNick(void) const; // retourner une référence ?
        std::string&    getReadBuffer(void);
        void            addToReadBuffer(std::string const& data);
        void            clearReadBuffer(void);
        void            resetReadBuffer(std::string const& newBuffer);
        void            addToWriteBuffer(std::string const& data);
        std::string&    getWriteBuffer(void);
        void            clearWriteBuffer(void);
};
