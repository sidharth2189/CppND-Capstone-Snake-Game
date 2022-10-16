#include "controller.h"
#include <iostream>

std::mutex Controller::_mtxController;

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  std::unique_lock<std::mutex> ulock(_mtxController); // snake is a shared resource with Game and Render
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  ulock.unlock();
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      std::lock_guard<std::mutex> lck(_mtxController);
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      if (e.key.keysym.sym == _up) {
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
      }

      else if (e.key.keysym.sym == _down) {
        ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
      }

      else if (e.key.keysym.sym == _left) {
        ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
      }

      else {
        ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
      }
    }
  }
}