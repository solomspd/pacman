#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>

#include "AnimationHelper.h"
#include "Screen.h"

class HighscoreScreen : public Screen {
 private:
  sf::Font* font;
  sf::Color selectedColor = sf::Color::Red,
            deselectedColor = sf::Color(255, 150, 0, 255);
  int curTick = 0;
  ScreenName nextScreen = HIGHSCORE;
  sf::Text scores[10];
  sf::Text loadingText;
  sf::Text backText;
  sf::Text title;
  bool loading = true;
  sf::Http::Request request;
  sf::Http http;
  sf::Http::Response response;
  struct user {
    std::string name;
    std::string score;
  };
  user users[10];
  int numUsers = 0;

 public:
  HighscoreScreen(sf::Font* f) {
    font = f;

    title.setFont(*font);
    title.setCharacterSize(12);
    title.setString("Online Highscores");
    title.setPosition(int((224 - title.getGlobalBounds().width) / 2), 0);

    loadingText.setFont(*font);
    loadingText.setCharacterSize(8);
    loadingText.setFillColor(deselectedColor);
    loadingText.setString("Loading Online Highscores...");
    loadingText.setPosition(
        int((224 - loadingText.getGlobalBounds().width) / 2), 130);

    backText.setFont(*font);
    backText.setCharacterSize(8);
    backText.setFillColor(selectedColor);
    backText.setString("Back");
    backText.setPosition(int((224 - backText.getGlobalBounds().width) / 2),
                         245);

    for (int i = 0; i < 10; i++) {
      scores[i].setFont(*font);
      scores[i].setCharacterSize(8);
    }

    // network stuff
    request.setMethod(sf::Http::Request::Get);
    request.setUri("/scores.php");
  }

  void init() {
    // network stuff
    http.setHost("http://stringcheese.000webhostapp.com/");

    curTick = 0;
    loading = true;
    numUsers = 0;
    title.setFillColor(sf::Color::Transparent);
    loadingText.setString("Loading Online Highscores...");
    loadingText.setPosition((224 - loadingText.getGlobalBounds().width) / 2,
                            130);
    nextScreen = HIGHSCORE;
  }

  ScreenName update() {
    // intro animations
    if (curTick < 30 * 3) {
      if (curTick == 1) {
        response = http.sendRequest(request);
        if (response.getStatus() == sf::Http::Response::Ok) {
          if (response.getBody() == "ERROR") {
            loadingText.setString("Server Error :(");
            loadingText.setPosition(
                (224 - loadingText.getGlobalBounds().width) / 2, 130);
          } else {
            parseScores(response.getBody());
          }
        } else {
          loadingText.setString("Connection Failed :(");
          loadingText.setPosition(
              (224 - loadingText.getGlobalBounds().width) / 2, 130);
        }
      } else if (loading == false) {
        for (int i = 0; i < numUsers; i++) {
          scores[i].setPosition(
              int(scores[i].getPosition().x),
              int(easeOutQuint(curTick - 2, 80, -10, 185 - i * 15)));
          scores[i].setFillColor(fadeInColor(curTick - 2, 30, deselectedColor));
        }
        title.setPosition(int(title.getPosition().x),
                          int(easeOutQuint(curTick - 2, 80, -10, 10)));
        title.setFillColor(fadeInColor(curTick - 2, 30, deselectedColor));
      }
      curTick++;
    }
    return nextScreen;
  }

  void parseScores(std::string body) {
    int texti = 0;
    std::string dots = "";
    while (texti < body.length() && numUsers < 10) {
      users[numUsers].name = "";
      users[numUsers].score = "";
      while (texti < body.length() && body.at(texti) != ':') {
        users[numUsers].name += body.at(texti);
        texti++;
      }
      texti++;
      while (texti < body.length() && body.at(texti) != ',') {
        users[numUsers].score += body.at(texti);
        texti++;
      }
      texti++;
      dots = "";
      for (int d = 0; d < 22 - users[numUsers].name.length() -
                              users[numUsers].score.length();
           d++) {
        dots += '.';
      }
      scores[numUsers].setString(users[numUsers].name + dots +
                                 users[numUsers].score);
      scores[numUsers].setPosition(
          (224 - scores[numUsers].getGlobalBounds().width) / 2, 0);
      scores[numUsers].setFillColor(sf::Color::Transparent);
      numUsers++;
    }
    loading = false;
  }

  void render(sf::RenderTexture* renderArea) {
    renderArea->clear(sf::Color::Transparent);
    if (loading) {
      renderArea->draw(loadingText);
    } else {
      for (int i = 0; i < numUsers; i++) {
        renderArea->draw(scores[i]);
      }
      renderArea->draw(title);
    }
    renderArea->draw(backText);
    renderArea->display();
  }

  void handleEvent(sf::Event* e) {
    if (e->type == sf::Event::KeyPressed) {
      if (e->key.code == sf::Keyboard::Space ||
          e->key.code == sf::Keyboard::Return ||
          e->key.code == sf::Keyboard::Escape) {
        nextScreen = MENU;
      }
    }
  }
};
