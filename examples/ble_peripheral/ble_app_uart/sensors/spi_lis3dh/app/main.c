/****************************************Copyright (c)************************************************
**                                      [����ķ�Ƽ�]
**                                        IIKMSIK 
**                            �ٷ����̣�https://acmemcu.taobao.com
**                            �ٷ���̳��http://www.e930bbs.com
**                                   
**--------------File Info-----------------------------------------------------------------------------
** File name:			     main.c
** Last modified Date:          
** Last Version:		   
** Descriptions:		   ʹ�õ�SDK�汾-SDK_15.2
**						
**----------------------------------------------------------------------------------------------------
** Created by:			
** Created date:		2019-8-24
** Version:			    1.0
** Descriptions:		SPI��ȡLIS3DH���ٶ�����
**---------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#include "app_uart.h"
#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

#include "nrf_drv_spi.h"
#include "lis3dh_drive.h"
/* ������Ҫ�õ�IK-52832DK�������е�ָʾ��D1��UART��ռ�õ�nRF52832������Դ����
P0.17�����������ָʾ��D1
P0.18�����������ָʾ��D2
P0.19�����������ָʾ��D3
P0.20�����������ָʾ��D4

P0.06�����������TXD
P0.08�����룺����RXD

��ʵ����Ҫ����LIS3DSHģ��
CS   :P0.25
CLK  :P0.22
MISO :P0.24
MOSI :P0.23
��Ҫ������ñ�̽�P0.17 P0.18 P0.19 P0.20 P0.06 P0.08
*/

#define UART_TX_BUF_SIZE 256       //���ڷ��ͻ����С���ֽ�����
#define UART_RX_BUF_SIZE 256       //���ڽ��ջ����С���ֽ�����


//�����¼��ص��������ú������ж��¼����Ͳ����д���
void uart_error_handle(app_uart_evt_t * p_event)
{
    //ͨѶ�����¼�
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    //FIFO�����¼�
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}
//���ڳ�ʼ�����ã�������115200bps���ر�Ӳ������
void uart_config(void)
{
  uint32_t err_code;
	
	//���崮��ͨѶ�������ýṹ�岢��ʼ��
  const app_uart_comm_params_t comm_params =
  {
    RX_PIN_NUMBER,//����uart��������
    TX_PIN_NUMBER,//����uart��������
    RTS_PIN_NUMBER,//����uart RTS���ţ�ע�����عرպ���Ȼ������RTS��CTS���ţ����ǲ�������
    CTS_PIN_NUMBER,//����uart CTS����
    APP_UART_FLOW_CONTROL_DISABLED,//�ر�uartӲ������
    false,//��ֹ��ż����
    NRF_UART_BAUDRATE_115200//uart����������Ϊ115200bps
  };
  //��ʼ�����ڣ�ע�ᴮ���¼��ص�����
  APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOWEST,
                         err_code);

  APP_ERROR_CHECK(err_code);		
}
/***************************************************************************
* ��  �� : main���� 
* ��  �� : �� 
* ����ֵ : int ����
**************************************************************************/
int main(void)
{
	//����һ���ṹ����������ڱ����LIS3DH�ж�ȡ�ļ��ٶ�����
	AxesRaw_t data;
	
	//������������LEDָʾ��D1 D2 D3 D4�����Žţ�������P0.13~P0.16Ϊ���������LED�ĳ�ʼ״̬����ΪϨ�� 
	bsp_board_init(BSP_INIT_LEDS); 
	//��ʼ������
  uart_config();
	
	//��ʼ��SPI��LIS3DH
	LIS3DH_Init();
 		
  while(true)
  {
		//��ȡLIS3DH���ٶ����ݣ���ȡ�ɹ���ͨ�����ڴ�ӡ������
		if(LIS3DH_GetAccAxesRaw(&data))
	  {
		  printf("X=%6d Y=%6d Z=%6d \r\n", data.AXIS_X, data.AXIS_Y, data.AXIS_Z);
			nrf_gpio_pin_toggle(LED_1);	
	  }
		//��ʱ300ms������۲��ȡ������
		nrf_delay_ms(300);	
  }
}
/********************************************END FILE**************************************/
