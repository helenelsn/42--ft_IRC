/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:24:09 by Helene            #+#    #+#             */
/*   Updated: 2024/08/22 17:03:05 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Client.hpp" 

#define    INVALID_ARGUMENTS    1

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        // print error msg
        return (INVALID_ARGUMENTS); 
    }
    
    try {
        Server IrcServer(argv[1], argv[2]);
        IrcServer.InitServer();
        IrcServer.RunServer();
    }
    catch (std::exception const& e) {
        // si erreurs dans les appels à socket(), bind(), listen(), accept(), ...
    }
    
}