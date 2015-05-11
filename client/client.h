#ifndef CLIENT_H
#define CLIENT_H

#define MAXPLAYERS 12

struct coord_t
{
	unsigned row;
	unsigned col;
};

// player information
struct player_data
{
	unsigned int id;

	// information about the player
	struct coord_t last_cow;
	struct coord_t last_fence_TL;
	struct coord_t last_fence_BR;
};

// my bot's data
extern struct player_data SELF;

// file descriptors
extern int _fdout, _fdin;

#endif
