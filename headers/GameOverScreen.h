#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "AnimationHelper.h"
#include "Screen.h"

class GameOverScreen : public Screen {
 private:
  ScreenName nextScreen = GAMEOVER;
  sf::Font* font;
  sf::String name = "";
  int* scorePtr;
  const int maxChars = 12;
  sf::Text title;
  sf::Text nameInst, scoreText;
  sf::Text options[3];
  sf::String optionStrings[3] = {"_", "Next", "Exit"};
  int selections = 3;
  int curSelection = 0;
  sf::Color selectedColor = sf::Color::Red, deselectedColor = sf::Color(255, 150, 0, 255);
  int animationTick = 0;
  bool win = false;
  // network stuff
  sf::Http::Request request;
  sf::Http http;
  sf::Http::Response response;

 public:
  GameOverScreen(sf::Font* f, int* score) {
    font = f;
    scorePtr = score;
    title.setString("Game Over");
    title.setFont(*font);
    title.setCharacterSize(20);
    title.setFillColor(selectedColor);
    title.setPosition(int((224 - title.getGlobalBounds().width) / 2), -20);

    scoreText.setString("Score: ");
    scoreText.setFont(*font);
    scoreText.setCharacterSize(8);
    scoreText.setFillColor(deselectedColor);

    nameInst.setString("Enter your name:");
    nameInst.setFont(*font);
    nameInst.setCharacterSize(8);
    nameInst.setFillColor(deselectedColor);
    nameInst.setPosition(int((224 - nameInst.getGlobalBounds().width) / 2),
                         -20);

    for (int i = 0; i < selections; i++) {
      options[i].setString(optionStrings[i]);
      options[i].setFont(*font);
      options[i].setCharacterSize(8);
      if (i != 0)
        options[i].setFillColor(deselectedColor);
      else
        options[i].setFillColor(selectedColor);
      // options[i].setPosition(int((224 -
      // options[i].getGlobalBounds().width) / 2), 151);
    }

    // option placement
    options[0].setPosition(int((224 - options[0].getGlobalBounds().width) / 2),
                           288);
    options[1].setPosition(int((224 - options[1].getGlobalBounds().width) / 2),
                           288);
    options[2].setPosition(int((224 - options[2].getGlobalBounds().width) / 2),
                           288);

    // network stuff
    request.setMethod(sf::Http::Request::Post);
    request.setField("Content-Type", "application/x-www-form-urlencoded");
    request.setUri("/scores.php");
  }

  void init() {
    // network stuff
    http.setHost("http://stringcheese.000webhostapp.com/");

    options[curSelection].setFillColor(deselectedColor);
    curSelection = 0;
    options[curSelection].setFillColor(selectedColor);

    scoreText.setString("Score: " + std::to_string(*scorePtr));
    scoreText.setPosition(int((224 - scoreText.getGlobalBounds().width) / 2),
                          -20);

    if (win) {
      title.setString("You Win!");
      title.setFillColor(sf::Color(141, 222, 53, 255));
    } else {
      title.setString("Game Over");
      title.setFillColor(sf::Color::Red);
    }

    title.setPosition(int((224 - title.getGlobalBounds().width) / 2), -20);
    nameInst.setPosition(nameInst.getPosition().x, -20);
    options[0].setPosition(options[0].getPosition().x, 288);
    options[1].setPosition(options[1].getPosition().x, 288);
    options[2].setPosition(options[2].getPosition().x, 288);

    nextScreen = GAMEOVER;

    animationTick = 0;
  }

  ScreenName update() {
    if (animationTick <= 30) {
      title.setPosition(title.getPosition().x,
                        easeOutQuint(animationTick, 30, -20, 80));
      scoreText.setPosition(scoreText.getPosition().x,
                            easeOutQuint(animationTick, 30, -20, 116));
      nameInst.setPosition(nameInst.getPosition().x,
                           easeOutQuint(animationTick, 30, -20, 151));

      options[0].setPosition(options[0].getPosition().x,
                             easeOutQuint(animationTick, 30, 288, 168));
      options[1].setPosition(options[1].getPosition().x,
                             easeOutQuint(animationTick, 30, 288, 194));
      options[2].setPosition(options[2].getPosition().x,
                             easeOutQuint(animationTick, 30, 288, 209));

      animationTick++;
    }
    return nextScreen;
  }

  void render(sf::RenderTexture* renderArea) {
    renderArea->clear(sf::Color::Transparent);
    renderArea->draw(title);
    renderArea->draw(scoreText);
    renderArea->draw(nameInst);
    for (int i = 0; i < selections; i++) {
      renderArea->draw(options[i]);
    }
    renderArea->display();
  }

  void postScore() {
    request.setBody("name=" + name + "&score=" + std::to_string(*scorePtr));
    response = http.sendRequest(request);
  }

  void handleEvent(sf::Event* e) {
    if (e->type == sf::Event::TextEntered && curSelection == 0) {
      // name input
      if (e->text.unicode == 8 && name.getSize() > 0) {  // backspace
        name.erase(name.getSize() - 1, 1);
      } else if (name.getSize() < maxChars) {
        // filter ascii
        if ((e->text.unicode <= 57 && e->text.unicode >= 48)  // digits
            || (e->text.unicode >= 65 &&
                e->text.unicode <= 90)  // uppercase letters
            || (e->text.unicode >= 97 &&
                e->text.unicode <= 122)  // lowercase letters
            || e->text.unicode == 95) {  // underscore
          name += char(e->text.unicode);
        }
      }
      options[0].setString(name + '_');
      options[0].setPosition(
          int((224 - options[0].getGlobalBounds().width) / 2), 168);
    }
    if (e->type == sf::Event::KeyReleased) {
      if (e->key.code == sf::Keyboard::Escape) nextScreen = MENU;
    } else if (e->type == sf::Event::KeyPressed) {
      if (e->key.code == sf::Keyboard::Up) {
        if (curSelection > 0) {
          options[curSelection].setFillColor(deselectedColor);
          curSelection--;
          options[curSelection].setFillColor(selectedColor);
        }
      } else if (e->key.code == sf::Keyboard::Down) {
        if (curSelection < selections - 1) {
          options[curSelection].setFillColor(deselectedColor);
          curSelection++;
          options[curSelection].setFillColor(selectedColor);
        }
      } else if (e->key.code == sf::Keyboard::Space ||
                 e->key.code == sf::Keyboard::Return) {
        switch (curSelection) {
          case 0:
            break;
          case 1:
            postScore();
            nextScreen = HIGHSCORE;
            win = false;
            break;
          case 2:
            nextScreen = MENU;
            win = false;
            break;
          default:
            break;
        }
      }
    }
  }

  void setWin(bool w) { win = w; }
};