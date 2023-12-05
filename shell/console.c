#include "console.h"
#include "portmap.h"

char* const VGA_BUFFER = (char*) 0xb8000; // Pointer to the memory address of our VGA Buffer

VGA_Color terminal_font_color = LIGHT_GRAY; // Default font color will be light gray

VGA_Color terminal_background_color = BLACK; // Default background color is black

int terminal_position = 0; // keeps track of where the next print statement should go

void clear_terminal() {

     for (int i=0; i < VGA_HEIGHT*VGA_WIDTH*2; i+=2) { // for every character

                   VGA_BUFFER[i] = 0; // remove the character

                   VGA_BUFFER[i+1] = 0x07;  // and store a light gray color as the style, which is the default VGA_BEHAVIOR)

    }
    update_cursor();
}

void set_terminal_font_color(VGA_Color col) {

     terminal_font_color = col;

}



void set_terminal_background_color(VGA_Color col) {

     terminal_background_color = col;

}

void print_character(char c) {

     print_character_with_color(c, terminal_background_color, terminal_font_color);

}

void print_string(char* str) {
    while (*str) {
        print_character_with_color(*str, terminal_background_color, terminal_font_color);
        ++str;
    }

}

void print_line(char* str) {
    print_string(str);
    print_character('\n'); // Move to the next line after printing the string
			   //
}


void print_line_with_color(char* str, VGA_Color bg_color, VGA_Color font_color) {

      print_string_with_color(str, bg_color, font_color);

      print_character_with_color('\n', bg_color, font_color);

}

void print_string_with_color(char* str, VGA_Color bg_color, VGA_Color font_color) {

     for (int i=0; str[i] != '\0'; i++) {

            print_character_with_color(str[i], bg_color, font_color);

     }

}

void print_character_with_color(char c, VGA_Color bg_color, VGA_Color font_color) {
    // Check if the character is a newline
    if (c == '\n') {
        // Reset terminal_position to the next multiple of 160 (80 columns * 2 bytes per character)
        terminal_position = (terminal_position / (VGA_WIDTH * VGA_BYTES_PER_CHARACTER) + 1) * (VGA_WIDTH * VGA_BYTES_PER_CHARACTER);
	update_cursor();
        return;
    }

    // Place the character in the buffer
    VGA_BUFFER[terminal_position++] = c;

    // Compute the style byte
    VGA_BUFFER[terminal_position++] = (bg_color << 4) | font_color;

    // Check if we reached the end of a line
    if (terminal_position % (VGA_WIDTH * VGA_BYTES_PER_CHARACTER) == 0) {
        // Move to the next line
        terminal_position += (VGA_WIDTH - 1) * VGA_BYTES_PER_CHARACTER;
    }

    update_cursor();
}

void update_cursor() {

     uint16_t cursor_position = terminal_position >> 1;

     outb(0x3D4, 0x0F);

     outb(0x3D5, (uint8_t) (cursor_position));

     outb(0x3D4, 0x0E);

     outb(0x3D5, (uint8_t) (cursor_position >> 8));

}
