#pragma once

#include <cmath>
#include <fstream>
#include <string>

#include "Screen.h"
#include "interaction_box.h"

const int page_maps = 20;
const int type_num = 2;

class map_select_screen : public Screen {
 private:
  sf::Text *maps;
  int custom_page_num, base_page_num;
  /*int *custom_per_page_num;
  int *base_per_page_num;*/
  int *per_page_num;
  sf::Text page;
  sf::RectangleShape test;
  sf::Font *font;
  int map_num, custom_num, base_num;
  std::ifstream in_file_index;
  int page_num, curr_page;
  int progression, selection;
  ScreenName nextScreen;
  sf::Text page_type[type_num];
  int custom_page;

 public:
  map_select_screen(sf::Font *);
  void init();
  void handleEvent(sf::Event *);
  void select();
  ScreenName update();
  void render(sf::RenderTexture *);
  void test_file();
  void next_page();
  void previous_page();
  void refresh_page_num();
  int accumu(int);
  ~map_select_screen();
};
