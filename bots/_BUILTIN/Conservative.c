#include <stdio.h>

#include "client.h"

// This is the name that the driver will refer to your bot as.
const char* BOT_NAME = "Conservative";

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

  SELF.fence1.x = rand()%BOARDSIZE;
  SELF.fence1.y = rand()%BOARDSIZE;
  SELF.fence2.x = SELF.fence1.x;
  SELF.fence2.y = SELF.fence1.y;

  SELF.cow.x = rand()%BOARDSIZE;
  SELF.cow.y = rand()%BOARDSIZE;
  
}

// This function is called at the end of the game, as a courtesy.
void game_end()
{

}

