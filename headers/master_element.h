#pragma once

#include <SFML/Graphics.hpp>

#include "AnimatedSprite.h"
#include "Screen.h"
#include "board.h"

const float default_speed = 0.1;

class master_element {
 private:
  float x, y, step;
  sf::RectangleShape collision;
  // sf::RectangleShape body;
 protected:
  AnimatedSprite body;

 public:
  master_element();
  master_element(board*, tile_types);
  float get_x();
  float get_y();
  void render(sf::RenderTexture*);
  void set_pos(float, float);
  void set_pos_x(float);
  void set_pos_y(float);
  void move(direc);
  void set_step(float);
  float get_step();
  void set_default_speed();
  void update_shape();
  ~master_element();
  void reset_pos(board* board, tile_types p);
};