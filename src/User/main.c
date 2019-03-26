#include "include.h"

int duty=       22400;                                   //定义风机高电平宽度
int time=0;                                             //进入范围的时间
int flag[2];                                            //进入范围开始计时标志位
int mode=6;                                               //模式
int scale=35;
int flag_p=0;
int f=1;
int scale_limit=50;
int time_i=0;
char time_i_txt_P[16];
char time_i_txt_F[16];
char time_txt_P[16];
char time_txt_F[16];

short Height_Present;                                   //定义当前实测高度 ，短整型
short Height_adj;                                       //定义滤波后的高度，短整型
short Height_set=150;                                   //定义设置高度，整型

short height[5];                                        //定义连续5次滤波后的高度，短整型

float E[3];                                             //定义连续三次pid误差
float Bias[3];
float Integral_Bias=0;                                  //误差累加和

int count=0;                                            //计数
int num_input;                                          //矩阵键盘输入的数字
char num_input_txt[8];                                  //矩阵键盘输入的数字
char num_txt[8];                                        //矩阵键盘输入的数字组成的字符串
int num;


float Kp=0.016;
float Ti=3000;                                          //二阶PID算法嵌套   上午11点     0.016 4000 0.5   1点 0.014 5000 0.5    5点 0.015  4200
float Td=0.5;
void Squ_Init(void)                                     //矩阵键盘初始化
{
   EXTI_Init(PTD,0,falling_up);                         //开启PTD0中断允许位
   EXTI_Init(PTD,1,falling_up);                         //开启PTD1中断允许位
   EXTI_Init(PTD,2,falling_up);                         //开启PTD2中断允许位
   EXTI_Init(PTD,7,falling_up);
   GPIO_Init(PORTD,3,GPO,0);
   GPIO_Init(PORTD,4,GPO,0);
   GPIO_Init(PORTD,5,GPO,0);
   GPIO_Init(PORTD,6,GPO,0);
   
   EXTI_Init(PTB,20,falling_up);                        //开启PTB20中断允许位
   EXTI_Init(PTB,21,falling_up);                        //开启PTB21中断允许位
   EXTI_Init(PTB,22,falling_up);                        //开启PTB22中断允许位

}

void Key_Int_Init(void)
{
   EXTI_Init(PTB,20,falling_up);                        //开启PTD0中断允许位
   EXTI_Init(PTB,21,falling_up);                        //开启PTD1中断允许位
   EXTI_Init(PTB,22,falling_up);                        //开启PTD2中断允许位

}


short filter(short x)                                  //软件滤波，复合（限幅消抖+加权平权），返回滤波后的当前高度  +  判断是否进入界限
{    

//     short value;
     height[0] = height[1];
     height[1] = height[2];
     height[2] = height[3];
     height[3] = height[4];
     height[4] = x;
     if(height[4] == 20)height[4] = height[3];          //当检测不到时，不记录
     if(((height[4]-height[3]) < 3)&((height[4]-height[3]) > (-3)))height[4] = height[3];      //限幅
     if(((height[4]-height[3]) >scale_limit)||((height[4]-height[3]) < (-1*scale_limit)))height[4] = height[3];  //消抖   
                                                                                
     height[4]=(short)(height[4]*0.7 + height[3]*0.15 + height[2]*0.1 + height[1]*0.04 + height[0]*0.01);    //加权平均
                                                                                
//     if( ( ( height[4]> (Height_set-100) )&( height[3] < (Height_set-100) ) )||( ( height[4] < (Height_set+100) ) & (height[3]> (Height_set+100) ) ) )
//     {
//       flag=1;
//     }
//     else if( ( ( height[4]< (Height_set-100) )&( height[3] > (Height_set-100) ) )||( ( height[4] > (Height_set+100) ) & (height[3]< (Height_set+100) ) ) )
//     {
//       flag=0;
//     }     
//     else 
//     {
//       flag=2;
//     }
     flag[0]=flag[1];
     if((height[4]> (Height_set-scale))&(height[4]< (Height_set+scale)))//判断是否进出界限，在内为1，在外为0
     {
        flag[1]=1;
     }
     else
     {
        flag[1]=0;
     }
         
     return height[4];
     
}

float PID_output(float Kp,float Ti,float Td,float T,short feedback,int target)
                                                                                //返回增量式PID输出增量。6个参数，比例、积分、微分、周期、反馈、输入预期
{
  
//     float Kp=0;//8    比例系数                         160
//     float Ti=0;//5       积分系数                99999999999
//     float Td=0;//2       微分系数
//     float T=10;
 Kp=Kp*((150.0)/(target));
 Ti=Ti*((150.0-50)/(target-50));
     float increment;                                            //定义增量
     E[2] = target*target-feedback*feedback;  
         if((E[2]<(target/10.0))&(E[2]>0))      //比例系数模糊控制
    {
      Kp=Kp*0.1;Ti=0.8*Ti;
    }
    else if((E[2]<(target/5.0))&(E[2]>0))
    {
      Kp=Kp*0.1;Ti=1*Ti;
    }
    else if((E[2]<(target/3.3))&(E[2]>0))
    {              
      Kp=Kp*0.1;Ti=1*Ti;
    }
    else if((E[2]<(target/2.5))&(E[2]>0))
    {
      Kp=Kp*0.2;Ti=1*Ti;
    }
    else if((E[2]<(target/2))&(E[2]>0))
    {
      Kp=Kp*0.5;Ti=1.2*Ti;
    }
    else if((E[2]<(target/1.6))&(E[2]>0))
    {
      Kp=Kp*0.8;Ti=1.4*Ti;
    }
    else if((E[2]<(target/1.4))&(E[2]>0))
    {
      Kp=Kp*1.3;Ti=1.6*Ti;
    }
    else if((E[2]<(target/1.2))&(E[2]>0))
    {
      Kp=Kp*1.8;Ti=1.8*Ti;
    }
    else if((E[2]>(target/1.2))&(E[2]>0))
    {
      Kp=Kp*1.8;  Ti=2*Ti;     //1.3
    }
    
    else if(E[2]>(-target/10.0))      //比例系数模糊控制
    {
      Kp=Kp*0.1;Ti=0.8*Ti;
    }
    else if(E[2]>(-target/5.0))
    {
      Kp=Kp*0.1;Ti=1*Ti;
    }
    else if(E[2]>(-target/3.3))
    {              
      Kp=Kp*0.1;Ti=1*Ti;
    }
    else if(E[2]>(-target/2.5))
    {
      Kp=Kp*0.1;Ti=1*Ti;
    }
    else if(E[2]>(-target/2))
    {
      Kp=Kp*0.3;Ti=1.2*Ti;
    }
    else if(E[2]>(-target/1.6))
    {
      Kp=Kp*0.6;Ti=1.4*Ti;
    }
    else if(E[2]>(-target/1.4))
    {
      Kp=Kp*0.7;Ti=1.6*Ti;
    }
    else if(E[2]>(-target/1.2))
    {
      Kp=Kp*0.8;Ti=1.8*Ti;
    }
    else if(E[2]<(-target/1.2))
    {
      Kp=Kp*1;  Ti=2.5*Ti;
    }
//    if(target<130)
//    {
//      Kp=0.04;
//      Ti=9000;
//    }
//     //输入预期-反馈
//     if((E[2]>E[1])&(E[1]>E[0]))Kp=10;
//     if((E[2]>20000))Ti=4000;
//     if((E[2]<-0))Ti=90000;
//     if((E[2]>-6000)&(E[2]<0)){Ti=90000;Kp=0.1;}
//     if((E[2]>0)&(E[2]<6000)){Ti=500;Kp=0.1;}
     increment=(E[2])*(Kp*(1+T/Ti+Td/T)) - Kp*(1+2*Td/T)*(E[1]) + (E[0])*Kp*Td/T;   
     E[0] = E[1];
     E[1] = E[2];                                               //PID增量式算法
     return increment;                                         //返回增量
}

float PID_Position(float Kp,float Ti,float Td,float T,short feedback,float target)
{
    float pwm;                                                 //定义位置输出
    Bias[2]= target-feedback;                                   //偏差
    
    if(Bias[2]<0){Kp=15;Ti=99999999999;}
    
//    if((Bias[2]<(-50))&(Bias[2]>(-100)))Ti=1000;
//    if((Bias[2]>(-50))&(Bias[2]<(0)))Ti=9999;
//    if((Bias[2]<(50))&(Bias[2]>(0)))Ti=10; 
//        if((Bias[2]>(75))||(Bias[2]<(-75)))Ti=30000;     
//  Kp=Kp*(Bias[2]/100.0);
//  Ti=Ti*(100.0/(abs(Bias[2])+100.0));
//  if((Bias[0]>Bias[1])&(Bias[1]>Bias[2])){Kp=0.8*Kp;Ti=0.8*Ti;}
    
    
    if((Bias[2]<(target/10.0))&(Bias[2]>0))      //比例系数模糊控制
    {
      Kp=Kp*0.1;Ti=0.2*Ti;
    }
    else if((Bias[2]<(target/5.0))&(Bias[2]>0))
    {
      Kp=Kp*0.1;Ti=0.4*Ti;
    }
    else if((Bias[2]<(target/3.3))&(Bias[2]>0))
    {              
      Kp=Kp*0.2;Ti=0.6*Ti;
    }
    else if((Bias[2]<(target/2.5))&(Bias[2]>0))
    {
      Kp=Kp*0.3;Ti=0.8*Ti;
    }
    else if((Bias[2]<(target/2))&(Bias[2]>0))
    {
      Kp=Kp*0.5;Ti=1*Ti;
    }
    else if((Bias[2]<(target/1.6))&(Bias[2]>0))
    {
      Kp=Kp*0.7;Ti=1.5*Ti;
    }
    else if((Bias[2]<(target/1.4))&(Bias[2]>0))
    {
      Kp=Kp*0.8;Ti=2*Ti;
    }
    else if((Bias[2]<(target/1.3))&(Bias[2]>0))
    {
      Kp=Kp*0.9;Ti=2.5*Ti;
    }
    else if((Bias[2]>(target/1.2))&(Bias[2]>0))
    {
      Kp=Kp*1;  Ti=3*Ti;
    }
    
    else if(Bias[2]>(-target/10.0))      //比例系数模糊控制
    {
      Kp=Kp*0.1;Ti=Ti;
    }
    else if(Bias[2]>(-target/5.0))
    {
      Kp=Kp*0.1;Ti=0.8*Ti;
    }
    else if(Bias[2]>(-target/3.3))
    {              
      Kp=Kp*0.2;Ti=0.6*Ti;
    }
    else if(Bias[2]>(-target/2.5))
    {
      Kp=Kp*0.3;Ti=0.6*Ti;
    }
    else if(Bias[2]>(-target/2))
    {
      Kp=Kp*0.3;Ti=0.6*Ti;
    }
    else if(Bias[2]>(-target/1.6))
    {
      Kp=Kp*0.3;Ti=0.5*Ti;
    }
    else if(Bias[2]>(-target/1.4))
    {
      Kp=Kp*0.4;Ti=0.4*Ti;
    }
    else if(Bias[2]>(-target/1.3))
    {
      Kp=Kp*0.6;Ti=0.3*Ti;
    }
    else if(Bias[2]<(-target/1.2))
    {
      Kp=Kp*0.8;  Ti=0.2*Ti;
    }
     LCD_SHOW((short)(Kp*1000),0,4);                    //显示Kp
     LCD_SHOW((short)(Ti),0,5);                         //显示Ti
//       Kp=Kp*((abs(Bias[2])+30)/300.0);
//       Ti=Ti*(((abs(Bias[2])+100.0))/100.0);
    Integral_Bias+=Bias[2];                             //累积偏差
    pwm = Kp*( Bias[2]  +  (T/Ti)*Integral_Bias  +  Td*(Bias[2]-Bias[1])/T );   //增量式算法
    if((feedback<20)&(count>1000))                      //当位于底部，快速上升                
    {
     pwm=12000;
//      FTM_PWM_Duty(FTM0,FTM_CH0,duty);
//      time_delay_ms(100);    
    }
  Bias[0]=Bias[1];
  Bias[1]=Bias[2];
  return pwm;
}

void PIT0_Interrupt()                                    //周期定时器中断
{
    PIT_Flag_Clear(PIT0);                                 //清中断标志位
 
    LED_Ctrl(LED3, RVS);                                //中断发生后LED闪烁 
    if(flag[1]==1)                                      //当在界限内，time++
    {
        time++;
    }
  if((flag[0]==0)&(flag[1]==1))time=0;                   //当进入界限，time清零
 //   LCD_SHOW(time,0,3);
}

void PIT1_Interrupt()                                    //周期定时器中断
{
    PIT_Flag_Clear(PIT1);                                 //清中断标志位
 

        time_i++;
    

}

void PORTB_Interrupt()                                  //按键外部中断
{
  int n;
  n=20;                                                 //key0外部中断
  if((PORTB_ISFR & (1<<n)))
  {
//      PORTB_ISFR |= (1<<n);
    
//      GPIO_Ctrl(PORTB,20,1);
      LED_Ctrl(LED1,RVS);                                //按下一次触发一次外部中断,对应LED亮一次
      LCD_CLS();                                         //清屏
      LCD_P6x8Str(0,0," PLEASE SET THE MODE");
      if(mode==0)                                        //显示当前模式
      {
         LCD_P6x8Str(0,1," CURRENT MODE : STAY");
      }
      else if(mode==1)
      {
         LCD_P6x8Str(0,1," CURRENT MODE : SET");
      }
            else if(mode==2)
      {
         LCD_P6x8Str(0,1," CURRENT MODE : RUSH");
      }
            else if(mode==3)
      {
         LCD_P6x8Str(0,1," CURRENT MODE : SWAY");
      }
            else if(mode==5)
      {
         LCD_P6x8Str(0,1," CURRENT MODE : ENTER");
      }
            else if(mode==6)
      {
         LCD_P6x8Str(0,1," CURRENT MODE : STOP");
      }
      LCD_P6x8Str(0,2,"   MODE_1 : STAY");              //模式A
      LCD_P6x8Str(0,3,"   MODE_2 : SET");               //模式B
      LCD_P6x8Str(0,4,"   MODE_3 : RUSH"); 
      LCD_P6x8Str(0,5,"   MODE_4 : SWAY"); 
      LCD_P6x8Str(0,6,"   MODE_5 : ENTER"); 
      LCD_P6x8Str(0,7,"   MODE_6 : STOP"); 
      
      for(unsigned long i=0;i<5000000;i++);           //消抖      
      int flag1=0;
      while(flag1==0)                                   //直至按键松开才进入扫描模式
      {
         if(GPIO_Get(PTB20))flag1=1;
      }
//      LCD_SHOW(flag1,0,1);
//      for(unsigned long i=0;i<5000000;i++);

//      for(unsigned long i=0;i<4000000;i++);
//      time_delay_us(5);
       strcpy(num_txt,"");                                //矩阵键盘输入字符串清零
       strcpy(num_input_txt,"");
       num_input=0;                                       //字符串合成的输入高度清零
       num=0;
          
       while(flag1)                                      //按键已松开
       {          

          Squ_Init(); 
          if(num==1)                             //扫描 如果KEY0按下
          {
              mode=0;                                       //模式0 stay
              flag1=0;                                      //将标志位清0，以退出扫描
              Height_set=150;                               //设置高度150mm
              scale=50;
              LCD_CLS();                                    //清屏
              LCD_P6x8Str(0,0,"mode0");                     //LCD显示模式
              LCD_SHOW(flag1,0,1);
//      for(unsigned long i=0;i<500000;i++);
//      int flag2=0;
//      while(flag2==0)
//      {
//      if(GPIO_Get(PTB20))flag2=1;
//      }
//      for(unsigned long i=0;i<500000;i++);
//      time_delay_us(50000);
//      time_delay_us(50000);
      
//       Height_set+=10;
          }
          else if(num==2)                                     //扫描，如果KEY2按下
          {
              mode=1;                                            //模式1 set——height
              flag1=0;                                           //将标志位清0，以退出扫描
            JMP:                                                 //goto 跳转
              LCD_CLS();                                         //清屏
              LCD_P6x8Str(0,0,"  PLEASE ENTER YOUR");            //显示输入你的高度
              LCD_P6x8Str(0,1,"    HEIGHT TERGET");
              LCD_P6x8Str(0,5,"Press Key1: CLEAR");
              LCD_P6x8Str(0,6,"Press Key2: OK");
              strcpy(num_txt,"");                                //矩阵键盘输入字符串清零
              strcpy(num_input_txt,"");
              num_input=0;                                       //字符串合成的输入高度清零
              num=0;
       
              for(unsigned long i=0;i<5000000;i++);            //消抖
              int flag3=0;
              while(flag3==0)                                    //知道按键按下才进入矩阵键盘输入
              {
                  if(GPIO_Get(PTB21))flag3=1;
              }
              while(flag3)
              {
                  Squ_Init();
//       strcat(num_txt,num_input_txt);
//       num = atoi(num_txt);
  //     LCD_SHOW(num_input,0,2);
                  LCD_P6x8Str(0,4,"HEIGHT TERGET:");                  //LCD显示目标高度
                  LCD_P6x8Str(88,4,(uint8*)num_txt);
                  LCD_P6x8Str(110,4,(uint8*)"mm");                   //显示单位
                  Height_set=num;                                   //把输入的值赋给高度
                  scale=30;
                  if(!KEY_Read(KEY2))                              //按键1按下，表示确定
                  {
                      if((num<100)||(num>200))                       //如果非法，重新输入
                      {
                          LCD_CLS();
                          LCD_Print(0,0,(uint8*)"   WRONG DATA");
                          LCD_P6x8Str(0,2,(uint8*)"  PLEASE ENTER NUM ");
                          LCD_P6x8Str(0,3,(uint8*)"  FROM 100 TO 200 ");
                          LCD_P6x8Str(0,5,(uint8*)" PLS REENTER THE NUM");
                          for(unsigned long i=0;i<150000000;i++);
                          goto JMP;                                   //重新输入
                      }
                      else
                      {
                          flag3=0;                                    //如果合法，退出循环
                      }
                  }
                  if(!KEY_Read(KEY1))                              //按键0按下，清零
                  {
                      strcpy(num_txt,"");
                      LCD_P6x8Str(88,4,(uint8*)"    mm");
                  }
              }
          }
     
          else if(num==3)                                    //如果按键3按下，进入mode2
          {
              mode=2;                                         //mode2
              flag1=0;                                        //退出循环    
          }
     
          else if(num==4)                                    //如果按键4按下，进入mode3
          {
              mode=3;                                         //mode3
              flag1=0;                                        //退出循环
          }
     
          else if(num==5)                                    //如果按键5按下，进入mode4       
          {  
              mode=5;                                         //mode4
              flag1=0;                                        //退出循环
          }
          else if(num==6)
          {
              mode=6;
              flag1=0; 
          }
     
        }
        PORTB_ISFR |= (1<<n);
  }
  
  n=21;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);

      LED_Ctrl(LED1,RVS);          //按下一次触发一次 外部中断,对应LED亮一次
            LCD_CLS();
  }
  
  n=22;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);

      LED_Ctrl(LED1,RVS);          //按下一次触发一次外部中断,对应LED亮一次
  }
}



void PORTD_Interrupt()
{
  int n;
  n=0;
  if((PORTD_ISFR & (1<<n)))
  {
      PORTD_ISFR |= (1<<n); 

            LED_Ctrl(LED0,ON);          //按下一次触发一次外部中断,对应LED亮一次
            GPIO_Init(PORTD,0,GPO,0);
            GPIO_Init(PORTD,1,GPO,0);
            GPIO_Init(PORTD,2,GPO,0);
            GPIO_Ctrl(PORTD,3,1);
            GPIO_Ctrl(PORTD,4,1);
            GPIO_Ctrl(PORTD,5,1);
            if(!GPIO_Get(PTD3))num_input=3;
            if(!GPIO_Get(PTD4))num_input=6;
            if(!GPIO_Get(PTD5))num_input=9;
            sprintf(num_input_txt,"%u",num_input);
  } 
  n=1;
  if((PORTD_ISFR & (1<<n)))
  {
      PORTD_ISFR |= (1<<n); 

            LED_Ctrl(LED1,ON);          //按下一次触发一次外部中断,对应LED亮一次
            GPIO_Init(PORTD,0,GPO,0);
            GPIO_Init(PORTD,1,GPO,0);
            GPIO_Init(PORTD,2,GPO,0);
            GPIO_Ctrl(PORTD,3,1);
            GPIO_Ctrl(PORTD,4,1);
            GPIO_Ctrl(PORTD,5,1);
            if(!GPIO_Get(PTD3))num_input=2;
            if(!GPIO_Get(PTD4))num_input=5;
            if(!GPIO_Get(PTD5))num_input=8;
                        sprintf(num_input_txt,"%u",num_input);
  } 
  n=2;
  if((PORTD_ISFR & (1<<n)))
  {
      PORTD_ISFR |= (1<<n); 

            LED_Ctrl(LED2,ON);          //按下一次触发一次外部中断,对应LED亮一次
            GPIO_Init(PORTD,0,GPO,0);
            GPIO_Init(PORTD,1,GPO,0);
            GPIO_Init(PORTD,2,GPO,0);
            GPIO_Ctrl(PORTD,3,1);
            GPIO_Ctrl(PORTD,4,1);
            GPIO_Ctrl(PORTD,5,1);
            if(!GPIO_Get(PTD3))num_input=1;
            if(!GPIO_Get(PTD4))num_input=4;
            if(!GPIO_Get(PTD5))num_input=7;
                        sprintf(num_input_txt,"%u",num_input);
  }
  
  n=7;
  if((PORTD_ISFR & (1<<n)))
  {
     
      for(unsigned long i=0;i<10000000;i++);
      GPIO_Ctrl(PORTD,7,1);
       if(!GPIO_Get(PTD6))num_input=0;
      num_input=0;
      sprintf(num_input_txt,"%u",num_input);
   PORTD_ISFR |=(1<<n);
  }
  for(unsigned long i=0;i<10000000;i++);
  strcat(num_txt,num_input_txt);
       num = atoi(num_txt);


//      int flag2=0;
//      while(flag2==0)
//      {
//      if(GPIO_Get(PTD0)&GPIO_Get(PTD1)&GPIO_Get(PTD2))flag2=1;
//      }
}



void main(void)
{    
   DisableInterrupts;                                                           //关闭中断
   PLL_Init(PLL200);                                                            //初始化PLL
   FTM_PWM_Init(FTM0,FTM_CH0,62500,0);
 //  time_delay_ms(175);
   I2C_Init(I2C1);                                                              //I2C初始化
   LED_Init();               //LED初始化,VG1核心板上LED为PTC0,A17,D15,E26,
   LED_Ctrl(LEDALL, OFF);     //LED全亮  
   KEY_Init();                                                                  //按键初始化
  
//   Squ_Init();
   PIT_Init(PIT0, 100);      //中断定时500ms，1s闪烁一次
   PIT_Init(PIT1, 100);
   LCD_Init();                                                                  //OLED模块初始化，默认使用PTB16-19
   Draw_LibLogo();                                                              //OLED显示LOGO
   time_delay_ms(100);
   LCD_CLS();                                                                   //LCD清屏  

   //频率：40M/(2^4)/50000=100HZ,每个脉冲为0.2us 
                                         //C1 低电平 Left，输出PWM
//   FTM_PWM_Init(FTM0,FTM_CH1,62500,0);//C2 低电平 Right
//   FTM_PWM_Init(FTM0,FTM_CH2,50000,50000);//C3 0-1 P3 Left
//   FTM_PWM_Init(FTM0,FTM_CH3,50000,50000);//C4 0-1 P4 Right 
   
   set_irq_priority(88,15);
   set_irq_priority(90,14);
   UART_Init (UART4,9600); //串口初始化,UART4,用PTE24,E25
                                                               //开启中断
   
   LCD_P6x8Str(0,5,"FUNC Key0: MODE SET");
   LCD_P6x8Str(0,3,"START MODE: STOP");
   LCD_P6x8Str(0,1,"Press Key0 to START");
   int a=1;
   while(a)
   {
   if(!GPIO_Get(PTB20))a=0;
   
   }
   time_delay_ms(500);
    Key_Int_Init();
   EnableInterrupts;
   while(1)
  {  
    
    count++;
    LED_Ctrl(LED2,RVS);
    Height_Present=read_dist();
//    Height_Present = (short)(/*read_dist()*/);                                     //将当前测量值转化为当前高度
    if(Height_Present>235)
    {
        Height_Present = (short)(Height_Present*(-1.389)+512.5);
    } 
    else
    {
        Height_Present = (short)(Height_Present*(-0.909)+404.5);
    } 
    if(Height_Present<200)Height_Present=(short)(Height_Present*15/17.0+39.7);
 //   time_delay_ms(50); 
    Height_adj = filter(Height_Present); 

    LCD_CLS();                                        //软件滤波得到滤波后的当前高度
//     Height_adj+=20;
//     if(Height_adj<10)
//     {
//    Kp=0.007;
//    Ti=5000; 
//    Td=0;
     duty += PID_output(Kp,Ti,Td,56.5,Height_adj,Height_set);              //PWM输出由PID算法得出   200   0.005,1000     100   0.015 2000  150 0.01  1500
//     }
//     else if(Height_adj<20)
//     {
//     duty += PID_output(5,10000,0,50,Height_adj,20);
//     }
//     else 
//     {
//     duty += PID_output(6,9990000,0,50,Height_adj,20);
//     }
//     if(Height_adj<0)duty+=((0-Height_adj));
//     if(Height_adj>0)duty-=((Height_adj-0));
//     duty=(int)PID_Position(Kp,Ti,Td,50,Height_adj,Height_set*1)+9900;
     if(duty>62500)duty = 62500;  if(duty<0)duty = 0;                                 //限制PWM输出在0-62500范围内
       
     
//     time_delay_us(500); //测量和控制的延迟
     if(mode==5)
     {
        scale_limit=330;
        if(Height_adj>25)
        {
while(f)
          {
        FTM_PWM_Duty(FTM0,FTM_CH0,60000);
        time_delay_ms(800);
        f=0;
          }
        Q:       Height_set=150;
        scale=35;
          FTM_PWM_Duty(FTM0,FTM_CH0,duty);
        }
        else
        {
                    if(f==0)goto Q;
          FTM_PWM_Duty(FTM0,FTM_CH0,0);
          
        }
        LCD_P6x8Str(90,1,"ENTER");
        
     }
     else if(mode==1)
     {
       FTM_PWM_Duty(FTM0,FTM_CH0,duty);
       scale=15;
       LCD_P6x8Str(90,1,"SET");
     }
     
     else if(mode==2)
     {
          FTM_PWM_Duty(FTM0,FTM_CH0,25300);
          Height_set=350;
          scale=15;
          LCD_P6x8Str(90,1,"RUSH");
     }
     else if(mode==3)
     {
       LCD_P6x8Str(90,1,"SWAY");
        if(flag_p==0)
        {
            Height_set=250;
            scale=50;
            FTM_PWM_Duty(FTM0,FTM_CH0,duty);
            if(time>30)
            {
              flag_p=1;
              time=0;
            }
        }
        else if(flag_p==1)
        {
            Height_set=50;
            FTM_PWM_Duty(FTM0,FTM_CH0,duty);
            if(time>30)
            {
              flag_p=2;
              time=0;
            }
        }
        else if(flag_p==2)
        {
            Height_set=250;
            FTM_PWM_Duty(FTM0,FTM_CH0,duty);
            if(time>30)
            {
              flag_p=3;
              time=0;
            }
        } 
        else if(flag_p==3)
       {
            Height_set=50;
            FTM_PWM_Duty(FTM0,FTM_CH0,duty);
            if(time>30)
            {
              flag_p=4;
              time=0;
            }
       }
       else if(flag_p==4)
       {
            FTM_PWM_Duty(FTM0,FTM_CH0,60000);
       }    
     }
     else if(mode==0)
     {
        FTM_PWM_Duty(FTM0,FTM_CH0,duty);
        Height_set=150;
        scale=35;
        LCD_P6x8Str(90,1,"STAY");
     }
     else if(mode==6)
     {
        //duty=0;
        FTM_PWM_Duty(FTM0,FTM_CH0,0);
        duty=       22300;
        LCD_P6x8Str(90,1,"STOP");
     }
     
//Kp=Kp*(abs(Bias[2])/100);
 // Ti=Ti*(100/(abs(Bias[2])+100));
     short bias=(short)(Height_set-Height_adj);
     //输出PWM宽度
     
     LCD_P6x8Str(0,1,"Mode:");
     LCD_SHOW(Height_set,90,2);                                              //第一行显示当前高度
     LCD_P6x8Str(0,2,"Height_Set:");  
     LCD_P6x8Str(110,2,"mm"); 
     LCD_P6x8Str(0,3,"Height_Current:") ;
     LCD_P6x8Str(110,3,"mm");      
     LCD_SHOW(Height_adj,90,3);                                              //第一行显示当前高度
     LCD_P6x8Str(0,4,"Bias_Current:");
     LCD_SHOW(bias,90,4);                                                  //第二行显示滤波后的当前高度
     LCD_P6x8Str(110,4,"mm");     
     LCD_P6x8Str(0,5,"Duty:")  ;   
     LCD_SHOW(duty,90,5);                                                        //显示当前PWM高电平宽度
     LCD_P6x8Str(0,6,"Time_Integral:");
     sprintf(time_i_txt_P,"%u",time_i/10);
     sprintf(time_i_txt_F,"%u",time_i%10);
       strcat(time_i_txt_P,".");
       strcat(time_i_txt_P,time_i_txt_F);
       LCD_P6x8Str(90,6,(unsigned char*)time_i_txt_P);
//     LCD_SHOW(time_i/10,90,6);
       LCD_P6x8Str(120,6,"s");
       LCD_P6x8Str(0,7,"Time_Inside:");
 //    LCD_P6x8Str(90,7,(uint8*)sprintf(time_txt,"%u",time));
       LCD_P6x8Str(120,7,"s");
       sprintf(time_txt_P,"%u",time/10);
       sprintf(time_txt_F,"%u",time%10);
       strcat(time_txt_P,".");
       strcat(time_txt_P,time_txt_F);
       LCD_P6x8Str(90,7,(unsigned char*)time_txt_P);
//     LCD_SHOW(count,90,7);
//     LCD_SHOW(count,0,3);
//     LCD_SHOW((short)(Kp*1000),0,4);
 //    LCD_SHOW((short)(Ti),0,5);
//     LCD_SHOW(mode,0,6);
     int Height_Send=200+Height_adj;
     char Height_Send_txt[16];
     sprintf(Height_Send_txt,"%u",Height_Send);
     UART_Put_Str(UART4,Height_Send_txt); 
     //LCD_SHOW(num,0,6);    
//     LCD_CLS();
//     if(!KEY_Read(KEY0))           //如果KEY1按下
//     {
//       
//       Height_set+=10;
//     }
//     else if(!KEY_Read(KEY1))     //如果KEY2按下
//     {
//       Height_set-=10;       
//     }
     // time_delay_ms(100);
  } 
}