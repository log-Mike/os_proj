#include "console.h" 
#include "io.h"
#include "keyboard.h"


void main() {
clear_terminal();

uint8_t byte;

while(1) {

     while ( byte = scan()) {

          print_character(charmap[byte]);

     }

}

}
