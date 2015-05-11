#pragma once

#define READ 0
#define WRITE 1

#define RUNNING 0
#define ERROR -1

#define MAXAGENTS 12

struct agent_t
{
	char name[256];       // bot name

// META:
	int status;           // bot's current status
	int fds[2];           // file descriptors to codriverunicate
	int pid;              // process id of agent
	int timeout;          // amount of time given to respond

// VISUALIZATION:
	void* vis;            // data for visualisation
};

