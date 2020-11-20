#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  NCursesDisplay::Display(system, 20);

  // TESTS:
  //system.TestSortFunction(); //TODO: Remove
}