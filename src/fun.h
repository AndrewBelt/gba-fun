#ifndef FUN_H
#define FUN_H

typedef struct {
  u8 px, py;
} game_t;

extern game_t game;

void gameRun();
void gameReset();
void gameStep();

#endif