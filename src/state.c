#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "state.h"

const int INITIAL_PALETTE_WIDTH = 80;
const int INITIAL_PALETTE_HEIGHT = 16;
const int BALL_RADIOUS = 5;

const int BALL_SPEED_Y = 6;
const int PALETTE_SPEED_X = 10;
const float MAX_BALL_HORIZONTAL_SPEED_CHANGE = 4;

int update_distance = 1;
// Distance between ball and palette on X axes
int distance_x;

double time_previous;

// Allocates and initializes a new state
state *state_new(const char *fname, const char *names){
    state *sta = malloc(sizeof(state));
    assert(sta!=NULL);

    sta->status = STATUS_WAITING;

    sta->level = level_load(fname, names, LEVEL_SIZE_Y, LEVEL_SIZE_X);
    // Center palette
    sta->palette.x = LEVEL_SIZE_X/2 - INITIAL_PALETTE_WIDTH/2;
    sta->palette.y = SCREEN_HEIGHT - INITIAL_PALETTE_HEIGHT*3/2;
    sta->palette.width = INITIAL_PALETTE_WIDTH;
    sta->palette.height = INITIAL_PALETTE_HEIGHT;

    // Center ball
    memset(&sta->ball,0,sizeof(ball));
    sta->ball.rad = BALL_RADIOUS;
    sta->ball.x = LEVEL_SIZE_X/2;
    sta->ball.y = LEVEL_SIZE_Y+2*BALL_RADIOUS;

    return sta;
}
// score* read(char* PATH){
//     score* new_text[5];
//     FILE* text = fopen(PATH, "r");
//     char *bufer[5];
//     int index = 0;
//     printf("\nTODO\n");
//     while (fgets(bufer,sizeof(bufer),text) != EOF){
//         printf("\nLo intenté\n");
//         printf("line-%i: %s", index, bufer);
//         strtok(bufer, "\n");
//         // char *token = strtok(bufer, " - ");
//         new_text[index]->name = "fabian";//&token[0];
//         new_text[index]->score =  4;//token[1]-'0';
//         index++;
//     }
//     printf("\nTODO\n");
//     fclose(text);
//     return *new_text;
// }
void ball_update(state *sta, ball *ball){

    // Remaining movement of the ball in this frame
    int move_x = (float) (ball->spd_x+signf(ball->spd_x)*0.95);
    int move_y = (int) (ball->spd_y);
    
    do{
        /* Move in X or in Y? */
        // square of the distance to dentiny if move in X
        float dist2_if_x = (move_x-signi(move_x))*(move_x-signi(move_x)) + move_y*move_y;
        // square of the distance to dentiny if move in Y
        float dist2_if_y = move_x*move_x + (move_y-signi(move_y))*(move_y-signi(move_y));

        // Current step
        int step_x = 0;
        int step_y = 0;
        if(dist2_if_x < dist2_if_y){
            step_x = signi(move_x);
        }else{
            step_y = signi(move_y);
        }

        // Move
        ball->x += step_x;
        ball->y += step_y;
        move_x -= step_x;
        move_y -= step_y;

        /* Collisions with game borders */
        if(ball->x - ball->rad < 0){
            if(sta->status!=STATUS_GAMEOVER)PlaySound(boing);
            // Bounce
            ball->x = ball->rad;
            if(ball->spd_x<0) ball->spd_x *= -1;
            // Stop current frame movement
            break;
        }
        if(ball->x + ball->rad >= LEVEL_SIZE_X){
            if(sta->status!=STATUS_GAMEOVER)PlaySound(boing);
            // Bounce
            ball->x = LEVEL_SIZE_X-ball->rad-1;
            if(ball->spd_x>0) ball->spd_x *= -1;
            // Stop current frame movement
            break;
        }
        if(ball->y - ball->rad < 0){
            if(sta->status!=STATUS_GAMEOVER)PlaySound(boing);
            // Bounce
            ball->y = ball->rad;
            if(ball->spd_y<0) ball->spd_y *= -1;
            // Stop current frame movement
            break;
        }

        /* Check if colliding with the palette */
        rectangle ballrect;
        ballrect.x = ball->x - ball->rad;
        ballrect.y = ball->y - ball->rad;
        ballrect.width = 2*ball->rad+1;
        ballrect.height = 2*ball->rad+1;
        
        if(rectangle_collision(ballrect,sta->palette)){
            if (IsKeyDown(KEY_X)&&!ball->glue){
                if (update_distance == 1) {
                    distance_x = abs(ball->x - sta->palette.x);
                    update_distance = 0;
                }
                ball->y = sta->palette.y - ball->rad;
                ball->x = sta->palette.x + distance_x;
            }
            else{
                if(update_distance==0){
                    ball->glue = 5;
                }
                else if (ball->glue>0)
                {
                    ball->glue -=1;
                }
                PlaySound(pop);
                update_distance = 1;
                // Move ball up
                ball->y = sta->palette.y - ball->rad - 1;
                // Reverse vertical speed
                if(ball->spd_y>0) ball->spd_y *= -1;

                // Change ball horizontal speed
                float pal_center_x = sta->palette.x + sta->palette.width/2.0;
                float drift = (ball->x-pal_center_x)/(sta->palette.width/2.0);
                ball->spd_x += drift*MAX_BALL_HORIZONTAL_SPEED_CHANGE;
                // Stop current frame movement
                break;
            }
        }

        /* Check if colliding with the level image */
        int col_top = 0;
        int col_bot = 0;
        int col_lef = 0;
        int col_rig = 0;
        for(int dy=-ball->rad; dy<=ball->rad; dy++){
            for(int dx=-ball->rad; dx<=ball->rad; dx++){
                // Get the block id in the current pixel of the ball
                int x = ball->x + dx;
                int y = ball->y + dy;
                int block_id = level_get_block_id(sta->level,y,x);
                if(block_id!=-1){
                    // Destroy the block
                    level_delete_a_block(sta->level,block_id);
                    PlaySound(bum);
                    sta->ball.block += 1;
                    // Increase collisions
                    if(dx==-ball->rad) col_lef = 1;
                    if(dx==+ball->rad) col_rig = 1;
                    if(dy==-ball->rad) col_top = 1;
                    if(dy==+ball->rad) col_bot = 1;
                }
            }
        }
        // Bounce
        if(col_lef && !col_rig){
            if(ball->spd_x<0) ball->spd_x *= -1;
        }
        if(col_rig && !col_lef){
            if(ball->spd_x>0) ball->spd_x *= -1;
        }
        if(col_top && !col_bot){
            if(ball->spd_y<0) ball->spd_y *= -1;
        }
        if(col_bot && !col_top){
            if(ball->spd_y>0) ball->spd_y *= -1;
        }
        // Stop current frame movement
        if(col_lef || col_rig || col_bot || col_top) break;

    }while(move_x!=0 || move_y!=0);

}

// Moves the state one frame forward in time
void state_update(state *sta){
    // Service ball
    if(sta->status==STATUS_WAITING && IsKeyPressed(KEY_SPACE)){
        sta->status = STATUS_PLAYING;
        sta->ball.spd_y = BALL_SPEED_Y;
    }

    // Game over detection
    if(IsKeyDown(KEY_C) && sta->ball.safety==0 && sta->ball.y > SCREEN_HEIGHT && sta->status != STATUS_GAMEOVER){
        time_previous = GetTime();
        // Move ball up
        sta->ball.y = SCREEN_HEIGHT - sta->ball.rad - 1;
        // Reverse vertical speed
        if(sta->ball.spd_y>0) sta->ball.spd_y *= -1;
        sta->ball.safety = 3;
    }
    else if(sta->ball.y > SCREEN_HEIGHT){
        sta->status = STATUS_GAMEOVER;
        if(sta->ball.y == SCREEN_HEIGHT)PlaySound(gameover);
        // if(sta->ball.y == SCREEN_HEIGHT+12){
        //     sta->ball.spd_y=0;
        //     sta->ball.spd_x=0;
        // }
    }
    double time_now = GetTime();
    if(sta->ball.safety>0 && abs(time_now-time_previous)>=1.0){
        time_previous = GetTime();
        sta->ball.safety -= 1;
    }


    // Move palette
    if(IsKeyDown(KEY_LEFT)) sta->palette.x -= PALETTE_SPEED_X;
    if(IsKeyDown(KEY_RIGHT)) sta->palette.x += PALETTE_SPEED_X;
    if(IsKeyPressed(KEY_LEFT)) sta->palette.x += (PALETTE_SPEED_X+1)/2;
    if(IsKeyPressed(KEY_RIGHT)) sta->palette.x -= (PALETTE_SPEED_X+1)/2;
    if(sta->palette.x<0) sta->palette.x = 0;
    if(sta->palette.x > LEVEL_SIZE_X - sta->palette.width) sta->palette.x = LEVEL_SIZE_X - sta->palette.width;
    
    // Update ball
    ball_update(sta,&sta->ball);
}

// Free a state
void state_free(state *sta){
    // Free level memory
    level_free(sta->level);
    // Free the state itself
    free(sta);
}
