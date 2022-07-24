#include "yellow_ghost.h"

yellow_ghost::yellow_ghost(board* board) : ghost(board, yellow) {
  ghostSpriteY = 16 * 8;
  changeSprite();
}

yellow_ghost::~yellow_ghost() {}
