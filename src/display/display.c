/* Includes */
#include "display.h"

/* I2C handle */
extern I2C_HandleTypeDef hi2c2;

/* Functions */

/**
 * @brief Init display LCD
 */
void init_display(void) {
    HAL_Delay(50); 

    // Basic configuration
    display_nibble_command(LCD_CMD_SET_MODE);
    display_send_command(LCD_CMD_SET_SIZE);
    display_send_command(LCD_CMD_SET_CURSOR_MODE);
    display_clear();
    HAL_Delay(2);
    display_send_command(LCD_CMD_SET_CURSOR_INCREMENT);

    // Creat custom characteres
    display_create_custom_chars();
}

/**
 * @brief Send data to display
 * 
 * @param data Data to send
 */
void display_send_data(uint8_t data) {
    // High part of command
    display_nibble_data(data >> 4);
    // Low part of command
    display_nibble_data(data & 0x0F);
}

/**
 * @brief Send a nibble of data
 * 
 * @param nibble High part of the byto to send
 */
void display_nibble_data(uint8_t nibble) {
    uint8_t data = (nibble << LCD_NIBBLE_SHIFT);

    // Turn backlight on and select data mode
    data |= LCD_BIT_BACKLIGHT;
    data |= LCD_BIT_RS_SEND_DATA;

    // Send data
    uint8_t steps[2] = {data | LCD_BIT_EN, data & ~LCD_BIT_EN};
    HAL_I2C_Master_Transmit(&hi2c2, LCD_I2C_ADDRESS, steps, 2, HAL_MAX_DELAY);
    HAL_Delay(1);
}

/**
 * @brief Send a command to display
 * 
 * @param cmd Command to send
 */
void display_send_command(uint8_t cmd) {
    // High part of command
    display_nibble_command(cmd >> 4);
    // Low part of command
    display_nibble_command(cmd & 0x0F);
}

/**
 * @brief Send a nibble of command
 * 
 * @param nibble High part of command
 */
void display_nibble_command(uint8_t nibble) {
    uint8_t data = (nibble << LCD_NIBBLE_SHIFT);
    data |= LCD_BIT_BACKLIGHT; // Backlight on

    // Send command
    uint8_t steps[2] = {data | LCD_BIT_EN, data & ~LCD_BIT_EN};
    HAL_I2C_Master_Transmit(&hi2c2, LCD_I2C_ADDRESS, steps, 2, HAL_MAX_DELAY);
    HAL_Delay(1);
}

/**
 * @brief Set cursor row and collum
 * 
 * @param row 
 * @param col 
 */
void display_setCursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0 ? LCD_ADDR_ROW0 : LCD_ADDR_ROW1) + col;
    display_send_command(LCD_CMD_SET_CURSOR | address);
}

/**
 * @brief Print a string on display
 * 
 * @param str 
 */
void display_print(const char *str) {
    while (*str) {
        display_send_data(*str++);
    }
}

/**
 * @brief Clear display
 */
void display_clear(void) {
    display_send_command(LCD_CMD_CLEAR_DISPLAY);
    HAL_Delay(2);
}

/**
 * @brief 
 * 
 * @param display_st 
 * @param row 
 * @param str 
 */
void display_set_line(st_display *display_st,  uint8_t row, const char *str) {
    switch (row){
    case LCD_ADDR_ROW0:
        strcpy(display_st->row0, str);
        break;

    case LCD_ADDR_ROW1:
        strcpy(display_st->row1, str);
        break;

    default:
        break;
    }

    display_st->update = true;    
}

/**
 * @brief Create custom characters in the LCD CGRAM
 */
void display_create_custom_chars(void) {
    uint8_t char1[8] = {
        0b11111,
        0b11111,
        0b10001,
        0b10111,
        0b10111,
        0b10001,
        0b11111,
        0b11111
    };
    
    uint8_t char2[8] = {
        0b11111,
        0b11111,
        0b10001,
        0b11011,
        0b11011,
        0b11011,
        0b11111,
        0b11111
    };
    
    // Set CGRAM address for first custom character
    display_send_command(LCD_CMD_CREAT_ESP_CHAR);
    for (int i = 0; i < 8; i++) {
        display_send_data(char1[i]);
    }
    
    // Set CGRAM address for second custom character
    display_send_command(LCD_CMD_CREAT_ESP_CHAR + 8);
    for (int i = 0; i < 8; i++) {
        display_send_data(char2[i]);
    }
}

/**
 * @brief Print temperature alert character
 * 
 * @param state 
 */
void display_temp_alert(bool state) {
    display_setCursor(1, 14);

    if(state) {
        display_send_data(LCD_CUSTOM_CHAR_TEMPERATURE_INDEX);
    } else {
        display_send_data(' ');
    }
}

/**
 * @brief Print gasoline alert character
 * 
 * @param state 
 */
void display_gasoline_alert(bool state) {
    display_setCursor(1, 15);

    if(state) {
        display_send_data(LCD_CUSTOM_CHAR_GASOLINE_INDEX);
    } else {
        display_send_data(' ');
    }
}