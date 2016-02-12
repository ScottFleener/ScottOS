#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_init(){
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  terminal_buffer = VGA_MEMORY;

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = make_vgaentry(' ', terminal_color);
    }
  }
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_scroll(){
  for (size_t y = 1; y < VGA_HEIGHT; y++){
    for (size_t x = 0; x < VGA_WIDTH; x++){
      const size_t index = y * VGA_WIDTH + x;
      const size_t index_above = (y - 1) * VGA_WIDTH + x;
      terminal_buffer[index_above] = terminal_buffer[index];
      terminal_putentryat(' ', terminal_color, x, y);
    }
  }
}

void terminal_putchar(char c) {
  if (c == '\n'){
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT){
      terminal_row--;
      terminal_scroll();
    }
  } else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      if (++terminal_row == VGA_HEIGHT) {
        terminal_row--;
        terminal_scroll();
      }
    }
  }
}

void terminal_write(char* data, size_t size){
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(char* data){
  terminal_write(data, strlen(data));
}


void terminal_setcolor(uint8_t color) {
  terminal_color = color;
}
