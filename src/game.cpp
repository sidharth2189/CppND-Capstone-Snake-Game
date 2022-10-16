#include "game.h"
#include <iostream>
#include <algorithm>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake_1(grid_width, grid_height),
      snake_2(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  // initialize position of snake 2
  while (snake_2.head_x == snake_1.head_x && snake_2.head_y == snake_1.head_y)
  {
    snake_2.head_x = random_w(engine); // random start point (x) of snake 2
    snake_2.head_y = random_h(engine); // random start point (y) of snake 2
  }
  PlaceFood();
}

void Game::Run(Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  std::vector<std::future<void>> futures;
  Controller player1(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT);
  Controller player2(SDLK_w, SDLK_s, SDLK_a, SDLK_d);

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    //controller.HandleInput(running, snake);
    futures.emplace_back(std::async(&Controller::HandleInput, player1, std::ref(running), std::ref(snake_1)));
    futures.emplace_back(std::async(&Controller::HandleInput, player2, std::ref(running), std::ref(snake_2)));
    futures.emplace_back(std::async(&Game::Update, this));
    std::for_each(futures.begin(), futures.end(), [](std::future<void> &ftr) {ftr.wait();});
    //Update();
    renderer.Render(snake_1, snake_2, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score_snake_1, score_snake_2, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake_1.SnakeCell(x, y) && !snake_2.SnakeCell(x, y)) {
      std::lock_guard<std::mutex> lockFood(_mtxGameFood);
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  std::unique_lock<std::mutex> lockSnake(_mtxGameSnake);
  if (!snake_1.alive || !snake_2.alive) return;

  snake_1.Update(snake_2);
  snake_2.Update(snake_1);

  int new_snake_1_x = static_cast<int>(snake_1.head_x);
  int new_snake_1_y = static_cast<int>(snake_1.head_y);
  int new_snake_2_x = static_cast<int>(snake_2.head_x);
  int new_snake_2_y = static_cast<int>(snake_2.head_y);

  // Check if there's food over here
  if (food.x == new_snake_1_x && food.y == new_snake_1_y) {
    score_snake_1++;
    PlaceFood();
    // Grow snake and increase speed.
    snake_1.GrowBody();
    snake_1.speed += 0.02;
  }
  else if (food.x == new_snake_2_x && food.y == new_snake_2_y) {
    score_snake_2++;
    PlaceFood();
    // Grow snake and increase speed.
    snake_2.GrowBody();
    snake_2.speed += 0.02;
  }
  lockSnake.unlock();
}

int Game::GetScoreSnake1() const { return score_snake_1; }
int Game::GetScoreSnake2() const { return score_snake_2; }
int Game::GetSizeSnake1() const { return snake_1.size; }
int Game::GetSizeSnake2() const { return snake_2.size; }