#ifndef NETWORK_H
#define NETWORK_H

// Dummy network.h for Vita port - networking disabled
// This provides only function stubs, not variable declarations that conflict with map.h

#ifdef __cplusplus
extern "C" {
#endif

#include "cseries.h"

// Network game protocol
#define NUMBER_OF_NETWORK_GAME_PROTOCOL_NAMES 1

// Network game protocol
typedef enum {
    _network_game_protocol_none = 0,
    _network_game_protocol_tcp = 1,
    _network_game_protocol_adhoc = 2
} network_game_protocol;

// Network message types
typedef enum {
    _network_message_none = 0,
    _network_message_game_data = 1,
    _network_message_chat = 2,
    _network_message_player_info = 3
} network_message_type;

// Network player info
typedef struct {
    short player_index;
    char name[32];
    int score;
    int ping;
    int team;
} network_player_info;

// Network game data
typedef struct {
    short game_type;
    short protocol;
    short max_players;
    short current_players;
    char game_name[64];
    char password[32];
    int ping;
} network_game_data;

// Function prototypes (stubs)
short NetGetUnconfirmedActionFlagsCount(void);
bool NetGameInProgress(void);
bool NetGameIsServer(void);
short NetGetGameType(void);
short NetGetProtocol(void);
short NetGetMaxPlayers(void);
short NetGetCurrentPlayers(void);
const char* NetGetGameName(void);
const char* NetGetPassword(void);
int NetGetPing(void);
bool NetSendGameData(const void* data, int size);
bool NetReceiveGameData(void* data, int* size);
bool NetSendChatMessage(const char* message);
bool NetReceiveChatMessage(char* message, int max_size);
bool NetSendPlayerInfo(const network_player_info* info);
bool NetReceivePlayerInfo(network_player_info* info);
void NetDisconnect(void);
bool NetConnect(const char* address, short port);
bool NetListen(short port);
bool NetIsConnected(void);

bool NetAllowCarnageMessages(void);
bool NetAllowSavingLevel(void);
bool NetAllowCrosshair(void);
bool NetAllowBehindview(void);
short get_network_compass_state(short player_index);
short get_player_net_ranking(short player_index, int* kills, int* deaths, bool flag);

void NetUpdateUnconfirmedActionFlags(void);
uint32 NetGetUnconfirmedActionFlag(short tick);

#ifdef __cplusplus
}
#endif

#endif