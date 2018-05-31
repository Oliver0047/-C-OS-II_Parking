#include "includes.h"

void SysTick_init(void)//ϵͳ�δ�ʱ����ʼ��
{
    SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC);
}
void ESP8266_Config(void)
{
    ESP8266_CH_ENABLE();//ʹ��ESP8266
    ESP8266_AT_Test ();//�����Ƿ�����
    ESP8266_Net_Mode_Choose ( AP );//ѡ��APģʽ������wifi�źţ��ȶ�
    ESP8266_Rst();//��λ��Ч

    while(!ESP8266_CIPAP(esp8266_ip ));//IP��ȷ���
    while(!ESP8266_BuildAP(esp8266_user, esp8266_password, WPA_WPA2_PSK) );//����ID�����룬�ŵ������ܷ�ʽ

    ESP8266_Enable_MultipleId ( ENABLE );//���������ӣ���������5���ͻ���
    while(!ESP8266_StartOrShutServer(ENABLE, esp8266_port,esp8266_overTime));//���ö˿ںźͳ�ʱʱ��
    
    /*�����������ͱ�־����*/
	strEsp8266_Fram_Record.InfBit.FramLength = 0;
	strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;	
    ESP8266_UnvarnishSend();//����͸��ģʽ
}
void BSP_Init(void)
{
    SysTick_init();//��ʱ��ʼ��
    TIM3_GPIO_Config();//TIM����PWMͨ��GPIO��ʼ��
    TIM3_PWM_Init();//PWM��ʼ��
    USART1_Init();//PC�˴���
    I2C_Configuration();//I2C��ʼ��
    OLED_Init();//OLED��ʼ��
    ESP8266_Init();//�������ã���������
    ESP8266_Config();//IP��ַ��������������
}
