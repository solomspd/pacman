#include "red_ghost.h"

red_ghost::red_ghost(board* board) : ghost(board, red) {
  ghostSpriteY = 16 * 5;
  changeSprite();
}

red_ghost::~red_ghost() {}