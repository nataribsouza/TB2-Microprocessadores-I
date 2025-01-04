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

void display_update(st_display *display_st) {
    uint32_t timer = HAL_GetTick();
    static uint32_t elased_time = TIME_UPDATE_DISPLAY_MS;

    // Update display at 2 FPS rate
    if(timer >= elased_time) {
        elased_time = timer + TIME_UPDATE_DISPLAY_MS;

        if(display_st->update) {
            display_st->update = false;

            display_clear();
            display_setCursor(0, 0);
            display_print(display_st->row0);
            display_setCursor(1, 0);
            display_print(display_st->row1);
        }
    }    
}