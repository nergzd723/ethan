#include <frame_buffer.h>
#include <string.h>
#include <panic.h>
#include <userapp.h>
#include <random.h>
#include <shutdown.h>
#include <logger.h>
#include <cpu_speed.h>
#include <memory.h>
#include <rtc.h>
#include <cpuid.h>
#include <stdbool.h>

bool paging = false;

time_t* current_time;
char lastcommbuf[FB_CELLS] = "";
int inputactive = 0;
char inputbuf[FB_CELLS] = "";
int inputpending = 0;

void inputlistener(char* comm){
    strcpy(inputbuf, comm);
}

void initinput(){
    inputactive = 1;
}

void closeinput(){
    inputactive = 0;
}

char* input(){
    if (inputactive == 1){
        printf("\n");
        return inputbuf;
    }
    return "";
}

void fb_newlinehandler(){
    char* command = lastcommbuf;
    if (inputactive == 1){
        if (inputpending == 0){
            inputpending = 1;
            return;
        }
        inputlistener(command);
        memset(&lastcommbuf[0], 0, sizeof(lastcommbuf));
        inputpending = 0;
        return;
    }
    if (strcmp(command, "add") == 0){
        add();
    }
    if (strcmp(command, "paging") == 0){
        if (paging){
            printf("\nPaging off");
            disable_paging();
        }
        else{
            paging = true;
            printf("\nPaging on. Warning: things go wrong!");
            init_paging();
        }
    }
    
    if (strcmp(command, "cpuid") == 0){
        printf("\n");
        CpuDetect();
    }
    if (strcmp(command, "input")== 0){
        initinput();
        printf(input());
        closeinput();
    }
    if (strcmp(command, "randomb") == 0){
        int a = rand();
        unsigned char c = a % 256;
        printf("\nYour random number is\n");
        printf(inttostr(c));
    }
    if (strcmp(command, "help") == 0){
        printf("\nEthanium operational. Part of MEOW project.\nAvailable commands: help, panic, jibber, randomb, reboot, shutdown");
    }
    if (strcmp(command, "panic") == 0){
        printf("\npanicking");
        panic("USER_DEMAND_PANIC");
    }
    if (strcmp(command, "time") == 0){
        gettime(current_time);
        printf("\nIt is ");
        printf(inttostr(current_time->day_of_month));
        printf("-");
        printf(inttostr(current_time->month));
        printf("-");
        printf(inttostr(current_time->year));
        printf(", ");
        printf(inttostr(current_time->hour));
        printf(":");
        printf(inttostr(current_time->minute));
        printf(":");
        printf(inttostr(current_time->second));
        printf("UTC");
    } 
    if (strcmp(command, "jibber") == 0){
        dojibberish();
    }
    if (strcmp(command, "shutdown") == 0){
        shutdown();
    }
    if (strcmp(command, "version") == 0){
        printf("\nEthanium -            H     H\n\
                      |     |\n\
                  H---C-----C---H\n\
                      |     |\n\
                      H     H\n");
    }
    if (strcmp(command, "reboot") == 0){
        logf("\nRebooting\n");
        reboot();
    }
    if (strcmp(command, "meminfo") == 0){
        printf("\n");
        printf(inttostr(lower_memory()+upper_memory()));
        printf("K mem total, ");
        printf(inttostr(lower_memory()));
        printf("K lowmem, ");
        printf(inttostr(upper_memory()));
        printf("K USABLE extended memory");
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
            panic("PAUSE/BREAK");
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
        case '\t':
            printf("     ");
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
