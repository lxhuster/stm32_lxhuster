#include<stm32f10x_flash.h>
#include<sys_init.h>

int main()
{
    bsp_init();
    while(1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
    }
}
