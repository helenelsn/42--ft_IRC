/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:46 by Helene            #+#    #+#             */
/*   Updated: 2024/09/24 12:06:55 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

// void    Client::connect_to_s(std::string const& port)
// {
//     int status;
//     struct sockaddr_in sa; // structure indiquant l'adresse IP (IPv4, pour IPv6 : sockaddr_in6). cette structure est contenue dans la structure addrinfo

//     memset(&sa, 0, sizeof sa);
//     sa.sin_family = AF_INET; // IPv4
//     sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
//     sa.sin_port = htons(atoi(port.c_str())); // manipuler le port autrement ? ou s'en fout car pas censé coder de client de tte facon
    
//     // on crée la socket et on la connecte au serveur distant
//     _sockFd = socket(sa.sin_family, SOCK_STREAM, 0);
//     if (_sockFd == -1) {
//         fprintf(stderr, "socket fd error: %s\n", strerror(errno));
//         // throw exception ;
//     } 

//     status = connect(_sockFd, (struct sockaddr *)&sa, sizeof sa);
//     if (status != 0) {
//         fprintf(stderr, "connect error: %s\n", strerror(errno));
//         // throw exception ;
//     }

//     printf("fd %d connected to server, on port %s\n", _sockFd, port.c_str());
// }


/* La fonction send() de la bibliothèque <sys/socket.h> nous permet d’envoyer des données via une socket de type « stream », 
qui utilise une connexion TCP. 

ssize_t send(int socket, const void *buf, size_t len, int flags); 
    socket : le descripteur de fichier de la socket via laquelle envoyer des données. 
    Chez un client, ce sera le fd qu’on a récupéré de notre appel à la fonction socket() 
    Du côté d’un serveur ce sera plutôt le fd du client qu’on aura récupéré grâce à notre appel à accept().
*/
// void Client::send_msg()
// {
//     std::string msg = "acabisous";
//     size_t msg_size = strlen(msg.c_str());
//     int bytes_sent = send(_sockFd, msg.c_str(), msg_size, 0);
//     if (bytes_sent == -1)
//         ;// exception
//     // else : checker si a pu envoyer la totalité du msg (si bytes_sent == msg_size)

// }


// void Client::receive_msg()
// {
//     char buffer[BUFSIZ];
//     int bytes_read = 1;
//     while (bytes_read >= 0) {
//         bytes_read = recv(_sockFd, buffer, BUFSIZ, 0);
//         if (bytes_read == 0) {
//             printf("Server closed connection.\n");
//             break ;
//         }
//         else if (bytes_read == -1) {
//             fprintf(stderr, "recv error: %s\n", strerror(errno));
//             break ;
//         }
//         else {
//             // Si on a bien reçu un message, on va l'imprimer
//             buffer[bytes_read] = '\0';
//             printf("Message received: \"%s\"\n", buffer);
//             break ;
//         }
//     }
// }



Client::Client(int fd)
: _sockFd(fd)
{
    
}

Client::~Client()
{
    //close(_sockFd);
}

Client Client::operator==(Client const& other) 
{
    if (this == &other) // a verif 
        return *this;
    
    this->_sockFd = other._sockFd;
    this->_hostname = other._hostname;
    this->_nickname = other._nickname;
    this->_realname = other._realname;
    this->_username = other._username;
    
    this->_readBuffer = other._readBuffer;
    this->_writeBuffer = other._writeBuffer;
    
    return *this;
}

int Client::getSockFd(void)
{
    return this->_sockFd;
}

std::string Client::getReadBuffer(void)
{
    return this->_readBuffer;
}

void    Client::writeToReadBuffer(std::string data)
{
    _readBuffer += data; 
}

void    Client::clearReadBuffer(void)
{
    _readBuffer.clear();
}
