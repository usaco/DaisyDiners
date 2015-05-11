#include <stdio.h>

#include "client.h"

// This is the name that the driver will refer to your bot as.
const char* BOT_NAME = "BOTNAME";

// Return whether setup was successful, bot dies if 0.
int client_setup(int *argc, char ***argv)
{
	return 1;
}

// This function is called when the game begins.
void game_setup(const struct player_data* players)
{

}

// When this function is called, your move is about to start. You can make decisions here if you would like.
void turn_start(unsigned int roundnum, const struct player_data* players)
{

}

// A unit (id + coordinates) is provided. Update its row/col before exiting.
// row/col are zero indexed and range from 0 to BOARDSIZE
void player_turn(struct player_unit* unit, const struct player_data* players)
{
	// if cowbot, unit->id = 0 for real cow, unit->id = 1 for decoy cow (if active)

	// unit->row
	// unit->col
}

// This function is called at the end of the game, as a courtesy.
void game_end()
{

}

