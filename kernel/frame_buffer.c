// Documentation:
// http://wiki.osdev.org/Printing_To_Screen

#include "frame_buffer.h"
#include "io.h"
#include <logger.h>

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

// Start of memory that maps to the frame buffer
char *fb = (char *) 0x000B8000;

uint16_t cursor_pos = 0;

int printf (const char * format, ...) {
  int i = 0;
  while (format[i]) {
    fb_write_byte(format[i]);
    i++;
  }
  return i;
}

//get current VGA row
int row(){
  int rr = cursor_pos / 80;
  return rr;
}

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int cell, char c, unsigned char fg, unsigned char bg)
{
    int i = cell*2;
    fb[i] = c;
    fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

void fill_screen(unsigned char color){
    for (int i = 0; i < FB_CELLS; i++) {
    fb_write_cell(i, ' ', color, color);
  }
}

char* inttostr( int zahl ) 
{ 
   static char text[20];
   int loc=19;
   text[19] = 0;
   while (zahl)
   {
       --loc;
       text[loc] = (zahl%10)+'0';
       zahl/=10;
   }
   if (loc==19)
   {
      --loc;
       text[loc] = '0';
   }
   return &text[loc];
}

void clear_screen()
{
  fill_screen(FB_BLACK);
}

void fb_newline(){
  int c = cursor_pos;
  c = (c/80) + 1;
  cursor_pos = c*10;
  move_cursor_to_pos(c*10);
}
/** move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void move_cursor_to_pos(unsigned short pos)
{
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT,    pos & 0x00FF);
}

void fb_write_byte(uint8_t b) {
  fb_write_cell(cursor_pos, b, FB_WHITE, FB_BLACK);
  cursor_pos++;
  // Stop the cursor from going off the screen
  // TODO: advance the screen
  if (cursor_pos < FB_CELLS) {
    move_cursor_to_pos(cursor_pos);
  }
  else{
    clear_screen();
    cursor_pos = 0;
  }
}

void fb_backspace() {
  if (cursor_pos-- < FB_COLS){
    cursor_pos++;
    logf(inttostr(cursor_pos));
  }
  cursor_pos--;
  fb_write_cell(cursor_pos, ' ', FB_WHITE, FB_BLACK);
  move_cursor_to_pos(cursor_pos);
}