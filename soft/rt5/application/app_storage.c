#include ".\APP_Cfg.h"

typedef enum
{
    STO_STATUS_IDLE   = 0x00,
    STO_STATUS_WRITE  = 0x01,
    STO_STATUS_READ   = 0x02,
}stoStatus_t;
static stoStatus_t  stoStatus = STO_STATUS_IDLE;

void app_sto_eeprom_configure(void)
{     
    uint8_t *ptSysPara = 0;      
    ptSysPara = (uint8_t *)controler_getSysParaRecordPt();    
    //StoRunParameter_Configure(sizeof(RecordPara_t));
    
//    if(ReadFlash_StoRunParameter(ptSysPara))
//    {    
//        app_con_gotoFac();        
//        WriteFlash_StoRunParameter(ptSysPara); 
//    }   
    mod_sto_configure(sizeof(RecordPara_t));
    if(mod_sto_readStoPara(ptSysPara))
    {
        SysPara_t *pt;  
        pt = controler_getSysParaPt();
        pt->record.autoGotoFac = false;
        app_con_gotoFac();    
        mod_sto_writeStoPara(ptSysPara);
    }
}

void app_sto_eeprom_scanTask(void)
{   
    static uint16_t lastStoSystick = 0;
    static bool     writeEnableFlag = false; 
    uint8_t *ptSysPara=  0;      
    ptSysPara = (uint8_t *)controler_getSysParaRecordPt();     
    if(STO_STATUS_WRITE == stoStatus)
    {       
        writeEnableFlag = true;
        stoStatus = STO_STATUS_IDLE;
        lastStoSystick = GetSysTickMillisecond();       
    }
    else if(STO_STATUS_READ == stoStatus)
    {
        if(mod_sto_readStoPara(ptSysPara))
        {
            app_con_gotoFac(); 
            mod_sto_writeStoPara(ptSysPara);   
        }     
        stoStatus = STO_STATUS_IDLE; 
    }
    
    if(writeEnableFlag)
    {
        uint16_t diffTick = 0;
        diffTick = GetSysTickMillisecond() - lastStoSystick;
        if(diffTick > ms_1000)
        {
            writeEnableFlag = false;           
            mod_sto_writeStoPara(ptSysPara);
        }
    }   
}

void app_sto_eeprom_write(void)
{    
    stoStatus = STO_STATUS_WRITE;
}

