#include <frame_buffer.h>
#include <string.h>
#include <logger.h>
#include <panic.h>

char lastcommbuf[FB_CELLS] = "";

void fb_newlinehandler(){
    char* command = lastcommbuf;
    logf(lastcommbuf);
    if (strcmp(command, "help") == 0){
        printf("\nEthan operational. Part of MEOW project.");
    }
    if (strcmp(command, "panic") == 0){
        printf("\npanicking");
        panic("USER_DEMAND_PANIC", 0);
    }
    fb_newline();
    printf(">>> ");
    //empty buffer????
    //lastcommbuf[0] = 0; maybe is a reason?
    memset(&lastcommbuf[0], 0, sizeof(lastcommbuf)); //really not the reason
}

void charbridge(char c){
    int len = strlen(lastcommbuf);
    switch(c)
    {
        case 208:
            uparrowp();
            break;
        case 209:
            downarrowp();
            break;
        case 174:
            leftarrowp();
            break;
        case 175:
            rightarrowp();
            break;
        case '\b':
            lastcommbuf[len-1] = '\0';
            fb_backspace();
            break;
        case '\n':
            fb_newlinehandler();
            break;
        default:
            append(lastcommbuf, c);
            fb_write_byte(c);
            break;
    }
}
