#include <frame_buffer.h>
char lastcommbuf[FB_COLS];
char terminal_buf[FB_CELLS];
void charbridge(char c){
    switch(c)
    {
        case '\b':
            fb_backspace();
            break;
        case '\n':
            fb_newline();
            break;
        default:
            str
            fb_write_byte(c);
            break;
    }
}