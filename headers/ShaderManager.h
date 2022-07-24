// TODO: Scaling

#pragma once
#include <SFML/Graphics.hpp>

class ShaderManager {
 private:
  sf::RenderTexture og;
  sf::RenderTexture scanlines;
  sf::RenderTexture blur1;
  sf::RenderTexture blur2;

  sf::RectangleShape bgShape;

  sf::Sprite tmpsprite;

  float bgScale = 1.0;

  sf::Shader chromaticA;
  sf::Shader blurShader;
  sf::Shader scanShader;
  sf::Shader crtShader;

  sf::Vector2f winSize = sf::Vector2f(1280.0, 720.0);

  sf::Texture mattetex;

  sf::Clock clock;

  float blurScaler = 5.0;

 public:
  // returns true if shaders loaded and supported
  bool loadShaders() {
    bool loaded = true;

    // load shaders from files
    loaded = chromaticA.loadFromFile("../assets/shaders/chromaticAberration.frag", sf::Shader::Fragment);
    loaded = loaded && blurShader.loadFromFile("../assets/shaders/gblur.frag", sf::Shader::Fragment);
    loaded = loaded && crtShader.loadFromFile("../assets/shaders/crt.frag", sf::Shader::Fragment);
    loaded = loaded && scanShader.loadFromFile("../assets/shaders/scanlines.frag", sf::Shader::Fragment);
    // load smudgy texture
    loaded = loaded && mattetex.loadFromFile("../assets/textures/Matte.png");

    if (loaded) {
      resize(winSize);
      // background shape
      bgShape.setFillColor(sf::Color(1, 8 / 3, 5, 255));
    }

    return chromaticA.isAvailable() && loaded;
  }

  void render(sf::RenderWindow &window, sf::RenderTexture &texture) {
    // clock.restart();
    tmpsprite.setTexture(texture.getTexture(), true);
    // tmpsprite.setPosition((1280 - texture.getSize().x * 2.0) / 2 +
    // (sin(clock.getElapsedTime().asSeconds() / 2) * 500), (720 -
    // texture.getSize().y * 2.0) / 2);
    tmpsprite.setPosition((winSize.x - texture.getSize().x * bgScale) / 2, (winSize.y - texture.getSize().y * bgScale) / 2);
    // draw original texture with chromatic aberration and scaling
    tmpsprite.setScale(bgScale, bgScale);
    og.setSmooth(false);
    og.clear();
    og.draw(bgShape);
    og.draw(tmpsprite, &chromaticA);
    og.setSmooth(true);
    og.display();

    tmpsprite.setPosition(0, 0);
    tmpsprite.setScale(1.0, 1.0);

    // draw scanlines
    scanlines.clear();
    tmpsprite.setTexture(og.getTexture(), true);
    scanShader.setUniform("tex", og.getTexture());
    scanShader.setUniform("time", clock.getElapsedTime().asSeconds());
    scanlines.draw(tmpsprite, &scanShader);

    // draw og to blur rendertexture
    tmpsprite.setTexture(scanlines.getTexture(), true);
    tmpsprite.setScale(1.0 / blurScaler, 1.0 / blurScaler);
    tmpsprite.setPosition(0, 0);
    blur1.clear();
    blurShader.setUniform("tex", scanlines.getTexture());
    blur1.draw(tmpsprite, &blurShader);

    tmpsprite.setScale(1.0, 1.0);
    tmpsprite.setTexture(blur1.getTexture(), true);

    // blur horizontally and vertically 25 iterations
    for (int i = 0; i < 15; i++) {
      blurShader.setUniform("tex", blur1.getTexture());
      blurShader.setUniform("horizontal", true);
      blur2.clear(sf::Color::Black);
      blur2.draw(tmpsprite, &blurShader);

      blurShader.setUniform("tex", blur2.getTexture());
      blurShader.setUniform("horizontal", false);
      blur1.clear(sf::Color::Black);
      blur1.draw(tmpsprite, &blurShader);
    }

    // clear window
    window.clear();
    // set sprite texture to blur1
    tmpsprite.setTexture(blur1.getTexture(), true);
    // set shader uniform variables
    crtShader.setUniform("tex", scanlines.getTexture());
    crtShader.setUniform("matteTex", mattetex);
    crtShader.setUniform("blurTex", blur1.getTexture());
    crtShader.setUniform("time", clock.getElapsedTime().asSeconds() + 10);
    // scale up blurred texture*/
    tmpsprite.setScale(blurScaler, blurScaler);

    // draw to window with crtshader
    window.draw(tmpsprite, &crtShader);
  }

  void resize(sf::Vector2f size) {
    winSize = size;
    // create original renderTexture
    og.create(size.x, size.y);
    // create scanlines renderTexture
    scanlines.create(size.x, size.y);
    // create temp blur rendertextures
    blur1.create(size.x / blurScaler, size.y / blurScaler);
    blur2.create(size.x / blurScaler, size.y / blurScaler);

    blur1.setSmooth(true);
    blur2.setSmooth(true);
    scanlines.setSmooth(true);

    sf::Vector2u ssize(224, 288);

    // ratio comparison to know whether to scale to fit vertically or
    // horizontally
    if ((size.x / size.y) > (ssize.x / ssize.y)) {
      // scale to fit vertically
      bgScale = size.y / ssize.y * 0.8;
    } else {
      bgScale = size.x / ssize.x * 0.8;
    }

    // background shape
    bgShape.setSize(sf::Vector2f(size.x - 100 * bgScale * 0.5, size.y - 100 * bgScale * 0.5));
    bgShape.setPosition(50 * bgScale * 0.5, 50 * bgScale * 0.5);
  }
};