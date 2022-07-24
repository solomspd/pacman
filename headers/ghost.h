#pragma once

#include <SFML/Graphics.hpp>

#include "master_element.h"

class ghost : public master_element {
 private:
  direc curr_direc;
  direc next_direc;
  int animationTick = 0;

 protected:
  int ghostSpriteY;  // y coordinate of ghost sprite in spritesheet
  bool isEdible = false;

 public:
  ghost(board*, tile_types);
  void changeSprite();
  void handleEvent_ghost(board*);
  bool wall_block(board*);
  void change_direction(board*);
  void setEdible(bool);
};