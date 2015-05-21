#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include <string.h>
#include <time.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <errno.h>
#include <signal.h>
#include <ctype.h>

#include "driver-base.h"
#include "driver-visual.h"

#define MSG_BFR_SZ 128

#define TIMEOUT_MS_BOT 500
#define TIMEOUT_MS_HMN 30000

#define xstr(s) _str(s)
#define _str(s) #s

#define PORTNO 1337
#define PORTSTR xstr(PORTNO)

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
  __typeof__ (b) _b = (b); \
  _a > _b ? _a : _b; })

#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
  __typeof__ (b) _b = (b); \
  _a < _b ? _a : _b; })

#define randf() ((double)rand()/(double)RAND_MAX)

int NUMROUNDS = -1;

int BOARDSIZE = 0;

int NUMAGENTS = -1;
struct agent_t agents[MAXAGENTS];

// file pointer to data for setting up the game
FILE* gamedata = NULL;

// setup a child and get its file descriptors
void setup_agent(int, struct agent_t*, char*);

// listen to a bot for a message
void listen_bot(char*, int);

// listen to a bot, with a limited amount of time to wait
void listen_bot_timeout(char*, int, int timeout_ms);

// tell a bot a message
void tell_bot(char*, int);

// tell all bots (but one, possibly) a piece of data
void tell_all(char*, int);

// close all bots' file descriptors
void cleanup_bots();

// socket file descriptor
int sockfd;

unsigned char socket_ready = 0u;
void socket_setup()
{
	if (socket_ready) return;
	socket_ready = 1u;
	
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	if (getaddrinfo(NULL, PORTSTR, &hints, &result))
	{ fprintf(stderr, "getaddrinfo failed\n"); exit(1); }

	for (rp = result; rp; rp = rp->ai_next)
	{
		sockfd = socket(rp->ai_family,
			rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1) continue;
		if (bind(sockfd, rp->ai_addr,
			rp->ai_addrlen) == 0) break;

		close(sockfd);
	}

	if (!rp) exit(1);
	freeaddrinfo(result);
	listen(sockfd, 8);
}

void setup_game(int argc, char** argv)
{
	unsigned int i; char msg[MSG_BFR_SZ];
	fgets(msg, MSG_BFR_SZ, gamedata);

	// basic stats
	sscanf(msg, "%d %d %d", &BOARDSIZE, &NUMROUNDS, &NUMAGENTS);

	// setup all bots provided
	for (i = 0; i < NUMAGENTS; ++i)
	{
		// get the command for this bot
		fgets(msg, MSG_BFR_SZ, gamedata);

		// remove newline for command
		char *p = strchr(msg, '\n');
		if (p) *p = 0;
		
		// setup the agent using this command
		for (p = msg; *p && isspace(*p); ++p);
		setup_agent(i, &agents[i], p);

		agents[i].score = 0u;
		agents[i].id = i;
	}

	for (i = 0; i < NUMAGENTS; ++i)
	{
		// tell the bot its id, it should respond with its name
		sprintf(msg, "INIT %u", i); tell_bot(msg, i);
		listen_bot_timeout(msg, i, agents[i].timeout);

		strncpy(agents[i].name, msg + 5, 255);
		agents[i].name[255] = 0;
	}

	sprintf(msg, "BOARD %u", BOARDSIZE);
	tell_all(msg, -1);

	sprintf(msg, "PLAYERS %u", NUMAGENTS);
	tell_all(msg, -1);

	sprintf(msg, "ROUNDS %u", NUMROUNDS);
	tell_all(msg, -1);
}

int fence_value(struct agent_t *a)
{ return (a->fence2.x - a->fence1.x) * (a->fence2.y - a->fence1.y); }

int play_game()
{
	char msg[MSG_BFR_SZ];
	struct agent_t *a;
	unsigned int i, j, rnum;
	
	int contains[MAXAGENTS];
	int scorediff[MAXAGENTS];
	
	for (rnum = 0; rnum < NUMROUNDS; ++rnum)
	{
		sprintf(msg, "ROUND %u", rnum);
		tell_all(msg, -1);

		tell_all("GO", -1);
		for (i = 0, a = agents; i < NUMAGENTS; ++a, ++i)
		{
			listen_bot_timeout(msg, i, a->timeout);
			if (a->status != RUNNING) continue;

			sscanf(msg, "%*s %u %u %u %u %u %u"
			,	&a->cow.x, &a->cow.y
			,	&a->fence1.x, &a->fence1.y
			,	&a->fence2.x, &a->fence2.y
			);

			contains[i] = 0u;
			scorediff[i] = 0u;
		}

		for (i = 0; i < NUMAGENTS; ++i)
		for (j = 0; j < NUMAGENTS; ++j) if (i != j)
		{
			// how many cows are contained within fence i
			if (agents[j].cow.x >= agents[i].fence1.x && agents[j].cow.x <= agents[i].fence2.x 
				&& agents[j].cow.y >= agents[i].fence1.y && agents[j].cow.y <= agents[i].fence2.y)
					contains[i]++;
		}

		for (i = 0; i < NUMAGENTS; ++i)
		for (j = 0; j < NUMAGENTS; ++j)
		{
			// score is the value of the fence subdivided
			if (agents[j].cow.x >= agents[i].fence1.x && agents[j].cow.x <= agents[i].fence2.x 
				&& agents[j].cow.y >= agents[i].fence1.y && agents[j].cow.y <= agents[i].fence2.y)
					scorediff[j] += fence_value(&agents[i]) / contains[i];
		}
		
		for (i = 0; i < NUMAGENTS; ++i)
		{
			// if your fence is empty, you get the score for it
			if (contains[i] == 0) scorediff[i] += fence_value(&agents[i]);

			// if you put your cow in your own fenced region, 0 points for the round
			if (agents[i].cow.x >= agents[i].fence1.x && agents[i].cow.x <= agents[i].fence2.x 
				&& agents[i].cow.y >= agents[i].fence1.y && agents[i].cow.y <= agents[i].fence2.y)
					scorediff[i] = 0;
		}

		for (i = 0, a = agents; i < NUMAGENTS; ++a, ++i)
		{
			a->score += scorediff[i];
			sprintf(msg, "PLAYER %u %u %u %u %u %u %u %u %u", i
			,	a->cow.x, a->cow.y
			,	a->fence1.x, a->fence1.y
			,	a->fence2.x, a->fence2.y
			,	scorediff[i], a->score
			);
			tell_all(msg, -1);
		}
		
		tell_all("NEXT", -1);
		update_bcb_vis(NUMAGENTS, agents, rnum);
	}

	tell_all("ENDGAME", -1);
	
	struct agent_t *best = NULL;
	for (i = 0, a = agents; i < NUMAGENTS; ++a, ++i)
		if (!best || a->score > best->score) best = a;
	
	return best ? best->id : -1;
}

void sighandler(int signum)
{
	fprintf(stderr, "!!! Signal %d\n", signum);
	close_bcb_vis();
	cleanup_bots();
	exit(1);
}

int main(int argc, char** argv)
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	signal(SIGPIPE, sighandler);
	signal(SIGTERM, sighandler);
	
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);

	if (argc < 2)
	{
		fprintf(stderr, "USAGE: %s <gamefile>\n", argv[0]);
		exit(0);
	}

	gamedata = fopen(argv[1], "r");
	setup_game(argc, argv);

	if (setup_bcb_vis(NUMAGENTS, agents, &argc, &argv))
	{
		tell_all("READY", -1);
		int result = play_game();
		close_bcb_vis();

		fprintf(stdout, "%d\n", result);
	}

	cleanup_bots();
	return 0;
}

// setup an agent and get its file descriptors
void setup_agent(int bot, struct agent_t* agent, char* filename)
{
	if (!strcmp(filename, "HUMAN"))
	{
		socket_setup();
		
		socklen_t clen;
		struct sockaddr_in cli_addr;
		int nsockfd;

		agent->status = ERROR;
		clen = sizeof cli_addr;

		if (DEBUG) fprintf(stderr, "Accepting connections"
				" for bot #%d\n", bot+1);

		nsockfd = accept(sockfd, 
				(struct sockaddr *) &cli_addr, &clen);
		if (nsockfd < 0) return;

		if (DEBUG) fprintf(stderr, "...bot #%d connected"
				" successfully!\n", bot+1);

		agent->status = RUNNING;
		agent->fds[READ] = agent->fds[WRITE] = nsockfd;
		agent->timeout = TIMEOUT_MS_HMN;
		return;
	}
	else
	{
		int i, pid, c2p[2], p2c[2];
		agent->timeout = TIMEOUT_MS_BOT;

		char *p, *arglist[100];
		for (i = 0, p = strtok(filename, " "); p; 
			++i, p = strtok(NULL, " ")) arglist[i] = strdup(p);
		arglist[i] = NULL;

		// setup anonymous pipes to replace child's stdin/stdout
		if (pipe(c2p) || pipe(p2c))
		{
			// the pipes were not properly set up (perhaps no more file descriptors)
			fprintf(stderr, "Couldn't set up codriverunication for bot%d\n", bot+1);
			exit(1);
		}

		// fork here!
		switch (pid = fork())
		{
		case -1: // error forking
			agent->status = ERROR;
			fprintf(stderr, "Could not fork process to run bot%d: '%s'\n", bot+1, filename);
			exit(1);

		case 0: // child process
			close(p2c[WRITE]); close(c2p[READ]);
			
			if (STDIN_FILENO != dup2(p2c[READ], STDIN_FILENO))
				fprintf(stderr, "Could not replace stdin on bot%d\n", bot+1);
			
			if (STDOUT_FILENO != dup2(c2p[WRITE], STDOUT_FILENO))
				fprintf(stderr, "Could not replace stdout on bot%d\n", bot+1);

			close(p2c[0]); close(c2p[1]);
			agent->status = RUNNING;
			execvp(arglist[0], arglist);
			
			agent->status = ERROR;
			fprintf(stderr, "Could not exec bot%d: [%d] %s\n", 
				bot, errno, strerror(errno));

			exit(1);
			break;

		default: // parent process
			agent->pid = pid;
			close(p2c[READ]); close(c2p[WRITE]);

			agent->fds[READ ] = c2p[READ ]; // save the file descriptors in the
			agent->fds[WRITE] = p2c[WRITE]; // returned parameter
			
			return;
		}
	}
}

// listen to a bot for a message
void listen_bot(char* msg, int bot)
{
	// read message from file descriptor for a bot
	bzero(msg, MSG_BFR_SZ);
	if (agents[bot].status != RUNNING) return;

	int br, bl; char* m = msg;
	for (bl = MSG_BFR_SZ; bl > 0; bl -= br, m += br)
		br = read(agents[bot].fds[READ], m, bl);

	// msg[strcspn(msg, "\r\n")] = 0; // clear out newlines
	if (DEBUG) fprintf(stderr, "==> RECV [%d]: (%d) %s\n", bot, br, msg);
}

// listen to a bot, with a limited amount of time to wait
void listen_bot_timeout(char* msg, int bot, int milliseconds)
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	if (agents[bot].status != RUNNING) return;

	// only care about the bot's file descriptor
	FD_ZERO(&rfds);
	FD_SET(agents[bot].fds[READ], &rfds);

	// timeout in milliseconds
	tv.tv_sec = 0;
	tv.tv_usec = milliseconds * 1000u;

	// wait on this file descriptor...
	retval = select(1+agents[bot].fds[READ], 
		&rfds, NULL, NULL, &tv);

	// error, bot failed
	if (retval < 1) agents[bot].status = ERROR;

	// if we have data to read, read it
	listen_bot(msg, bot);
}

// tell a bot a message
void tell_bot(char* msg, int bot)
{
	// write message to file descriptor for a bot
	int br, bl; char* m = msg;
	for (bl = MSG_BFR_SZ; bl > 0; bl -= br, m += br)
		br = write(agents[bot].fds[WRITE], m, bl);
	
	if (DEBUG) fprintf(stderr, "<-- SEND [%d]: (%d) %s\n", bot, br, msg);
}

// tell all bots (but one, possibly) a piece of data
void tell_all(char* msg, int exclude)
{
	unsigned int i;
	for (i = 0; i < NUMAGENTS; ++i)
		if (i != exclude) tell_bot(msg, i);
}

// close all bots' file descriptors
void cleanup_bots()
{
	int i; for (i = 0; i < NUMAGENTS; ++i)
	{
		close(agents[i].fds[0]);
		kill(agents[i].pid, SIGTERM);
	}
}

