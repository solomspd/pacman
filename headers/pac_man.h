#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "board.h"
#include "ghost.h"
#include "master_element.h"

const int score_inc = 10;  // requires balance
const int score_big_coin = 50;
const int score_eat_ghost = 200;

class pac_man : public master_element {
 private:
  direc curr_direc;
  direc next_direc;
  int *score;
  int *lives;
  int animationTick = 0;
  bool eatghoststemp = false;
  sf::SoundBuffer chomp, death;
  sf::Sound pacman_chomp, pacman_death;
  bool *useSFX;
  bool play_chomp, play_death;

 public:
  pac_man(board *, int *, int *, bool *);
  void handleEvent_pac(sf::Event *, board *);
  void coin_collision(board *, int &);
  void ghost_collision(board *, ghost *);
  bool eat_ghosts(board *board, ghost *ghost);
  bool eat_ghosts_flag();
  void reset_eat_ghosts_flag();
  bool wall_block(board *);
  void movepac(board *board);
  // void render(sf::RenderTexture*);
  ~pac_man();
};
