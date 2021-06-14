#ifndef ARKAVOID_LEVEL_H
#define ARKAVOID_LEVEL_H

#include <raylib.h>

typedef struct  {
    char* name;
    int score;
}score;

typedef struct {
    // Size of the level (and the block matrix)
    int size_y, size_x;
    // Matrix of block indexes for each pixel
    int **block_id;
    // Number of remaining blocks
    int blocks_remaining;
    // Current aimage (stored in CPU)
    Image image;
    // Current texture (stored in GPU)
    Texture2D texture;
    // Name of the level
    const char* name;
    score high_scores;
} level;


// Load a level from an image file
level *level_load(const char *fname,const char *names, int size_y, int size_x);

// Get the block_id of a given pixel
int level_get_block_id(const level *lev, int y, int x);

// Delete a block of pixels given its id
// The level texture is updated
void level_delete_a_block(level *lev, int id);

// Free a level from memory
void level_free(level *lev);

#endif
