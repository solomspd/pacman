#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "AnimatedSprite.h"
#include "AnimationHelper.h"
#include "Screen.h"

class MenuScreen : public Screen {
 private:
  sf::RectangleShape shape;
  sf::Texture menuSprites;
  sf::Sprite title;
  sf::Sprite bottomBG;
  sf::Sprite bottomFG;
  sf::Sprite stringCheese;
  AnimatedSprite selector;
  sf::Font* font;
  const int textYStart = 90;
  const int textSeparation = 20;
  const int selections = 6;
  std::string menuItems[6] = {"Start Game", "Highscores", "Map Selector",
                              "Map Editor", "Settings",   "Quit"};
  sf::Text menuTexts[6];
  sf::Color selectedColor = sf::Color::Red, deselectedColor = sf::Color(255, 150, 0, 255);
  int curSelection = 0;
  int curTick = 0;
  int selectorTick = 10;
  int selStartY = 0, selEndY = 0;
  ScreenName nextScreen = MENU;

 public:
  MenuScreen(sf::Font* f) {
    menuSprites.loadFromFile("../assets/textures/MenuSpritesheet.png");
    font = f;

    title.setTexture(menuSprites);
    title.setTextureRect(sf::IntRect(0, 0, 162, 38));
    bottomBG.setTexture(menuSprites);
    bottomBG.setTextureRect(sf::IntRect(0, 182, 224, 74));
    bottomFG.setTexture(menuSprites);
    bottomFG.setTextureRect(sf::IntRect(0, 112, 213, 68));
    stringCheese.setTexture(menuSprites);
    stringCheese.setTextureRect(sf::IntRect(0, 56, 78, 5));
    selector.setPosition(0, -30);
    selector.setTexture(menuSprites);
    selector.setAnim(LOOPF, 0, 40, 16, 16, 4, true);

    title.setPosition(31, -200);
    bottomBG.setPosition(0, 300);
    bottomFG.setPosition(0, 300);
    bottomFG.setColor(sf::Color(127, 127, 125, 255));
    bottomBG.setColor(sf::Color(127, 127, 125, 255));
    stringCheese.setPosition(73, 270);
    stringCheese.setColor(sf::Color::Transparent);

    selEndY = textYStart - 4;

    for (int i = 0; i < selections; i++) {
      menuTexts[i].setString(menuItems[i]);
      menuTexts[i].setFont(*font);
      menuTexts[i].setCharacterSize(8);

      menuTexts[i].setPosition(
          int((224 - menuTexts[i].getGlobalBounds().width) / 2), -10);

      if (i == curSelection)
        menuTexts[i].setFillColor(selectedColor);
      else
        menuTexts[i].setFillColor(deselectedColor);
    }
  }

  void init() { nextScreen = MENU; }

  ScreenName update() {
    // intro animations
    if (curTick < 30 * 3) {
      title.setPosition(31, easeOutQuint(curTick, 65, -200, 23));
      bottomBG.setPosition(0, easeOutQuint(curTick, 90, 290, 218));
      bottomFG.setPosition(0, easeOutQuint(curTick, 70, 290, 215));
      stringCheese.setColor(fadeInColor(curTick, 70, sf::Color::White));

      for (int i = 0; i < selections; i++) {
        menuTexts[i].setPosition(
            menuTexts[i].getPosition().x,
            int(easeOutQuint(curTick, 50, 20,
                             textYStart + i * textSeparation)));
        menuTexts[i].setFillColor(
            fadeInColor(curTick, 30, menuTexts[i].getFillColor()));
      }

      selector.setColor(fadeInColor(curTick, 40, sf::Color::White));

      curTick++;
    }

    if (selectorTick > 0) {
      selector.setPosition(
          45, easeOutQuint(10 - selectorTick, 10, selStartY, selEndY));
      selectorTick--;
    }

    return nextScreen;
  }

  void render(sf::RenderTexture* renderArea) {
    renderArea->clear(sf::Color::Transparent);
    renderArea->draw(title);
    renderArea->draw(bottomBG);
    renderArea->draw(bottomFG);
    renderArea->draw(stringCheese);
    selector.setPosition(45, selector.getPosition().y);
    renderArea->draw(selector);
    selector.setPosition(165, selector.getPosition().y);
    renderArea->draw(selector);

    for (int i = 0; i < selections; i++) {
      renderArea->draw(menuTexts[i]);
    }

    renderArea->display();
  }

  void handleEvent(sf::Event* e) {
    if (e->type == sf::Event::KeyPressed) {
      if (e->key.code == sf::Keyboard::Space || e->key.code == sf::Keyboard::Return) {
        std::ofstream is_whole;
        std::string next_map_name;
        switch (curSelection) {
          case 1:
            nextScreen = HIGHSCORE;
            break;
          case 2:
            // std::ofstream is_whole;
            is_whole.open("whole_game.txt");
            is_whole << 0;
            is_whole.close();
            nextScreen = MAPSELECT;
            break;
          case 3:
            nextScreen = MAPEDITOR;
            break;
          case 4:
            nextScreen = SETTINGS;
            break;
          case 5:
            nextScreen = QUIT;
            break;
          case 0:  // start game
            is_whole.open("whole_game.txt");
            is_whole << 1;
            is_whole.close();
            std::ifstream next_map_in;
            next_map_in.open("base_map_index.txt");
            next_map_in >> next_map_name;
            next_map_in.close();
            std::ofstream next_map_out;
            next_map_out.open("next_map.txt");
            next_map_out << next_map_name;
            // std::cout << next_map_name;
            next_map_out.close();
            nextScreen = GAME;
            break;
        }
      } else if (e->key.code == sf::Keyboard::Down) {
        if (curSelection < selections - 1) {
          curSelection++;
          menuTexts[curSelection].setFillColor(selectedColor);
          menuTexts[curSelection - 1].setFillColor(deselectedColor);
          selStartY = selector.getPosition().y;
          selEndY = textYStart - 4 + curSelection * textSeparation;
          selectorTick = 10;
          selector.nextFrame();
        }
      } else if (e->key.code == sf::Keyboard::Up) {
        if (curSelection > 0) {
          curSelection--;
          menuTexts[curSelection].setFillColor(selectedColor);
          menuTexts[curSelection + 1].setFillColor(deselectedColor);
          selStartY = selector.getPosition().y;
          selEndY = textYStart - 4 + curSelection * textSeparation;
          selectorTick = 10;
          selector.prevFrame();
        }
      }
    }
  }
};
