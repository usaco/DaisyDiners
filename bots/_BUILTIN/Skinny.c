#include <stdio.h>

#include "client.h"

// This is the name that the driver will refer to your bot as.
const char* BOT_NAME = "Skinny";

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

  int row_or_col = rand()%2;

  if(row_or_col) {
    //picking a random row
    SELF.fence1.x = 0;
    SELF.fence1.y = rand()%BOARDSIZE;
    SELF.fence2.x = BOARDSIZE-1;
    SELF.fence2.y = SELF.fence1.y;
  }
  else {
    //picking a random column
    SELF.fence1.x = rand()%BOARDSIZE;
    SELF.fence1.y = 0;
    SELF.fence2.x = SELF.fence1.x;
    SELF.fence2.y = BOARDSIZE-1;
  }

  int corner = rand()%4;
  
  if(corner==0) {
    SELF.cow.x = 0;
    SELF.cow.y = 0;
  }
  else if(corner==1) {
    SELF.cow.x = 0;
    SELF.cow.y = BOARDSIZE-1;
  }
  else if(corner==2) {
    SELF.cow.x = BOARDSIZE-1;
    SELF.cow.y = BOARDSIZE-1;
  }
  else if(corner==3) {
    SELF.cow.x = BOARDSIZE-1;
    SELF.cow.y = 0;
  }

}

// This function is called at the end of the game, as a courtesy.
void game_end()
{

}

