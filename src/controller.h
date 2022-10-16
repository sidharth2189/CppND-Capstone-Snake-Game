#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <mutex>
#include "snake.h"

class Controller {
 public:
  Controller (SDL_Keycode up, SDL_Keycode down, SDL_Keycode left, SDL_Keycode right):
  _up(up),
  _down(down),
  _left(left),
  _right(right) {}
  void HandleInput(bool &running, Snake &snake) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
  SDL_Keycode _up;
  SDL_Keycode _down;
  SDL_Keycode _left;
  SDL_Keycode _right;
  static std::mutex _mtxController; // useful to lock the shared resource "runninh"
};

#endif