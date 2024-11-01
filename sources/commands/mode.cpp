/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:59:01 by Helene            #+#    #+#             */
/*   Updated: 2024/11/01 16:54:16 by hlesny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/irc.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

typedef struct s_tuple
{
   char        mode;
   std::string modeArg;
}  t_tuple;



/*
The MODE command is a dual-purpose command in IRC. It allows both
   usernames and channels to have their mode changed.
   When parsing MODE messages, it is recommended that the entire message
   be parsed first and then the changes which resulted then passed on.
*/

static void    addChanModes(Channel *channel, std::vector<t_tuple> const& modes, Client &client)
{
   for (std::vector<t_tuple>::const_iterator it = modes.begin(), end = modes.end(); it != end; it++)
   {
      if (it->mode == 'i')
         channel->setInviteOnlyMode(true);
      else if (it->mode == 't')
         channel->setTopicRestrictionMode(true);
      else if ()
         
   }
}

static void removeChanModes(Channel *channel, std::vector<t_tuple> const& modes, Client &client)
{
   for (std::vector<t_tuple>::const_iterator it = modes.begin(), end = modes.end(); it != end; it++)
   {
      
   }
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
      std::string validModes = "itkol";
      std::string addedModes;
      std::string removedModes;
      std::vector<t_tuple> removedParams;
      std::vector<t_tuple> addedParams;
      /*
      Parse le <mode_string> caractère par caractère. Si tombe sur un caractère invalide, l'ignore (?), et 
         continue à parser la suite. 
      Si un mode requiert un parametre et qu aucun n est passe, l'ignore et continue a parser
         Ajouter un mode : peut ne pas avoir de '+' au debut.
         Il y a cependant toujours un '-' au début pour retirer un mode.

      ->
         
      */
      
      std::vector<std::string> modeParams(params.begin() + 3, params.end());
      std::vector<std::string>::iterator itParams = modeParams.begin(); // empty if modeParams.begin() == params.end()
      
      for (std::string::const_iterator it = mode.begin(), end = mode.end(); it != end; ++it) // const_iterator for read-only
      {
         if (*it == '-')
         {
            while (it != end && *it != '+')
            {
               if (validModes.find(*it) != std::string::npos)
               {
                  if (!removedModes.empty() && removedModes.find(*it) == std::string::npos && addedModes.find(*it) == std::string::npos) // eviter les doublons + ne rien faire si a deja ete added plus tot dans la commande (a verif)
                  {
                     t_tuple mode;
                     mode.mode = *it;
                     if (*it == 'k' || *it == 'o' || *it == 'l')
                     {
                        if (itParams != modeParams.end())
                        {
                           mode.modeArg = *itParams;
                           itParams++;    
                        }
                     }
                     removedParams.push_back(mode);
                  }
               }
               it++;
            }
         }
         else
         {
            while (it != end && *it != '-')
            {
               if (validModes.find(*it) != std::string::npos)
               {
                  if (!addedModes.empty() && addedModes.find(*it) == std::string::npos && removedModes.find(*it) == std::string::npos)
                  {
                     t_tuple mode;
                     mode.mode = *it;
                     if (*it == 'k' || *it == 'o' || *it == 'l')
                     {
                        if (itParams != modeParams.end())
                        {
                           mode.modeArg = *itParams;
                           itParams++;    
                        }
                     }
                     addedParams.push_back(mode);
                  }
               }
               it++;
            }
         }
      }
      addChanModes(channel, addedParams, ctx._client);
      removeChanModes(channel, removedParams, ctx._client); 
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