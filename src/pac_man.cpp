
// inclusing iostream for testing purposes

#include "pac_man.h"

#include <iostream>
#include <cmath>

pac_man::pac_man(board* board, int* score, int* lives, bool* SFX)
    : master_element(board, pacman) {
  this->lives = lives;
  this->score = score;
  body.setAnim(LOOPF, 0, 0, 16, 16, 3, true);
  death.loadFromFile("pacman_death.wav");
  chomp.loadFromFile("pacman_chomp.wav");
  pacman_chomp.setBuffer(chomp);
  pacman_death.setBuffer(death);

  this->useSFX = SFX;
  play_chomp = *useSFX, play_death = *useSFX;
}

bool pac_man::wall_block(board* board) {
  int temp_x = get_x(), temp_y = get_y();
  switch (curr_direc) {
    case (up):
      if (board->get_tile_type(temp_x, temp_y - 1) == wall &&
          (get_y() - temp_y <= 0.5)) {
        return true;
      } else
        return false;
      break;
    case (down):
      if (board->get_tile_type(temp_x, temp_y + 1) == wall &&
          (temp_y + 1 - get_y() <= 0.5)) {
        return true;
      } else
        return false;
      break;
    case (right):
      if (board->get_tile_type(temp_x + 1, temp_y) == wall &&
          (temp_x + 1 - get_x() <= 0.5)) {
        return true;
      } else
        return false;
      break;
    case (left):
      if (board->get_tile_type(temp_x - 1, temp_y) == wall &&
          (get_x() - temp_x <= 0.5)) {
        return true;
      } else
        return false;
      break;
  }
}

void pac_man::movepac(board* board) {
  int temp_x = get_x(), temp_y = get_y();
  float originalx, originaly;
  board->find_position(originalx, originaly, pacman);
  bool collide = wall_block(board);

  if (next_direc != curr_direc &&
      (get_x() - temp_x == 0.5 && get_y() - temp_y == 0.5 ||
       (temp_x == int(originalx) && temp_y == int(originaly)))) {
    direc temp = curr_direc;
    curr_direc = next_direc;

    if (wall_block(board)) {
      curr_direc = temp;
      // next_direc = temp;	//comment this out for smoothness
    } else {
      // this changes the sprite animation based on the new direction
      // (only when the direction changes)
      switch (next_direc) {
        case (up):
          body.setAnim(LOOPF, 0, 16 * 2, 16, 16, 3, true);
          break;
        case (down):
          body.setAnim(LOOPF, 0, 16 * 3, 16, 16, 3, true);
          break;
        case (left):
          body.setAnim(LOOPF, 0, 0, 16, 16, 3, true);
          break;
        case (right):
          body.setAnim(LOOPF, 0, 16, 16, 16, 3, true);
          break;
      }
    }
  }

  if (!collide) {
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
    // next frame in the animation every other update, only when pacman is
    // moving
    if (animationTick == 3) {
      body.nextFrame();
      animationTick = 0;
    } else {
      animationTick++;
    }
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
  // test up and down later if new map requires it.
}

void pac_man::handleEvent_pac(sf::Event* key_press, board* board) {
  if (key_press->type == sf::Event::KeyPressed) switch (key_press->key.code) {
      case (sf::Keyboard::Up):
        next_direc = up;
        break;
      case (sf::Keyboard::Down):
        next_direc = down;
        break;
      case (sf::Keyboard::Left):
        next_direc = left;
        break;
      case (sf::Keyboard::Right):
        next_direc = right;
        break;
    }
}

void pac_man::coin_collision(board* board, int& countdown) {
  int temp_x = get_x();
  int temp_y = get_y();
  // small coin
  if (board->get_tile_type(temp_x, temp_y) == coin) {
    board->switch_coin(temp_x, temp_y);
    (*score) += score_inc;
    if (play_chomp) {
      pacman_chomp.setLoop(false);
      pacman_chomp.play();
      play_chomp = false;
    }
  }
  // big coin
  else if (board->get_tile_type(temp_x, temp_y) == big_coin) {
    board->switch_coin(temp_x, temp_y);
    (*score) += score_big_coin;
    eatghoststemp = true;
    countdown = 150;
  }
  play_chomp = *useSFX;
}

void pac_man::ghost_collision(board* board, ghost* ghost) {
  int temp_x = get_x(), temp_y = get_y();
  if (fabs(get_x() - ghost->get_x()) <= 0.4 &&
      fabs(get_y() - ghost->get_y()) <= 0.4) {
    (*lives)--;
    reset_pos(board, pacman);
    if (play_death) {
      pacman_death.setLoop(false);
      pacman_death.play();
      play_death = false;
    }
  }
  play_death = *useSFX;
}

bool pac_man::eat_ghosts(board* board, ghost* ghost) {
  int temp_x = get_x(), temp_y = get_y();
  if (fabs(get_x() - ghost->get_x()) <= 0.4 &&
      fabs(get_y() - ghost->get_y()) <= 0.4) {
    (*score) += score_eat_ghost;
    return true;
  } else
    return false;
}
bool pac_man::eat_ghosts_flag() { return eatghoststemp; }

void pac_man::reset_eat_ghosts_flag() { eatghoststemp = false; }

pac_man::~pac_man() {}
