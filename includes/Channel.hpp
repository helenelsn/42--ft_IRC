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

typedef enum 
{
	Normal = 0,
	Founder = 1 << 0,
	Operator = 1 << 1
}	e_memberState;

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
		std::string 	_founder;
        
    public:
        Channel();
		Channel(const std::string& name, Client& client);
        Channel (const Channel& other);
        ~Channel();

        Channel& operator = (const Channel& other);

		bool				isMember(const std::string& nick);
        Client&				getMember(const std::string& nick);
		unsigned int		getNumberOfMembers();
		void				addMember(const Client& client);
		void				removeMember(const Client& client);

		bool				isOperator(const std::string& nick);
		Client& 			getOperator(const std::string& nick);
		unsigned int		getNumberOfOperators();
		void				addOperator(const Client& client);
		void				removeOperator(const Client& client);

		bool				isInvited(const std::string& nick);
		Client&				getInvitedUsers(const std::string& nick);
		unsigned int		getNumberOfInvitedUsers();
		void				addInvitedUser(const Client& client);
		void				removeInvitedUser(const Client& client);

		const std::string&	getName();
		bool				hasTopic();
		const std::string&	getTopic();
		const bool&			getTopicRestrictionMode();
		const bool&			getInviteOnlyMode();
		const bool&			getPasswordMode();
		const std::string&	getPassword();
		const bool&			getUserLimitMode();
		const unsigned int&	getUserLimit();
		bool 				isFull();

		// send the given message to all clients of this channel, excluding the client given as parameter
		void				sendToAll(Client const& client, std::string const& msg);
		members&			getAllMembers(void);
		std::string 		getFounder();
		bool 				isFounder(std::string const& client);
};
