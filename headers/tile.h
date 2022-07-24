#pragma once
#include <SFML/Graphics.hpp>

enum tile_types {
  pacman,
  red,
  yellow,
  pink,
  blue,
  coin,
  big_coin,
  wall,
  empty
};

const int tile_size = 8;

class tile {
 public:
  enum wallType {
    TRCORNER,
    TLCORNER,
    RSIDE,
    LSIDE,
    BRCORNER,
    BLCORNER,
    IBRCORNER,
    IBLCORNER,
    ITRCORNER,
    ITLCORNER,
    TSIDE,
    BSIDE,
    IBSIDE,
    ITSIDE,
    ILSIDE,
    IRSIDE,
    IVTRCORNER,
    IVTLCORNER,
    IVLTCORNER,
    IVLBCORNER,
    IVRTCORNER,
    IVRBCORNER,
    VTRCORNER,
    VTLCORNER,
    VLTCORNER,
    VLBCORNER,
    VRTCORNER,
    VRBCORNER,
    LTTCORNER,
    RTTCORNER,
    TTLCORNER,
    TTRCORNER,
    BTRCORNER,
    BTLCORNER,
    TLCAP,
    TRCAP,
    BLCAP,
    BRCAP,
    TRSCORNER,
    TLSCORNER,
    BRSCORNER,
    BLSCORNER,
    TLSCAP,
    TRSCAP
  };
  sf::Sprite sprite;
  sf::Texture* spritesheet;
  tile();
  void setWall(wallType);
  wallType getWall();
  tile_types get_type();
  void setTexture(sf::Texture*);
  void set_type(tile_types);
  sf::Sprite* getSprite();

  ~tile();

 private:
  tile_types type;
  wallType wType;
};
// test
