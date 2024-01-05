#include "../include/displayManager.hpp"
#include "../include/constants.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

DisplayManager::DisplayManager() {}

void DisplayManager::clearScreen() { std::cout << "\033[2J\033[1;1H"; }

void DisplayManager::displayFrame(string frame, string mode) {
  if (mode == "maze") {
    for (char ch : frame) {
      switch (ch) {
      case BORDER: {
        std::cout << BORDER_COLOR(string(1, ch));
        break;
      }
      case VISITED: {
        std::cout << VISITED_COLOR(string(1, ch));
        break;
      }
      case PATH: {
        std::cout << PATH_COLOR(string(1, ch));
        break;
      }
      case FINISH:
      case START: {
        std::cout << START_FINISH_COLOR(string(1, ch));
        break;
      }
      default: {
        std::cout << ch;
        break;
      }
      }
    }
  } else if (mode == "red") {
    std::cout << red(frame);
  } else if (mode == "blue") {
    std::cout << blue(frame);
  } else if (mode == "green") {
    std::cout << green(frame);
  } else if (mode == "yellow") {
    std::cout << yellow(frame);
  } else {
    std::cout << frame;
  }
}

void DisplayManager::sleep(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

string DisplayManager::red(string text) {
  return "\033[1;31m" + text + "\033[0m";
}
string DisplayManager::blue(string text) {
  return "\033[1;34m" + text + "\033[0m";
}
string DisplayManager::green(string text) {
  return "\033[1;32m" + text + "\033[0m";
}
string DisplayManager::yellow(string text) {
  return "\033[1;33m" + text + "\033[0m";
}
