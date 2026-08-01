/*

	Copyright (C) 1991-2001 and beyond by Bungie Studios, Inc.
	and the "Aleph One" developers.
 
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	This license is contained in the file "COPYING",
	which is included with this source code; it is available online at
	http://www.gnu.org/licenses/gpl.html

*/
/*
 *  network_dummy.cpp - Dummy network functions
 */

#include "cseries.h"
#include "map.h"
#include "network.h"
#include "network_games.h"
#include "InfoTree.h"
#include "StarGameProtocol.h"
#include "network_dialog_widgets_sdl.h"
#include "metaserver_messages.h"


void NetExit(void)
{
}

bool NetSync(void)
{
	return true;
}

bool NetUnSync(void)
{
	return true;
}

short NetGetLocalPlayerIndex(void)
{
	return 0;
}

short NetGetPlayerIdentifier(short player_index)
{
	return 0;
}

short NetGetNumberOfPlayers(void)
{
	return 1;
}

void *NetGetPlayerData(short player_index)
{
	return NULL;
}

void *NetGetGameData(void)
{
	return NULL;
}

bool NetChangeMap(struct entry_point *entry)
{
	return false;
}

int32 NetGetNetTime(void)
{
	return 0;
}

void display_net_game_stats(void)
{
}

bool network_gather(void)
{
	return false;
}

int network_join(void)
{
	return false;
}

bool current_game_has_balls(void)
{
	return false;
}

bool NetAllowBehindview(void)
{
	return false;
}

bool NetAllowCrosshair(void)
{
	return false;
}

bool NetAllowTunnelVision(void)
{
	return false;
}

short get_network_compass_state(short player_index)
{
	return 0;
}

long get_player_net_ranking(short player_index, short *kills, short *deaths,
	bool game_is_over)
{
	if (kills) *kills = 0;
	if (deaths) *deaths = 0;
	return 0;
}

bool NetAllowCarnageMessages()
{
	return false;
}

bool NetAllowSavingLevel()
{
	return false;
}

void match_starts_with_existing_players(player_start_data* ioStartArray, short* ioStartCount)
{
}

InfoTree StarPreferencesTree()
{
	return InfoTree();
}

void StarGameProtocol::ParsePreferencesTree(InfoTree prefs, std::string version)
{
}

static int32 dummy_hub_minimum_send_period = 0;
int32& hub_get_minimum_send_period()
{
	return dummy_hub_minimum_send_period;
}

void hub_set_minimum_send_period(int32 new_minimum)
{
	dummy_hub_minimum_send_period = new_minimum;
}

std::string GameListMessage::GameListEntry::game_string() const
{
	return std::string();
}

w_players_in_game2::w_players_in_game2(bool inPostgameLayout)
{
	(void)inPostgameLayout;
}

w_players_in_game2::~w_players_in_game2()
{
}

void w_players_in_game2::update_display(bool inFromDynamicWorld)
{
	(void)inFromDynamicWorld;
}

void w_players_in_game2::draw(SDL_Surface* s) const
{
	(void)s;
}

void w_players_in_game2::click(int x, int y)
{
	(void)x; (void)y;
}

bool NetAllowOverlayMap(void)
{
	return false;
}

bool NetCheckWorldUpdate(void)
{
	return false;
}

std::string NetSessionIdentifier(void)
{
	return std::string();
}

void NetUpdateUnconfirmedActionFlags(void)
{
}

int32 NetGetUnconfirmedActionFlagsCount(void)
{
	return 0;
}

uint32 NetGetUnconfirmedActionFlag(int32 offset)
{
	(void)offset;
	return 0;
}

int32 NetGetLatency(void)
{
	return 0;
}

const NetworkStats& NetGetStats(int player_index)
{
	static NetworkStats sDefault;
	return sDefault;
}

void calculate_player_rankings(struct player_ranking_data *rankings)
{
	(void)rankings;
}

void calculate_ranking_text(char *buffer, long ranking)
{
	(void)buffer;
	(void)ranking;
}

extern int32 team_netgame_parameters[NUMBER_OF_TEAM_COLORS][2];
int32 team_netgame_parameters[NUMBER_OF_TEAM_COLORS][2] = {};
