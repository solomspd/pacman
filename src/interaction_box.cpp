#include "interaction_box.h"

#include <iostream>

interaction_box::interaction_box() {
  body.setFillColor(sf::Color::Transparent);
  body.setOutlineThickness(2);
  body.setOutlineColor(sf::Color::White);
  body.setPosition(0, 0);
  tex.loadFromFile("../assets/textures/spritesheet.png");
  text.setFillColor(sf::Color::White);
  body.setSize(sf::Vector2f(12, 12));
  hover = false;
  select = false;
}

void interaction_box::set_size(int x, int y) {
  body.setSize(sf::Vector2f(x, y));
}

bool interaction_box::check_mouse_coll(sf::RectangleShape mouse) {
  if (body.getGlobalBounds().intersects(mouse.getGlobalBounds())) {
    toggle_hover(true);
    return true;
  }
  toggle_hover(false);
  return false;
}

bool interaction_box::check_mouse_click(sf::Event* event,
                                        sf::RectangleShape mouse) {
  if (event->key.code == sf::Mouse::Left && check_mouse_coll(mouse)) {
    toggle_select(true);
    // std::cout << "wtf";
    return true;
  } else {
    toggle_select(false);
    return false;
  }
}

void interaction_box::still(int type) {
  sprite.setTexture(tex);
  sprite.setScale(sf::Vector2f(0.75, 0.75));
  switch (type) {
    case (0):
      sprite.setTextureRect(sf::IntRect(8 * 2, 0, 16, 16));
      break;
    case (1):
      sprite.setTextureRect(sf::IntRect(0, 16 * 5, 16, 16));
      break;
    case (2):
      sprite.setTextureRect(sf::IntRect(0, 16 * 8, 16, 16));
      break;
    case (3):
      sprite.setTextureRect(sf::IntRect(8 * 2, 16 * 6, 16, 16));
      break;
    case (4):
      sprite.setTextureRect(sf::IntRect(8 * 2, 16 * 7, 16, 16));
      break;
    case (5):
      sprite.setScale(sf::Vector2f(1, 1));
      sprite.setPosition(x + 1000, y = 100);
      sprite.setTextureRect(sf::IntRect(19 * 8, 2 * 8, 8, 8));
      break;
    case (6):
      sprite.setScale(sf::Vector2f(1, 1));
      sprite.move(20, 20);
      sprite.setTextureRect(sf::IntRect(21 * 8, 2 * 8, 8, 8));
      break;
    case (7):
      body.setFillColor(sf::Color::Blue);
      sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
  }
}

int interaction_box::get_x() { return body.getPosition().x; }

int interaction_box::get_y() { return body.getPosition().y; }

void interaction_box::toggle_select(bool is_pressed) {
  if (is_pressed) {
    body.setOutlineColor(sf::Color::Red);
  } else {
    body.setOutlineColor(sf::Color::Blue);
  }
}

void interaction_box::toggle_select() {
  if (select) {
    body.setOutlineColor(sf::Color::Red);
  } else {
    body.setOutlineColor(sf::Color::Blue);
  }
}

void interaction_box::toggle_hover(bool is_pressed) {
  if (is_pressed) {
    body.setOutlineColor(sf::Color::Blue);
  } else {
    body.setOutlineColor(sf::Color::White);
  }
}

void interaction_box::toggle_hover() {
  if (hover) {
    body.setOutlineColor(sf::Color::Blue);
  } else {
    body.setOutlineColor(sf::Color::White);
  }
}

void interaction_box::draw(sf::RenderTexture* renderer) {
  renderer->draw(body);
  renderer->draw(sprite);
}

void interaction_box::set_position(int new_x, int new_y) {
  x = new_x;
  y = new_y;
  body.setPosition(x, y);
  sprite.setPosition(x, y);
}

interaction_box::~interaction_box() {}
