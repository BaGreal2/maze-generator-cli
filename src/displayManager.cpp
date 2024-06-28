#include "../include/displayManager.hpp"
#include "../include/constants.hpp"
#include <chrono>
#include <fmt/core.h>
#include <iostream>
#include <thread>

DisplayManager::DisplayManager() {}

void DisplayManager::clearScreen() { std::cout << "\033[2J\033[1;1H"; }

void DisplayManager::displayFrame(string frame, string mode) {
  string buffer;
  if (mode == "maze") {
    for (char ch : frame) {
      switch (ch) {
      case BORDER: {
        buffer += BORDER_COLOR(string(1, ch));
        break;
      }
      case VISITED: {
        buffer += VISITED_COLOR(string(1, ch));
        break;
      }
      case PATH: {
        buffer += PATH_COLOR(string(1, ch));
        break;
      }
      case FINISH:
      case START: {
        buffer += START_FINISH_COLOR(string(1, ch));
        break;
      }
      default: {
        buffer += ch;
        break;
      }
      }
    }

  } else if (mode == "red") {
    buffer = red(frame);
  } else if (mode == "blue") {
    buffer = blue(frame);
  } else if (mode == "green") {
    buffer = green(frame);
  } else if (mode == "yellow") {
    buffer = yellow(frame);
  } else {
    buffer = frame;
  }
  fmt::print("{}", buffer);
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
