#include <SFML/Graphics.hpp>
#include <iostream>

#include "ScreenManager.h"
#include "Settings.h"
#include "ShaderManager.h"

void renderScaling(sf::RenderWindow &window, sf::Sprite &sprite);

int main() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Pac Man");

  // do all rendering on this renderTexture
  sf::RenderTexture renderArea;
  sf::Sprite tmpsprite;

  // settings class
  Settings settings;
  bool vsync = false;
  bool fscreen = false;
  window.setVerticalSyncEnabled(false);

  ShaderManager shaders;
  ScreenManager screenManager(&settings);

  sf::Clock updateClock;
  sf::Time updateRate = sf::seconds(1.0 / 30.0);  // 30 Hz update rate
  sf::Time prevUpdate = sf::seconds(0);
  sf::Clock *clockptr = &updateClock;

  // create render area with original game's resolution
  renderArea.create(224, 288);

  // load shaders
  shaders.loadShaders();

  // scale stuff
  renderScaling(window, tmpsprite);
  srand(time(NULL));
  // render and update stuff here
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::Resized) {
        // resize window and scale correctly
        window.setView(
            sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        renderScaling(window, tmpsprite);
        shaders.resize(window.getView().getSize());
      } else {
        screenManager.handleEvent(&event);
        if (screenManager.getCurrScreen() == MAPEDITOR) {
          screenManager.mouse_update(window);
        }
      }
    }
    // update
    prevUpdate += updateClock.restart();  // add elapsed time to previous update
    while (prevUpdate >=
           updateRate) {  // update as long as the time since the previous
                          // update is greater than the update rate
      // fullscreen
      if (fscreen != settings.useFullScreen()) {
        fscreen = settings.useFullScreen();
        if (fscreen) {
          window.create(sf::VideoMode::getFullscreenModes().at(0), "Pac Man",
                        sf::Style::Fullscreen);
        } else {
          window.create(sf::VideoMode(1280, 720), "Pac Man");
        }
        window.setView(sf::View(
            sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));
        renderScaling(window, tmpsprite);
        shaders.resize(window.getView().getSize());
        vsync = !settings.useVsync();
      }
      // vsync
      if (vsync != settings.useVsync()) {
        vsync = settings.useVsync();
        window.setVerticalSyncEnabled(vsync);
      }
      if (settings.quit) {
        window.close();
      }
      // do update stuff
      screenManager.update();
      // Subtract update rate from previousupdate
      prevUpdate -= updateRate;
    }

    // render
    // draw pacman texture on render area
    screenManager.render(&renderArea);
    // render with shaders if theyre toggled
    if (settings.useShaders() && screenManager.getCurrScreen() != MAPEDITOR) {
      shaders.render(window, renderArea);
    } else {
      window.clear();
      tmpsprite.setTexture(renderArea.getTexture(), true);
      window.draw(tmpsprite);
    }

    window.display();
  }

  return 0;
}

void renderScaling(sf::RenderWindow &window, sf::Sprite &sprite) {
  float scale = 1.0;
  sf::Vector2f wsize = window.getView().getSize();
  sf::Vector2u ssize(224, 288);

  // ratio comparison to know whether to scale to fit vertically or
  // horizontally
  if (wsize.x / wsize.y > ssize.x / ssize.y) {
    // scale to fit vertically
    scale = wsize.y / ssize.y;
    sprite.setScale(sf::Vector2f(scale, scale));
  } else {
    scale = wsize.x / ssize.x;
    sprite.setScale(sf::Vector2f(scale, scale));
  }
  // center positon as well
  sprite.setPosition((wsize.x - ssize.x * scale) / 2,
                     (wsize.y - ssize.y * scale) / 2);
}