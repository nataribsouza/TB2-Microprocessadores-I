/**
 * @file display.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef DISPLAY_H
#define DISPLAY_H

/* Include */
#include <string.h>
#include "stm32f1xx_hal.h"
#include "stdbool.h"

/* Defines */
#define LCD_DISPLAY_NUM_COL 16
#define LCD_DISPLAY_NUM_ROW 2
#define LCD_I2C_ADDRESS (0x27 << 1) 
#define LCD_NIBBLE_SHIFT 4
#define LCD_BIT_EN (0x04)
#define LCD_BIT_BACKLIGHT (0x08)
#define LCD_BIT_RS_SEND_DATA (0b01)
#define LCD_BIT_RS_SEND_COMMAND (0b00)
#define LCD_ROW0 0
#define LCD_ROW1 1
#define LCD_BUFFER_LEN 17
#define LCD_ADDR_ROW0 0x00
#define LCD_ADDR_ROW1 0x40
#define LCD_CMD_CLEAR_DISPLAY 0x01
#define LCD_CMD_SET_MODE 0x02
#define LCD_CMD_SET_SIZE 0x28
#define LCD_CMD_SET_CURSOR 0x80
#define LCD_CMD_SET_CURSOR_MODE 0x0C
#define LCD_CMD_SET_CURSOR_INCREMENT 0x06
#define LCD_DELAY_SHOW_START_SCREE 3000
#define LAST_POS_4CHAR_STR 3
#define LAST_POS_2CHAR_STR 1
#define ASCII_NUM_0
#define ASCII_NUM_9
#define TIME_UPDATE_DISPLAY_MS 500

/* Screens display */
// Start 
#define LCD_SCREEN_START "   GMN-Motors   "


/* Enums */
typedef enum {
    ENUM_SCREEN_LOGIN,
    ENUM_SCREEN_MAIN_MENU_USER_ADM,
    ENUM_SCREEN_SLCT_BLOCK_AREA_USER_ADM,
    ENUM_SCREEN_SLCT_LEN_USER_ADM,
    ENUM_SCREEN_SLCT_HGT_USER_ADM,
    ENUM_SCREEN_SLCT_POS_X_USER_ADM,
    ENUM_SCREEN_SLCT_POS_Y_USER_ADM,
    ENUM_SCREEN_CHNG_HOUR_USER_ADM,
    ENUM_SCREEN_MAIN_MENU_USER_COM
} en_screen;

/* Structures */
typedef struct {
    bool update;
    char row0[LCD_DISPLAY_NUM_COL+1]; 
    char row1[LCD_DISPLAY_NUM_COL+1]; 
    en_screen screen_en;
}st_display;


/* Prototypes */
void init_display(void);
void display_update(st_display *display_st);
void display_nibble(uint8_t, uint8_t control);
void display_nibble_data(uint8_t nibble);
void display_nibble_command(uint8_t nibble);
void display_send_data(uint8_t data);
void display_send_command(uint8_t data);
void display_setCursor(uint8_t row, uint8_t col);
void display_print(const char *str);
void display_clear(void);
void display_set_line(st_display *display_st, uint8_t row,  const char *str);

#endif /* DISPLAY_H */