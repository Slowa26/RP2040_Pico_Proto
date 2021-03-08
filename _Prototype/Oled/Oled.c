#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "OLED_Driver.h"
#include "OLED_GFX.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16 //NOT USED. 1-WAY COMMUNICATION WITH OLED
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

#define PIN_DC   28
#define PIN_RST  27

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

void lcdTestPattern(void)
{
  uint32_t i,j;
  Set_Coordinate(0, 0);
  
  for(i=0;i<128;i++)  {
    for(j=0;j<128;j++)  {
      if(i<16)  {
        Set_Color(RED);
        Write_Data(color_byte[0]);
        Write_Data(color_byte[1]);
      }
      else if(i<32) {
        Set_Color(YELLOW);
        Write_Data(color_byte[0]);
        Write_Data(color_byte[1]);
      }
      else if(i<48) {
        Set_Color(GREEN);
        Write_Data(color_byte[0]);
        Write_Data(color_byte[1]);
      }
      else if(i<64) {
        Set_Color(CYAN);
        Write_Data(color_byte[0]);
        Write_Data(color_byte[1]);
      }
      else if(i<80) {
        Set_Color(BLUE);
        Write_Data(color_byte[0]);
        Write_Data(color_byte[1]);
      }
      else if(i<96) {
        Set_Color(MAGENTA);
        Write_Data(color_byte[0]);
        Write_Data(color_byte[1]);
      }
      else if(i<112)  {
        Set_Color(BLACK);
        Write_Data(color_byte[0]);
        Write_Data(color_byte[1]);
      }
      else {
        Set_Color(WHITE);
        Write_Data(color_byte[0]);
        Write_Data(color_byte[1]);
      }
    }
  }
}

int main()
{
  stdio_init_all();

  // SPI initialisation. This example will use SPI at 4.5MHz.
  spi_init(SPI_PORT, 4500*1000); //4.5Mhz, (4500 * 1000)
  gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
  gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
  gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
  
  // Chip select is active-low, so we'll initialise it to a driven-high state
  gpio_init(PIN_CS);
  gpio_set_dir(PIN_CS, GPIO_OUT);
  gpio_put(PIN_CS, 1);

  gpio_init(PIN_DC);
  gpio_set_dir(PIN_DC, GPIO_OUT);
  gpio_put(PIN_DC, 0);

  gpio_init(PIN_RST);
  gpio_set_dir(PIN_RST, GPIO_OUT);
  gpio_put(PIN_RST, 1);

  // Timer example code - This example fires off the callback after 2000ms
  //add_alarm_in_ms(2000, alarm_callback, NULL, false);

  sleep_ms(500);

  Device_Init();

  sleep_ms(500);

  Clear_Screen();

  sleep_ms(500);

  lcdTestPattern();

  puts("Hello, world!");

  return 0;
}