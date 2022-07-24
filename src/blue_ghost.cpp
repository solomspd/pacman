#include "blue_ghost.h"

blue_ghost::blue_ghost(board* board) : ghost(board, blue) {
  ghostSpriteY = 16 * 7;
  changeSprite();
}

blue_ghost::~blue_ghost() {}
