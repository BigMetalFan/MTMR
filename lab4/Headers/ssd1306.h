#ifndef SSD1306_H_
#define SSD1306_H_

#include "i2c.h"
#include "font5x7.h"

#define SSD1306_ADDR 0x3C

#define SSD1306_CMD 0x00
#define SSD1306_DATA 0x40

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_PAGES 8

void ssd1306Init(void);
void ssd1306SendCmd(uint8_t cmd);
void ssd1306SendData(uint8_t data);
void ssd1306SetCursor(uint8_t page, uint8_t column);
void ssd1306Clear(void);
void ssd1306DrawChar(uint8_t page, uint8_t column, char c);
void ssd1306DrawString(uint8_t page, uint8_t column, const char *str);
void ssd1306DrawInt(uint8_t page, uint8_t column, int32_t num);
void ssd1306DrawFloat(uint8_t page, uint8_t column, float num, uint8_t decimals);

#endif /* SSD1306_H_ */
