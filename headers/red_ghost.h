#pragma once
#include <SFML/Graphics.hpp>

#include "board.h"
#include "ghost.h"
#include "master_element.h"

class red_ghost : public ghost {
 private:
  direc curr_direc;
  direc next_direc;

 public:
  red_ghost(board*);
  ~red_ghost();
};