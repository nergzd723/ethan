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
  cursor_pos = 0;
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

int printf (const char * format, ...) {
  int i = 0;
  while (format[i]) {
    fb_write_byte(format[i]);
    i++;
  }
  return i;
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

// down arrow pointer
void downarrowp()
{
//  int i = cursor_pos += 79;
//  if (i > FB_CELLS){
//    return;
//  }
  cursor_pos += 79;
  move_cursor_to_pos(cursor_pos);
}
// up arrow pointer
void uparrowp()
{
  int i = cursor_pos - 79;
  if (i < 0){
    return;
  } 
  cursor_pos -= 79;
  move_cursor_to_pos(cursor_pos);
}

// left arrow pointer
void leftarrowp(){
  if (cursor_pos == 0){
    return;
  }
  cursor_pos -= 1;
  move_cursor_to_pos(cursor_pos);
}

// right arrow pointer
void rightarrowp(){
  if (cursor_pos == FB_CELLS){
    cursor_pos = 0;
    return;
}
  cursor_pos += 1;
  move_cursor_to_pos(cursor_pos);
}
//\n like func
void fb_newline()
{
  cursor_pos += 80;
  int a = cursor_pos % 80;
  cursor_pos -= a;
  if (cursor_pos > FB_CELLS){
    cursor_pos = 0;
  }
  move_cursor_to_pos(cursor_pos);
}

//reset cursor pos, need for other tasks
void reset_cursor(){
  cursor_pos = 0;
}

void fb_write_byte(uint8_t b) {
  if (b == '\n'){
    fb_newline();
    return;
  }
  fb_write_cell(cursor_pos, b, FB_WHITE, FB_BLACK);
  cursor_pos++;
  // Stop the cursor from going off the screen
  // TODO: advance the screen
  if (cursor_pos < FB_CELLS) {
    move_cursor_to_pos(cursor_pos++);
  }
  else{
    clear_screen();
    cursor_pos = 0;
    move_cursor_to_pos(cursor_pos);
  }
}

void fb_backspace() {
  if (cursor_pos == 0){
    return;
  }
  cursor_pos--;
  fb_write_cell(cursor_pos, ' ', FB_WHITE, FB_BLACK);
  move_cursor_to_pos(cursor_pos);
}
