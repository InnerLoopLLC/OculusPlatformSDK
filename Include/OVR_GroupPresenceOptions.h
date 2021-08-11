// This file was @generated with LibOVRPlatform/codegen/main. Do not modify it!

#ifndef OVR_GROUP_PRESENCE_OPTIONS_H
#define OVR_GROUP_PRESENCE_OPTIONS_H

#include "OVR_Platform_Defs.h"
#include "OVR_Types.h"
#include <stddef.h>
#include <stdbool.h>


struct ovrGroupPresenceOptions;
typedef struct ovrGroupPresenceOptions* ovrGroupPresenceOptionsHandle;

OVRP_PUBLIC_FUNCTION(ovrGroupPresenceOptionsHandle) ovr_GroupPresenceOptions_Create();
OVRP_PUBLIC_FUNCTION(void) ovr_GroupPresenceOptions_Destroy(ovrGroupPresenceOptionsHandle handle);
/// This the unique API Name that refers to an in-app destination
OVRP_PUBLIC_FUNCTION(void) ovr_GroupPresenceOptions_SetDestinationApiName(ovrGroupPresenceOptionsHandle handle, const char * value);
/// Set whether or not the person is shown as joinable or not to others
OVRP_PUBLIC_FUNCTION(void) ovr_GroupPresenceOptions_SetIsJoinable(ovrGroupPresenceOptionsHandle handle, bool value);
/// This is a session that represents a group/squad/party of users that are to
/// remain together across multiple rounds, matches, levels maps, game modes,
/// etc. Users with the same lobby session id in their rich presence will be
/// considered together.
OVRP_PUBLIC_FUNCTION(void) ovr_GroupPresenceOptions_SetLobbySessionId(ovrGroupPresenceOptionsHandle handle, const char * value);
/// This is a session that represents all the users that are playing a specific
/// instance of a map, game mode, round, etc. This can include users from
/// multiple different lobbies that joined together and the users may or may
/// not remain together after the match is over. Users with the same match
/// session id in their rich presence is also considered to be together but
/// have a looser connection than those together in a lobby session.
OVRP_PUBLIC_FUNCTION(void) ovr_GroupPresenceOptions_SetMatchSessionId(ovrGroupPresenceOptionsHandle handle, const char * value);

#endif
