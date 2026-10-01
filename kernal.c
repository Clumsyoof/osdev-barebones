#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "use a cross-compiler"
#endif

#if !defined(__i386__)
#error "use i386 compiler"
#endif

#define VGA_WIDTH 80
#define VGA_HEIGTH 25
#define VGA_MEM 0xB8000

enum vga_colour {
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_colour(enum vga_colour fg,enum vga_colour bg){
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char c,uint8_t colour){
    return (uint16_t) c | (uint16_t) colour << 8;
}

size_t strlen(const char* str){
    size_t length = 0;
    while(str[length]){
        length++;
    }
    return length;
}

size_t terminal_row;
size_t terminal_col;
uint8_t terminal_colour;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEM;

void terminal_init(void){
    terminal_row = 0;
    terminal_col = 0;
    terminal_colour = vga_entry_colour(VGA_COLOR_LIGHT_RED,VGA_COLOR_BLACK);

    for(size_t y = 0; y < VGA_HEIGTH; y++){
        for(size_t x = 0; x<VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_colour);
        }
    }
}

void terminal_set_colour(uint8_t colour){
    terminal_colour = colour;
}

void terminal_putentryat(char c,uint8_t colour,size_t x,size_t y){
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, colour);
}

void terminal_putchar(char c){
    terminal_putentryat(c,terminal_colour,terminal_col,terminal_row);
    if(++terminal_col == VGA_WIDTH){
        terminal_col = 0;
        if(++terminal_row == VGA_HEIGTH){
            terminal_row = 0;
        }
    }
}

void terminal_write(const char* data,size_t size){
    for(size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_write_string(const char* data){
    terminal_write(data, strlen(data));
}

void kernal_main(void) {
    terminal_init();
    terminal_write_string("quack quack mf");
}
