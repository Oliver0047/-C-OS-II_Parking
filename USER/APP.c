#include "includes.h"
#include "bmp.h"
char cStr[100]={0};
char *p,*pCh;//wifi�����׵�ַ��ƥ���׵�ַ
char oledcar[50],oledinfo[50];//OLED������Ϣ
u8 ucId;//�ŵ�
int carnum=0,carin=0,carout=0;//ͣ������������������������
u8 link_status;//����״̬
/*�ͻ�����ҳ*/
char page[]="<html><body><a href=\"/ENTER\"><input type=\"Button\" value=\"ENTER\" style=\"position:absolute;top:50%;left:40%\"></input></a><a href=\"/PLAY\"><input type=\"Button\" value=\"PLAY\" style=\"position:absolute;top:50%;left:50%\"></input></a><a href=\"/LEAVE\"><input type=\"Button\" value=\"LEAVE\" style=\"position:absolute;top:50%;left:60%\"></input></a></body></html>";
/*������ʾ*/
void printInfo(void)
{
    OLED_Fill(0x00);//����
    OLED_ShowStr(10,0,(unsigned char*)"Oliver-parking lot!",1);//��ʾ����
    OLED_ShowStr(20,2,(unsigned char*)"Welcome home!",1);//��ʾ��Ϣ
}
void Task_esp8266_sendHTML(void *p_arg)
{
     u8 i;
    (void)p_arg;
    while(1)
    {
        link_status=ESP8266_Get_IdLinkStatus();//��ȡ���µ��������
        for(i=0;i<5;i++)
        {
            if(((link_status>>i) & 0x01)!=0)
            {
                ESP8266_SendString ( DISABLE, page, strlen ( page ), ( ENUM_ID_NO_TypeDef ) i );//��ÿ���ͻ��˷�����ҳ
            }
        }
        OSTimeDlyHMSM(0,0,1,0);
    }
}
void Task_esp8266(void *p_arg)
{ 
    (void)p_arg;
    printInfo();//��ʾ����
    sprintf(oledinfo,"      <Start> ");
    OLED_ShowStr(0,4,(unsigned char*)oledinfo,1);//��ʾ�����������
    sprintf(oledcar,"Car number: %d",carnum);
    OLED_ShowStr(20,6,(unsigned char*)oledcar,1);//��ʾͣ�������
    ESP8266_Inquire_ApIp(cStr, 20 );//��ȡ����IP
    while(1)
    {
        p=ESP8266_ReceiveString(DISABLE);//�������ݣ�pΪ���յ������ݵ��׵�ַ
        if(strEsp8266_Fram_Record .InfBit .FramFinishFlag )//������յ�������
		{
			USART_ITConfig (USART2, USART_IT_RXNE, DISABLE ); //���ô��ڽ����ж�
            //printf("���յ�������: %s\r\n",strEsp8266_Fram_Record.Data_RX_BUF);
			if ( ((bool)strstr (strEsp8266_Fram_Record.Data_RX_BUF, "ENTER" ) ) != 0 ) //����ǽ����ź�
            {
                if(carnum==5)//��λ�Ѿ�����
                {
                    sprintf(oledinfo,"Filled!Filled!Filled!");
                }
                else
                {
                    Gate1Open();//�򿪽�������
                    carin++;//��������1
                    carnum++;//��λ��1
                    sprintf(oledinfo,"  %d cars has entered!",carin);
                    
                } 
            }
            else if ( ((bool)strstr (strEsp8266_Fram_Record.Data_RX_BUF, "LEAVE" ) ) != 0 ) //����ǳ����ź�
            {
                if(carnum==0)//û�����Ͳ��ܳ���
                {
                    sprintf(oledinfo,"  Are you kidding?");
                }
                else
                {
                    
                    carnum--;//��λ��1
                    Gate2Open();//�򿪳�������
                    carout++;//��������1
                    sprintf(oledinfo,"  %d cars has leaved!",carout);
                }
            }
            else if( ((bool)strstr (strEsp8266_Fram_Record.Data_RX_BUF, "PLAY" ) ) != 0 )
            {
                OLED_DrawBMP(0,0,128,8,BMP3);
                OSTimeDlyHMSM(0,0,2,0);
            }
            if ( ( pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD," ) ) != 0 ) //���ͷ�����Ϣ��������IP
			{
				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';//���������׵����ŵ��л���������Ҫʵʱ��ȡ�ŵ�
				ESP8266_SendString ( DISABLE, cStr, strlen ( cStr ), ( ENUM_ID_NO_TypeDef ) ucId );//��������
			}
            OSTimeDlyHMSM(0,0,1,0);//��ʱ1s��ر�����
            Gate1Close();
            Gate2Close();
            printInfo();//��ʾ����
            OLED_ShowStr(0,4,(unsigned char*)oledinfo,1);//��ʾ�����������
            sprintf(oledcar,"Car number: %d",carnum);
            OLED_ShowStr(20,6,(unsigned char*)oledcar,1);//��ʾͣ�������
        }
        strEsp8266_Fram_Record .InfBit .FramLength = 0;
	    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;		
		USART_ITConfig (USART2, USART_IT_RXNE, ENABLE ); //ʹ�ܴ��ڽ����ж�
        OSTimeDlyHMSM(0,0,1,0);
    }
}
