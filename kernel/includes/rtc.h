#ifndef _RTC_H_
#define _RTC_H_

typedef struct {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day_of_week;
    unsigned char day_of_month;
    unsigned char month;
    unsigned char year;
} time_t;


extern void gettime(time_t* time);
extern void rtc_install(void);

#endif