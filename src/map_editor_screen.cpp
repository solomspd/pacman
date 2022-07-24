#include "map_editor_screen.h"

map_editor_screen::map_editor_screen(sf::Font* f) {
  nextscreen = MAPEDITOR;
  mouse_coll.setFillColor(sf::Color::Transparent);
  mouse_coll.setSize(sf::Vector2f(1, 1));
  mouse_x = 0;
  mouse_y = 0;
  background.setFillColor(sf::Color::White);
  background.setSize(sf::Vector2f(224, 249));
  background.setPosition(0, 0);
  font = f;
  sf_file_name.setFont(*font);
  sf_file_name.setFillColor(sf::Color::White);
  sf_file_name.setCharacterSize(8);
  dupl_msg.setCharacterSize(6);
  dupl_msg.setFillColor(sf::Color::Red);
  dupl_msg.setFont(*font);
  dupl_msg.setString("Saving will overwrite file");
  dupl_msg.setPosition(int(112 - dupl_msg.getLocalBounds().width / 2), 267);
  for (int i = 0; i < inter_num; i++) {
    type_selector[i].still(i);
    type_selector[i].set_position(i * 20 + 4, 252);
  }
  tex.loadFromFile("../assets/textures/spritesheet.png");
  curr_sprite.setTexture(tex);
}

void map_editor_screen::init() {
  nextscreen = MAPEDITOR;
  set_curr_type(7);
  std::ifstream custom_num_in;
  custom_num_in.open("custom_map_num.txt");
  if (custom_num_in.is_open()) {
    custom_num_in >> custom_map_num;
  } else {
    // error function
  }
  orig_name = "Custom_map_" + std::to_string(custom_map_num + 1);
  // file_name = orig_name;
  sf_file_name.setString(orig_name);
  custom_num_in.close();
  import_index();
  flag = false;
}

void map_editor_screen::handleEvent(sf::Event* event) {
  temp_box.setPosition(((corr_x * tile_size) + 1), ((corr_y * tile_size) + 1));
  temp_box.setSize(sf::Vector2f(tile_size - 2, tile_size - 2));
  // std::cout << "x: " << mouse_x << " y: " << mouse_y << std::endl << "cx: "
  // << corr_x << " cy: " << corr_y << std::endl;
  temp_box.setFillColor(curr_color);
  for (int i = 0; i < 7; i++) {
    // std::cout << i << " : x: " << type_selector[i].get_x() << " y: " <<
    // type_selector[i].get_y() << std::endl << "mx: " << corr_x << " my: "
    // << corr_y;
    type_selector[i].check_mouse_coll(mouse_coll);
  }
  if (event->type == sf::Event::KeyPressed) {
    if (file_name.size() > 1) {
      if (event->key.code == sf::Keyboard::BackSpace) {
        file_name.pop_back();
        if (flag) {
          file_name.pop_back();
        }
        flag = true;
      }
    }
    if (event->key.code == sf::Keyboard::Escape) {
      nextscreen = MENU;
    }
    if (event->key.code == sf::Keyboard::Return) {
      reverse_parse();
    }
  }
  if (event->type == sf::Event::TextEntered) {
    if (file_name.size() < 22) {
      file_name += event->text.unicode;
      sf_file_name.setString(file_name);
    }
  }

  if (event->type = sf::Event::MouseButtonPressed) {
    if (event->key.code == sf::Mouse::Left) {
      editor_board.set_tile_type(corr_y, corr_x, curr_type);
      for (int i = 0; i < inter_num; i++) {
        if (type_selector[i].check_mouse_click(event, mouse_coll)) {
          set_curr_type(i);
        }
      }
      // editor_board.update_sprites();
    } else if (event->key.code == sf::Mouse::Right) {
      editor_board.set_tile_type(corr_y, corr_x, empty);
    }
    // editor_board.update_sprites();
  }
}

void map_editor_screen::set_curr_type(int new_type) {
  // temp_box.setScale(1, 1);
  switch (new_type) {
    case (pacman):
      curr_type = pacman;
      temp_box.setFillColor(sf::Color::Yellow);
      break;
    case (red):
      curr_type = red;
      temp_box.setFillColor(sf::Color::Red);
      break;
    case (yellow):
      curr_type = yellow;
      temp_box.setFillColor(sf::Color::Yellow);
      break;
    case (pink):
      curr_type = pink;
      temp_box.setFillColor(sf::Color::Magenta);
      break;
    case (blue):
      curr_type = blue;
      temp_box.setFillColor(sf::Color::Cyan);
      break;
    case (coin):
      curr_type = coin;
      temp_box.setFillColor(sf::Color::Green);
      temp_box.setScale(0.5, 0.5);
      break;
    case (big_coin):
      curr_type = big_coin;
      temp_box.setFillColor(sf::Color::Green);
      break;
    case (wall):
      curr_type = wall;
      temp_box.setFillColor(sf::Color::Blue);
      break;
  }
}

void map_editor_screen::reverse_parse() {
  if (file_name == orig_name) {
    std::ofstream custom_num_out;
    custom_num_out.open("custom_map_num.txt");
    custom_num_out << custom_map_num + 1;
    custom_num_out.close();
  }
  if (check_dupl_map()) {
    index.push_back(file_name);
    std::ofstream index_out;
    index_out.open("map_index.txt");
    for (int i = 0; i < index.size(); i++) {
      index_out << index[i];
      index_out << std::endl;
    }
    index_out.close();
  }
  file_name = sf_file_name.getString();
  std::string temp_name;
  temp_name = file_name + ".txt";
  std::ofstream out_file;
  out_file.open(temp_name.c_str());
  if (out_file.is_open()) {
    for (int i = 0; i < vtiles; i++) {
      for (int j = 0; j < htiles; j++) {
        switch (editor_board.get_tile_type(j, i)) {
          case (wall):
            out_file << 'W';
            break;
          case (empty):
            out_file << 'E';
            break;
          case (coin):
            out_file << 'C';
            break;
          case (big_coin):
            out_file << 'S';
            break;
          case (pacman):
            out_file << 'M';
            break;
          case (pink):
            out_file << 'P';
            break;
          case (red):
            out_file << 'R';
            break;
          case (yellow):
            out_file << 'Y';
            break;
          case (blue):
            out_file << 'B';
            break;
        }
      }
      out_file << std::endl;
    }
  } else {
    std::cout << "its not working";
    // error function
  }
  out_file.close();
  nextscreen = MENU;
}

bool map_editor_screen::check_dupl_map() {
  for (int i = 0; i < index.size(); i++) {
    if (index[i] == sf_file_name.getString()) {
      return true;
    }
  }
  return false;
}

void map_editor_screen::import_index() {
  map_index.open("map_index.txt");
  while (!map_index.eof()) {
    std::string temp;
    map_index >> temp;
    index.push_back(temp);
  }
  map_index.close();
}

void map_editor_screen::mouse_update(sf::RenderWindow& window) {
  mouse_x = sf::Mouse::getPosition(window).x;
  mouse_y = sf::Mouse::getPosition(window).y;
  int win_x;
  win_x = window.getSize().x;
  sf::Vector2f wsize = window.getView().getSize();
  sf::Vector2u ssize(224, 288);
  float scale;
  if (wsize.x / wsize.y > ssize.x / ssize.y) {
    scale = wsize.y / ssize.y;
  } else {
    scale = wsize.x / ssize.x;
  }
  corr_x = (mouse_x - (win_x - (win_x / 2) - ((224 / 2) * scale))) /
           (scale * tile_size);
  corr_y = mouse_y / (scale * tile_size);
  mouse_coll.setPosition(corr_x * tile_size, corr_y * tile_size);
}

void map_editor_screen::grid(sf::RenderTexture* renderer) {
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      sf::RectangleShape temp_box;
      temp_box.setFillColor(sf::Color::Black);
      temp_box.setSize(sf::Vector2f(tile_size - 1, tile_size - 1));
      temp_box.setPosition(j * tile_size + 1, i * tile_size + 1);
      renderer->draw(temp_box);
    }
  }
}

void map_editor_screen::render(sf::RenderTexture* renderer) {
  sf::RectangleShape asthetics;
  asthetics.setFillColor(sf::Color::White);
  asthetics.setPosition(223, 0);
  asthetics.setSize(sf::Vector2f(1, 248));
  renderer->clear();
  renderer->draw(background);
  // editor_board.test_board();
  grid(renderer);
  if (background.getGlobalBounds().intersects(temp_box.getGlobalBounds())) {
    renderer->draw(temp_box);
  }
  sf_file_name.setPosition(int(112 - sf_file_name.getLocalBounds().width / 2),
                           275);
  // std::cout << file_name << std::endl;
  if (check_dupl_map()) {
    renderer->draw(dupl_msg);
  }
  for (int i = 0; i < inter_num; i++) {
    type_selector[i].draw(renderer);
  }
  renderer->draw(sf_file_name);
  renderer->draw(mouse_coll);
  editor_board.render_alternate(renderer);
  renderer->draw(asthetics);
  renderer->display();
}

ScreenName map_editor_screen::update() { return nextscreen; }

map_editor_screen::~map_editor_screen() { map_index.close(); }
