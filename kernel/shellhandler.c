#include <frame_buffer.h>
#include <string.h>
#include <logger.h>

char lastcommbuf[FB_COLS] = "";
char terminal_buf[FB_CELLS] = "";

void fb_newlinehandler(){
    char* command = lastcommbuf;
    logf(lastcommbuf);
    logf(terminal_buf);
    logf(inttostr(strcmp(command, "work")));
    if (strcmp(command, "work") == 0){
        printf("YAAAAAAAAY\n\n\n\n\n\n\n");
    }
    fb_newline();
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
            strcatbyte(lastcommbuf, c);
            strcatbyte(terminal_buf, c);
            fb_write_byte(c);
            break;
    }
}