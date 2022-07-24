#include "board.h"

board::board() {
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      map[i][j].set_type(empty);
    }
  }
}

board::board(std::ifstream& infile, sf::Texture* sprites, bool* SFX) {
  this->useSFX = SFX;
  // save spritesheet pointer
  spritesheet = sprites;

  // creating an array of character to derive mapshapes from
  char map_txt[vtiles][htiles];
  // if (infile.is_open) {
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      char temp;
      infile.get(temp);
      if (temp != '\n') {
        map_txt[i][j] = temp;
      } else {
        j--;
      }
    }
  }
  //}
  // else {
  // log and error output function to be place here
  //}

  // analysing text array to figure out placements of all elements
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      // set spritesheet of tile and sprite position
      map[i][j].setTexture(sprites);
      map[i][j].getSprite()->setPosition(j * tile_size, i * tile_size);

      if (map_txt[i][j] == 'M') {
        map[i][j].set_type(pacman);
      } else if (map_txt[i][j] == 'R') {
        map[i][j].set_type(red);
      } else if (map_txt[i][j] == 'B') {
        map[i][j].set_type(blue);
      } else if (map_txt[i][j] == 'P') {
        map[i][j].set_type(pink);
      } else if (map_txt[i][j] == 'Y') {
        map[i][j].set_type(yellow);  // orange
      } else if (map_txt[i][j] == 'C') {
        map[i][j].set_type(coin);
        numOfCoins++;
      } else if (map_txt[i][j] == 'S') {
        map[i][j].set_type(big_coin);
        numOfCoins++;
      } else if (map_txt[i][j] == 'E') {
        map[i][j].set_type(empty);
      } else if (map_txt[i][j] == 'W') {
        map[i][j].set_type(wall);

        // setting walltype based on neighbor walls
        tile::wallType horizontalWall;
        tile::wallType verticalWall;
        if ((i < vtiles / 2 && !(i > 7 && i < 12)) ||
            (i > 14 && i < 19)) {  // if upper half or below tunnel
          horizontalWall = tile::TSIDE;
        } else {
          horizontalWall = tile::BSIDE;
        }

        if (j < htiles / 2) {  // if left half
          verticalWall = tile::LSIDE;
        } else {
          verticalWall = tile::RSIDE;
        }

        // check if inner horizontal bottom side
        if (i > 0 && i < (vtiles - 1) && j > 0 && j < (htiles - 1) &&
            map_txt[i - 1][j] == 'W' && map_txt[i][j + 1] == 'W' &&
            map_txt[i][j - 1] == 'W') {
          map[i][j].setWall(tile::IBSIDE);
          // check if inner horizontal top side
        } else if (i > 0 && i < (vtiles - 1) && j > 0 && j < (htiles - 1) &&
                   map_txt[i + 1][j] == 'W' && map_txt[i][j + 1] == 'W' &&
                   map_txt[i][j - 1] == 'W') {
          map[i][j].setWall(tile::ITSIDE);
          // check if inner left side
        } else if (i > 0 && i < (vtiles - 1) && j > 0 && j < (htiles - 1) &&
                   map_txt[i + 1][j] == 'W' && map_txt[i - 1][j] == 'W' &&
                   map_txt[i][j + 1] == 'W') {
          map[i][j].setWall(tile::ILSIDE);
          // check if inner right side
        } else if (i > 0 && i < (vtiles - 1) && j > 0 && j < (htiles - 1) &&
                   map_txt[i + 1][j] == 'W' && map_txt[i - 1][j] == 'W' &&
                   map_txt[i][j - 1] == 'W') {
          map[i][j].setWall(tile::IRSIDE);
          // check if tiles above and below
        } else if (i > 0 && i < (vtiles - 1) && map_txt[i - 1][j] == 'W' &&
                   map_txt[i + 1][j] == 'W') {
          map[i][j].setWall(verticalWall);
          // check if tiles right and left
        } else if (j > 0 && j < (htiles - 1) && map_txt[i][j - 1] == 'W' &&
                   map_txt[i][j + 1] == 'W') {
          map[i][j].setWall(horizontalWall);
        } else {
          map[i][j].setWall(tile::BLCORNER);  // placeholder for corners
        }
      } else {
        // log and error output function to be placed here
      }
    }
  }

  // another pass for corner wall tiles
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      // is corner?
      if (map[i][j].get_type() == wall &&
          map[i][j].getWall() == tile::BLCORNER) {
        // is topleft thick corner?
        if ((j == 0 || map[i][j - 1].get_type() != wall) &&
            (j < htiles - 1 && map[i][j + 1].getWall() == tile::TSIDE) &&
            (i < vtiles - 1 && map[i + 1][j].getWall() == tile::LSIDE)) {
          map[i][j].setWall(tile::TLCORNER);
          // is topright thick corner?
        } else if ((j == htiles - 1 || map[i][j + 1].get_type() != wall) &&
                   (j > 0 && map[i][j - 1].getWall() == tile::TSIDE) &&
                   (i < vtiles - 1 && map[i + 1][j].getWall() == tile::RSIDE)) {
          map[i][j].setWall(tile::TRCORNER);
          // is bottomright thick corner?
        } else if ((j == htiles - 1 || map[i][j + 1].get_type() != wall) &&
                   (j > 0 && map[i][j - 1].getWall() == tile::BSIDE) &&
                   (i == vtiles - 1 ||
                    map[i - 1][j].getWall() == tile::RSIDE)) {
          map[i][j].setWall(tile::BRCORNER);
          // is topleft thin corner?
        } else if ((j == 0 || map[i][j - 1].get_type() != wall) &&
                       ((j < htiles - 1 &&
                         map[i][j + 1].getWall() == tile::ITSIDE) &&
                        (i < vtiles - 1 &&
                         map[i + 1][j + 1].getWall() == tile::IBSIDE)) ||
                   ((i < vtiles - 1 &&
                     map[i + 1][j + 1].getWall() == tile::IRSIDE) &&
                    (i > 0 && map[i + 1][j].getWall() == tile::ILSIDE))) {
          map[i][j].setWall(tile::ITLCORNER);
          // is topright thin corner?
        } else if ((j == htiles - 1 || map[i][j + 1].get_type() != wall) &&
                       ((j > 0 && map[i][j - 1].getWall() == tile::ITSIDE) &&
                        (i < vtiles - 1 &&
                         map[i + 1][j - 1].getWall() == tile::IBSIDE)) ||
                   ((i < vtiles - 1 &&
                     map[i + 1][j - 1].getWall() == tile::ILSIDE) &&
                    (i > 0 && map[i + 1][j].getWall() == tile::IRSIDE))) {
          map[i][j].setWall(tile::ITRCORNER);
          // is bottomright thin corner?
        } else if ((j == htiles - 1 || map[i][j + 1].get_type() != wall) &&
                       ((j > 0 && map[i][j - 1].getWall() == tile::IBSIDE) &&
                        (i == vtiles - 1 ||
                         map[i - 1][j - 1].getWall() == tile::ITSIDE)) ||
                   ((i > 0 && map[i - 1][j].getWall() == tile::IRSIDE) &&
                    (i > 0 && map[i - 1][j - 1].getWall() == tile::ILSIDE))) {
          map[i][j].setWall(tile::IBRCORNER);
          // is bottomleft thin corner?
        } else if ((j == 0 || map[i][j - 1].get_type() != wall) &&
                       ((j < htiles - 1 &&
                         map[i][j + 1].getWall() == tile::IBSIDE) &&
                        (i > 0 &&
                         map[i - 1][j + 1].getWall() == tile::ITSIDE)) ||
                   ((i > 0 && map[i - 1][j + 1].getWall() == tile::IRSIDE) &&
                    (i > 0 && map[i - 1][j].getWall() == tile::ILSIDE))) {
          map[i][j].setWall(tile::IBLCORNER);
          // is left to top corner?
        } else if (map[i - 1][j].getWall() == tile::LSIDE &&
                   map[i][j + 1].getWall() == tile::TSIDE) {
          map[i][j].setWall(tile::LTTCORNER);
          // is right to top corner?
        } else if (map[i - 1][j].getWall() == tile::RSIDE &&
                   map[i][j - 1].getWall() == tile::TSIDE) {
          map[i][j].setWall(tile::RTTCORNER);
          // is top to right corner?
        } else if (map[i + 1][j].getWall() == tile::RSIDE &&
                   map[i][j + 1].getWall() == tile::TSIDE) {
          map[i][j].setWall(tile::TTRCORNER);
          // is top to left corner?
        } else if (map[i + 1][j].getWall() == tile::LSIDE &&
                   map[i][j - 1].getWall() == tile::TSIDE) {
          map[i][j].setWall(tile::TTLCORNER);
          // is bottom to left corner?
        } else if (map[i + 1][j].getWall() == tile::LSIDE &&
                   map[i][j + 1].getWall() == tile::BSIDE) {
          map[i][j].setWall(tile::BTLCORNER);
          // is bottom to right corner?
        } else if (map[i + 1][j].getWall() == tile::RSIDE &&
                   map[i][j - 1].getWall() == tile::BSIDE) {
          map[i][j].setWall(tile::BTRCORNER);
          // is top right cap?
        } else if (map[i][j + 1].getWall() == tile::TSIDE &&
                   map[i][j - 1].get_type() != wall &&
                   map[i + 1][j].get_type() != wall &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::TRCAP);
          // is top left cap?
        } else if (map[i][j - 1].getWall() == tile::TSIDE &&
                   map[i][j + 1].get_type() != wall &&
                   map[i + 1][j].get_type() != wall &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::TLCAP);
          // is bottom right cap?
        } else if (map[i][j + 1].getWall() == tile::BSIDE &&
                   map[i][j - 1].get_type() != wall &&
                   map[i + 1][j].get_type() != wall &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::BRCAP);
          // is bottom left cap?
        } else if (map[i][j - 1].getWall() == tile::BSIDE &&
                   map[i][j + 1].get_type() != wall &&
                   map[i + 1][j].get_type() != wall &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::BLCAP);
          // is top right single corner?
        } else if (map[i][j - 1].getWall() == tile::BSIDE &&
                   map[i][j + 1].get_type() != wall &&
                   map[i + 1][j].getWall() == tile::LSIDE &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::TRSCORNER);
          // is top left single corner?
        } else if (map[i][j + 1].getWall() == tile::BSIDE &&
                   map[i][j - 1].get_type() != wall &&
                   map[i + 1][j].getWall() == tile::RSIDE &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::TLSCORNER);
          // is bottom right single corner?
        } else if (map[i][j - 1].getWall() == tile::TSIDE &&
                   map[i][j + 1].get_type() != wall &&
                   map[i - 1][j].getWall() == tile::LSIDE &&
                   map[i + 1][j].get_type() != wall) {
          map[i][j].setWall(tile::BRSCORNER);
          // is bottom left single corner?
        } else if (map[i][j + 1].getWall() == tile::TSIDE &&
                   map[i][j - 1].get_type() != wall &&
                   map[i - 1][j].getWall() == tile::RSIDE &&
                   map[i + 1][j].get_type() != wall) {
          map[i][j].setWall(tile::BLSCORNER);
          // is inverted top right corner
        } else if (map[i - 1][j].getWall() ==
                   tile::ILSIDE) {  // && map[i][j + 1].getWall() ==
                                    // tile::ITSIDE) {
          map[i][j].setWall(tile::BLSCORNER);
          // is brscorner
        } else if (map[i - 1][j].getWall() ==
                   tile::IRSIDE) {  // && map[i][j + 1].getWall() ==
                                    // tile::ITSIDE) {
          map[i][j].setWall(tile::BRSCORNER);
        } else if (map[i][j - 1].get_type() != wall &&
                   map[i + 1][j].getWall() == tile::RSIDE &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::TRSCAP);
          // is top left cap?
        } else if (map[i][j + 1].get_type() != wall &&
                   map[i][j - 1].get_type() != wall &&
                   map[i + 1][j].getWall() == tile::LSIDE &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::TLSCAP);
          // is bottom right cap?
        }
      } else if (map[i][j].get_type() == wall &&
                 map[i][j].getWall() == tile::TSIDE) {
        // is inverted top left corner?
        if (map[i + 1][j].getWall() == tile::ILSIDE) {
          map[i][j].setWall(tile::VTLCORNER);
          // is inverted top right corner?
        } else if (map[i + 1][j].getWall() == tile::IRSIDE) {
          map[i][j].setWall(tile::VTRCORNER);
        }
      } else if (map[i][j].get_type() == wall &&
                 map[i][j].getWall() == tile::LSIDE) {
        // is inverted left top corner?
        if (map[i][j + 1].getWall() == tile::ITSIDE) {
          map[i][j].setWall(tile::VLTCORNER);
          // is inverted left bottom corner?
        } else if (map[i][j + 1].getWall() == tile::IBSIDE) {
          map[i][j].setWall(tile::VLBCORNER);
        }
      } else if (map[i][j].get_type() == wall &&
                 map[i][j].getWall() == tile::RSIDE) {
        // is inverted Right top corner?
        if (map[i][j - 1].getWall() == tile::ITSIDE) {
          map[i][j].setWall(tile::VRTCORNER);
          // is inverted right bottom corner?
        } else if (map[i][j - 1].getWall() == tile::IBSIDE) {
          map[i][j].setWall(tile::VRBCORNER);
        }
      } else if (map[i][j].get_type() == wall &&
                 map[i][j].getWall() == tile::IBSIDE) {
        // is inverted internal right top corner?
        if (map[i - 1][j].getWall() == tile::IRSIDE) {
          map[i][j].setWall(tile::IVRTCORNER);
          // is inverted internal right bottom corner?
        } else if (map[i + 1][j].getWall() == tile::IRSIDE) {
          map[i][j].setWall(tile::IVRBCORNER);
          // is inverted internal left top corner?
        } else if (map[i - 1][j].getWall() == tile::ILSIDE) {
          map[i][j].setWall(tile::IVLTCORNER);
          // is inverted internal top right corner?
        } else if (map[i + 1][j].getWall() == tile::ILSIDE) {
          map[i][j].setWall(tile::IVLBCORNER);
        }
      }
    }
  }

  // prerender walls because faster that way
  wallsRendered.create(htiles * 8, vtiles * 8);
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      if (map[i][j].get_type() == wall) {
        wallsRendered.draw(*map[i][j].getSprite());
      }
    }
  }
  wallsRendered.display();
}

void board::update_sprites() {
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      // is corner?
      if (map[i][j].get_type() == wall &&
          map[i][j].getWall() == tile::BLCORNER) {
        // is topleft thick corner?
        if ((j == 0 || map[i][j - 1].get_type() != wall) &&
            (j < htiles - 1 && map[i][j + 1].getWall() == tile::TSIDE) &&
            (i < vtiles - 1 && map[i + 1][j].getWall() == tile::LSIDE)) {
          map[i][j].setWall(tile::TLCORNER);
          // is topright thick corner?
        } else if ((j == htiles - 1 || map[i][j + 1].get_type() != wall) &&
                   (j > 0 && map[i][j - 1].getWall() == tile::TSIDE) &&
                   (i < vtiles - 1 && map[i + 1][j].getWall() == tile::RSIDE)) {
          map[i][j].setWall(tile::TRCORNER);
          // is bottomright thick corner?
        } else if ((j == htiles - 1 || map[i][j + 1].get_type() != wall) &&
                   (j > 0 && map[i][j - 1].getWall() == tile::BSIDE) &&
                   (i == vtiles - 1 ||
                    map[i - 1][j].getWall() == tile::RSIDE)) {
          map[i][j].setWall(tile::BRCORNER);
          // is topleft thin corner?
        } else if ((j == 0 || map[i][j - 1].get_type() != wall) &&
                       ((j < htiles - 1 &&
                         map[i][j + 1].getWall() == tile::ITSIDE) &&
                        (i < vtiles - 1 &&
                         map[i + 1][j + 1].getWall() == tile::IBSIDE)) ||
                   ((i < vtiles - 1 &&
                     map[i + 1][j + 1].getWall() == tile::IRSIDE) &&
                    (i > 0 && map[i + 1][j].getWall() == tile::ILSIDE))) {
          map[i][j].setWall(tile::ITLCORNER);
          // is topright thin corner?
        } else if ((j == htiles - 1 || map[i][j + 1].get_type() != wall) &&
                       ((j > 0 && map[i][j - 1].getWall() == tile::ITSIDE) &&
                        (i < vtiles - 1 &&
                         map[i + 1][j - 1].getWall() == tile::IBSIDE)) ||
                   ((i < vtiles - 1 &&
                     map[i + 1][j - 1].getWall() == tile::ILSIDE) &&
                    (i > 0 && map[i + 1][j].getWall() == tile::IRSIDE))) {
          map[i][j].setWall(tile::ITRCORNER);
          // is bottomright thin corner?
        } else if ((j == htiles - 1 || map[i][j + 1].get_type() != wall) &&
                       ((j > 0 && map[i][j - 1].getWall() == tile::IBSIDE) &&
                        (i == vtiles - 1 ||
                         map[i - 1][j - 1].getWall() == tile::ITSIDE)) ||
                   ((i > 0 && map[i - 1][j].getWall() == tile::IRSIDE) &&
                    (i > 0 && map[i - 1][j - 1].getWall() == tile::ILSIDE))) {
          map[i][j].setWall(tile::IBRCORNER);
          // is bottomleft thin corner?
        } else if ((j == 0 || map[i][j - 1].get_type() != wall) &&
                       ((j < htiles - 1 &&
                         map[i][j + 1].getWall() == tile::IBSIDE) &&
                        (i > 0 &&
                         map[i - 1][j + 1].getWall() == tile::ITSIDE)) ||
                   ((i > 0 && map[i - 1][j + 1].getWall() == tile::IRSIDE) &&
                    (i > 0 && map[i - 1][j].getWall() == tile::ILSIDE))) {
          map[i][j].setWall(tile::IBLCORNER);
          // is left to top corner?
        } else if (map[i - 1][j].getWall() == tile::LSIDE &&
                   map[i][j + 1].getWall() == tile::TSIDE) {
          map[i][j].setWall(tile::LTTCORNER);
          // is right to top corner?
        } else if (map[i - 1][j].getWall() == tile::RSIDE &&
                   map[i][j - 1].getWall() == tile::TSIDE) {
          map[i][j].setWall(tile::RTTCORNER);
          // is top to right corner?
        } else if (map[i + 1][j].getWall() == tile::RSIDE &&
                   map[i][j + 1].getWall() == tile::TSIDE) {
          map[i][j].setWall(tile::TTRCORNER);
          // is top to left corner?
        } else if (map[i + 1][j].getWall() == tile::LSIDE &&
                   map[i][j - 1].getWall() == tile::TSIDE) {
          map[i][j].setWall(tile::TTLCORNER);
          // is bottom to left corner?
        } else if (map[i + 1][j].getWall() == tile::LSIDE &&
                   map[i][j + 1].getWall() == tile::BSIDE) {
          map[i][j].setWall(tile::BTLCORNER);
          // is bottom to right corner?
        } else if (map[i + 1][j].getWall() == tile::RSIDE &&
                   map[i][j - 1].getWall() == tile::BSIDE) {
          map[i][j].setWall(tile::BTRCORNER);
          // is top right cap?
        } else if (map[i][j + 1].getWall() == tile::TSIDE &&
                   map[i][j - 1].get_type() != wall &&
                   map[i + 1][j].get_type() != wall &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::TRCAP);
          // is top left cap?
        } else if (map[i][j - 1].getWall() == tile::TSIDE &&
                   map[i][j + 1].get_type() != wall &&
                   map[i + 1][j].get_type() != wall &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::TLCAP);
          // is bottom right cap?
        } else if (map[i][j + 1].getWall() == tile::BSIDE &&
                   map[i][j - 1].get_type() != wall &&
                   map[i + 1][j].get_type() != wall &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::BRCAP);
          // is bottom left cap?
        } else if (map[i][j - 1].getWall() == tile::BSIDE &&
                   map[i][j + 1].get_type() != wall &&
                   map[i + 1][j].get_type() != wall &&
                   map[i - 1][j].get_type() != wall) {
          map[i][j].setWall(tile::BLCAP);
        }
      } else if (map[i][j].get_type() == wall &&
                 map[i][j].getWall() == tile::TSIDE) {
        // is inverted top left corner?
        if (map[i + 1][j].getWall() == tile::ILSIDE) {
          map[i][j].setWall(tile::VTLCORNER);
          // is inverted top right corner?
        } else if (map[i + 1][j].getWall() == tile::IRSIDE) {
          map[i][j].setWall(tile::VTRCORNER);
        }
      } else if (map[i][j].get_type() == wall &&
                 map[i][j].getWall() == tile::LSIDE) {
        // is inverted left top corner?
        if (map[i][j + 1].getWall() == tile::ITSIDE) {
          map[i][j].setWall(tile::VLTCORNER);
          // is inverted left bottom corner?
        } else if (map[i][j + 1].getWall() == tile::IBSIDE) {
          map[i][j].setWall(tile::VLBCORNER);
        }
      } else if (map[i][j].get_type() == wall &&
                 map[i][j].getWall() == tile::RSIDE) {
        // is inverted Right top corner?
        if (map[i][j - 1].getWall() == tile::ITSIDE) {
          map[i][j].setWall(tile::VRTCORNER);
          // is inverted right bottom corner?
        } else if (map[i][j - 1].getWall() == tile::IBSIDE) {
          map[i][j].setWall(tile::VRBCORNER);
        }
      } else if (map[i][j].get_type() == wall &&
                 map[i][j].getWall() == tile::IBSIDE) {
        // is inverted internal right top corner?
        if (map[i - 1][j].getWall() == tile::IRSIDE) {
          map[i][j].setWall(tile::IVRTCORNER);
          // is inverted internal right bottom corner?
        } else if (map[i + 1][j].getWall() == tile::IRSIDE) {
          map[i][j].setWall(tile::IVRBCORNER);
          // is inverted internal left top corner?
        } else if (map[i - 1][j].getWall() == tile::ILSIDE) {
          map[i][j].setWall(tile::IVLTCORNER);
          // is inverted internal Left bottom corner?
        } else if (map[i + 1][j].getWall() == tile::ILSIDE) {
          map[i][j].setWall(tile::IVLBCORNER);
        }
      }
    }
  }
}

void board::switch_coin(int x, int y) {
  map[y][x].set_type(empty);
  numOfCoins--;
}

tile_types board::get_tile_type(int x, int y) { return map[y][x].get_type(); }

void board::find_tile(int& x, int& y, tile_types querry) {
  bool found = false;
  for (x = 0; !found && x < htiles; x++) {
    for (y = 0; !found && y < vtiles; y++) {
      if (querry == map[y][x].get_type()) {
        found = true;
      }
    }
  }
  x--;
  y--;
}

void board::set_tile_type(int x, int y, tile_types new_type) {
  map[x][y].set_type(new_type);
}

void board::test_board() {
  std::cout << std::endl;
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      std::cout << map[i][j].get_type();
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void board::render(sf::RenderTexture* renderer) {
  // render prerendered walls
  sf::RectangleShape temp_box;
  temp_box.setSize(sf::Vector2f(htiles * 8, vtiles * 8));
  temp_box.setPosition(0, 0);
  temp_box.setTexture(&wallsRendered.getTexture());
  renderer->draw(temp_box);
  temp_box.setTexture(spritesheet);
  for (int i = 0; i < vtiles - 1; i++) {
    for (int j = 0; j < htiles; j++) {
      temp_box.setPosition(j * tile_size, i * tile_size);
      temp_box.setSize(sf::Vector2f(tile_size, tile_size));
      switch (map[i][j].get_type()) {
        case (empty):  // temp_box.setFillColor(sf::Color::Black);
          break;
        case (wall):
          // temp_box.setFillColor(sf::Color::Blue);
          break;
        case (big_coin):
          temp_box.setTextureRect(sf::IntRect(21 * 8, 2 * 8, 8, 8));
          renderer->draw(temp_box);
          // temp_box.setFillColor(sf::Color::White);
          break;
        case (coin):
          temp_box.setTextureRect(sf::IntRect(19 * 8, 2 * 8, 8, 8));
          // temp_box.setFillColor(sf::Color::Red);
          renderer->draw(temp_box);
          break;
      }
      renderer->display();
    }
  }
}

void board::render_alternate(sf::RenderTexture* renderer) {
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      sf::RectangleShape temp_box;
      temp_box.setPosition(j * tile_size + 1, i * tile_size + 1);
      temp_box.setSize(sf::Vector2f(tile_size - 1, tile_size - 1));
      temp_box.setScale(1, 1);
      switch (map[i][j].get_type()) {
        case (empty):
          temp_box.setFillColor(sf::Color::Black);
          break;
        case (pacman):
          temp_box.setFillColor(sf::Color::Yellow);
          break;
        case (wall):
          temp_box.setFillColor(sf::Color::Blue);
          break;
        case (red):
          temp_box.setFillColor(sf::Color::Red);
          break;
        case (yellow):
          temp_box.setFillColor(sf::Color::Yellow);
          break;
        case (blue):
          temp_box.setFillColor(sf::Color::Cyan);
          break;
        case (pink):
          temp_box.setFillColor(sf::Color::Magenta);
          break;
        case (big_coin):
          temp_box.setFillColor(sf::Color::White);
          break;
        case (coin):
          temp_box.setFillColor(sf::Color::White);
          temp_box.setSize(
              sf::Vector2f((tile_size - 1) / 2, (tile_size - 1) / 2));
          temp_box.move(2, 2);
          break;
      }
      renderer->draw(temp_box);
      // renderer->display();
    }
  }
}

void board::setBoardName(sf::String boardName) { this->boardName = boardName; }
sf::String board::getBoardName() { return boardName; }

sf::Texture* board::getSpritesheet() { return spritesheet; }

board::~board() {}

void board::find_position(float& x, float& y, tile_types p) {
  for (int i = 0; i < vtiles; i++) {
    for (int j = 0; j < htiles; j++) {
      if (map[i][j].get_type() == p) x = j, y = i;
    }
  }
}