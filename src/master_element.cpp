#include <cmath>

#include "master_element.h"

master_element::master_element() {}

master_element::master_element(board* board, tile_types querry) {
  // x = 0.0;
  // y = 0.0;
  int temp_x, temp_y;
  step = default_speed;
  board->find_tile(temp_x, temp_y, querry);
  x = temp_x;
  y = temp_y + 0.5;
  // body.setSize(sf::Vector2f(tile_size , tile_size ));
  // body.setFillColor(sf::Color::Yellow);
  collision.setSize(sf::Vector2f(tile_size, tile_size));
  update_shape();
  body.setOrigin(tile_size, tile_size);
  // displaying is for testing purposses
  // collision.setFillColor(sf::Color::Blue);
  body.setTexture(*(board->getSpritesheet()));
}

float master_element::get_x() { return x; }

float master_element::get_y() { return y; }

void master_element::render(sf::RenderTexture* renderer) {
  // drawing collision box for testing purposes
  renderer->draw(body);
  // renderer->draw(collision);
}

void master_element::set_pos_x(float new_x) {
  x = new_x;
  update_shape();
}

void master_element::set_pos_y(float new_y) {
  y = new_y;
  update_shape();
}

void master_element::set_pos(float new_x, float new_y) {
  x = new_x;
  y = new_y;
  update_shape();
}

void master_element::set_step(float new_step) { step = new_step; }

float master_element::get_step() { return step; }

void master_element::update_shape() {
  // body.setPosition(y*tile_size - tile_size * 2, x*tile_size - tile_size -
  // tile_size / 2); collision.setPosition(y * tile_size - tile_size -
  // tile_size / 2, x* tile_size - tile_size);
  body.setPosition(x * tile_size, y * tile_size);
  collision.setPosition(x * tile_size, y * tile_size);
}

void master_element::move(direc new_direc) {
  switch (new_direc) {
    case (up):
      y -= step;
      y *= 10;
      y = round(y);
      y /= 10.0;
      break;
    case (down):
      y += step;
      y *= 10;
      y = round(y);
      y /= 10.0;
      break;
    case (right):
      x += step;
      x *= 10;
      x = round(x);
      x /= 10.0;
      break;
    case (left):
      x -= step;
      x *= 10;
      x = round(x);
      x /= 10.0;
      break;
  }
  update_shape();
}

void master_element::set_default_speed() { step = default_speed; }

master_element::~master_element() {}

void master_element::reset_pos(board* board, tile_types p) {
  float x, y;
  board->find_position(x, y, p);
  set_pos(x, y + 0.5);
}
