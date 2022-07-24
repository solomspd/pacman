#pragma once
#include <SFML/Graphics.hpp>
//#include "Screen.h"
#include "GameOverScreen.h"
#include "GameScreen.h"
#include "HighscoreScreen.h"
#include "MenuScreen.h"
#include "Settings.h"
#include "SettingsScreen.h"
#include "map_editor_screen.h"
#include "map_select_screen.h"

const int NUM_OF_SCREENS = 7;

class ScreenManager {
 private:
  Screen *screens[NUM_OF_SCREENS];
  ScreenName prevScreen = GAME;
  ScreenName currentScreen = MENU;
  sf::Font font;
  Settings *settings;

 public:
  ScreenManager(Settings *settings) {
    this->settings = settings;
    font.loadFromFile("../assets/fonts/emulogic.ttf");

    screens[MENU] = new MenuScreen(&font);
    screens[GAME] = new GameScreen(settings->SFXPtr(), &font);
    screens[SETTINGS] = new SettingsScreen(settings, &font);
    screens[HIGHSCORE] = new HighscoreScreen(&font);
    screens[MAPEDITOR] = new map_editor_screen(&font);
    screens[MAPSELECT] = new map_select_screen(&font);
    screens[GAMEOVER] =
        new GameOverScreen(&font, ((GameScreen *)screens[GAME])->scorePtr);
    screens[currentScreen]->init();
  }

  // destructor
  ~ScreenManager() {
    for (int i = 0; i < NUM_OF_SCREENS; i++) {
      delete screens[i];
    }
  }

  void update() {
    currentScreen = screens[currentScreen]->update();
    if (currentScreen == QUIT) {
      (*settings).quit = true;
      currentScreen = MENU;
    }
    if (currentScreen == WINSCREEN) {
      currentScreen = GAMEOVER;
      ((GameOverScreen *)screens[GAMEOVER])->setWin(true);
    }
    if (currentScreen == SETTINGS && prevScreen == GAME) {
      ((SettingsScreen *)screens[SETTINGS])->setPaused(true);
    } else if (currentScreen == GAME &&
               (prevScreen == MENU || prevScreen == MAPSELECT)) {
      ((GameScreen *)screens[GAME])->loadMaps();
      ((GameScreen *)screens[GAME])->wholeGame = (prevScreen == MENU);
    }
    if (currentScreen != prevScreen) {
      screens[currentScreen]->init();
      prevScreen = currentScreen;
    }
  }

  void render(sf::RenderTexture *renderArea) {
    screens[currentScreen]->render(renderArea);
  }

  void handleEvent(sf::Event *e) { screens[currentScreen]->handleEvent(e); }

  void mouse_update(sf::RenderWindow &window) {
    screens[currentScreen]->mouse_update(window);
  }

  ScreenName getCurrScreen() { return currentScreen; }

  void setScreen(ScreenName screen) {
    // todo transitions
    currentScreen = screen;
  }

  void moveghosts(int counter) {
    if (currentScreen == 1) screens[currentScreen]->moveghosts(counter);
  }
};