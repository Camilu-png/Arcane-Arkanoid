#ifndef ARKAVOID_CONSTANTS_H
#define ARKAVOID_CONSTANTS_H

// Horizontal size of the level image
#define LEVEL_SIZE_X 400

// Maximum height of the level image
#define LEVEL_SIZE_Y 400

// Horizontal size of the hud
#define HUD_SIZE_X 240

// Width of the screen
#define SCREEN_WIDTH (LEVEL_SIZE_X+HUD_SIZE_X)

// Height of the screen
#define SCREEN_HEIGHT 600

//Sound when the ball bounces off the wall
#define boing LoadSound("levels/sounds/boing.mp3")

//Sound when the ball bounces off the palette
#define pop LoadSound("levels/sounds/pop.mp3")

//Sound when the ball bounces off the image
#define bum LoadSound("levels/sounds/bum.mp3")

//Sound Game Over
#define gameover LoadSound("levels/sounds/gameover.mp3")

#endif
