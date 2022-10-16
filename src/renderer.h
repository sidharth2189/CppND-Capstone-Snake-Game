#ifndef RENDERER_H
#define RENDERER_H

#include <mutex>
#include "snake.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const &snake_1, Snake const &snake_2, SDL_Point const &food);
  void UpdateWindowTitle(int score_player1, int score_player2, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  std::mutex _mtxRenderSnake; // useful to lock the shared resource snake
  std::mutex _mtxRenderFood;  // useful to lock the shared resource food
};

#endif