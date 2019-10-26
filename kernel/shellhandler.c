#include <frame_buffer.h>
#include <string.h>
#include <logger.h>

char lastcommbuf[FB_CELLS] = "";

void fb_newlinehandler(){
    char* command = lastcommbuf;
    logf(lastcommbuf);
    if (strcmp(command, "help") == 0){
        printf("Ethan operational. Part of MEOW project.");
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
