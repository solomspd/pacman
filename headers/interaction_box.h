#pragma once

#include <SFML/Graphics.hpp>

class interaction_box {
 private:
  sf::RectangleShape body;
  sf::Texture tex;
  sf::Text text;
  sf::Sprite sprite;
  bool select, hover;
  int x, y;

 public:
  interaction_box();
  void set_size(int, int);
  bool check_mouse_coll(sf::RectangleShape);
  void set_position(int, int);
  void toggle_hover();
  void toggle_hover(bool);
  bool check_mouse_click(sf::Event*, sf::RectangleShape);
  void toggle_select(bool);
  int get_x();
  int get_y();
  void set_texture(sf::Texture);
  void still(int);
  void draw(sf::RenderTexture*);
  void toggle_select();
  ~interaction_box();
};
