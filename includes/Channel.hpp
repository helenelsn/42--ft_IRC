// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   Channel.hpp                                        :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: hepompid <hepompid@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/10/03 17:22:29 by hepompid          #+#    #+#             */
// /*   Updated: 2024/10/03 18:45:14 by hepompid         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #ifndef CHANNEL_HPP
// # define CHANNEL_HPP

// # include "irc.hpp"
// # include "Client.hpp"

// class Channel
// {
//     private:
//         typedef std::map<std::string, Client> members;
//         typedef std::map<std::string, Client> operators;
//         typedef std::vector<std::string> usersInvited;
        
// 		members			members_;
// 		operators		operators_;
//         std::string     name_;
//         std::string     topic_;
//         bool            topicRestrictionMode_;
//         bool            inviteOnlyMode_;
// 		usersInvited	usersInvited_;
//         bool            passwordMode_;
//         std::string     password_;
//         bool            userLimitMode_;
//         unsigned int    userLimit_;
        
//     public:
//         Channel();
//         Channel (const Channel& other);
//         ~Channel();

//         Channel& operator = (const Channel& other);

        

    
// };

// #endif
