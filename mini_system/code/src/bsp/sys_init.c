#include <stm32f10x_conf.h>
#include <bsp_config.h>

void rcc_config(void)
{
/* init rcc */
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    while(RCC_WaitForHSEStartUp() == ERROR);

    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div2);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    while(RCC_GetSYSCLKSource() != 0x08);

/** 
 * enable clock, this should be change on specify board
 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}

void led_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void usart_config(void)
{
    NVIC_InitTypeDef   NVIC_InitStruct;
    USART_InitTypeDef  USART_InitStruct;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQChannel;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    
    USART_DeInit(USART1);
    USART_InitStruct.USART_BaudRate=115200;
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;
    USART_InitStruct.USART_StopBits=USART_StopBits_1;
    USART_InitStruct.USART_Parity=USART_Parity_No;
    USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None; 
    USART_Init(USART1,&USART_InitStruct);

    //USART_ITConfig(USART1,USART_IT_TC,D);
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  
    USART_Cmd(USART1,ENABLE);
}

/**
 * author: lxhuster
 * abstract: init the device on chip
 */
void bsp_init(void)
{
    rcc_config();
    
#ifdef BSP_USE_LED
    led_config();
#endif
    
#ifdef BSP_USE_USART
    usart_config();
#endif
}
