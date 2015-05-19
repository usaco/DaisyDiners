#include <stdio.h>
#include <stdlib.h>

#include "client.h"

// This is the name that the driver will refer to your bot as.
const char* BOT_NAME = "Random";

// Return whether setup was successful, bot dies if 0.
int client_setup(int *argc, char ***argv)
{
	return 1;
}

// This function is called when the game begins.
void game_setup(const struct player_data* players)
{
}

// This function is called when your turn comes up. Make changes to SELF to affect the next round.
void player_turn(unsigned int roundnum, const struct player_data* players)
{
	struct coord_t origin;
	origin.x = origin.y = 0;

	// this is just a lazy hack to make sure that the randomness doesn't favor
	// fences in the (<n/2,<n/2) quadrant
	do
	{
		SELF.fence1.x = rand() % BOARDSIZE;
		SELF.fence1.y = rand() % BOARDSIZE;
		
		SELF.fence2.x = rand() % BOARDSIZE;
		SELF.fence2.y = rand() % BOARDSIZE;
	}
	while (!is_inside_fence(&SELF.fence1, &origin, &SELF.fence2));

	// cow inside our own fence gets us zero points. this bot is just BARELY
	// smarter than that, but not by much.
	do
	{
		SELF.cow.x = rand() % BOARDSIZE;
		SELF.cow.y = rand() % BOARDSIZE;
	}
	while (is_inside_fence(&SELF.cow, &SELF.fence1, &SELF.fence2));
}

// This function is called at the end of the game, as a courtesy.
void game_end()
{

}

