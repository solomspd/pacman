#include "tile.h"

tile::tile() {}

void tile::setTexture(sf::Texture* tex) {
  spritesheet = tex;
  sprite.setTexture(*spritesheet);
}

void tile::set_type(tile_types new_type) { type = new_type; }

tile_types tile::get_type() { return type; }

sf::Sprite* tile::getSprite() { return &sprite; }

void tile::setWall(wallType t) {
  wType = t;
  switch (t) {
    case tile::TRCORNER:
      sprite.setTextureRect(sf::IntRect(6 * 8, 0, 8, 8));
      break;
    case tile::TLCORNER:
      sprite.setTextureRect(sf::IntRect(7 * 8, 0, 8, 8));
      break;
    case tile::RSIDE:
      sprite.setTextureRect(sf::IntRect(8 * 8, 0, 8, 8));
      break;
    case tile::LSIDE:
      sprite.setTextureRect(sf::IntRect(9 * 8, 0, 8, 8));
      break;
    case tile::BRCORNER:
      sprite.setTextureRect(sf::IntRect(10 * 8, 0, 8, 8));
      break;
    case tile::BLCORNER:
      sprite.setTextureRect(sf::IntRect(11 * 8, 0, 8, 8));
      break;
    case tile::IBRCORNER:
      sprite.setTextureRect(sf::IntRect(16 * 8, 8, 8, 8));
      break;
    case tile::IBLCORNER:
      sprite.setTextureRect(sf::IntRect(17 * 8, 8, 8, 8));
      break;
    case tile::ITRCORNER:
      sprite.setTextureRect(sf::IntRect(12 * 8, 8, 8, 8));
      break;
    case tile::ITLCORNER:
      sprite.setTextureRect(sf::IntRect(13 * 8, 8, 8, 8));
      break;
    case tile::TSIDE:
      sprite.setTextureRect(sf::IntRect(16 * 8, 0, 8, 8));
      break;
    case tile::BSIDE:
      sprite.setTextureRect(sf::IntRect(18 * 8, 0, 8, 8));
      break;
    case tile::IBSIDE:
      // TODO
      sprite.setTextureRect(sf::IntRect(10 * 8, 8, 8, 8));
      break;
    case tile::ITSIDE:
      sprite.setTextureRect(sf::IntRect(20 * 8, 0, 8, 8));
      break;
    case tile::ILSIDE:
      sprite.setTextureRect(sf::IntRect(15 * 8, 8, 8, 8));
      break;
    case tile::IRSIDE:
      sprite.setTextureRect(sf::IntRect(14 * 8, 8, 8, 8));
      break;
    case tile::VTRCORNER:
      sprite.setTextureRect(sf::IntRect(16 * 8, 8 * 2, 8, 8));
      break;
    case tile::VTLCORNER:
      sprite.setTextureRect(sf::IntRect(17 * 8, 8 * 2, 8, 8));
      break;
    case tile::VLTCORNER:
      sprite.setTextureRect(sf::IntRect(13 * 8, 0, 8, 8));
      break;
    case tile::VLBCORNER:
      sprite.setTextureRect(sf::IntRect(15 * 8, 0, 8, 8));
      break;
    case tile::VRTCORNER:
      sprite.setTextureRect(sf::IntRect(12 * 8, 0, 8, 8));
      break;
    case tile::VRBCORNER:
      sprite.setTextureRect(sf::IntRect(14 * 8, 0, 8, 8));
      break;
    case tile::IVTRCORNER:
      sprite.setTextureRect(sf::IntRect(10 * 8, 8 * 2, 8, 8));
      break;
    case tile::IVTLCORNER:
      sprite.setTextureRect(sf::IntRect(17 * 8, 8 * 2, 8, 8));
      break;
    case tile::IVLTCORNER:
      sprite.setTextureRect(sf::IntRect(11 * 8, 8 * 2, 8, 8));
      break;
    case tile::IVLBCORNER:
      sprite.setTextureRect(sf::IntRect(9 * 8, 8 * 2, 8, 8));
      break;
    case tile::IVRTCORNER:
      sprite.setTextureRect(sf::IntRect(10 * 8, 8 * 2, 8, 8));
      break;
    case tile::IVRBCORNER:
      sprite.setTextureRect(sf::IntRect(8 * 8, 8 * 2, 8, 8));
      break;
    case tile::LTTCORNER:
      sprite.setTextureRect(sf::IntRect(14 * 8, 8 * 3, 8, 8));
      break;
    case tile::RTTCORNER:
      sprite.setTextureRect(sf::IntRect(13 * 8, 8 * 3, 8, 8));
      break;
    case tile::TTRCORNER:
      sprite.setTextureRect(sf::IntRect(16 * 8, 8 * 3, 8, 8));
      break;
    case tile::TTLCORNER:
      sprite.setTextureRect(sf::IntRect(15 * 8, 8 * 3, 8, 8));
      break;
    case tile::BTRCORNER:
      sprite.setTextureRect(sf::IntRect(17 * 8, 8 * 3, 8, 8));
      break;
    case tile::BTLCORNER:
      sprite.setTextureRect(sf::IntRect(18 * 8, 8 * 3, 8, 8));
      break;
    case tile::TLCAP:
      sprite.setTextureRect(sf::IntRect(6 * 8, 8, 8, 8));
      break;
    case tile::TRCAP:
      sprite.setTextureRect(sf::IntRect(7 * 8, 8, 8, 8));
      break;
    case tile::TLSCAP:
      sprite.setTextureRect(sf::IntRect(22 * 8, 8 * 3, 8, 8));
      break;
    case tile::TRSCAP:
      sprite.setTextureRect(sf::IntRect(21 * 8, 8 * 3, 8, 8));
      break;
    case tile::BLCAP:
      sprite.setTextureRect(sf::IntRect(8 * 8, 8, 8, 8));
      break;
    case tile::BRCAP:
      sprite.setTextureRect(sf::IntRect(9 * 8, 8, 8, 8));
      break;
    case tile::TRSCORNER:
      sprite.setTextureRect(sf::IntRect(12 * 8, 8, 8, 8));
      break;
    case tile::TLSCORNER:
      sprite.setTextureRect(sf::IntRect(13 * 8, 8, 8, 8));
      break;
    case tile::BRSCORNER:
      sprite.setTextureRect(sf::IntRect(16 * 8, 8, 8, 8));
      break;
    case tile::BLSCORNER:
      sprite.setTextureRect(sf::IntRect(17 * 8, 8, 8, 8));
      break;
    default:
      break;
  }
}

tile::wallType tile::getWall() { return wType; }

tile::~tile() {}