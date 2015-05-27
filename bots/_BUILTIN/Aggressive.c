#include <stdio.h>

#include "client.h"

// This is the name that the driver will refer to your bot as.
const char* BOT_NAME = "Aggressive";

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

  //return a random large rectangle.
  
  //pick to be roughly N/2 x N/2
  int width = (BOARDSIZE)/2;
  int mod = BOARDSIZE-width+1;

  SELF.fence1.x = rand()%mod;
  SELF.fence1.y = rand()%mod;

  SELF.fence2.x = SELF.fence1.x + width-1;
  SELF.fence2.y = SELF.fence1.y + width-1;
  

  if(SELF.fence1.x > 0) {
    SELF.cow.x = SELF.fence1.x - 1;
    SELF.cow.y = BOARDSIZE/2;
  }
  else if(SELF.fence1.y > 0) {
    SELF.cow.x = BOARDSIZE/2;
    SELF.cow.y = SELF.fence1.y - 1;
  }
  else {
    SELF.cow.x = SELF.fence2.x + 1;
    SELF.cow.y = SELF.fence2.y + 1;
  }

}

// This function is called at the end of the game, as a courtesy.
void game_end()
{

}

