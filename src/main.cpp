#include "ncurses_display.h"
#include "system.h"

// JUST TO DEBUG --- DELETE BEFORE SENDING PROJECT
#include<string>
#include<linux_parser.h>
#include<iostream>

int main() {
  System system;
  NCursesDisplay::Display(system);
}