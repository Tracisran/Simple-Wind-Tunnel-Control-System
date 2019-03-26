#include "include.h"
#include "VL53L0X.h"
uint16_t acnt ;
uint16_t scnt ;
uint16_t dist ;                                         //距离
uint16_t DeviceRangeStatusInternal;

  
uint16_t makeuint16(int lsb, int msb)                   //高8位和低8位合成16位
{
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}

void IIC_Read_Ceju(unsigned char slave,unsigned char * p)//从寄存器中读取距离
{
  


  p[0]  =    I2C_ReadAddr(I2C1,address,slave);
//  Pause();
  p[7]  =    I2C_ReadAddr(I2C1,address,slave+0x07);
//  Pause();
  p[6]  =    I2C_ReadAddr(I2C1,address,slave+0x06);
//  Pause();
  p[9]  =    I2C_ReadAddr(I2C1,address,slave+0x09);
//  Pause();
  p[8]  =    I2C_ReadAddr(I2C1,address,slave+0x08);
//  Pause();
  p[11]  =   I2C_ReadAddr(I2C1,address,slave+0x0B);
//  Pause();
  p[10]  =   I2C_ReadAddr(I2C1,address,slave+0x0A);
//  Pause();

  acnt = makeuint16(p[7], p[6]);
  scnt = makeuint16(p[9], p[8]);
  dist = makeuint16(p[11], p[10]);
  DeviceRangeStatusInternal = ((p[0] & 0x78) >> 3);
}

void VL53L0X_SHOW(void)                                 //显示 revision id ,device id, pre_range_config_vcsel_period,final_range_config_vcsel_period
{
      char  val1_txt[16];
      u8 val1 = I2C_ReadAddr(I2C1,address,VL53L0X_REG_IDENTIFICATION_REVISION_ID); //选择I2C1 PTE1、0  显示revision id
      sprintf(val1_txt,"%u",val1); 
      char Revision_ID[16]= "Revision ID: ";
      strcat(Revision_ID,val1_txt);
      LCD_P6x8Str(0,0,(uint8*)Revision_ID);
      
      val1 = I2C_ReadAddr(I2C1,address,VL53L0X_REG_IDENTIFICATION_MODEL_ID);       //显示device id
      sprintf(val1_txt,"%u",val1);    
      char Device_ID[16]= "Device ID: ";
      strcat(Device_ID,val1_txt);
      LCD_P6x8Str(0,1,(uint8*)Device_ID);
      
      val1 = I2C_ReadAddr(I2C1,address,VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD); //显示PRE_RANGE_CONFIG_VCSEL_PERIOD
      sprintf(val1_txt,"%u",val1);    
      char PRE_RANGE_CONFIG_VCSEL_PERIOD[16]= "PRE= ";
      strcat(PRE_RANGE_CONFIG_VCSEL_PERIOD,val1_txt);
      LCD_P6x8Str(0,2,(uint8*)PRE_RANGE_CONFIG_VCSEL_PERIOD);
      
      val1 = I2C_ReadAddr(I2C1,address,VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD);//显示FINAL_RANGE_CONFIG_VCSEL_PERIOD
      sprintf(val1_txt,"%u",val1);
      char FINAL_RANGE_CONFIG_VCSEL_PERIOD[16]= "FINAL= ";
      strcat(FINAL_RANGE_CONFIG_VCSEL_PERIOD,val1_txt);
      LCD_P6x8Str(0,3,(uint8*)FINAL_RANGE_CONFIG_VCSEL_PERIOD);
               

}

u16 read_dist(void)//读取dist
{

      u8 bit8_data[12];
//  char  acnt_txt[16];
//  char  scnt_txt[16];
//  char  DeviceRangeStatusInternal_txt[16];
      I2C_WriteAddr(I2C1,address,VL53L0X_REG_SYSRANGE_START,0x01);                  //开始测距 给VL53L0X_REG_SYSRANGE_START发送0x01
      u8 val = 0;                                                                   //判忙
      int cnt = 0;
      while (cnt < 100)                                                             
      {                                                                             // 1 second waiting time max
        time_delay_ms(10);
        val = I2C_ReadAddr(I2C1,address,VL53L0X_REG_RESULT_RANGE_STATUS);
        if (val & 0x01) break;
        cnt++;
      }
      if (val & 0x01) 
      {
         IIC_Read_Ceju(VL53L0X_REG_RESULT_RANGE_STATUS,bit8_data);                                             //不忙则显示ready

         return(dist);
       }
        else 
      {
        return(0)   ;                                            //忙 unready
       }


}
