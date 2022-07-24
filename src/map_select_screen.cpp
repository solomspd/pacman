#include "map_select_screen.h"

#include <iostream>

map_select_screen::map_select_screen(sf::Font *f) {
  font = f;
  std::ifstream progression_in;
  progression_in.open("progression.txt");
  if (!progression_in.is_open()) {
    // error function
  }
  progression_in >> progression;
  selection = 0;
  progression_in.close();
}

ScreenName map_select_screen::update() { return nextScreen; }

void map_select_screen::init() {
  custom_page = 0;
  custom_num = 0;
  base_num = 0;
  in_file_index.open("base_map_index.txt");
  page_type[0].setString("Base Maps");
  page_type[1].setString("Custom Maps");
  for (int i = 0; i < type_num; i++) {
    page_type[i].setFillColor(sf::Color::White);
    page_type[i].setFont(*font);
    page_type[i].setCharacterSize(12);
    page_type[i].setPosition(
        int(244 / 2 - page_type[i].getLocalBounds().width / 2), 240);
  }
  nextScreen = MAPSELECT;
  if (!in_file_index.is_open()) {
    // error function
  }
  map_num = 0;
  std::string dummy;
  while (!in_file_index.eof()) {
    in_file_index >> dummy;
    base_num++;
  }
  in_file_index.close();
  std::ifstream custom_in;
  custom_in.open("map_index.txt");
  while (!custom_in.eof()) {
    custom_in >> dummy;
    custom_num++;
  }
  custom_in.close();
  map_num = custom_num + base_num;
  maps = new sf::Text[map_num];
  base_page_num = ceil(float(base_num) / float(page_maps));
  custom_page_num = ceil(float(custom_num) / float(page_maps));
  // std::cout <<base_page_num;
  page_num = base_page_num + custom_page_num;
  per_page_num = new int[page_num];
  for (int i = 0; i < base_page_num - 1; i++) {
    *(per_page_num + i) = page_num;
  }
  *(per_page_num + base_page_num - 1) = base_num % page_maps;
  // std::cout << *(per_page_num + 0);
  for (int i = base_page_num; i < page_num - 1; i++) {
    *(per_page_num + i) = page_num;
  }
  *(per_page_num + page_num - 1) = custom_num % page_maps;
  in_file_index.open("base_map_index.txt");
  for (int i = 0; !in_file_index.eof(); i++) {
    (maps + i)->setCharacterSize(10);
    (maps + i)->setFillColor(sf::Color(255, 150, 0, 255));
    (maps + i)->setFont(*font);
    std::string string;
    in_file_index >> string;
    (maps + i)->setString(string);
  }
  in_file_index.close();
  std::ifstream custom_index_in;
  custom_index_in.open("map_index.txt");
  for (int i = base_num; !custom_index_in.eof(); i++) {
    (maps + i)->setCharacterSize(10);
    (maps + i)->setFillColor(sf::Color(255, 150, 0, 255));
    (maps + i)->setFont(*font);
    std::string string;
    custom_index_in >> string;
    (maps + i)->setString(string);
  }
  custom_index_in.close();
  page_num = custom_page_num + base_page_num;
  // std::cout << map_num;
  int accu = 0;
  std::cout << page_num;
  for (int i = 0; i < page_num; i++) {
    for (int j = 0; j < *(per_page_num + i); j++) {
      // std::cout << i;
      // std::cout << *(per_page_num + i) << std::endl << i;
      // std::cout << i << " " << j << std::endl;
      int y_pos = 0;
      y_pos = j * 12 + 5;
      (maps + accu + j)
          ->setPosition(int(112 - (maps + accu)->getLocalBounds().width / 2),
                        y_pos);
    }
    accu = *(per_page_num + i);
  }
  curr_page = 0;
  page.setCharacterSize(10);
  page.setFillColor(sf::Color::White);
  refresh_page_num();
  page.setFont(*font);
  page.setPosition(int(224 / 2 - page.getLocalBounds().width / 2), 255);
}

void map_select_screen::handleEvent(sf::Event *event) {
  if (event->type == sf::Event::KeyPressed) {
    switch (event->key.code) {
      case (sf::Keyboard::Up):
        if (selection > 0) {
          selection--;
          select();
        }
        break;
      case (sf::Keyboard::Down):
        if (selection < map_num - 1) {
          selection++;
          select();
        }
        break;
      case (sf::Keyboard::Right):
        if (curr_page < page_num - 1) {
          next_page();
          refresh_page_num();
        }
        break;
      case (sf::Keyboard::Left):
        if (curr_page > 0) {
          previous_page();
          refresh_page_num();
        }
        break;
      case (sf::Keyboard::Escape):
        nextScreen = MENU;
        break;
      case (sf::Keyboard::Return):
        std::ofstream out_file;
        out_file.open("next_map.txt");
        std::string temp = (maps + selection)->getString();
        out_file << temp.c_str();
        out_file.close();
        nextScreen = GAME;
        break;
    }
  }
}

void map_select_screen::refresh_page_num() {
  std::string temp;
  temp = std::to_string(curr_page + 1) + " / " + std::to_string(page_num);
  page.setString(temp);
}

void map_select_screen::next_page() {
  selection += *(per_page_num + curr_page);
  curr_page++;
}

void map_select_screen::previous_page() {
  selection -= *(per_page_num + curr_page);
  curr_page--;
}

int map_select_screen::accumu(int page) {
  int accu = 0;
  for (int i = 0; i < page; i++) {
    accu += *(per_page_num + page - 1);
  }
  return accu;
}

void map_select_screen::render(sf::RenderTexture *renderer) {
  renderer->clear(sf::Color::Transparent);
  // std::cout << selection;
  // std::cout << accumu(curr_page + 1);
  std::cout << map_num;
  for (int i = accumu(curr_page); i < accumu(curr_page + 1); i++) {
    // std::cout << i;
    renderer->draw(*(maps + i));
  }
  renderer->draw(page);
  if (custom_page_num > curr_page) {
    renderer->draw(page_type[0]);
  } else {
    renderer->draw(page_type[1]);
  }
  // test_file();
  renderer->display();
}

void map_select_screen::test_file() {
  for (int i = 0; i < map_num; i++) {
    std::string temp;
    temp = (maps + i)->getString();
    std::cout << temp << " ";
  }
}

void map_select_screen::select() {
  for (int i = 0; i < map_num; i++) {
    maps[i].setFillColor(sf::Color(255, 150, 0, 255));
  }
  maps[selection].setFillColor(sf::Color::Red);
}

map_select_screen::~map_select_screen() {}
