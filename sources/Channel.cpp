/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hepompid <hepompid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:51:52 by Helene            #+#    #+#             */
/*   Updated: 2024/10/05 01:16:18 by hepompid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel() : _topic(""), _topicRestrictionMode(0), _inviteOnlyMode(0),
	_passwordMode(0), _userLimitMode(0) {}

Channel::Channel(const Channel& other)
{
	this->_members = other._members;
	this->_operators = other._operators;
	this->_name = other._name;
	this->_topic = other._topic;
	this->_topicRestrictionMode = other._topicRestrictionMode;
	this->_inviteOnlyMode = other._inviteOnlyMode;
	this->_invitedUsers = other._invitedUsers;
	this->_passwordMode = other._passwordMode;
	this->_password = other._password;
	this->_userLimitMode = other._userLimitMode;
	this->_userLimit = other._userLimit;
}

Channel::Channel(const std::string& name, Client& member) : _topic(""),
	_topicRestrictionMode(0), _inviteOnlyMode(0), _passwordMode(0),
	_userLimitMode(0)
{
	this->_name = name;
	this->_members[member.getNickname()] = member;
	this->_operators[member.getNickname()] = member;
}

Channel::~Channel() {}

Channel& Channel::operator = (const Channel& other)
{
	this->_members = other._members;
	this->_operators = other._operators;
	this->_name = other._name;
	this->_topic = other._topic;
	this->_topicRestrictionMode = other._topicRestrictionMode;
	this->_inviteOnlyMode = other._inviteOnlyMode;
	this->_invitedUsers = other._invitedUsers;
	this->_passwordMode = other._passwordMode;
	this->_password = other._password;
	this->_userLimitMode = other._userLimitMode;
	this->_userLimit = other._userLimit;
	return (*this);
}

bool Channel::isMember(const std::string& nick)
{
	std::map<std::string, Client>::const_iterator it;

	it = this->_members.find(nick);
	if (it != this->_members.end())
		return true;
	return false;
}

Client& Channel::getMember(const std::string& nick)
{
	std::map<std::string, Client>::iterator it;

	it = this->_members.find(nick);
	return it->second;
}

unsigned int Channel::getNumberOfMembers()
{
	std::map<std::string, Client>::iterator	it;
	int										n;

	n = 0;
	for (it = this->_members.begin(); it != this->_members.end(); it++)
		n++;
	return n;
}

bool Channel::isOperator(const std::string& nick)
{
	std::map<std::string, Client>::const_iterator it;

	it = this->_operators.find(nick);
	if (it != this->_operators.end())
		return true;
	return false;
}

Client& Channel::getOperator(const std::string& nick)
{
	std::map<std::string, Client>::iterator it;

	it = this->_operators.find(nick);
	return it->second;
}

unsigned int Channel::getNumberOfOperators()
{
	std::map<std::string, Client>::iterator	it;
	int										n;

	n = 0;
	for (it = this->_operators.begin(); it != this->_operators.end(); it++)
		n++;
	return n;
}

bool Channel::isInvited(const std::string& nick)
{
	std::map<std::string, Client>::const_iterator it;

	it = this->_invitedUsers.find(nick);
	if (it != this->_invitedUsers.end())
		return true;
	return false;
}

Client& Channel::getInvitedUsers(const std::string& nick)
{
	std::map<std::string, Client>::iterator it;

	it = this->_invitedUsers.find(nick);
	return it->second;
}

unsigned int Channel::getNumberOfInvitedUsers()
{
	std::map<std::string, Client>::iterator	it;
	int										n;

	n = 0;
	for (it = this->_invitedUsers.begin(); it != this->_invitedUsers.end(); it++)
		n++;
	return n;
}

const std::string& Channel::getName()
{
	return this->_name;
}

const std::string& Channel::getTopic()
{
	return this->_topic;
}

const bool& Channel::getTopicRestrictionMode()
{
	return this->_topicRestrictionMode;
}

const bool& Channel::getInviteOnlyMode()
{
	return this->_inviteOnlyMode;
}

const bool& Channel::getPasswordMode()
{
	return this->_passwordMode;
}

const std::string& Channel::getPassword()
{
	return this->_password;
}

const bool& Channel::getUserLimitMode()
{
	return this->_userLimitMode;
}

const unsigned int& Channel::getUserLimit()
{
	return this->_userLimit;
}