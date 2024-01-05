#include "../include/menu.hpp"
#include <iostream>

int main() {
  DisplayManager display;
  Menu menu(display);
  bool running = true;
  while (running)
    running = menu.run();
  return 0;
}
