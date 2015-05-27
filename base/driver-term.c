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
	fprintf(stderr, "Round #%d\n", turn);

	int i; struct agent_t *a;
	for (i = 0; i < numagents; ++i)
	{
		a = agents + i;
		fprintf(stderr, "- %s: Fence=(%d,%d -> %d,%d), Cow=(%d,%d), Score=%d(+%d)\n", a->name,
			a->fence1.x, a->fence1.y, a->fence2.x, a->fence2.y, a->cow.x, a->cow.y,
			a->score, a->_dscore);
	}

	usleep(100000L);
	return 1;
};

void close_bcb_vis()
{

};

