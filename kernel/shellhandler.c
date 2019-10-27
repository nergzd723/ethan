#include <frame_buffer.h>
#include <string.h>
#include <panic.h>
#include <userapp.h>
#include <random.h>
#include <shutdown.h>
#include <logger.h>

char lastcommbuf[FB_CELLS] = "";

void fb_newlinehandler(){
    char* command = lastcommbuf;
    if (strcmp(command, "randomb") == 0){
        int a = rand();
        unsigned char c = a % 256;
        printf("\nYour random number is\n");
        printf(inttostr(c));
    }
    if (strcmp(command, "help") == 0){
        printf("\nEthan operational. Part of MEOW project.\nAvailable commands: help, panic, jibber, randomb");
    }
    if (strcmp(command, "panic") == 0){
        printf("\npanicking");
        panic("USER_DEMAND_PANIC", 0);
    }
    if (strcmp(command, "jibber") == 0){
        dojibberish();
    }
    if (strcmp(command, "shutdown_qemu") == 0){
        shutdown_qemu_vm();
    }
    if (strcmp(command, "reboot") == 0){
        logf("\nRebooting\n");
        reboot();
    }
    fb_newline();
    printf(">>> ");
    //empty buffer????
    //lastcommbuf[0] = 0; maybe is a reason?
    memset(&lastcommbuf[0], 0, sizeof(lastcommbuf)); //really not the reason
}

void charbridge(unsigned char c){
    int len = strlen(lastcommbuf);
    switch(c)
    {
        case 205:
            panic("PAUSE/BREAK", 1);
            break;
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
