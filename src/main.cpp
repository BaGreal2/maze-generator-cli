#include "../include/menu.hpp"

int main() {
  DisplayManager display;
  FileManager filesystem(display);
  Menu menu(display, filesystem);

  bool running = true;
  while (running)
    running = menu.run();

  return 0;
}
