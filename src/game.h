#ifndef GAME_H
#define GAME_H

#include <random>
#include <thread>
#include <future>
#include "controller.h"
#include "renderer.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScoreSnake1() const;
  int GetScoreSnake2() const;
  int GetSizeSnake1() const;
  int GetSizeSnake2() const;

 private:
  Snake snake_1;
  Snake snake_2;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score_snake_1{0};
  int score_snake_2{0};
  std::mutex _mtxGameSnake; // useful to lock the shared resource snake
  std::mutex _mtxGameFood;  // useful to lock the shared resource food

  void PlaceFood();
  void Update();
};

#endif