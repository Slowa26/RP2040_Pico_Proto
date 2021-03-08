# Project: OLED

## Table of Contents
- [Project: OLED](#project-oled)
  - [Table of Contents](#table-of-contents)
- [1. Introduction](#1-introduction)
- [2. Getting Started](#2-getting-started)
- [3. Hardware Configuration](#3-hardware-configuration)
  - [Waveshare 1.5" 128x128 RGB OLED Module](#waveshare-15-128x128-rgb-oled-module)
  - [Raspberry Pi Pico](#raspberry-pi-pico)
- [4. Project Fundamentals](#4-project-fundamentals)
  - [Serial Peripheral Interface (SPI) For the Raspberry Pi Pico](#serial-peripheral-interface-spi-for-the-raspberry-pi-pico)
  - [OLED_GFX.c and ASCII_Font.h](#oled_gfxc-and-ascii_fonth)
  - [Electrical Considerations - Digital Communication Interfaces](#electrical-considerations---digital-communication-interfaces)
- [5. Authors](#5-authors)
- [6. Acknowledgements](#6-acknowledgements)
- [7. Lessons Learned](#7-lessons-learned)
# 1. Introduction
This project is an attempt to get the [Raspberry Pi Pico](https://www.amazon.com/gp/product/B08TN4VLN7/ref=as_li_tl?ie=UTF8&camp=1789&creative=9325&creativeASIN=B08TN4VLN7&linkCode=as2&tag=slowa26-20&linkId=ae42d2ee12203ada1915efe067bac877) to communicate with the [Waveshare 1.5" RGB OLED Display Module](https://www.amazon.com/gp/product/B07DBXMFSN/ref=as_li_tl?ie=UTF8&camp=1789&creative=9325&creativeASIN=B07DBXMFSN&linkCode=as2&tag=slowa26-20&linkId=0a307335585ef575b14555c0e9951a7b) [Amazon Affiliate Links].

The OLED Module Features:  
- 1.5" 128x128 resolution, 65k color display.
- 4 or 3-wire [Serial Peripheral Interface](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface) (SPI)
- SDD1351 OLED/PLED Segment/Common Driver with Controller

Waveshare provides development resources and a manual which comes with examples for the Raspberry PI/Arduino/STM32. The majority of this project involved porting the C++ code from Waveshare's driver library and example software to the C language, and integrating it with the [Raspberry Pi Pico's Offical C/C++ SDK](https://raspberrypi.github.io/pico-sdk-doxygen/).

Users interested in simply learning how to control the Waveshare OLED module should skip to [here](#serial-periphrial-interface-spi-for-the-raspberry-pi-pico).

# 2. Getting Started
This project was built in VS Code, and I've provided the .vscode configuration files that allow my workspace to build on Arch Linux and debug with a properly configured [Picoprobe](https://www.youtube.com/watch?v=0i2gLeBal9Y).

The software is written in c. To build the software, the user will have to install and configure the Pico's C/C++ toolchain.

Setting up the C/C++ toolchain can be cumbersome for someone doing it the first time. My advice is to follow the directions *explicitly* in the [Getting started with Raspberry Pi Pico Guide](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf). I've found that getting up and running in Linux was considerably easier than Windows.

Getting the picoprobe playing nicely with VS Code was also a learning experience. Again, follow the directions in the guide above. For Win10, I remember consulting [the following forum post](https://www.element14.com/community/community/raspberry-pi/blog/2021/02/07/debugging-the-raspberry-pi-pico-on-windows-10) from element14 when having issues with the IDE and debugger integration.

# 3. Hardware Configuration

## Waveshare 1.5" 128x128 RGB OLED Module

[User Manual](https://www.waveshare.com/w/upload/5/5b/1.5inch_RGB_OLED_Module_User_Manual_EN.pdf)

[SSD1351 Datasheet](https://www.newhavendisplay.com/app_notes/SSD1351.pdf)

View RP2040_Pico_Proto/_Prototype/Oled/_Docs/_EE/HardwareConfig.ods for Pico wiring instructions.

The important bits:
The OLED module supports 4 and 3-wire SPI interfaces. 

- From the factory, the module is configured for 4-wire SPI. A surface mounted resistor will need to be moved to enable 3-wire SPI. This project's source code is configured for 4-wire communication, however, 3-wire communication exists within the code but has been ifdef-ed out and hasn't been tested.

## Raspberry Pi Pico
[Datasheet](https://datasheets.raspberrypi.org/pico/pico-datasheet.pdf)

[Getting Started Documentation](https://www.raspberrypi.org/documentation/rp2040/getting-started/)

I'm interfacing my Pico with a [Picoprobe](https://www.youtube.com/watch?v=0i2gLeBal9Y) for on-target debugging. My prototype software utilizes UART0 to pass serial data through the Picoprobe back to my development machine for debugging purposes. Interfacing with UART0 is not necessary for operation of the project's target hardware.

Additionally, HardwareConfig.ods has Picoprobe wiring defined - users can ignore connections made to P1_SWCLK, P1_GND, and P1_SWDIO.

# 4. Project Fundamentals
## Serial Peripheral Interface (SPI) For the Raspberry Pi Pico

1. Define Port and Pins
```c
(RP2040_Pico_Proto/_Prototype/Oled/Oled.c)

#define SPI_PORT spi0
#define PIN_MISO 16 //NOT USED. 1-WAY COMMUNICATION WITH OLED
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
```
There are two available SPI channels [SPI0 and SPI1] on the Pico's RP2040 micro-controller. These two channels can be muxed to most of the RP204's GPIO pins. The Pico's Getting Started Guide has a nifty image of the development board's IO that show which SPI pins can be mapped to which GPIO pins:

![](https://www.raspberrypi.org/documentation/rp2040/getting-started/static/64b50c4316a7aefef66290dcdecda8be/Pico-R3-SDK11-Pinout.svg) 

```c
(RP2040_Pico_Proto/_Prototype/Oled/Oled.c)

#define PIN_DC   28
#define PIN_RST  27
```

PIN_DC and PIN_RST are extra digital outputs that the software will need to control to communicate to the Waveshare OLED module.

This project #defines these pins to make future configuration more verbose.

2. Configure SPI channel and GPIO Pins
```c
(RP2040_Pico_Proto/_Prototype/Oled/Oled.c)

  // SPI initialization. This example will use SPI at 4.5MHz.
  spi_init(SPI_PORT, 4500*1000); //4.5Mhz, (4500 * 1000)
  gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
  gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
  gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
```
Here, we utilize the Pico's SDK to initialize the SPI_PORT (spi0 channel) and configure it's clock to 4.5 Mhz. 

Calls to gpio_set_function tell the RP2040 micro-controller that these general purpose pins (MISO, SCK, and MOSI) need to function as SPI pins.

```c
(RP2040_Pico_Proto/_Prototype/Oled/Oled.c)

  // Chip select is active-low, so we'll initialize it to a driven-high state
  gpio_init(PIN_CS);
  gpio_set_dir(PIN_CS, GPIO_OUT);
  gpio_put(PIN_CS, 1);
```

Here, we configure PIN_CS to be a digital output and set it's current output state to HI. The CS input on the Waveshare module is *active-low*. This means the Waveshare responds in some kind of way when it sees this pin's output low (0). Since we're not trying to control anything at initialization, it's best to initialize the pin to a state that isn't commanding the Waveshare OLED module.

```c
(RP2040_Pico_Proto/_Prototype/Oled/Oled.c)

  gpio_init(PIN_DC);
  gpio_set_dir(PIN_DC, GPIO_OUT);
  gpio_put(PIN_DC, 0);
```

Similarly, we need to configure PIN_DC similarly to PIN_CS. PIN_DC does not have an inactive state. It's value, upon setting PIN_CS low, will determine whether the SDD1351 OLED/PLED Controller will write data it receives over the SPI bus to Memory, or to interpret that data as a command.

```c
(RP2040_Pico_Proto/_Prototype/Oled/Oled.c)

  gpio_init(PIN_RST);
  gpio_set_dir(PIN_RST, GPIO_OUT);
  gpio_put(PIN_RST, 1);
```
Finally, PIN_RST needs to be initialized the same way as PIN_CS. This signal is also *active-low*, and setting it HI (1) will reset the SDD1351 OLED/PLED Controller. Since we're not trying to reset the OLED controller at initialization, it's best to initialize the pin to a state that isn't commanding the Waveshare OLED module.

3. Write to the SPI Bus
   
The project writes to the OLED Module in two places, within OLED_Driver.c, but you'll notice the mechanics are the same; here is *Write_Command*:

```c
(RP2040_Pico_Proto/_Prototype/Oled/OLED_Drivers.c)

void Write_Command(uint8_t cmd)  {

  OLED_CS(LOW);

  //Chip Select Setup Time 20ns min
  //asm volatile("nop \n nop"); 

#if  INTERFACE_4WIRE_SPI

  OLED_DC(LOW);

  //Address Setup Time: 15ns min
  ////asm volatile("nop \n nop"); 
  spi_write_blocking(SPI_PORT, &cmd, 1);

  //Address Hold Time, Chip Select Hold Time: 42 ns min
  ////asm volatile("nop \n nop \n nop \n nop"); 
  
#elif INTERFACE_3WIRE_SPI

  uint8_t i;

  OLED_SCK(LOW);
  OLED_DIN(LOW);
  OLED_SCK(HIGH);

  for(i = 0x80; i > 0; i >>= 1) {

    OLED_SCK(LOW);
    
    if(cmd & i)
      OLED_DIN(HIGH);
    else
      OLED_DIN(LOW);
    
    OLED_SCK(HIGH);
  }

#endif

  OLED_CS(HIGH);
}
```

The software, sets the chip select pin LOW to tell the OLED/PLED driver that the host is about to address it.

Pin DC is set LOW or HI depending on whether we are writing data or a command to the OLED/PLED driver.

and finally,

```c
(RP2040_Pico_Proto/_Prototype/Oled/OLED_Drivers.c)

spi_write_blocking(SPI_PORT, &cmd, 1);
```

Write_Command() and Write_Address also shows us how to talk to the OLED module. The mechanics can be summarized like so:

- Setting PIN_CS high to select the device.
- Setting PIN_DC high or low (PIN_DC's state determines whether the SD1351 will issue a command, or write data to an address).
- Writing data to the bus with *spi_write_blocking*.
- Resetting PIN_CS (this isn't necessary in this project since we're only dealing with one device on the buss, but it's important to note).

The pico-sdk function call to [*spi_write_blocking*](https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__spi.html#ga541ec2a1eee4c7fee2134a8415f3b4c8).

Please note that the last parameter is length in bytes. Somehow I missed this, and spent several hours with a debugger trying to figure out what the RP2040 was doing.

## OLED_GFX.c and ASCII_Font.h
I've included the OLED_GFX.c file I've ported from Waveshare's provided Arduino library. This file contains a bunch of functions that allow users to draw points, lines, and shapes to the display with relative ease.

Additionally, ASCII_Font.h can be included by any source files that need to reference the data. I've defined the data arrays as ```static ``` as a simple way to include them in any source files that needs them. There are better ways to do this - but this is a prototype, after all.

## Electrical Considerations - Digital Communication Interfaces
For developers who have never communicated to a device over a digital communication interface, such as SPI, I2C, UART, etc. it's worth noting that there are electrical timing requirements that a host and client must follow. Some of these timing requirements are handled by the function calls to the SDK, but you'll notice this project controls PIN_CS and PIN_DC on it's own. The developer would have to be sure they are controlling these pins to the requirement's specifications.

You may have notice some in-line assembly commands that have been commented out within Write_Command() and Write_Address(). These were my attempts at implementing the [waveform timing specifications](https://www.newhavendisplay.com/app_notes/SSD1351.pdf) defined on page 52 of the SD1351's datasheet. 

However, since there is enough timing overhead within the Pico's C/C++ SDK's calls to write digital IO and write data to the SPI bus, these noop calls weren't required. One call to noop corresponds to two instruction cycles and 15ns of time. A cycle's period for any given device can be calculated:

$$
Cycle Period = (1/Clock Frequency)
$$

For thie pico, clocked at 133Mhz this means:

$$
Cycle Period = (1/1.33e8) = 7.51879699e-9
$$

$$
Cycle Period = 7.5ns
$$

Keep this in mind when developing with hardware that requires longer dwell/hold times than the SDK functions naturally provide due to timing overhead.

# 5. Authors

- Christopher Sowa - *Initial Work* - [Slowa26](https://github.com/Slowa26)

# 6. Acknowledgements 

- Waveshare: For providing example source code I was able to port over to the Pico.

# 7. Lessons Learned
This was my first stab at writing something for the Pico targeting external hardware. This is also the first time I've talked to a device over SPI, which turned out being much more intuitive than I expected thanks to the Pico's C/C++ SDK. 

Because I originally botched the call to spi_write_blocking, I was forced to learn more about the electrical characteristics of SPI communication and how to debug the communication signals with benchtop equipment. I feel like this new knowledge will pay dividends as I integrate more hardware in the future.

I'm also using this as an opportunity to learn Git, creating markdown files, and documenting simple personal projects since I didn't find the official Pico examples too helpful during my first stab at this. 

In hindsight, I think the examples are fine - but having no other references mentioning the exact hardware I was attempting to integrate was a bummer. I hope this helps someone else out there, since the pico doesn't have the extensive libraries offered by Arduino or other popular embedded platforms.