#include <unistd.h>
#include <stdio.h>

#include "driver-base.h"
#include "driver-visual.h"

int setup_bcb_vis(int numagents, struct agent_t *agents, int *argc, char ***argv)
{
	return 1;
};

int update_bcb_vis(int numagents, struct agent_t *agents, const int turn)
{
	usleep(100000L);
	return 1;
};

void close_bcb_vis()
{

};

