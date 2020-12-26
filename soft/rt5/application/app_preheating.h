#ifndef _APP_PREHEATING_H
#define _APP_PREHEATING_H



typedef struct
{
    bool      switchflag;  
    uint16_t  addTime[6];
}
preheatPara_t;

bool app_preheat_getStatus(void);
void app_preheat_task(void);
#endif


