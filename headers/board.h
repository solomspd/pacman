#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "tile.h"

enum direc { up, down, left, right };

const int htiles = 28, vtiles = 31;

class board {
 private:
  tile map[vtiles][htiles];
  sf::Texture* spritesheet;
  sf::RenderTexture wallsRendered;
  sf::String boardName;

 public:
  int numOfCoins = 0;
  bool* useSFX;
  board();
  board(std::ifstream&, sf::Texture*, bool*);
  void find_tile(int&, int&, tile_types);
  tile_types get_tile_type(int, int);
  void set_tile_type(int, int, tile_types);
  void update_sprites();
  void render(sf::RenderTexture*);
  void switch_coin(int, int);
  void test_board();
  void render_alternate(sf::RenderTexture*);
  void setBoardName(sf::String);
  sf::String getBoardName();
  sf::Texture* getSpritesheet();
  void find_position(float& x, float& y, tile_types p);
  ~board();
};