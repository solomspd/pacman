#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>

#include "AnimationHelper.h"
#include "Screen.h"
#include "blue_ghost.h"
#include "board.h"
#include "pac_man.h"
#include "pink_ghost.h"
#include "red_ghost.h"
#include "yellow_ghost.h"

bool eatghosts = false;
const int time_edible_ghosts = 5 * 30; // seconds * 30

class GameScreen : public Screen {
private:
  sf::Texture pmantex;
  ScreenName nextScreen = GAME;
  pac_man *pacmanPtr;
  board *boardPtr;
  red_ghost *red_ghost_ptr;
  blue_ghost *blue_ghost_ptr;
  yellow_ghost *yellow_ghost_ptr;
  pink_ghost *pink_ghost_ptr;
  sf::Font *font;
  int tick = 0;
  int countdown = time_edible_ghosts;
  int score;
  int lives;
  int level;
  bool thats_it;
  bool *useSFX;

  sf::Text levelTitle, titleShadow;
  sf::Text scoreText, scoreNumber, livesText;
  sf::Sprite lifeSprite;
  bool showTitle =
      true; // set this to false if you dont wanna see the level titles
  bool showingTitle = true;
  bool isPaused = false;
  // paused stuff
  sf::RectangleShape fadeShape;
  sf::Text pauseTitle;
  sf::Text pauseMenu[3];
  sf::String pauseStrings[3] = {"Resume", "Settings", "Quit Game"};
  sf::Color selectedColor = sf::Color::Red,
            deselectedColor = sf::Color(255, 150, 0, 255);
  int selections = 3;
  int curSelection = 0;
  int last_level;

  sf::SoundBuffer intermission, ghostsmove, eatghost, intro;
  sf::Sound pacman_intermission, pacman_ghostsmove, pacman_eatghost,
      pacman_intro;
  bool play_intermission, play_ghostsmove, play_eatghost, play_intro;

public:
  bool wholeGame = true;
  int *scorePtr;
  GameScreen(bool *useSFX, sf::Font *f) {
    this->useSFX = useSFX;
    pmantex.loadFromFile("../assets/textures/spritesheet.png");
    font = f;

    last_level = 0;
    std::ifstream in_file;
    in_file.open("../assets/maps/base_map_index.txt");
    for (last_level = 0; !in_file.eof(); last_level++) {
      std::string dummy;
      in_file >> dummy;
    }
    in_file.close();

    scorePtr = &score;

    levelTitle.setFont(*font);
    levelTitle.setFillColor(sf::Color::Yellow);
    levelTitle.setCharacterSize(25);

    scoreText.setFont(*font);
    scoreText.setCharacterSize(8);
    scoreText.setString("Score");
    scoreText.setPosition(144, 247 + 5);

    scoreNumber.setFont(*font);
    scoreNumber.setCharacterSize(9);
    scoreNumber.setString("0");
    scoreNumber.setPosition(144, 247 + 15);

    livesText.setFont(*font);
    livesText.setCharacterSize(8);
    livesText.setString("Lives");
    livesText.setPosition(42, 247 + 5);

    lifeSprite.setTexture(pmantex);
    lifeSprite.setTextureRect(sf::IntRect(16, 0, 16, 16));
    // pause menu stuff
    fadeShape.setPosition(0, 0);
    fadeShape.setSize(sf::Vector2f(224, 288));
    fadeShape.setFillColor(sf::Color(100, 100, 100, 255));

    pauseTitle.setFont(*font);
    pauseTitle.setCharacterSize(20);
    pauseTitle.setString("Paused");
    pauseTitle.setFillColor(deselectedColor);
    pauseTitle.setPosition(int((224 - pauseTitle.getGlobalBounds().width) / 2),
                           90);

    for (int i = 0; i < selections; i++) {
      pauseMenu[i].setFont(*font);
      pauseMenu[i].setCharacterSize(8);
      pauseMenu[i].setString(pauseStrings[i]);
      pauseMenu[i].setPosition(
          int((224 - pauseMenu[i].getGlobalBounds().width) / 2), 120 + i * 20);
      if (i != curSelection)
        pauseMenu[i].setFillColor(deselectedColor);
      else
        pauseMenu[i].setFillColor(selectedColor);
    }

    intermission.loadFromFile("../assets/audio/pacman_intermission.wav");
    ghostsmove.loadFromFile("../assets/audio/pacman_ghostmove.wav");
    eatghost.loadFromFile("../assets/audio/pacman_eatghost.wav");
    intro.loadFromFile("../assets/audio/pacman_intro.wav");

    pacman_ghostsmove.setBuffer(ghostsmove);
    pacman_intro.setBuffer(intro);
    pacman_eatghost.setBuffer(eatghost);
    pacman_intermission.setBuffer(intermission);

    level = 1;
  }

  void loadMaps() {
    lives = 3;
    std::ifstream in_file, next_map;
    next_map.open("../assets/maps/next_map.txt");
    std::string file_name;
    next_map >> file_name;
    next_map.close();
    std::cout << file_name;
    std::string file_name_txt = file_name + ".txt";
    setLevel(file_name_txt, file_name);
  }

  void init() {
    score = 0;
    nextScreen = GAME;
    resetSoundBools();
    lives = 3;
    std::ifstream is_whole;
    is_whole.open("../assets/maps/whole_game.txt");
    is_whole >> wholeGame;
    is_whole.close();
  }

  void setLevel(std::string filename, std::string levelname) {
    delete boardPtr;
    delete pacmanPtr;
    delete red_ghost_ptr;
    delete blue_ghost_ptr;
    delete yellow_ghost_ptr;
    delete pink_ghost_ptr;

    std::ifstream in_file;
    in_file.open(filename.c_str());
    boardPtr = new board(in_file, &pmantex, useSFX);

    boardPtr->setBoardName(levelname);

    pacmanPtr = new pac_man(boardPtr, &score, &lives, useSFX);
    red_ghost_ptr = new red_ghost(boardPtr);
    blue_ghost_ptr = new blue_ghost(boardPtr);
    yellow_ghost_ptr = new yellow_ghost(boardPtr);
    pink_ghost_ptr = new pink_ghost(boardPtr);
    in_file.close();

    levelTitle.setString(levelname);
    levelTitle.setPosition(int((224 - levelTitle.getGlobalBounds().width) / 2),
                           -25);

    tick = 0;
    showingTitle = showTitle;
  }

  bool check_thats_it(std::ifstream &in_file) {
    if (!in_file.eof()) {
      return true;
      thats_it = true;
    }
    return false;
  }

  ScreenName update() {
    if (!showingTitle && !isPaused) {
      if (lives < 0) {
        nextScreen = GAMEOVER;
      } else if (boardPtr->numOfCoins == 0) { // no more coins (win map)
        if (wholeGame) {
          std::ifstream next_map;
          level++;
          if (level == last_level + 1) {
            nextScreen = WINSCREEN;
          }
          std::string new_map_name;
          next_map.open("../assets/maps/base_map_index.txt");
          for (int i = 0; i < level; i++) {
            next_map >> new_map_name;
          }
          next_map.close();
          std::ofstream next_map_out;
          next_map_out.open("../assets/maps/next_map.txt");
          next_map_out << new_map_name;
          next_map_out.close();
          loadMaps();
        } else {
          nextScreen = WINSCREEN;
        }
      }
      moveghosts(tick);
      // pacmanPtr->reset_pos()
      pacmanPtr->movepac(boardPtr); // so pacman is twice the speed of ghosts
      pacmanPtr->movepac(boardPtr);

      if (play_ghostsmove) {
        pacman_ghostsmove.setLoop(true);
        pacman_ghostsmove.play();
        play_ghostsmove = false;
      }

      if (pacmanPtr->eat_ghosts_flag()) // checks if big coin is eaten
                                        // and set flag
        eatghosts = true;

      if (eatghosts) { // if flag is true then start countdown and make
                       // ghosts edible
        if (pacmanPtr->eat_ghosts(boardPtr, blue_ghost_ptr)) {
          blue_ghost_ptr->reset_pos(boardPtr, blue);
          if (play_eatghost)
            soundeatingghosts();
        }
        if (pacmanPtr->eat_ghosts(boardPtr, red_ghost_ptr)) {
          red_ghost_ptr->reset_pos(boardPtr, red);
          if (play_eatghost)
            soundeatingghosts();
        }
        if (pacmanPtr->eat_ghosts(boardPtr, pink_ghost_ptr)) {
          pink_ghost_ptr->reset_pos(boardPtr, pink);
          if (play_eatghost)
            soundeatingghosts();
        }
        if (pacmanPtr->eat_ghosts(boardPtr, yellow_ghost_ptr)) {
          yellow_ghost_ptr->reset_pos(boardPtr, yellow);
          if (play_eatghost)
            soundeatingghosts();
        }

        if (countdown == time_edible_ghosts) {
          pink_ghost_ptr->setEdible(true);
          red_ghost_ptr->setEdible(true);
          blue_ghost_ptr->setEdible(true);
          yellow_ghost_ptr->setEdible(true);
        }
        countdown--;
      } else if (countdown == time_edible_ghosts) {
        pacmanPtr->ghost_collision(boardPtr, pink_ghost_ptr);
        pacmanPtr->ghost_collision(boardPtr, red_ghost_ptr);
        pacmanPtr->ghost_collision(boardPtr, blue_ghost_ptr);
        pacmanPtr->ghost_collision(boardPtr, yellow_ghost_ptr);
      }

      if (play_intermission && eatghosts) {
        pacman_ghostsmove.stop();
        pacman_intermission.setLoop(true);
        pacman_intermission.play();
        play_intermission = false;
      }
      if (countdown == 0) { // end countdown and reset flag
        eatghosts = false;
        pacmanPtr->reset_eat_ghosts_flag();

        pacman_intermission.stop();
        resetSoundBools();

        pink_ghost_ptr->setEdible(false);
        red_ghost_ptr->setEdible(false);
        blue_ghost_ptr->setEdible(false);
        yellow_ghost_ptr->setEdible(false);

        countdown = time_edible_ghosts;
      }
      // update score text
      scoreNumber.setString(std::to_string(score));

      tick++;

      if (tick == 60)
        tick = 0;

    } else if (showingTitle) {
      // titleAnimation
      if (tick < 30) {
        levelTitle.setPosition(levelTitle.getPosition().x,
                               easeOutQuint(tick, 30, -25, 110));
      } else {
        levelTitle.setPosition(levelTitle.getPosition().x,
                               easeInQuint(tick - 30, 25, 110, 288));
      }

      tick++;
      if (tick == 60) {
        showingTitle = false;
        tick = 0;
      }
    }
    if (nextScreen != GAME) {
      stopSounds();
      resetSoundBools();
    }

    return nextScreen;
  }

  void render(sf::RenderTexture *renderArea) {
    renderArea->clear(sf::Color::Transparent);
    if (!showingTitle && !isPaused) {
      // displaying board
      boardPtr->render(renderArea);
      pacmanPtr->render(renderArea);
      red_ghost_ptr->render(renderArea);
      blue_ghost_ptr->render(renderArea);
      yellow_ghost_ptr->render(renderArea);
      pink_ghost_ptr->render(renderArea);
      renderArea->draw(scoreText);
      renderArea->draw(scoreNumber);
      renderArea->draw(livesText);
      // render lives
      if (lives >= 1) {
        lifeSprite.setPosition(37, 257 + 5);
        renderArea->draw(lifeSprite);
      }
      if (lives >= 2) {
        lifeSprite.setPosition(37 + 18, 257 + 5);
        renderArea->draw(lifeSprite);
      }
      if (lives >= 3) {
        lifeSprite.setPosition(37 + 18 * 2, 257 + 5);
        renderArea->draw(lifeSprite);
      }
    } else if (showingTitle) {
      // draw shadow
      levelTitle.setPosition(levelTitle.getPosition() + sf::Vector2f(0, 2.0));
      levelTitle.setFillColor(sf::Color(255, 0, 222, 255));
      renderArea->draw(levelTitle);
      // draw Title
      levelTitle.setPosition(levelTitle.getPosition() - sf::Vector2f(0, 2.0));
      levelTitle.setFillColor(sf::Color::Yellow);
      renderArea->draw(levelTitle);
    } else if (isPaused) {
      // draw board
      boardPtr->render(renderArea);
      // draw fade
      renderArea->draw(fadeShape, sf::BlendMultiply);
      // draw text
      renderArea->draw(pauseTitle);
      for (int i = 0; i < selections; i++) {
        renderArea->draw(pauseMenu[i]);
      }
    }
    renderArea->display();
  }

  void handleEvent(sf::Event *e) {
    if (!showingTitle && !isPaused) {
      pacmanPtr->handleEvent_pac(e, boardPtr);
      if (e->type == sf::Event::KeyReleased) {
        if (e->key.code == sf::Keyboard::Escape) {
          isPaused = true;
          resetSoundBools();
          stopSounds();
          // nextScreen = GAMEOVER;
        }
      }
    } else if (isPaused) {
      if (e->type == sf::Event::KeyReleased) {
        if (e->key.code == sf::Keyboard::Escape)
          isPaused = false;
      } else if (e->type == sf::Event::KeyPressed) {
        if (e->key.code == sf::Keyboard::Up) {
          if (curSelection > 0) {
            pauseMenu[curSelection].setFillColor(deselectedColor);
            curSelection--;
            pauseMenu[curSelection].setFillColor(selectedColor);
          }
        } else if (e->key.code == sf::Keyboard::Down) {
          if (curSelection < selections - 1) {
            pauseMenu[curSelection].setFillColor(deselectedColor);
            curSelection++;
            pauseMenu[curSelection].setFillColor(selectedColor);
          }
        } else if (e->key.code == sf::Keyboard::Space ||
                   e->key.code == sf::Keyboard::Return) {
          switch (curSelection) {
          case 0:
            isPaused = false;
            break;
          case 1:
            nextScreen = SETTINGS;
            break;
          case 2:
            isPaused = false;
            nextScreen = MENU;
            pauseMenu[curSelection].setFillColor(deselectedColor);
            curSelection = 0;
            pauseMenu[curSelection].setFillColor(selectedColor);
            break;
          default:
            break;
          }
        }
      }
    }
  }

  ~GameScreen() {
    delete boardPtr;
    delete pacmanPtr;
    delete red_ghost_ptr;
  }

  void moveghosts(int counter) {
    if (counter == 0) {
      red_ghost_ptr->change_direction(boardPtr);
      red_ghost_ptr->handleEvent_ghost(boardPtr);
      pink_ghost_ptr->change_direction(boardPtr);
      pink_ghost_ptr->handleEvent_ghost(boardPtr);
      blue_ghost_ptr->change_direction(boardPtr);
      blue_ghost_ptr->handleEvent_ghost(boardPtr);
      yellow_ghost_ptr->change_direction(boardPtr);
      yellow_ghost_ptr->handleEvent_ghost(boardPtr);

    } else {
      red_ghost_ptr->handleEvent_ghost(boardPtr);
      pink_ghost_ptr->handleEvent_ghost(boardPtr);
      blue_ghost_ptr->handleEvent_ghost(boardPtr);
      yellow_ghost_ptr->handleEvent_ghost(boardPtr);
    }
    pacmanPtr->coin_collision(boardPtr, countdown);
  }

  void resetSoundBools() {
    play_intermission = *useSFX;
    play_ghostsmove = *useSFX;
    play_eatghost = *useSFX;
  }

  void stopSounds() {
    pacman_intermission.stop();
    pacman_ghostsmove.stop();
    pacman_eatghost.stop();
  }

  void soundintro() {
    pacman_intro.setLoop(true);
    pacman_intro.play();
    play_intro = false;
  }

  void soundeatingghosts() {
    if (pacman_eatghost.getStatus() == 2) {
      pacman_ghostsmove.stop();
    }
    pacman_eatghost.setLoop(false);
    pacman_eatghost.play();
  }
};
