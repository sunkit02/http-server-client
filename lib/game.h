#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

typedef struct __attribute__((__packed__)) {
    int32_t id;
    bool hit;
} Player;
// typedef struct {
//     int id;
//     bool hit;
// } Player;

#endif // !GAME_H
