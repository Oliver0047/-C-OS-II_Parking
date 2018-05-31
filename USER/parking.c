#include "includes.h"	   
//�����������ȼ�
#define ESP8266_TASK_PRIO 9 
#define HTML_TASK_PRIO 10
//����ջ��С
#define ESP8266_STK_SIZE 256
#define HTML_STK_SIZE 128
//����ջ
static OS_STK TASK_ESP8266_STK[ESP8266_STK_SIZE];
static OS_STK TASK_HTML_STK[HTML_STK_SIZE];
int main(void)
{
    BSP_Init();//�����ʼ��
    OSInit();//����ϵͳ��ʼ��
    OSTaskCreate(Task_esp8266,(void *)0,
	   &TASK_ESP8266_STK[ESP8266_STK_SIZE-1], ESP8266_TASK_PRIO);//����ESP8266��������
    OSTaskCreate(Task_esp8266_sendHTML,(void *)0,
	   &TASK_HTML_STK[HTML_STK_SIZE-1], HTML_TASK_PRIO);//����ESP8266������ҳ����
	OSStart();//��������ϵͳ
    return 0;
}
