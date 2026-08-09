/*
GAME_WINDOW.C

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

Thursday, December 30, 1993 9:51:24 PM

Tuesday, May 24, 1994 6:42:32 PM
	functions beginning with underscores are mac-specific and always have a corresponding portable
	function which sets up game-specific information (see update_compass and _update_compass for
	an example).
Friday, June 10, 1994 3:56:57 AM
	gutted, rewritten.  Much cleaner now.
Sunday, September 4, 1994 6:32:05 PM
	made scroll_inventory() non-static so that shell.c can call it.
Saturday, September 24, 1994 10:33:19 AM
	fixed some things, twice...
Friday, October 7, 1994 3:13:22 PM
	New interface.  Draws panels from PICT resources for memory consumption.  Inventory is 
	different panels, which are switched to whenever you grab an item.  There is no scrolling.
Tuesday, June 6, 1995 3:37:50 PM
	Marathon II modifications.
Tuesday, August 29, 1995 4:02:15 PM
	Reestablishing a level of portablility...

Feb 4, 2000 (Loren Petrich):
	Added SMG display stuff
	
Apr 30, 2000 (Loren Petrich): Added XML parser object for all the screen_drawing data,
	and also essentiall all the data here.

May 28, 2000 (Loren Petrich): Added support for buffering the Heads-Up Display

Jul 2, 2000 (Loren Petrich):
	The HUD is now always buffered

Jul 16, 2001 (Loren Petrich):
	Using "temporary" as storage space for count_text and weapon_name;
	it is 256 bytes long, which should be more than enough for most text.
	This fixes the long-weapon-name bug.

Mar 08, 2002 (Woody Zenfell):
    SDL network microphone support
*/

#include "cseries.h"

#include <array>

#include "OGL_Headers.h"

#include "HUDRenderer_SW.h"
#include "game_window.h"

// LP addition: color and font parsers
#include "FontHandler.h"
#include "screen.h"
#include "render.h"

#include "shell.h"
#include "preferences.h"
#include "screen.h"
#include "screen_definitions.h"
#include "images.h"
#include "InfoTree.h"
#include "interface_menus.h"
#include "player.h"
#include "motion_sensor.h"
#ifdef VITA
#include "../../VitaPlatform/vita_build_profile.h"
#endif

extern void draw_panels(void);
extern void validate_world_window(void);
extern struct view_data *world_view;
static void set_current_inventory_screen(short player_index, short screen);
extern bool shapes_file_is_m1();
#ifdef VITA
static bool vita_draw_m1_status_bars(bool force_redraw);
extern void vita_m1_note_hud_full_redraw();
extern void vita_m1_note_hud_dirty_rect(int x, int y, int w, int h);
#endif

/* --------- globals */

// LP addition: whether or not the motion sensor is active
bool MotionSensorActive = true;

struct interface_state_data interface_state;

#define NUMBER_OF_WEAPON_INTERFACE_DEFINITIONS	10
struct weapon_interface_data weapon_interface_definitions[NUMBER_OF_WEAPON_INTERFACE_DEFINITIONS] =
{
	/* Mac, the knife.. */
	{
		_i_knife,
		UNONE,
		433, 432,
		NONE, NONE,
		0, 0,
		false,
		{
			{ _unused_interface_data, 0, 0, 0, 0, 0, 0, UNONE, UNONE, true},
			{ _unused_interface_data, 0, 0, 0, 0, 0, 0, UNONE, UNONE, true}
		},
		UNONE, UNONE,
		0, 0
	},
	
	/* Harry, the .44 */
	{
		_i_magnum,
		BUILD_DESCRIPTOR(_collection_interface, _magnum_panel),
		432, 444,
		420, NONE,
		366, 517, 
		true,
		{
			{ _uses_bullets, 517, 412, 8, 1, 5, 14, BUILD_DESCRIPTOR(_collection_interface, _magnum_bullet), BUILD_DESCRIPTOR(_collection_interface, _magnum_casing), false},
			{ _uses_bullets, 452, 412, 8, 1, 5, 14, BUILD_DESCRIPTOR(_collection_interface, _magnum_bullet), BUILD_DESCRIPTOR(_collection_interface, _magnum_casing), true}
		},
		BUILD_DESCRIPTOR(_collection_interface, _left_magnum),
		BUILD_DESCRIPTOR(_collection_interface, _left_magnum_unusable),
		-97, 0
	},

	/* Ripley, the plasma pistol. */
	{
		_i_plasma_pistol,
		BUILD_DESCRIPTOR(_collection_interface, _zeus_panel),
		431, 443,
		401, NONE,
		366, 475, 
		false,
		{
			{ _uses_energy, 414, 366, 20, 0, 38, 57, _energy_weapon_full_color, _energy_weapon_empty_color, true},
			{ _unused_interface_data, 450, 410, 50, 0, 62, 7, _energy_weapon_full_color, _energy_weapon_empty_color, true}
		},
		UNONE, UNONE,
		0, 0
	},
	
	/* Arnold, the assault rifle */	
	{
		_i_assault_rifle,
		BUILD_DESCRIPTOR(_collection_interface, _assault_panel),
		430, 452,
		439, NONE, //••
		366, 460, 
		false,
		{
			{ _uses_bullets, 391, 368, 13, 4, 4, 10, BUILD_DESCRIPTOR(_collection_interface, _assault_rifle_bullet), BUILD_DESCRIPTOR(_collection_interface, _assault_rifle_casing), true},
			{ _uses_bullets, 390, 413, 7, 1, 8, 12, BUILD_DESCRIPTOR(_collection_interface, _assault_rifle_grenade), BUILD_DESCRIPTOR(_collection_interface, _assault_rifle_grenade_casing), true},
		},
		UNONE, UNONE,
		0, 0
	},
		
	/* John R., the missile launcher */	
	{
		_i_missile_launcher,
		BUILD_DESCRIPTOR(_collection_interface, _missile_panel),
		433, 445,
		426, NONE,
		365, 419, 
		false,
		{
			{ _uses_bullets, 385, 376, 2, 1, 16, 49, BUILD_DESCRIPTOR(_collection_interface, _missile), BUILD_DESCRIPTOR(_collection_interface, _missile_casing), true},
			{ _unused_interface_data, 0, 0, 0, 0, 0, 0, UNONE, UNONE, true }
		},
		UNONE, UNONE,
		0, 0
	},

	/* ???, the flame thrower */	
	{
		_i_flamethrower,
		BUILD_DESCRIPTOR(_collection_interface, _flamethrower_panel),
		433, 445,
		398, NONE,
		363, 475, 
		false,
		{
			/* This weapon has 7 seconds of flamethrower carnage.. */
			{ _uses_energy, 427, 369, 7*TICKS_PER_SECOND, 0, 38, 57, _energy_weapon_full_color, _energy_weapon_empty_color, true},
			{ _unused_interface_data, 450, 410, 50, 0, 62, 7, _energy_weapon_full_color, _energy_weapon_empty_color, true}
		},
		UNONE, UNONE,
		0, 0
	},

	/* Predator, the alien shotgun */	
	{
		_i_alien_shotgun,
		BUILD_DESCRIPTOR(_collection_interface, _alien_weapon_panel),
		418, 445,
		395, 575,
		359, 400, 
		false,
		{
			{ _unused_interface_data, 425, 411, 50, 0, 96, 7, _energy_weapon_full_color, _energy_weapon_empty_color, true},
			{ _unused_interface_data, 450, 410, 50, 0, 62, 7, _energy_weapon_full_color, _energy_weapon_empty_color, true}
		},
		UNONE, UNONE,
		0, 0
	},

	/* Shotgun */
	{
		_i_shotgun,
		BUILD_DESCRIPTOR(_collection_interface, _single_shotgun),
		432, 444,
		420, NONE,
		373, 451, 
		true,
		{
			{ _uses_bullets, 483, 411, 2, 1, 12, 16, BUILD_DESCRIPTOR(_collection_interface, _shotgun_bullet), BUILD_DESCRIPTOR(_collection_interface, _shotgun_casing), true},
			{ _uses_bullets, 451, 411, 2, 1, 12, 16, BUILD_DESCRIPTOR(_collection_interface, _shotgun_bullet), BUILD_DESCRIPTOR(_collection_interface, _shotgun_casing), true}
		},
		BUILD_DESCRIPTOR(_collection_interface, _double_shotgun),
		UNONE,
		0, -12
	},

	/* Ball */
	{
		_i_red_ball, // statistically unlikely to be valid (really should be SKULL)
		BUILD_DESCRIPTOR(_collection_interface, _skull),
		432, 444,
		402, NONE,
		366, 465, 
		false,
		{
			{ _unused_interface_data, 451, 411, 2, 1, 12, 16, BUILD_DESCRIPTOR(_collection_interface, _shotgun_bullet), BUILD_DESCRIPTOR(_collection_interface, _shotgun_casing), true},
			{ _unused_interface_data, 483, 411, 2, 1, 12, 16, BUILD_DESCRIPTOR(_collection_interface, _shotgun_bullet), BUILD_DESCRIPTOR(_collection_interface, _shotgun_casing), true}
		},
		UNONE, UNONE,
		0, 0
	},
	
	/* LP addition: SMG (clone of assault rifle) */	
	{
		_i_smg,
		BUILD_DESCRIPTOR(_collection_interface, _smg),
		430, 452,
		439, NONE, //••
		366, 460, 
		false,
		{
			{ _uses_bullets, 405, 382, 8, 4, 5, 10, BUILD_DESCRIPTOR(_collection_interface, _smg_bullet), BUILD_DESCRIPTOR(_collection_interface, _smg_casing), true},
			{ _unused_interface_data, 390, 413, 7, 1, 8, 12, BUILD_DESCRIPTOR(_collection_interface, _assault_rifle_grenade), BUILD_DESCRIPTOR(_collection_interface, _assault_rifle_grenade_casing), true},
		},
		UNONE, UNONE,
		0, 0
	},
};

// Software rendering
HUD_SW_Class HUD_SW;

/* --------- code */

void initialize_game_window(void)
{
	initialize_motion_sensor(
		BUILD_DESCRIPTOR(_collection_interface, _motion_sensor_mount),
		BUILD_DESCRIPTOR(_collection_interface, _motion_sensor_virgin_mount),
		BUILD_DESCRIPTOR(_collection_interface, _motion_sensor_alien),
		BUILD_DESCRIPTOR(_collection_interface, _motion_sensor_friend),
		BUILD_DESCRIPTOR(_collection_interface, _motion_sensor_enemy),
		BUILD_DESCRIPTOR(_collection_interface, _network_compass_shape_nw),
		MOTION_SENSOR_SIDE_LENGTH);
}

/* draws the entire interface */
void draw_interface(void)
{
	if (alephone::Screen::instance()->openGL())
		return;
#ifdef VITA
	if (world_view && world_view->terminal_mode_active)
		return;
#endif

	if (!game_window_is_full_screen())
	{
		/* draw the frame */
		draw_panels();
	}
		
	validate_world_window();
}

/* updates only what needs changing (time_elapsed==NONE means redraw everything no matter what,
	but skip the interface frame) */
void update_interface(short time_elapsed)
{
	if (time_elapsed == NONE)
        reset_motion_sensor(current_player_index);
	if (alephone::Screen::instance()->openGL() || alephone::Screen::instance()->lua_hud())
        return;
#ifdef VITA
	if (!alephone::Screen::instance()->hud())
		return;
	if (world_view && world_view->terminal_mode_active)
		return;
#endif

	if (!game_window_is_full_screen()
#ifdef VITA
		|| true
#endif
	)
	{
		// LP addition: don't force an update unless explicitly requested
		bool force_update = (time_elapsed == NONE);

		ensure_HUD_buffer();
#ifdef VITA
#if A1_VITA_M1_COCKPIT
		if (time_elapsed == NONE) {
			draw_panels();
			vita_m1_note_hud_full_redraw();
		}
#endif
#endif

		// LP addition: added support for HUD buffer;
		_set_port_to_HUD();
		if (HUD_SW.update_everything(time_elapsed)) {
			force_update = true;
#ifdef VITA
#if A1_VITA_M1_COCKPIT
			if (shapes_file_is_m1())
				vita_m1_note_hud_full_redraw();
#endif
#endif
		}
#ifdef VITA
#if A1_VITA_M1_COCKPIT
		if (shapes_file_is_m1() &&
			!world_view->terminal_mode_active) {
			if (vita_update_m1_motion_sensor(force_update))
				force_update = true;
			if (vita_draw_m1_status_bars(force_update))
				force_update = true;
		}
#endif
#endif
		_restore_port();
		
		// Draw the whole thing if doing so is requested
		// (may need some smart way of drawing only what has to be drawn)
		if (force_update)
			RequestDrawingHUD();
	}
}

void mark_interface_collections(bool loading)
{
	loading ? mark_collection_for_loading(_collection_interface) : 
		mark_collection_for_unloading(_collection_interface);
}

void mark_weapon_display_as_dirty(void)
{
	interface_state.weapon_is_dirty = true;
}

void mark_ammo_display_as_dirty(void)
{
	interface_state.ammo_is_dirty = true;
}

void mark_shield_display_as_dirty(void)
{
	interface_state.shield_is_dirty = true;
}

void mark_oxygen_display_as_dirty(void)
{
	interface_state.oxygen_is_dirty = true;
}

void mark_player_inventory_screen_as_dirty(short player_index, short screen)
{
	struct player_data *player= get_player_data(player_index);

	set_current_inventory_screen(player_index, screen);
	SET_INVENTORY_DIRTY_STATE(player, true);
}

void mark_player_inventory_as_dirty(short player_index, short dirty_item)
{
	struct player_data *player= get_player_data(player_index);

	/* If the dirty item is not NONE, then goto that item kind display.. */
	if(dirty_item != NONE)
	{
		short item_kind= get_item_kind(dirty_item);
		short current_screen= GET_CURRENT_INVENTORY_SCREEN(player);

		/* Don't change if it is a powerup, or you are in the network statistics screen */
		if(item_kind != _powerup && item_kind != current_screen) // && current_screen!=_network_statistics)
		{
			/* Goto that type of item.. */
			set_current_inventory_screen(player_index, item_kind);
		}
	}
	SET_INVENTORY_DIRTY_STATE(player, true);
}

void mark_player_network_stats_as_dirty(short player_index)
{
	if (GET_GAME_OPTIONS()&_live_network_stats)
	{
		struct player_data *player= get_player_data(player_index);
	
		set_current_inventory_screen(player_index, _network_statistics);
		SET_INVENTORY_DIRTY_STATE(player, true);
	}
}

void scroll_inventory(short dy)
{
	short mod_value, index, current_inventory_screen, section_count, test_inventory_screen = 0;
	short section_items[NUMBER_OF_ITEMS];
	short section_counts[NUMBER_OF_ITEMS];
	
	current_inventory_screen= GET_CURRENT_INVENTORY_SCREEN(current_player);

	if(dynamic_world->player_count>1)
	{
		mod_value= NUMBER_OF_ITEM_TYPES+1;
	} else {
		mod_value= NUMBER_OF_ITEM_TYPES;
	}

	if(dy>0)
	{
		for(index= 1; index<mod_value; ++index)
		{
			test_inventory_screen= (current_inventory_screen+index)%mod_value;
			
			assert(test_inventory_screen>=0 && test_inventory_screen<NUMBER_OF_ITEM_TYPES+1);			
			if(test_inventory_screen != NUMBER_OF_ITEM_TYPES)
			{
				calculate_player_item_array(current_player_index, test_inventory_screen,
					section_items, section_counts, &section_count);
				if(section_count) break; /* Go tho this one! */
			} else {
				/* Network statistics! */
				break; 
			}
		}
		
		current_inventory_screen= test_inventory_screen;
	} else {
		/* Going down.. */
		for(index= mod_value-1; index>0; --index)
		{
			test_inventory_screen= (current_inventory_screen+index)%mod_value;

			assert(test_inventory_screen>=0 && test_inventory_screen<NUMBER_OF_ITEM_TYPES+1);			
			if(test_inventory_screen != NUMBER_OF_ITEM_TYPES)
			{
				calculate_player_item_array(current_player_index, test_inventory_screen,
					section_items, section_counts, &section_count);
				if(section_count) break; /* Go tho this one! */
			} else {
				/* Network statistics! */
				break; 
			}
		}		

		current_inventory_screen= test_inventory_screen;
	}
	set_current_inventory_screen(current_player_index, current_inventory_screen);

	SET_INVENTORY_DIRTY_STATE(current_player, true);	
}

static void set_current_inventory_screen(
	short player_index,
	short screen)
{
	struct player_data *player= get_player_data(player_index);
	
	assert(screen>=0 && screen<7);
	
	player->interface_flags&= ~INVENTORY_MASK_BITS;
	player->interface_flags|= screen;
	player->interface_decay= 5*TICKS_PER_SECOND;
}

// From sreen_sdl.cpp
extern SDL_Surface *HUD_Buffer;
extern void build_sdl_color_table(const color_table *color_table, SDL_Color *colors);

// From game_window.cpp
extern HUD_SW_Class HUD_SW;

extern void draw_panels(void);

void ensure_HUD_buffer(void) {

  // Allocate surface for HUD if not present
  if (HUD_Buffer == NULL) {
    HUD_Buffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32,
									  0x00ff0000,
									  0x0000ff00,
									  0x000000ff,
									  0xff000000);
    if (HUD_Buffer == NULL)
      alert_out_of_memory();
  }
}

#ifdef VITA
static bool vita_draw_m1_status_bars(bool force_redraw)
{
	if (!HUD_Buffer || !current_player || !shapes_file_is_m1())
		return false;

	const int bar_top = 151;
	const int bar_height = 100;
	const int bar_width = 14;
	const int oxygen_left = 88;
	const int shield_left = 138;

	const int oxygen = PIN(current_player->suit_oxygen, 0, PLAYER_MAXIMUM_SUIT_OXYGEN);
	int shield_layer = current_player->suit_energy % PLAYER_MAXIMUM_SUIT_ENERGY;
	if (shield_layer == 0 && current_player->suit_energy > 0)
		shield_layer = PLAYER_MAXIMUM_SUIT_ENERGY;
	const int shield = PIN(shield_layer, 0, PLAYER_MAXIMUM_SUIT_ENERGY);

	static int last_oxygen = -1;
	static int last_shield = -1;
	if (!force_redraw && oxygen == last_oxygen && shield == last_shield)
		return false;
	last_oxygen = oxygen;
	last_shield = shield;

	const auto draw_vertical_bar = [&](int x, int value, int maximum, uint8 r, uint8 g, uint8 b) {
		SDL_Rect clear_rect = { x, bar_top, bar_width, bar_height };
		SDL_FillRect(HUD_Buffer, &clear_rect, SDL_MapRGB(HUD_Buffer->format, 0x00, 0x00, 0x00));

		const int fill_height = maximum > 0 ? (bar_height * value) / maximum : 0;
		SDL_Rect fill_rect = { x, bar_top + bar_height - fill_height, bar_width, fill_height };
		SDL_FillRect(HUD_Buffer, &fill_rect, SDL_MapRGB(HUD_Buffer->format, r, g, b));

		if (fill_height > 0) {
			SDL_Rect highlight = { x + 1, fill_rect.y, 2, fill_rect.h };
			SDL_FillRect(HUD_Buffer, &highlight, SDL_MapRGB(HUD_Buffer->format,
				MIN(255, r + 0x28), MIN(255, g + 0x28), MIN(255, b + 0x28)));
		}

		const uint32 grid = SDL_MapRGB(HUD_Buffer->format, 0x00, 0x50, 0x00);
		for (int y = bar_top + 10; y < bar_top + bar_height; y += 10) {
			SDL_Rect line = { x, y, bar_width, 1 };
			SDL_FillRect(HUD_Buffer, &line, grid);
		}
	};

	draw_vertical_bar(oxygen_left, oxygen, PLAYER_MAXIMUM_SUIT_OXYGEN, 0x18, 0x28, 0xd8);
	draw_vertical_bar(shield_left, shield, PLAYER_MAXIMUM_SUIT_ENERGY, 0xd8, 0x10, 0x10);
	vita_m1_note_hud_dirty_rect(oxygen_left, bar_top, bar_width, bar_height);
	vita_m1_note_hud_dirty_rect(shield_left, bar_top, bar_width, bar_height);
	return true;
}
#endif

/*
 *  Draw HUD (to HUD surface)
 */

extern int LuaTexturePaletteSize();

void draw_panels(void)
{
	if (alephone::Screen::instance()->openGL())
		return;

	ensure_HUD_buffer();

	// Draw static HUD picture
	static std::shared_ptr<SDL_Surface> static_hud_pict = std::shared_ptr<SDL_Surface>(nullptr, SDL_FreeSurface);
	static bool hud_pict_not_found = false;
	if (!static_hud_pict && !hud_pict_not_found) {
		LoadedResource rsrc;
		if (get_picture_resource_from_images(INTERFACE_PANEL_BASE, rsrc))
			static_hud_pict = picture_to_surface(rsrc);
	#ifdef VITA
		else if (get_picture_resource_from_images(128, rsrc))
			static_hud_pict = picture_to_surface(rsrc);
	#endif
		else
			hud_pict_not_found = true;
		if (static_hud_pict)
			SDL_SetSurfaceBlendMode(static_hud_pict.get(), SDL_BLENDMODE_NONE);
	} 

	if (!hud_pict_not_found) {
		SDL_Rect dst_rect = {0, 320, 640, 160};
		if (!LuaTexturePaletteSize()) {
			SDL_Rect src_rect = {0, 0, 640, 160};
		#ifdef VITA
			if (static_hud_pict && static_hud_pict->w >= 640 && static_hud_pict->h >= 480) {
				src_rect.y = 0;
				src_rect.h = 480;
				dst_rect.y = 0;
				dst_rect.h = 480;
			}
		#endif
			SDL_BlitSurface(static_hud_pict.get(), &src_rect, HUD_Buffer, &dst_rect);
		} else
			SDL_FillRect(HUD_Buffer, &dst_rect, 0);
	}

	// Add dynamic elements
	_set_port_to_HUD();
	HUD_SW.update_everything(NONE);
#ifdef VITA
#if A1_VITA_M1_COCKPIT
	vita_draw_m1_status_bars(true);
#endif
#endif
	_restore_port();

#ifdef VITA
	static int vita_hud_debug_counter = 0;
	if (++vita_hud_debug_counter == 60) {
		uint32 nonzero_rgb = 0;
		uint32 nonzero_alpha = 0;
		uint32 sampled = 0;
		SDL_LockSurface(HUD_Buffer);
		for (int y = 320; y < 480; y += 4) {
			const uint32 *row = (const uint32 *)((const uint8 *)HUD_Buffer->pixels + y * HUD_Buffer->pitch);
			for (int x = 0; x < 640; x += 4) {
				uint32 pixel = row[x];
				if (pixel & 0x00ffffff)
					++nonzero_rgb;
				if (pixel & 0xff000000)
					++nonzero_alpha;
				++sampled;
			}
		}
		SDL_UnlockSurface(HUD_Buffer);

		FILE *file = fopen(A1_VITA_LOG_DIR "/vita_hud_debug.log", "a");
		if (file) {
			fprintf(file,
				"hud_debug found=%d pict=%dx%d lua_palette=%d nonzero_rgb=%u nonzero_alpha=%u sampled=%u\n",
				hud_pict_not_found ? 0 : 1,
				static_hud_pict ? static_hud_pict->w : 0,
				static_hud_pict ? static_hud_pict->h : 0,
				LuaTexturePaletteSize(),
				(unsigned)nonzero_rgb,
				(unsigned)nonzero_alpha,
				(unsigned)sampled);
			fclose(file);
		}
	}
#endif

	// Tell main loop to render the HUD in the next run
	RequestDrawingHUD();
}

extern short vidmasterStringSetID; // shell.cpp
extern short vidmasterLevelOffset;
struct weapon_interface_data *original_weapon_interface_definitions = NULL;

extern std::array<int, iAbout> menu_item_order;
std::vector<int> original_menu_item_order;

void reset_mml_interface()
{
	if (original_weapon_interface_definitions) {
		for (unsigned i = 0; i < NUMBER_OF_WEAPON_INTERFACE_DEFINITIONS; i++)
			weapon_interface_definitions[i] = original_weapon_interface_definitions[i];
		free(original_weapon_interface_definitions);
		original_weapon_interface_definitions = NULL;
	}
	
	if (original_menu_item_order.size())
	{
		std::copy(original_menu_item_order.begin(),
				  original_menu_item_order.end(),
				  menu_item_order.begin());

		original_menu_item_order.clear();
	}
}

void parse_mml_interface(const InfoTree& root)
{
	// back up old values first
	if (!original_weapon_interface_definitions) {
		original_weapon_interface_definitions = (struct weapon_interface_data *) malloc(sizeof(struct weapon_interface_data) * NUMBER_OF_WEAPON_INTERFACE_DEFINITIONS);
		assert(original_weapon_interface_definitions);
		for (unsigned i = 0; i < NUMBER_OF_WEAPON_INTERFACE_DEFINITIONS; i++)
			original_weapon_interface_definitions[i] = weapon_interface_definitions[i];
	}

	if (!original_menu_item_order.size())
	{
		original_menu_item_order.assign(menu_item_order.begin(), menu_item_order.end());
	}
	
	root.read_attr("motion_sensor", MotionSensorActive);
	
	for (const InfoTree &rect : root.children_named("rect"))
	{
		int16 index;
		if (!rect.read_indexed("index", index, NUMBER_OF_INTERFACE_RECTANGLES))
			continue;
		screen_rectangle *r = get_interface_rectangle(index);
		
		int16 top, left, bottom, right;
		if (rect.read_attr("top", top) &&
			rect.read_attr("left", left) &&
			rect.read_attr("bottom", bottom) &&
			rect.read_attr("right", right))
		{
			r->top = top;
			r->left = left;
			r->bottom = bottom;
			r->right = right;
		}
	}

	for (const InfoTree& menu_item : root.children_named("menu_item"))
	{
		int16_t index;
		if (!menu_item.read_indexed("index", index, menu_item_order.size()))
			continue;

		int16_t item;
		if (menu_item.read_indexed("item", item, iAbout + 1))
		{
			menu_item_order[index] = item;
		}
	}
	
	for (const InfoTree &color : root.children_named("color"))
	{
		int16 index;
		if (!color.read_indexed("index", index, NUMBER_OF_INTERFACE_COLORS))
			continue;
		color.read_color(get_interface_color(index));
	}
	for (const InfoTree &font : root.children_named("font"))
	{
		int16 index;
		if (!font.read_indexed("index", index, NUMBER_OF_INTERFACE_FONTS))
			continue;
		font.read_font(get_interface_font(index));
	}
	
	for (const InfoTree &vid : root.children_named("vidmaster"))
	{
		vidmasterStringSetID = -1;
		vid.read_attr_bounded<int16>("stringset_index", vidmasterStringSetID, -1, SHRT_MAX);
		vid.read_attr_bounded<int16>("level_offset", vidmasterLevelOffset, 0, 1);
	}
	
	for (const InfoTree &weapon : root.children_named("weapon"))
	{
		int16 index;
		if (!weapon.read_indexed("index", index, MAXIMUM_WEAPON_INTERFACE_DEFINITIONS))
			continue;
		weapon_interface_data& def = weapon_interface_definitions[index];
		
		weapon.read_attr("shape", def.weapon_panel_shape);
		weapon.read_attr("start_y", def.weapon_name_start_y);
		weapon.read_attr("end_y", def.weapon_name_end_y);
		weapon.read_attr("start_x", def.weapon_name_start_x);
		weapon.read_attr("end_x", def.weapon_name_end_x);
		weapon.read_attr("top", def.standard_weapon_panel_top);
		weapon.read_attr("left", def.standard_weapon_panel_left);
		weapon.read_attr("multiple", def.multi_weapon);
		weapon.read_attr("multiple_shape", def.multiple_shape);
		weapon.read_attr("multiple_unusable_shape", def.multiple_unusable_shape);
		weapon.read_attr("multiple_delta_x", def.multiple_delta_x);
		weapon.read_attr("multiple_delta_y", def.multiple_delta_y);
		
		for (const InfoTree &ammo : weapon.children_named("ammo"))
		{
			int16 index;
			if (!ammo.read_indexed("index", index, NUMBER_OF_WEAPON_INTERFACE_ITEMS))
				continue;
			weapon_interface_ammo_data& adef = def.ammo_data[index];
			
			ammo.read_attr("type", adef.type);
			ammo.read_attr("left", adef.screen_left);
			ammo.read_attr("top", adef.screen_top);
			ammo.read_attr("across", adef.ammo_across);
			ammo.read_attr("down", adef.ammo_down);
			ammo.read_attr("delta_x", adef.delta_x);
			ammo.read_attr("delta_y", adef.delta_y);
			ammo.read_attr("bullet_shape", adef.bullet);
			ammo.read_attr("empty_shape", adef.empty_bullet);
			ammo.read_attr("right_to_left", adef.right_to_left);
		}
	}
}
