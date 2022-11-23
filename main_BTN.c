#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "soc/uart_struct.h"
#define ECHO_TEST_TXD  17
#define ECHO_TEST_RXD  16
#define PUSH_BUTTON_PIN  32
#define BUF_SIZE 1024
//an example of echo test without hardware flow control on UART2
static void echo_task()
{
	gpio_set_direction(PUSH_BUTTON_PIN, GPIO_MODE_INPUT);
    const int uart_num = UART_NUM_2;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    //Configure UART1 parameters
    uart_param_config(uart_num, &uart_config);
    //Set UART1 pins(TX: IO4, RX: I05)
    uart_set_pin(uart_num, ECHO_TEST_TXD, ECHO_TEST_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    //Install UART driver (we don't need an event queue here)
    //In this example we don't even use a buffer for sending data.
    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);
    int data = (int) malloc(BUF_SIZE);
    while(1)
    {
        //Write data back to UART
    	 if ( gpio_get_level(PUSH_BUTTON_PIN) == 1)
    	 {data=1;}
    	 else
    	 {data=0;}
    	vTaskDelay(1);
    	uart_write_bytes(uart_num, data, 1);
    }
}
void app_main()
{
    //A uart read/write example without event queue;
	xTaskCreate(echo_task, "uart_echo_task", 1024, NULL, 10, NULL);
}
