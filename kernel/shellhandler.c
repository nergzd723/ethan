#include <frame_buffer.h>
#include <string.h>
#include <panic.h>
#include <userapp.h>
#include <random.h>
#include <shutdown.h>
#include <io.h>
#include <logger.h>
#include <vga.h>
#include <cpu_speed.h>
#include <memory.h>
#include <rtc.h>
#include <sound.h>
#include <cpuid.h>
#include <stdbool.h>
#include <fall.h>
#include <boot.h>
#include <isr.h>
#include <liballoc.h>
#include <gfx.h>
#include <liballoc.h>
#include <timer.h>
#include <v8086m.h>
#include <real.h>
#include <sysinit.h>
#include <acpi.h>
#include <syscall.h>

bool paging = false;
time_t* current_time;
char lastcommbuf[FB_CELLS] = "";
int inputactive = 0;
char inputbuf[FB_CELLS] = "";
int inputpending = 0;
bool getcharflag = false;
char gcbuf = '\0';
char termbuf[FB_CELLS] = "";

void reset_shell()
{
    clear_screen();
    memset(&lastcommbuf[0], 0, sizeof(lastcommbuf));
    printf(">>> ");
    for(;;);
}
void reinit(){
    nosound();
    disable_paging();
    asm volatile ("cli");
    clear_screen();
    boot_stage1();
}
void inputlistener(char* comm){
    strcpy(inputbuf, comm);
}

void initinput(){
    inputactive = 1;
}

void closeinput(){
    inputactive = 0;
}

char getchar(){
    getcharflag = false;
    return gcbuf;
}

void initgetchar(){
    getcharflag = true;
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
    if (strcmp(command, "syswr") == 0){
        wrstr("Mandt");
    }
    if (strcmp(command, "beep") == 0){
        beep();
    }
    if (strcmp(command, "hb") == 0){
        happy();
    }
    if (strcmp(command, "star") == 0){
        staralligns();
    }
    if (strcmp(command, "reinit") == 0){
        reinit();
    }
    if (strcmp(command, "gfx") == 0){
        disable_paging();
        memset((char *)0xA0000, FB_LIGHT_BROWN, (320*200));
    }
    if (strcmp(command, "paging") == 0){
        if (paging){
            printf("\nPaging off");
            disable_paging();
            logf("Paging off\n");
            paging = false;
        }
        else{
            paging = true;
            printf("\nPaging on.");
            init_paging();
        }
    }
    
    if (strcmp(command, "cpuid") == 0){
        printf("\n");
        CpuDetect();
    }
    if (strcmp(command, "zerodiv") == 0){
        int i = 0;
        int b = 1;
        int d = b/i;
        printf("\nRetuned from zero division exception");
    }
    if (strcmp(command, "input")== 0){
        initinput();
        printf(input());
        closeinput();
    }
    if (strcmp(command, "overflow") == 0){
        logf("Let the hellrace begin!");
        while (1){
            malloc(sizeof(uint8_t));
        }
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
        if (paging){
        current_time = (time_t*)malloc(sizeof(time_t));
        }
        else{
            time_t* current_time;
        }
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
        shutdown_qemu_vm();
        acpiPowerOff();
    }
    if (strcmp(command, "version") == 0){
        printf("\nEthanium -            H     H\n\
                      |     |\n\
                  H---C-----C---H\n\
                      |     |\n\
                      H     H\n");
        printf("\nCodename: Fluffy Flaafy");
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
    if (strcmp(command, "syscall") == 0){
        write_eax(1);
        scall();
        printf("HUGGIES?");
    }
    fb_newline();
    printf(">>> ");
    //empty buffer????
    //lastcommbuf[0] = 0; maybe is a reason?
    memset(&lastcommbuf[0], 0, sizeof(lastcommbuf)); //really not the reason
}

void charbridge(unsigned char c){
    int len = strlen(lastcommbuf);
    int tlen = strlen(termbuf);
    if (getcharflag){
        gcbuf = c;
        return;
    }
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
            for (int i = 0; i < 5; i++)
            {
                append(termbuf, ' ');
            }
            printf("     ");
            break;
        case '\b':
            termbuf[tlen-1] = '\0';
            lastcommbuf[len-1] = '\0';
            fb_backspace();
            break;
        case '\n':
            fb_newlinehandler();
            break;
        default:
            if (strlen(termbuf) == FB_CELLS-1){
                memset(&termbuf[0], 0, sizeof(termbuf)); //really not the reason
            }
            append(termbuf, c);
            append(lastcommbuf, c);
            fb_write_byte(c);
            break;
    }
}
