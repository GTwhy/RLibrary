#include "distance.h"
#include "usart.h"

u8 get_distance(u8 parameter,u16 rssi)
{
	u8 distance;
	rssi/=10;//ȡrssiƽ��ֵ
	distance = rssi;//�������
	return distance;
}


void send_distance(u8 distance)
{
	USART_SendData(USART2, 0xff);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���,
	USART_SendData(USART2, id);//����id
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���,
	USART_SendData(USART2, distance);//���;���
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���,
	
}