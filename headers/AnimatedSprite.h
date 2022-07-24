#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum animType { LOOPF = 0, ONCE, PINGPONG, NONE };
class AnimatedSprite : public sf::Sprite {
 private:
  int curFrame = 0;
  bool increasing = true;
  std::vector<sf::IntRect> rects;
  animType type;

 public:
  void setAnim(animType type, int x, int y, int width, int height, int frames,
               bool horizontal) {
    rects.clear();
    this->type = type;
    for (int i = 0; i < frames; i++) {
      if (horizontal) {
        sf::IntRect rect(x + width * i, y, width, height);
        rects.push_back(rect);
      } else {
        sf::IntRect rect(x, y + height * i, width, height);
        rects.push_back(rect);
      }
    }
    this->setTextureRect(rects.at(curFrame));
  }

  void addFrame(sf::IntRect frameRect) { rects.push_back(frameRect); }

  void nextFrame() {
    if (type == LOOPF) {
      curFrame++;
      if (curFrame >= rects.size()) curFrame = 0;
    } else if (type == ONCE) {
      curFrame++;
      if (curFrame >= rects.size()) curFrame = rects.size() - 1;
    } else if (type == PINGPONG) {
      if (increasing) {
        curFrame++;
        if (curFrame >= rects.size()) {
          curFrame = rects.size() - 2;
          increasing = false;
        }
      } else {
        curFrame--;
        if (curFrame <= -1) {
          curFrame = 1;
          increasing = true;
        }
      }
    }

    this->setTextureRect(rects.at(curFrame));
  }

  void prevFrame() {
    if (type == LOOPF) {
      curFrame--;
      if (curFrame < 0) curFrame = rects.size() - 1;
    } else if (type == ONCE) {
      curFrame--;
      if (curFrame < 0) curFrame = 0;
    } else if (type == PINGPONG) {
      if (increasing) {
        curFrame--;
        if (curFrame <= -1) {
          curFrame = 1;
          increasing = false;
        }
      } else {
        curFrame--;
        if (curFrame >= rects.size()) {
          curFrame = rects.size() - 2;
          increasing = true;
        }
      }
    }

    this->setTextureRect(rects.at(curFrame));
  }
};