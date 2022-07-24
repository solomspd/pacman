#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#include "AnimationHelper.h"
#include "Screen.h"
#include "Settings.h"

class SettingsScreen : public Screen {
 private:
  sf::Font *font;
  const int textYStart = 115;
  const int selections = 4;
  std::string settingsItems[4] = {"Shaders", "Vsync", "Sound Effects",
                                  "FullScreen"};
  sf::Text settingsTexts[4];
  sf::Text settingsChoices[4];
  sf::Text backText;
  sf::Text title;
  sf::Color selectedColor = sf::Color::Red, deselectedColor = sf::Color(255, 150, 0, 255);
  int curSelection = 0;
  ScreenName nextScreen = MENU;
  Settings *settings;
  sf::String onString = "< ON  >", offString = "< OFF >";
  bool pauseScreen = false;

  void toggleSelection() {
    if (curSelection < selections) {
      switch (curSelection) {
        case 0:
          if (settings->useShaders()) {
            settings->setShaders(false);
            settingsChoices[curSelection].setString(offString);
          } else {
            settings->setShaders(true);
            settingsChoices[curSelection].setString(onString);
          }
          break;
        case 1:
          if (settings->useVsync()) {
            settings->setVsync(false);
            settingsChoices[curSelection].setString(offString);
          } else {
            settings->setVsync(true);
            settingsChoices[curSelection].setString(onString);
          }
          break;
        case 2:
          if (settings->useSFX()) {
            settings->setSFX(false);
            settingsChoices[curSelection].setString(offString);
          } else {
            settings->setSFX(true);
            settingsChoices[curSelection].setString(onString);
          }
          break;
        case 3:
          if (settings->useFullScreen()) {
            settings->setFullScreen(false);
            settingsChoices[curSelection].setString(offString);
          } else {
            settings->setFullScreen(true);
            settingsChoices[curSelection].setString(onString);
          }
          break;
      }
    }
  }

 public:
  SettingsScreen(Settings *settings, sf::Font *f) {
    this->settings = settings;
    font = f;

    backText.setString("Back");
    backText.setFont(*font);
    backText.setCharacterSize(8);
    backText.setFillColor(deselectedColor);

    title.setFont(*font);
    title.setCharacterSize(20);
    title.setString("Settings");
    title.setFillColor(deselectedColor);
    title.setPosition(int((224 - title.getGlobalBounds().width) / 2), 70);

    backText.setPosition(int((224 - backText.getGlobalBounds().width) / 2),
                         textYStart + selections * 20 + 10);

    for (int i = 0; i < selections; i++) {
      settingsTexts[i].setString(settingsItems[i]);
      settingsTexts[i].setFont(*font);
      settingsTexts[i].setCharacterSize(8);
      settingsTexts[i].setPosition(20, textYStart + i * 20);
      settingsChoices[i].setString(onString);
      settingsChoices[i].setFont(*font);
      settingsChoices[i].setCharacterSize(8);
      settingsChoices[i].setPosition(160, textYStart + i * 20);

      if (i == curSelection) {
        settingsTexts[i].setFillColor(selectedColor);
        settingsChoices[i].setFillColor(selectedColor);
      } else {
        settingsTexts[i].setFillColor(deselectedColor);
        settingsChoices[i].setFillColor(deselectedColor);
      }
    }
  }

  void init() {
    nextScreen = SETTINGS;

    if (settings->useShaders())
      settingsChoices[0].setString(onString);
    else
      settingsChoices[0].setString(offString);

    if (settings->useVsync())
      settingsChoices[1].setString(onString);
    else
      settingsChoices[1].setString(offString);

    if (settings->useSFX())
      settingsChoices[2].setString(onString);
    else
      settingsChoices[2].setString(offString);

    if (settings->useFullScreen())
      settingsChoices[3].setString(onString);
    else
      settingsChoices[3].setString(offString);
  }

  ScreenName update() { return nextScreen; }

  void render(sf::RenderTexture *renderArea) {
    renderArea->clear(sf::Color::Transparent);
    renderArea->draw(backText);
    renderArea->draw(title);
    for (int i = 0; i < selections; i++) {
      renderArea->draw(settingsTexts[i]);
      renderArea->draw(settingsChoices[i]);
    }

    renderArea->display();
  }

  void handleEvent(sf::Event *e) {
    if (e->type == sf::Event::KeyPressed) {
      if (e->key.code == sf::Keyboard::Space || e->key.code == sf::Keyboard::Return) {
        if (curSelection == selections) {
          if (pauseScreen) {
            nextScreen = GAME;
            pauseScreen = false;
          } else {
            nextScreen = MENU;
          }
        } else {
          toggleSelection();
        }
      } else if (e->key.code == sf::Keyboard::Escape) {
        if (pauseScreen) {
          nextScreen = GAME;
          pauseScreen = false;
        } else {
          nextScreen = MENU;
        }
      } else if (e->key.code == sf::Keyboard::Down) {
        if (curSelection < selections) {
          curSelection++;
          if (curSelection < selections) {
            settingsTexts[curSelection].setFillColor(selectedColor);
            settingsChoices[curSelection].setFillColor(selectedColor);
          } else {
            backText.setFillColor(selectedColor);
          }
          settingsTexts[curSelection - 1].setFillColor(deselectedColor);
          settingsChoices[curSelection - 1].setFillColor(deselectedColor);
        }
      } else if (e->key.code == sf::Keyboard::Up) {
        if (curSelection > 0) {
          curSelection--;
          settingsTexts[curSelection].setFillColor(selectedColor);
          settingsChoices[curSelection].setFillColor(selectedColor);
          if (curSelection != selections - 1) {
            settingsTexts[curSelection + 1].setFillColor(deselectedColor);
            settingsChoices[curSelection + 1].setFillColor(deselectedColor);
          } else {
            backText.setFillColor(deselectedColor);
          }
        }
      } else if (e->key.code == sf::Keyboard::Left ||
                 e->key.code == sf::Keyboard::Right) {
        if (curSelection < selections) {
          toggleSelection();
        }
      }
    }
  }

  void setPaused(bool p) { pauseScreen = p; }
};
