#define GPKCON0  (*((volatile unsigned long *)0x7f008800))
#define GPKDAT  (*((volatile unsigned long *)0x7f008808))
/* Bit Operate */
#define SET_BIT(x,y) (x|=(1<<y))
#define CLR_BIT(x,y) (x&=~(1<<y))
#define CPL_BIT(x,y) (x^=(1<<y))
#define GET_BIT(x,y) (x&(1<<y))
void delay()
{
 volatile unsigned long i=0x10000;
 while(i--);
}
void led()
{
 int i=0;
 GPKCON0=0x11110000;
 GPKDAT=0XF0;
 for(i=0;i<100;i++)
 {
  if(i%2==0)
   CLR_BIT(GPKDAT,4);
  else
   SET_BIT(GPKDAT,4);
  delay();
 }
}