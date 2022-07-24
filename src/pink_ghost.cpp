#include "pink_ghost.h"

pink_ghost::pink_ghost(board* board) : ghost(board, pink) {
  ghostSpriteY = 16 * 6;
  changeSprite();
}

pink_ghost::~pink_ghost() {}
