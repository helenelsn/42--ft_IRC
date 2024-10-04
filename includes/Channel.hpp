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

#pragma once

#include "irc.hpp"
#include "Client.hpp"

class Channel
{
    private:
        typedef std::map<std::string, Client> members;
        typedef std::map<std::string, Client> operators;
        typedef std::map<std::string, Client> invitedUsers;
        
		members			_members;
		operators		_operators;
        std::string     _name;
        std::string     _topic;
        bool            _topicRestrictionMode;
        bool            _inviteOnlyMode;
		invitedUsers	_invitedUsers;
        bool            _passwordMode;
        std::string     _password;
        bool            _userLimitMode;
        unsigned int    _userLimit;
        
    public:
        Channel();
		Channel(const std::string& name, Client& client);
        Channel (const Channel& other);
        ~Channel();

        Channel& operator = (const Channel& other);

		bool				isMember(const std::string& nick);
        Client&				getMember(const std::string& nick);
		unsigned int		getNumberOfMembers();
		bool				isOperator(const std::string& nick);
		Client& 			getOperator(const std::string& nick);
		unsigned int		getNumberOfOperators();
		bool				isInvited(const std::string& nick);
		Client&				getInvitedUsers(const std::string& nick);
		unsigned int		getNumberOfInvitedUsers();
		const std::string&	getName();
		const std::string&	getTopic();
		const bool&			getTopicRestrictionMode();
		const bool&			getInviteOnlyMode();
		const bool&			getPasswordMode();
		const std::string&	getPassword();
		const bool&			getUserLimitMode();
		const unsigned int&	getUserLimit();
};
