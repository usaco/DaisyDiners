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

extern void player_turn(unsigned int /*roundnum*/, const struct player_data* /*players*/);

extern void game_end();

// ########################################################

int NUMPLAYERS = -1;
struct player_data players[MAXPLAYERS];

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
	}

	copyself(); game_setup(players);

	unsigned int rnum;
	while ((cc = recv(msg)))
	{
		sscanf(msg, "%s", tag);
		
		if (!strcmp(tag, "ENDGAME")) break;
		
		// got an unexpected message...
		else EXPECTED(tag, "ROUND/ENDGAME/MOVE/UPDATE");
	}

	quit: game_end();
	return EXIT_SUCCESS;
}
