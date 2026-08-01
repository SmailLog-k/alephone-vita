#ifndef NETWORK_GAMES_H
#define NETWORK_GAMES_H

// Dummy network_games.h for Vita port - networking disabled

#ifdef __cplusplus
extern "C" {
#endif

#include "cseries.h"

// Network game entry
typedef struct {
    char name[64];
    char password[32];
    short game_type;
    short protocol;
    short max_players;
    short current_players;
    int ping;
    bool password_protected;
} network_game_entry;

// Function prototypes (stubs)
short NetGetGameCount(void);
bool NetGetGameEntry(short index, network_game_entry* entry);
bool NetRefreshGameList(void);
bool NetCreateGame(const char* name, const char* password, short game_type, short protocol, short max_players);
bool NetJoinGame(short index, const char* password);
bool NetLeaveGame(void);
bool NetStartGame(void);
bool NetIsGameInProgress(void);

#ifdef __cplusplus
}
#endif

#endif