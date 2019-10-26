#include <frame_buffer.h>
#include <string.h>
#include <logger.h>

char lastcommbuf[FB_COLS] = "";

void fb_newlinehandler(){
    char* command = lastcommbuf;
    logf(lastcommbuf);
    if (strcmp(command, "work") == 0){
        printf("YAAAAAAAAY\n\n\n\n\n\n\n");
    }
    fb_newline();
    //empty buffer????
    lastcommbuf[0] = 0;
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
            strcat(lastcommbuf, &c);
            fb_write_byte(c);
            break;
    }
}