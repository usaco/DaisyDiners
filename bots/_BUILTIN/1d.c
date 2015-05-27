#include <stdio.h>

#include "client.h"

// This is the name that the driver will refer to your bot as.
const char* BOT_NAME = "1d";

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

  SELF.fence1.x = 0;
  SELF.fence2.x = BOARDSIZE-1;
  SELF.fence1.y = rand()%BOARDSIZE;
  SELF.fence2.y = SELF.fence1.y + (rand()%(BOARDSIZE-SELF.fence1.y));
  
  SELF.cow.x = BOARDSIZE/2;

  if(SELF.fence1.y > 0) {
    SELF.cow.y = SELF.fence1.y - 1;
  }
  else if(SELF.fence2.y < BOARDSIZE-1){
    SELF.cow.y = SELF.fence2.y + 1;
  }
  else {
    SELF.cow.y = BOARDSIZE/2;
  }

}

// This function is called at the end of the game, as a courtesy.
void game_end()
{

}

