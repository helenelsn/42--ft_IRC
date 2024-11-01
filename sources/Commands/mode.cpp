/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Helene <Helene@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:59:01 by Helene            #+#    #+#             */
/*   Updated: 2024/11/01 13:07:54 by Helene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

typdef enum
{
   add,
   remove
}  e_action_type;

/*
The MODE command is a dual-purpose command in IRC. It allows both
   usernames and channels to have their mode changed.
   When parsing MODE messages, it is recommended that the entire message
   be parsed first and then the changes which resulted then passed on.
*/

static void    addChanMode(Channel *channel, std::string const& mode, Client &client)
{
   
}

static void removeChanMode(Channel *channel, std::string const& mode, Client &client)
{
   
}

/*
Channel modes : 
   Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
   (or) Parameters: <target> [<modestring> [<mode arguments>...]]

To implement : i - invite-only channel flag;
               t - topic settable by channel operator only flag;
               k - set a channel key (password).
               o - give/take channel operator privileges;
               l - set the user limit to channel;
*/
void  channelMode(CommandContext &ctx) // todo : 
{
   std::vector<std::string> params = ctx._parameters;
   std::string channelName = params[0];
   Channel *channel = ctx._server.getChannel(channelName);
   
   if (!channel)
      ctx._client.addToWriteBuffer(ERR_NOSUCHCHANNEL(ctx._client.getNickname(), channelName));
   else if (params.size() == 1) // check if is member of that channel ? ie ERR_NOTONCHANNEL ? 
         ;// ctx._client.addToWriteBuffer(RPL_CHANNELMODEIS(ctx._client.getNickname(), channelName, ))
   else if (! channel->isOperator(ctx._client.getNickname()))
      ctx._client.addToWriteBuffer(ERR_CHANOPRIVSNEEDED(ctx._client.getNickname(), channelName));
   else 
   {
      /*
      todo :   checker si peut avoir -ik par ex (ie plusieurs modes d'un coup)
               si c'est le cas, checker le comportement à avoir quand a un mode invalide 
                  parmi une chaine de modes valides
                  (ie process ceux valides, ou print juste un msg d'erreur et ignore les valides ?)
      */
      std::string mode = params[1];
      std::string validModes = "+itkol";

      /*
      Parse le <mode_string> caractère par caractère. Si tombe sur un caractère invalide, l'ignore (?), et 
         continue à parser la suite. 
         Ajouter un mode : peut ne pas avoir de '+' au debut.
         Il y a cependant toujours un '-' au début pour retirer un mode.
      */
      
      for (std::string::const_iterator it = mode.begin(), end = mode.end(); it != end; ++it) // const_iterator for read-only
      {
         
      }
      
   }
}

/*
User modes :
    Parameters: <nickname> {[+|-]|i|w|s|o}
    
A user MODE command may only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.
If a user attempts to make themselves an operator using the "+o"
   flag, the attempt should be ignored (since it would be bypassing
    the OPER command).  There is no restriction,
   however, on anyone `deopping' themselves (using "-o").
*/
void  userMode(CommandContext &ctx)
{
   
}

void  cmdMode(CommandContext &ctx)
{
   std::vector<std::string> params = ctx._parameters;
   if (params.empty())
   {
       ctx._client.addToWriteBuffer(ERR_NEEDMOREPARAMS(ctx._client.getNickname(), ctx.getCommand()));
       return ;
   }
   if (params[0][0] == '#' || params[0][0] == '&')
      channelMode(ctx);
   else
      userMode(ctx);
}