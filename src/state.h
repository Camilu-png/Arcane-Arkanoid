#ifndef ARKAVOID_STATE_H
#define ARKAVOID_STATE_H

#include "constants.h"
#include "level.h"
#include "utils.h"

#include <math.h>

typedef struct {
    // Ball position
    int x,y;
    // Ball radious
    int rad;
    // Speed
    float spd_x,spd_y;
    // Cooldown
    int glue,safety;
} ball;

typedef enum {
    STATUS_WAITING = 0,
    STATUS_PLAYING = 1,
    STATUS_GAMEOVER = 2,
} game_status;

typedef struct {
    // Currrent game status
    game_status status;

    // Pointer to the level
    level *level;

    // Player's palette
    rectangle palette;

    // Ball
    ball ball;
} state;



// Allocates and initializes a new state
state *state_new(const char *fname);

// Moves the state one frame forward in time
void state_update(state *sta);

// Free a state
void state_free(state *sta);



#endif
