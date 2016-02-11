#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "Must be compiled with an ix86-elf compiler"
#endif


typedef enum vga_color {
  COLOR_BLACK = 0,
  COLOR_BLUE = 1,
  COLOR_GREEN = 2,
  COLOR_CYAN = 3,
  COLOR_RED = 4,
  COLOR_MAGENTA = 5,
  COLOR_BROWN = 6,
  COLOR_LIGHT_GREY = 7,
  COLOR_DARK_GREY = 8,
  COLOR_LIGHT_BLUE = 9,
  COLOR_LIGHT_GREEN = 10,
  COLOR_LIGHT_CYAN = 11,
  COLOR_LIGHT_RED = 12,
  COLOR_LIGHT_MAGENTA = 13,
  COLOR_LIGHT_BROWN = 14,
  COLOR_WHITE = 15,
} vga_color;

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

uint8_t make_color(vga_color fg, vga_color bg);
uint16_t make_vgaentry(char c, uint8_t color);

size_t strlen(char* str);

void terminal_init();
void terminal_writestring(char* data);
void terminal_putchar(char c);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_setcolor(uint8_t color);
void terminal_scroll();

void kernel_main() {
  terminal_init();

  for (int i = 0; i < 26; i++){
    terminal_writestring("Hello kernel world! ");
    terminal_putchar('0' + (i / 10));
    terminal_putchar('0' + (i % 10));
    terminal_putchar('\n');
  }
}

void terminal_init(){
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = make_vgaentry(' ', terminal_color);
    }
  }
}

void terminal_writestring(char* data){
  size_t datalen = strlen(data);
  for (size_t i = 0; i < datalen; i++)
    terminal_putchar(data[i]);
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

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_setcolor(uint8_t color) {
  terminal_color = color;
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

size_t strlen(char* str) {
  size_t ret = 0;
  while (str[ret] != 0)
    ret++;
  return ret;
}

uint16_t make_vgaentry(char c, uint8_t color){
  uint16_t c16 = c;
  uint16_t color16 = color;
  return c16 | color16 << 8;
}

uint8_t make_color (vga_color fg, vga_color bg) {
  return fg | bg << 4;
}