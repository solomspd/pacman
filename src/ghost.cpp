#include "ghost.h"

ghost::ghost(board* board, tile_types type) : master_element(board, type) {}

void ghost::changeSprite() {
  if (!isEdible) {
    switch (curr_direc) {
      case up:
        body.setAnim(LOOPF, 0, ghostSpriteY, 16, 16, 2, true);
        ;
        break;
      case down:
        body.setAnim(LOOPF, 16 * 2, ghostSpriteY, 16, 16, 2, true);
        break;
      case right:
        body.setAnim(LOOPF, 16 * 6, ghostSpriteY, 16, 16, 2, true);
        break;
      case left:
        body.setAnim(LOOPF, 16 * 4, ghostSpriteY, 16, 16, 2, true);
        break;
    }
  } else {
    body.setAnim(LOOPF, 0, 16 * 9, 16, 16, 2, true);
  }
}

void ghost::setEdible(bool edible) {
  this->isEdible = edible;
  changeSprite();
}

bool ghost::wall_block(board* board) {
  int temp_x = get_x(), temp_y = get_y();
  switch (curr_direc) {
    case (up):
      if (board->get_tile_type(temp_x, temp_y - 1) == wall &&
          (get_y() - temp_y == 0.5)) {
        return true;
      } else
        return false;
      break;
    case (down):
      if (board->get_tile_type(temp_x, temp_y + 1) == wall &&
          (temp_y + 1 - get_y() == 0.5)) {
        return true;
      } else
        return false;
      break;
    case (right):
      if (board->get_tile_type(temp_x + 1, temp_y) == wall &&
          (temp_x + 1 - get_x() == 0.5)) {
        return true;
      } else
        return false;
      break;
    case (left):
      if (board->get_tile_type(temp_x - 1, temp_y) == wall &&
          (get_x() - temp_x == 0.5)) {
        return true;
      } else
        return false;
      break;
  }
}

void ghost::change_direction(board* board) {
  int temp_x = get_x(), temp_y = get_y();

  int moves = rand() % 4;

  switch (moves) {
    case 1:
      curr_direc = up;
      break;
    case 2:
      curr_direc = down;
      break;
    case 3:
      curr_direc = right;
      break;
    case 0:
      curr_direc = left;
      break;
  }
  changeSprite();
}

void ghost::handleEvent_ghost(board* board) {
  if (animationTick == 2) {
    body.nextFrame();
    animationTick = 0;
  }
  animationTick++;
  if (!wall_block(board)) {
    switch (curr_direc) {
      case (up):
        move(up);
        break;
      case (down):
        move(down);
        break;
      case (left):
        move(left);
        break;
      case (right):
        move(right);
        break;
    }
  } else {
    change_direction(board);
  }
  if (get_x() == 0 || get_x() <= 0.5)  // left
  {
    set_pos(get_x() + htiles - 1.5, get_y());
  }
  if (get_x() >= htiles - 0.5 || get_x() == htiles)  // right
  {
    set_pos(get_x() - htiles + 1, get_y());
  }
  if (get_y() == 0 || get_y() <= 0.5)  // up
  {
    set_pos(get_x(), get_y() + vtiles - 1.5);
  }
  if (get_y() >= vtiles - 0.5 || get_y() == vtiles)  // down
  {
    set_pos(get_x(), get_y() - vtiles + 1);
  }
}