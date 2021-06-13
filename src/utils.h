#ifndef ARKAVOID_UTILS_H
#define ARKAVOID_UTILS_H

// A rectangle with integer coordinates
typedef struct {
    int x,y,width,height;
} rectangle;

// Wheter two rectangles are colliding
static inline int rectangle_collision(rectangle r1, rectangle r2){
    if(r1.x+r1.width <= r2.x) return 0;
    if(r2.x+r2.width <= r1.x) return 0;
    if(r1.y+r1.height <= r2.y) return 0;
    if(r2.y+r2.height <= r1.y) return 0;
    return 1;
}

// The sign function for ints
static inline int signi(int x){
    return x==0? 0: (x>0? +1 : -1);
}

// The sign function for floats
static inline float signf(float x){
    return x==0? 0: (x>0? +1 : -1);
}

#endif
