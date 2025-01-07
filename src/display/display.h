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
#define LCD_CMD_CREAT_ESP_CHAR 0x40
#define LCD_CMD_SET_MODE 0x02
#define LCD_CMD_SET_SIZE 0x28
#define LCD_CMD_SET_CURSOR 0x80
#define LCD_CMD_SET_CURSOR_MODE 0x0C
#define LCD_CMD_SET_CURSOR_INCREMENT 0x06
#define LCD_DELAY_SHOW_START_SCREE 3000
#define LCD_CUSTOM_CHAR_GASOLINE_INDEX 0
#define LCD_CUSTOM_CHAR_TEMPERATURE_INDEX 1
#define LAST_POS_4CHAR_STR 3
#define LAST_POS_2CHAR_STR 1
#define TIME_UPDATE_SCREEN_INSTANT_SPEED_MS 500
#define TIM_UPDATE_SCREEN_AVERAGE_SPEED_MS 1000
#define TIME_UPDATE_SCREEN_ODOMETER_MS 1000
#define TIME_UPDATE_SCREEN_TEMPERATURE_MS 500
#define TIME_UPDATE_SCREEN_GASOLINE_LEVEL_MS 500
#define TIME_UPDATE_SCREEN_AUTONOMI_MS 2000
#define DISPLAY_INIT_TIME_MS 3000

/* Screens display */
// Start 
#define LCD_SCREEN_START "   GMN-Motors   "
// Instant speed
#define LCD_SCREEN_INSTANT_SPEED_ROW0 "< Vel. Instant >"
#define LCD_SCREEN_INSTANT_SPEED_ROW1 "%3d Km/h        "
// Average speed
#define LCD_SCREEN_AVERAGE_SPEED_ROW0 "< Vel.   media >"
#define LCD_SCREEN_AVERAGE_SPEED_ROW1 "%3d Km/h        "
// Odometer
#define LCD_SCREEN_ODOMETER_ROW0 "<  Hodometro   >"
#define LCD_SCREEN_ODOMETER_ROW1 "%5lu m       "
// Temperature
#define LCD_SCREEN_TEMPERATURE_ROW0 "< Temperatura  >"
#define LCD_SCREEN_TEMPERATURE_ROW1 "%2d \xDF""C %s"
// Gasoline level
#define LCD_SCREEN_GASOLINE_LEVEL_ROW0 "<   Gasolina   >"
#define LCD_SCREEN_GASOLINE_LEVEL_ROW1 "%2d L          "
// Autonomi
#define LCD_SCREEN_AUTONOMI_ROW0 "<   Autonomia  >"
#define LCD_SCREEN_AUTONOMI_ROW1 "%2d Km/L         "

/* Enums */
typedef enum {
    ENUM_SCREEN_INSTANT_SPEED,
    ENUM_SCREEN_AVERAGE_SPEED,
    ENUM_SCREEN_ODOMETER,
    ENUM_SCREEN_TEMPERATURE,
    ENUM_SCREEN_GASOLINE_LEVEL,
    ENUM_SCREEN_AUTONOMI
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
void display_nibble(uint8_t, uint8_t control);
void display_nibble_data(uint8_t nibble);
void display_nibble_command(uint8_t nibble);
void display_send_data(uint8_t data);
void display_send_command(uint8_t data);
void display_setCursor(uint8_t row, uint8_t col);
void display_print(const char *str);
void display_clear(void);
void display_set_line(st_display *display_st, uint8_t row,  const char *str);
void display_create_custom_chars(void);
void display_temp_alert(bool state);
void display_gasoline_alert(bool state);

#endif /* DISPLAY_H */