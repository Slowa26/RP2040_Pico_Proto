#ifndef __OLED_GFX_GFX_H
#define __OLED_GFX_GFX_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "OLED_Driver.h"

typedef enum  {
  FONT_5X8 = 0,
  FONT_8X16
} FONT_SIZE;

/* I don't think there is any reason this needs to exist. Font_Size is the only
   object that is ever referenced.*/
typedef struct 
{
  uint8_t Row;
  uint8_t Column;
  FONT_SIZE Font_size;
} OLED;

// Public prototypes - Changing names since removing object orientation from c++ makes
// these names less verbose.
void OLED_GFX_Set_FontSize(FONT_SIZE size);

void OLED_GFX_Draw_Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

void OLED_GFX_Draw_Rect(int16_t x, int16_t y, int16_t w, int16_t h);
void OLED_GFX_Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

void OLED_GFX_Fill_Circle(int16_t x0, int16_t y0, int16_t r);
void OLED_GFX_Draw_Circle(int16_t x0, int16_t y0, int16_t r);

void OLED_GFX_Draw_RoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
void OLED_GFX_Draw_Triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

void OLED_GFX_print_String(uint8_t x, uint8_t y, const uint8_t *text, FONT_SIZE size);
void OLED_GFX_Display_String_5x8(uint8_t x, uint8_t y, const uint8_t *text);
void OLED_GFX_Display_String_8x16(uint8_t x, uint8_t y, const uint8_t *text);

#endif

