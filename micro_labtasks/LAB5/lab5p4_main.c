
#include "bsp.h"
#include "stdlib.h"

int main(void) {

	track_index = 0; /* read write index for array */
	state = start; /* start state */
	track_is_full[0] = 0;
	track_is_full[1] = 0;
	track_is_full[2] = 0;
	track_is_full[3] = 0;
	empty_tracks = 4;
	track_selection = track_one;
	BSP_system_init();

	while(1)
    {

    }

    return 0;
}
