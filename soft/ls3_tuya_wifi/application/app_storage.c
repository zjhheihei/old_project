#include ".\APP_Cfg.h"

typedef enum
{
    STO_STATUS_IDLE   = 0x00,
    STO_STATUS_WRITE  = 0x01,
    STO_STATUS_READ   = 0x02,
}stoStatus_t;
//------------------------------E N D-------------------------------------------

static stoStatus_t  stoStatus = STO_STATUS_IDLE;


void app_sto_eeprom_configure(void)
{     
     uint8_t *ptSysPara = 0;      
     ptSysPara = (uint8_t *)controler_getSysParaRecordPt();
     
     StoRunParameter_Configure(sizeof(RecordPara_t));
     if(ReadFlash_StoRunParameter(ptSysPara))
     {    
         app_con_gotoFac(true);        
         WriteFlash_StoRunParameter(ptSysPara); 
     }   
}
//------------------------------E N D-------------------------------------------


void app_sto_eeprom_scanTask(void)
{   
    static uint16_t lastStoSystick = 0;
    static bool     writeEnableFlag = false; 
    uint8_t *ptSysPara=  0;      
    ptSysPara = (uint8_t *)controler_getSysParaRecordPt( );  
    
    
    if(STO_STATUS_WRITE == stoStatus)
    {       
        writeEnableFlag = true;
        stoStatus = STO_STATUS_IDLE;
        lastStoSystick = GetSysTickMillisecond();       
    }
    else if(STO_STATUS_READ == stoStatus)
    {
        if(ReadFlash_StoRunParameter(ptSysPara))
        {
            app_con_gotoFac(true); 
            WriteFlash_StoRunParameter(ptSysPara);   
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
            WriteFlash_StoRunParameter(ptSysPara); 
        }
    }   
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************传出存储参数***************************************
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************压入存储参数****************************************
void app_sto_eeprom_write(void)
{    
    stoStatus = STO_STATUS_WRITE;
}
//------------------------------E N D-------------------------------------------


//--------------------------APP_Storage.c--END----------------------------------