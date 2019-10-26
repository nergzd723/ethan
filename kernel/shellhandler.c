#include <frame_buffer.h>
#include <string.h>

char lastcommbuf[FB_COLS];
char terminal_buf[FB_CELLS];

void fb_newlinehandler(){
    char* command = lastcommbuf;
    if (strcmp(command, "work")){
        printf("YAAAAAAAAY\n\n\n\n\n\n\n");
    }
}

void charbridge(char c){
    switch(c)
    {
        case '\b':
            fb_backspace();
            break;
        case '\n':
            fb_newlinehandler();
            break;
        default:
            strcat(lastcommbuf, c);
            strcat(terminal_buf, c);
            fb_write_byte(c);
            break;
    }
}