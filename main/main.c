#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <u8g2.h>

#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

#define PIN_CLK 14
#define PIN_MOSI 16
#define PIN_RESET 18
#define PIN_DC 27
#define PIN_CS 15
static char tag[] = "test_uc1601_12864";


void app_main() {

  u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
  u8g2_esp32_hal.bus.spi.clk = PIN_CLK;
  u8g2_esp32_hal.bus.spi.mosi = PIN_MOSI;
  u8g2_esp32_hal.bus.spi.cs = PIN_CS;
  u8g2_esp32_hal.dc = PIN_DC;
  u8g2_esp32_hal.reset = PIN_RESET;
  u8g2_esp32_hal_init(u8g2_esp32_hal);
  printf("hal complete\n");

  u8g2_t u8g2; // a structure which will contain all the data for one display
  u8g2_Setup_uc1601_128x64_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure

  u8g2_InitDisplay(&u8g2);  // send init sequence to the display, display is in
                            // sleep mode after this,

  u8g2_SetPowerSave(&u8g2, 0);  // wake up display

  xTaskCreate(task1, "task1", 2048, NULL, 12, NULL);

}

void task1(void){
  u8g2_ClearBuffer(&u8g2);
  u8g2_DrawBox(&u8g2, 10, 20, 20, 30);
  u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
  u8g2_DrawStr(&u8g2, 0, 9, "Hello World!");
  u8g2_SendBuffer(&u8g2);

  

  ESP_LOGD(tag, "All done!");

  vTaskDelete(NULL);
}