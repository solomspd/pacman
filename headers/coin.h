#pragma once
#include <SFML/Graphics.hpp>

class coin {
 private:
  sf::RectangleShape body;

 public:
  coin();
  int get_position_x();
  int get_position_y();
  void set_position_x(int);
  void set_position_y(int);
  void set_postion(int, int);
};