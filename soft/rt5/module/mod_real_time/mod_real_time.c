//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\mod_real_time.h"
#include ".\depend\bsp_real_time.h"
//------------------------------E N D-------------------------------------------

typedef enum
{
    RTC_STATUS_INIT = ((uint8_t)0x00), 
    RTC_STATUS_RUN,
}realRtcStatus_t;


static uint8_t   *ptRealRtc;




void hexToBcd_8bits(uint8_t *buf,uint8_t *_out_buf)
{
    uint8_t i = 0;
    uint8_t ten = 0;
    uint8_t units = 0;
    for(;i < 7;i++)
    {
        ten = buf[i] / 10;
        units = buf[i] % 10;
        _out_buf[i] = ((ten << 4) | units);
    }
}

void bcdToHex_8bits(uint8_t *buf,uint8_t *_out_buf)
{
    uint8_t i = 0;
    uint8_t ten = 0;
    uint8_t units = 0;
    for(;i < 7;i++)
    {
        ten = buf[i] / 16;
        units = buf[i] % 16;
        _out_buf[i] = ((ten * 10) + units);
    }
}

bool mod_real_time_read(void)
{
    if(bsp_real_time_read_enable())
    {
        uint8_t i = 0;
        volatile uint8_t *ptRtcReg;
        uint8_t buf[7];
        ptRtcReg = &RTC_ADDRESS;  
        for(i = 0;i < 7;i++)
        {
            buf[i] = *ptRtcReg;        
            ptRtcReg++;       
        }
        buf[3] = buf[5] >> 5;
        uint8_t *ptBuf = buf;     
        bcdToHex_8bits(ptBuf,ptRealRtc);
        return true;
    }    
    return false;
}

bool mod_real_time_write(void)
{
    uint8_t buf[7];
    hexToBcd_8bits(ptRealRtc,buf);
    if(bsp_real_time_configure(buf))
    {
        return true;
    }
    return false;
}

void mod_real_time_configure(uint8_t *ptRtc)
{
    ptRealRtc = ptRtc;
}









