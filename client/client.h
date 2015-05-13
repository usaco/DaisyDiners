#ifndef CLIENT_H
#define CLIENT_H

#define MAXPLAYERS 12

struct coord_t
{
	unsigned x;
	unsigned y;
};

// player information
struct player_data
{
	unsigned int id;

	// information about the player
	struct coord_t cow;
	struct coord_t fence1;
	struct coord_t fence2;

	int lastscore;
	int score;
};

extern int BOARDSIZE;
extern int NUMPLAYERS;
extern int NUMROUNDS;

// my bot's data
extern struct player_data SELF;

// file descriptors
extern int _fdout, _fdin;

#endif
