#ifndef METASERVER_MESSAGES_H
#define METASERVER_MESSAGES_H

// Dummy header for Vita port - networking disabled

#ifdef __cplusplus
extern "C" {
#endif

#include <string>
#include <vector>

struct GameListEntry {
    std::string name;
    std::string password;
    int players;
    int max_players;
    int ping;
    int game_type;
    int flags;
};

struct GameListMessage {
    std::vector<GameListEntry> entries;
};

struct MetaserverPlayerInfo {
    std::string name;
    int score;
    int ping;
    int team;
};

struct prospective_joiner_info {
    std::string name;
    std::string password;
    int player_count;
};

#ifdef __cplusplus
}
#endif

#endif