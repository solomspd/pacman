#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
// various animation easing functions

float easeOutQuint(float time, float duration, float start, float end) {
  if (time < duration)
    time /= duration;
  else
    time = 1.0;
  time--;
  return (end - start) * (std::pow(time, 5) + 1) + start;
}

float easeInQuint(float time, float duration, float start, float end) {
  if (time < duration)
    time /= duration;
  else
    time = 1.0;
  return (end - start) * std::pow(time, 5) + start;
}

sf::Color fadeInColor(float time, float duration, sf::Color color) {
  if (time < duration)
    color.a = (time / duration) * 255;
  else
    color.a = 255;
  return color;
}