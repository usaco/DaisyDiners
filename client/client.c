#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "client.h"

#define MSG_BFR_SZ 128

// helper macro, sorry for the mess...
#define EXPECTED(m, s) { fprintf(stderr, "[%u] Expected command %s," \
	" received %s.\n", SELF.id, s, m); return EXIT_FAILURE; }
#define copyself() memcpy(&SELF, &players[SELF.id], sizeof SELF)

/* these functions should be defined by the bot author */

extern const char* BOT_NAME;

extern int client_setup(int* /*argc*/, char*** /*argv*/);

extern void game_setup(const struct player_data* /*players*/);

extern void turn_start(unsigned int /*roundnum*/, const struct player_data* /*players*/);

extern void player_turn(struct player_unit* /*unit*/, const struct player_data* /*players*/);

extern void game_end();

// ########################################################

struct coord_t locFlowers;
struct coord_t locTeleporter;
struct coord_t locCloak;
struct coord_t locDecoy;

double P = 1.0;
unsigned int T = 100;

struct player_data players[2];

struct player_data SELF;

int _fdout = STDOUT_FILENO, _fdin = STDIN_FILENO;

int recv(char* msg)
{
	bzero(msg, MSG_BFR_SZ);

	// read message from file descriptor for a bot
	int bl, br; char* m = msg;
	for (bl = MSG_BFR_SZ; bl > 0; m += br)
		bl -= br = read(_fdin, m, bl);

	return br;
}

int send(char* msg)
{
	// write message to file descriptor for a bot
	int bl, br; char* m = msg;
	for (bl = MSG_BFR_SZ; bl > 0; m += br)
		bl -= br = write(_fdout, m, bl);

	return br;
}

int main(int argc, char **argv)
{
	int i, cc;
	char msg[MSG_BFR_SZ];
	char tag[MSG_BFR_SZ];

	struct player_data *p = NULL;

	--argc; ++argv;
	setbuf(stdout, NULL);
	setbuf(stdin , NULL);
	
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);

	if (!client_setup(&argc, &argv))
		return EXIT_FAILURE;

	recv(msg); sscanf(msg, "%*s %d", &SELF.id);
	sprintf(msg, "NAME %s", BOT_NAME); send(msg);

	while ((cc = recv(msg)))
	{
		sscanf(msg, "%s", tag);
		
		if (!strcmp(tag, "READY")) break;
		else if (!strcmp(tag, "P"))
			sscanf(msg, "%*s %lf", &P);
		else if (!strcmp(tag, "T"))
			sscanf(msg, "%*s %d", &T);
		else if (!strcmp(tag, "POS"))
		{
			sscanf(msg, "%*s %d", &i);
			switch(i)
			{
				case 1: sscanf(msg, "%*s %*d %d %d", &locCloak.row, &locCloak.col); break;
				case 2: sscanf(msg, "%*s %*d %d %d", &locDecoy.row, &locDecoy.col); break;
				case 3: sscanf(msg, "%*s %*d %d %d", &locFlowers.row, &locFlowers.col); break;
				case 4: sscanf(msg, "%*s %*d %d %d", &locTeleporter.row, &locTeleporter.col); break;
			}
		}
	}

	// cow = 0
	players[0].id = 0;
	players[0].count = 1;
	players[0].units[0].id = 0;
	players[0].units[0].row = BOARDSIZE - 1;
	players[0].units[0].col = BOARDSIZE - 1;

	// farmer = 1
	players[1].id = 1;
	players[1].count = 1;
	players[1].units[0].id = 0;
	players[1].units[0].row = 0;
	players[1].units[0].col = 0;

	copyself(); game_setup(players);

	unsigned int rnum;
	while ((cc = recv(msg)))
	{
		sscanf(msg, "%s", tag);
		
		if (!strcmp(tag, "ENDGAME")) break;
		else if (!strcmp(tag, "ROUND"))
		{
			sscanf(msg, "%*s %u", &rnum);

			copyself();
			turn_start(rnum, players);
		}
		else if (!strcmp(tag, "UPDATE"))
		{
			unsigned int u, x, row, col, silent;
			sscanf(msg, "%*s %u %u %u %u %d", &u, &x, &row, &col, &silent);

			players[u].units[x].row = row;
			players[u].units[x].col = col;
			if (!silent) players[u].units[x].last_update = rnum;
		}
		else if (!strcmp(tag, "MOVE"))
		{
			unsigned int x, row, col;
			sscanf(msg, "%*s %u %u %u", &x, &row, &col);
			copyself(); player_turn(SELF.units + x, players);
			
			sprintf(msg, "%d %d", SELF.units[x].row, SELF.units[x].col);
			send(msg);
		}
		else if (!strcmp(tag, "COUNT"))
		{
			unsigned int x, c;
			sscanf(msg, "%*s %u %u", &x, &c);
			players[x].count = c;
		}
		// got an unexpected message...
		else EXPECTED(tag, "ROUND/ENDGAME/MOVE/UPDATE");
	}

	quit: game_end();
	return EXIT_SUCCESS;
}
