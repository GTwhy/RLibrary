#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "OLED_I2C.h"
#include "rfid.h"
#include "parser.h"
#include "distance.h"
 
 
/************************************************
����˵��
1.UART1�������ݶ��н�������ʽ���ն��������ص�24λ����
2.UART2��������λ���������ݣ������жϷ������ĩβ�س��жϣ�BUF�洢
3.
************************************************/


 int main(void)
 {		
 	u8 t=0,i,j,k,times,flag,param=1;  	
	u8 x=0,y=0,distance=0;
	u8 comm[]={0xBB,0x00,0x28,0x00,0x00,0x28,0x7E,0x0d,0x0a};
	u32 dis[20][3],temp[10];
 	data_parser  = parser_init(data_header, 1,data_back,1,24);
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	uart2_init(115200);
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();
	USART_GetFlagStatus(USART1,USART_FLAG_TC);
	USART_GetFlagStatus(USART2,USART_FLAG_TC);
	for(j=0;j<9;j++)
	{
		USART_SendData(USART1, comm[j]);//�򴮿�1��������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���,
	}
	OLED_ShowStr(0,0,"---detecting---",2);
	OLED_ShowStr(0,2,"I  D :",2);
	OLED_ShowStr(0,4,"RSSI :",2);
	OLED_ShowStr(0,6,"DIST :",2);
	
 	while(1)
	{
		if(USART_RX_STA==1)//�յ�rfid��ǩ��Ϣ
		{
			for(j=0;j<10;j++)//��ѯ�������ж��Ƿ����¼���
			{
				if(temp[j]==id)//�ڻ��������ҵ���ǰid
				{
					flag=1;
					break;
				}
			}
			
			if(flag!=1)//��������û�д�id
			{
				temp[t]=id;//���뻺����
				t++; //������ָ�����
				if(t>9) t=0;	//���¸��ǻ�����
				dis[x][0]=id;		//���ۼ�����ӵ�ǰ��ǩ
				dis[x][1]=1;	//rssi������1
				dis[x][2]=rssi;	//�����һ��rssi
				x++; //�ۼ���ָ���1
				
			}
			else//�������������id˵���ձ�ɨ������ô�Ϳ�ʼ�ۼ�
			{	flag=0;
				for(j=0;j<20;j++)	//�ҵ���ǰid���ۼ�����λ��
				{
					if(dis[j][0]==id)	//�ҵ�id������
					{	
						dis[j][1]++;	//rssi������һ
						dis[j][2]+=rssi;	//�ۼ�rssi
						if(dis[j][1]==10)//��ǰɨ���ı�ǩ�Ѿ��ɼ�����10��
						{
							distance=get_distance(param,dis[j][2]);
							send_distance(distance);
							dis[j][0]=0;
							x--;
						}
						break;
					}
				}
			}

			
			//��ʾID��RSSI������
			OLED_ShowNum(60,2,id,2,2,0);
			OLED_ShowNum(60,4,rssi,3,2,0);
			OLED_ShowNum(60,6,distance,3,2,0);
			//��ձ�־λ
			USART_RX_STA=0;
			
//			USART_SendData(USART2, 0xff);//�򴮿�1��������
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���,
//			USART_SendData(USART2, id);//����id
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���,
//			USART_SendData(USART2, distance);//���;���
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���,
		}
		
	
	}	 	
 }

