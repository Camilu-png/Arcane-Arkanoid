#include <stdlib.h>

#include "draw.h"

const int MESSAGE_FONT_SIZE = 24;

void state_draw(const state *state){
    // Draw game area
    DrawRectangle(0,0,LEVEL_SIZE_X,SCREEN_HEIGHT,MAROON);
    DrawRectangleGradientV(4,4,LEVEL_SIZE_X-8,SCREEN_HEIGHT-8,LIGHTGRAY,RED);

    // Draw level image
    DrawTexture(state->level->texture,0,0,WHITE);

    // Draw palette
    rectangle pal = state->palette;
    DrawRectangle(pal.x,pal.y,pal.width,pal.height,WHITE);

    // Draw ball
    ball ba = state->ball;
    DrawRectangle(ba.x-ba.rad,ba.y-ba.rad,2*ba.rad+1,2*ba.rad+1,WHITE);

    /* Draw message */
    const char *message = "";
    if(state->status==STATUS_WAITING){
        message = "PRESS SPACE TO SERVE";
    }else if(state->status==STATUS_GAMEOVER){
        message = "GAME OVER";
    }
        // message = state->fname;

    // Center text message
    int text_width = MeasureText(message,MESSAGE_FONT_SIZE);
    int rx = rand()%5-2;
    int ry = rand()%5-2;
    DrawText(message,LEVEL_SIZE_X/2-text_width/2+rx,LEVEL_SIZE_Y+20+ry,MESSAGE_FONT_SIZE,WHITE);
    
    //Draw score
    DrawText(TextFormat("SCORE: %i", 10), LEVEL_SIZE_X+8, 130, 20, MAROON);
    DrawText(TextFormat("HI-SCORE: %i", 10), LEVEL_SIZE_X+8, 200, 30, BLACK);
    DrawText(TextFormat("Glue: %i", state->ball.glue), LEVEL_SIZE_X+8, 230,15, BLACK);
    DrawText(TextFormat("Safety floor: %i", state->ball.safety), LEVEL_SIZE_X+8, 245,15, BLACK);
}
