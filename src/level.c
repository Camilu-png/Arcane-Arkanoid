#include "level.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Treshold of when to consider a pixel as part of a block
#define ALPHA_TRESHOLD 14


void flood_fill(level *lev, Color *pixels, int cy, int cx, int value){
    // If already of the target value, skip
    if(lev->block_id[cy][cx]==value) return;

    // Set the group fo this pixel to the target value
    lev->block_id[cy][cx] = value;

    for(int d=0;d<4;d++){
        // Neighbor position
        int nx = cx + (d==0) - (d==2);
        int ny = cy + (d==1) - (d==3);

        // Check if neighbor is inside
        if(nx<0 || ny<0 || nx>=lev->size_x || ny>=lev->size_y) continue;

        // Check that the neighbor isn't already of the target value
        if(lev->block_id[ny][nx]==value) continue;

        // Neighbor color
        Color ncolor = pixels[lev->size_x*ny+nx];

        if(ncolor.a > ALPHA_TRESHOLD){
            // Spread value
            flood_fill(lev,pixels,ny,nx,value);
        }
    }
}


// Load a level from an image file
level *level_load(const char *fname, const char *names, int size_y, int size_x){
    // Initialize new level
    level *lev = malloc(sizeof(level));
    assert(lev!=NULL);
    memset(lev,0,sizeof(level));
    lev->name = names;
    lev->size_x = size_x;
    lev->size_y = size_y;
    //High Score
    // text = fopen("levels/scores.txt", "rt");
    if(strcmp(names, "Marcianito 100% real")==0){
        lev->high_scores = 3;
        }
    if(strcmp(names, "Cerebro")==0)lev->high_scores = 10;
    if(strcmp(names, "Shrek")==0)lev->high_scores = 5;
    if(strcmp(names, "Robi")==0)lev->high_scores = 20;

    // Allocate block_id matrix
    lev->block_id = malloc(sizeof(int *)*size_y);
    assert(lev->block_id!=NULL);
    for(int y=0;y<size_y;y++){
        lev->block_id[y] = malloc(sizeof(int)*size_x);
        assert(lev->block_id[y]!=NULL);
    }

    // Fill matrix with zeros
    for(int y=0;y<size_y;y++){
        for(int x=0;x<size_x;x++){
            lev->block_id[y][x] = -1;
        }
    }
    lev->blocks_remaining = 0;

    // Generate transparent image
    lev->image = GenImageColor(size_x,size_y,(Color){0,0,0,0});

    { // Load image and put it into the level image
        Image loaded_img = LoadImage(fname);

        int start_x = (lev->image.width - loaded_img.width)/2;
        int start_y = (lev->image.height - loaded_img.height)/2;
        if(start_y>0) start_y = 0;

        Rectangle src_rect;
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = loaded_img.width;
        src_rect.height = loaded_img.height;

        Rectangle dst_rect;
        dst_rect.x = start_x;
        dst_rect.y = start_y;
        dst_rect.width = loaded_img.width;
        dst_rect.height = loaded_img.height;

        ImageDraw(&lev->image,loaded_img,src_rect,dst_rect,WHITE);

        //
        UnloadImage(loaded_img);
    }

    // Copy the image in GPU
    lev->texture = LoadTextureFromImage(lev->image);

    { // Detect block ids
        // Number of groups
        int n_groups = 0;

        // Ask for array of pixels
        Color *pixels = LoadImageColors(lev->image);

        for(int y=0;y<lev->size_y;y++){
            for(int x=0;x<lev->size_x;x++){
                // Pixel color
                Color color = pixels[lev->size_x*y+x];
                if(lev->block_id[y][x]==-1 && color.a>ALPHA_TRESHOLD){
                    flood_fill(lev,pixels,y,x,n_groups);
                    n_groups += 1;
                }
            }
        }
        lev->blocks_remaining = n_groups;
        printf("blocks_remaining %d\n",lev->blocks_remaining);

        // v
        UnloadImageColors(pixels);
    }

    // v
    return lev;
}

// Get the block_id of a given pixel
int level_get_block_id(const level *lev, int y, int x){
    if(x<0 || y<0 || x>=lev->size_x || y>=lev->size_y) return -1;
    return lev->block_id[y][x];
}

// Delete a block of pixels given its id
// The level texture is updated
void level_delete_a_block(level *lev, int id){
    if(id==-1) return;

    // If any pixel was deleted
    int any_deleted = 0;
    // Detect all pixels with the given id
    for(int y=0;y<lev->size_y;y++){
        for(int x=0;x<lev->size_x;x++){
            if(lev->block_id[y][x]==id){
                // Make the pixel transparent
                ImageDrawPixel(&lev->image,x,y,(Color){0,0,0,0});
                // Set the pixel block_id to -1
                lev->block_id[y][x] = -1;
                // v
                any_deleted = 1;
            }
        }
    }
    // Update remaining blocks counter
    if(any_deleted) lev->blocks_remaining -= 1;

    // Update texture
    UpdateTexture(lev->texture,lev->image.data);
}

// Free a level from memory
void level_free(level *lev){
    // Free block_id matrix
    for(int y=0;y<lev->size_y;y++) free(lev->block_id[y]);
    free(lev->block_id);
    // v
    UnloadImage(lev->image);
    // v
    UnloadTexture(lev->texture);
    // Free the level
    free(lev);
}
