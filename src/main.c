#include <raylib.h>

#include "state.h"
#include "draw.h"
#include "constants.h"
#include <dirent.h> 

int main(int argc, const char *args[]){
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"ARKAVOID");

    InitAudioDevice();      // Initialize audio device

    Sound music = LoadSound("levels/sounds/fondo.mp3");
    PlaySound(music);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // All levels
    const char* levels[4] = {"levels/marcianito_100_real.png","levels/cerebro.png","levels/shrek.png","levels/robi.png"};
    const char* names[4] = {"Marcianito 100% real","Cerebro","Shrek","Robi"};
    int index_level = 0;
    // score* text = read("levels/scores.txt");

    while (!WindowShouldClose()){
        // Load level image
        state *sta = state_new(levels[index_level], names[index_level]);
        

        // The game should restart?
        int restart = 0;

        // Main game loop
        while (!WindowShouldClose() && !restart){    // Detect window close button or ESC key
            // Update
            state_update(sta);

            // Restart logic
            if(sta->status==STATUS_GAMEOVER && IsKeyPressed(KEY_SPACE)){
                restart = 1;
                StopSound(music);
                PlaySound(music);
                
            }
            else if(IsKeyPressed(KEY_F1)){
                if(index_level < 3){
                    index_level += 1;
                    sta = state_new(levels[index_level], names[index_level]);
                }
                else{
                    index_level = 0;
                    sta = state_new(levels[index_level], names[index_level]);
                }
                
                state_update(sta);
                StopSound(music);
                PlaySound(music);
            }

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
                // Clear background
                ClearBackground(GRAY);

                // Draw state
                state_draw(sta);

            EndDrawing();
            //----------------------------------------------------------------------------------

        }
        state_free(sta);
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
