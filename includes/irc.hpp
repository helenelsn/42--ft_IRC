/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 22:10:00 by Helene            #+#    #+#             */
/*   Updated: 2024/09/26 18:00:40 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <unistd.h> // for close()
#include <netdb.h> // for struct addrinfo
#include <fcntl.h> // for fcntl()
#include <poll.h> // for poll()
#include <sys/socket.h> // for socket()
#include <sys/types.h> // for socket()

#include <sys/select.h> // for select()
#include <sys/time.h> // for select()

#include <netinet/in.h> // for sockaddr_in
#include <arpa/inet.h> // for inet_ntoa()
#include <csignal> // for signal()

#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <errno.h>
#include <map>

#include <sstream> // std::stringstream
#include <algorithm>
#include <stdlib.h>

# define CRLF "\r\n"

typedef struct  s_command {
    std::string prefix;
    std::string command;
    std::string parameters;
}       t_command;