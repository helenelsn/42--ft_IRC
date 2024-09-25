/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:24:09 by Helene            #+#    #+#             */
/*   Updated: 2024/09/25 17:06:10 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp" 

#define    INVALID_ARGUMENTS    1

bool serverShutdown = false;

void handleSignal(int signal)
{
    // print message sur Logger ?

    (void) signal;
    serverShutdown = true;
    
    // reset signal handler ? signal(signal, SIG_DFL); 
}

// Vérifier comment sont Ctrl+Z et Ctrl+D 
void setSignalHandlers()
{
    struct sigaction act;
    
    bzero(&act, sizeof(act));
    act.sa_handler = handleSignal;
    
    sigaction(SIGINT, &act, NULL); // Applique cette structure avec la fonction à invoquer au signal SIGINT 
    sigaction(SIGQUIT, &act, NULL);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        // print error msg
        return (INVALID_ARGUMENTS); 
    }

    setSignalHandlers();

    Server IrcServer(argv[1], argv[2]);
    
    // boucle while utile que si peut restart le serveur. sinon, un seul appel a initServer et runServer suffit.
    // dans le cas où peut restart, un booléen ne suffira plus (running, restarting, to be stopped)
    while (!serverShutdown)
    {
        try {
            IrcServer.InitServer();
            IrcServer.RunServer();
        }
        catch (std::exception const& e) {
            std::cout << "Error (main): " << e.what() << std::endl;
        }
    }
    // shutdown server
    IrcServer.ShutdownServer();
}