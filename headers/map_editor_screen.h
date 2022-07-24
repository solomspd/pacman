#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Screen.h"
#include "board.h"
#include "interaction_box.h"

const int inter_num = 8;

class map_editor_screen : public Screen {
 private:
  board editor_board;
  interaction_box type_selector[inter_num];
  sf::Texture tex, curr_tex;
  sf::Sprite curr_sprite;
  int mouse_x, mouse_y, corr_x, corr_y;
  sf::RectangleShape background;
  sf::RectangleShape temp_box;
  sf::RectangleShape mouse_coll;
  tile_types curr_type;
  sf::RenderWindow* temp_window;
  sf::Color curr_color;
  sf::Text sf_file_name;
  std::string file_name;
  int custom_map_num;
  std::ifstream map_index;
  sf::Font* font;
  ScreenName nextscreen;
  std::vector<std::string> index;
  std::string orig_name;
  sf::Text dupl_msg;
  bool flag;

 public:
  map_editor_screen(sf::Font*);
  void handleEvent(sf::Event*);
  void render(sf::RenderTexture*);
  void mouse_update(sf::RenderWindow&);
  void grid(sf::RenderTexture*);
  void reverse_parse();
  void init();
  void set_curr_type(int);
  bool check_dupl_map();
  void import_index();
  ScreenName update();
  void dupl_map_msg();
  ~map_editor_screen();
};
