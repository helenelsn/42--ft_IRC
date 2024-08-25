/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:56:26 by Helene            #+#    #+#             */
/*   Updated: 2024/08/23 18:16:24 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once // tester si suffit pour les inclusions multiples, 

#include <iostream>
#include <vector>
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()

#include <netdb.h> // for struct addrinfo

/* Contains all informations about a client inside a server. 
Read incoming data on the associated socket and parse the messages. */
// A uniquement besoin d'une socket qui se connecte a un serveur distant
class Client 
{
    private :
        int _fd;
        // int state; logged in, registered, disconnected, ?
        

    public :
        Client();
        ~Client();
        Client(Client const& other) {} // private ?
        Client operator==(Client const& other) {return *this;} // private ?
        void    connect_to_s(std::string const& port);
        void    send_msg();
        void    receive_msg();
        
};


void    Client::connect_to_s(std::string const& port)
{
    int status;
    struct sockaddr_in sa; // structure indiquant l'adresse IP (IPv4, pour IPv6 : sockaddr_in6). cette structure est contenue dans la structure addrinfo

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET; // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
    sa.sin_port = htons(atoi(port.c_str())); // manipuler le port autrement ? ou s'en fout car pas censé coder de client de tte facon
    
    // on crée la socket et on la connecte au serveur distant
    _fd = socket(sa.sin_family, SOCK_STREAM, 0);
    if (_fd == -1) {
        fprintf(stderr, "socket fd error: %s\n", strerror(errno));
        // throw exception ;
    } 

    status = connect(_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status != 0) {
        fprintf(stderr, "connect error: %s\n", strerror(errno));
        // throw exception ;
    }

    printf("fd %d connected to server, on port %s\n", _fd, port.c_str());
}


/* La fonction send() de la bibliothèque <sys/socket.h> nous permet d’envoyer des données via une socket de type « stream », 
qui utilise une connexion TCP. 

ssize_t send(int socket, const void *buf, size_t len, int flags); 
    socket : le descripteur de fichier de la socket via laquelle envoyer des données. 
    Chez un client, ce sera le fd qu’on a récupéré de notre appel à la fonction socket() 
    Du côté d’un serveur ce sera plutôt le fd du client qu’on aura récupéré grâce à notre appel à accept().
*/
void Client::send_msg()
{
    std::string msg = "acabisous";
    size_t msg_size = strlen(msg.c_str());
    int bytes_sent = send(_fd, msg.c_str(), msg_size, 0);
    if (bytes_sent == -1)
        ;// exception
    // else : checker si a pu envoyer la totalité du msg (si bytes_sent == msg_size)

    // pour apres : 
}


void Client::receive_msg()
{
    char buffer[BUFSIZ];
    int bytes_read = 1;
    while (bytes_read >= 0) {
        bytes_read = recv(_fd, buffer, BUFSIZ, 0);
        if (bytes_read == 0) {
            printf("Server closed connection.\n");
            break ;
        }
        else if (bytes_read == -1) {
            fprintf(stderr, "recv error: %s\n", strerror(errno));
            break ;
        }
        else {
            // Si on a bien reçu un message, on va l'imprimer
            buffer[bytes_read] = '\0';
            printf("Message received: \"%s\"\n", buffer);
            break ;
        }
    }
}

Client::Client()
{
    connect_to_s("4240");
    //send_msg();
    //receive_msg();
    
    //close(_fd);
    //sleep(10);
        
}

Client::~Client()
{
    //close(_fd);
}