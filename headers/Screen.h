#pragma once
#include <SFML/Graphics.hpp>

// abstract class
enum ScreenName {
  MENU = 0,
  GAME = 1,
  SETTINGS = 2,
  HIGHSCORE = 3,
  MAPEDITOR = 4,
  MAPSELECT = 5,
  GAMEOVER = 6,
  WINSCREEN = -6,
  QUIT = -1
};
class Screen {
 public:
  virtual ScreenName update() { return MENU; };
  virtual void render(sf::RenderTexture*){};
  virtual void handleEvent(sf::Event*){};
  virtual void mouse_update(sf::RenderWindow&){};
  virtual void init(){};
  virtual void moveghosts(int counter){};
};