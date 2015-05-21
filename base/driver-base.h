#pragma once

#define READ 0
#define WRITE 1

#define RUNNING 0
#define ERROR -1

#define MAXAGENTS 12

extern int BOARDSIZE;

struct coord_t
{
	unsigned int x;
	unsigned int y;
};

struct agent_t
{
	int id;
	char name[256];        // bot name

	struct coord_t cow;    // cow location
	struct coord_t fence1; // fence top left
	struct coord_t fence2; // fence bottom right
	unsigned int score;    // score

// META:
	int status;            // bot's current status
	int fds[2];            // file descriptors to codriverunicate
	int pid;               // process id of agent
	int timeout;           // amount of time given to respond

// VISUALIZATION:
	void* vis;             // data for visualisation
};

