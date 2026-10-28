#include "ssd1306.h"

void ssd1306SendCmd(uint8_t cmd) {

	i2cWriteRegister(SSD1306_ADDR, SSD1306_CMD, &cmd, 1);
}

void ssd1306SendData(uint8_t data) {

	i2cWriteRegister(SSD1306_ADDR, SSD1306_DATA, &data, 1);
}

void ssd1306Init(void) {

	for (volatile uint32_t i = 0; i < 100000; i++)
		//__NOP(); // small pause

	ssd1306SendCmd(0xAE); // display OFF (recommended)
	ssd1306SendCmd(0x20); // addressing mode...
	ssd1306SendCmd(0x00); // ...horizontal addressing mode (very important)
	ssd1306SendCmd(0xB0); // page start address = 0 (optional)
	ssd1306SendCmd(0xC8); // COM scan direction: remapped (important)
	ssd1306SendCmd(0x00); // low column = 0 (optional)
	ssd1306SendCmd(0x10); // high column = 0 (optional)
	ssd1306SendCmd(0x40); // start line = 0 (default value)
	ssd1306SendCmd(0x81); // set contrast...
	ssd1306SendCmd(0x7F); // ...contrast (default value)
	ssd1306SendCmd(0xA1); // segment remap (important)
	ssd1306SendCmd(0xA6); // normal display (optional)
	ssd1306SendCmd(0xA8); // number of COM lines...
	ssd1306SendCmd(0x3F); // ...multiplex ratio = 1/64 (very important)
	ssd1306SendCmd(0xA4); // output follows RAM (optional)
	ssd1306SendCmd(0xD3); // display offset...
	ssd1306SendCmd(0x00); // ... = 0 (optional)
	ssd1306SendCmd(0xD5); // internal oscillator set...
	ssd1306SendCmd(0x80); // ...clock divide / oscillator (recommended)
	ssd1306SendCmd(0xD9); // set precharge period...
	ssd1306SendCmd(0xF1); // ...typical for 128x64 display (recommended)
	ssd1306SendCmd(0xDA); // COM pins config...
	ssd1306SendCmd(0x12); // ...serial COM connection, no remap, typical for 128x64 (very important)
	ssd1306SendCmd(0xDB); // VCOMH deselect level...
	ssd1306SendCmd(0x40); // ...0x20 - default, 0x40 - better (optional)
	ssd1306SendCmd(0x8D); // internal voltage regulator ON...
	ssd1306SendCmd(0x14); // ...without this display will not operate on 3.3V (very important)
	ssd1306SendCmd(0xAF); // display ON (very important)

	ssd1306Clear();
}

void ssd1306SetCursor(uint8_t page, uint8_t column) {

	ssd1306SendCmd(0xB0 + (page & 0x07)); // page number
	ssd1306SendCmd(0x00 + (column & 0x0F)); // column LSB
	ssd1306SendCmd(0x10 + ((column >> 4) & 0x0F)); // column MSB
}

void ssd1306Clear(void) {

	for (uint8_t page = 0; page < SSD1306_PAGES; page++) {

		ssd1306SetCursor(page, 0);
		for (uint8_t col = 0; col < SSD1306_WIDTH; col++) {

			ssd1306SendData(0x00);
		}
	}
}

void ssd1306DrawChar(uint8_t page, uint8_t column, char c) {

	uint8_t index = (uint8_t) (c - 32);

	for (uint8_t i = 0; i < 5; i++) { // every symbol is 5 bytes wide - 5 columns

		ssd1306SetCursor(page, column + i);
		ssd1306SendData(fontTable[index][i]);
	}

	ssd1306SetCursor(page, column + 5); // empty columns between symbols
	ssd1306SendData(0x00);
}

void ssd1306DrawString(uint8_t page, uint8_t column, const char *str) {

	while (*str) {
		if (column > SSD1306_WIDTH - 6) {
			column = 0;
			page++;
			if (page >= SSD1306_PAGES)
				break; // display is over
		}
		ssd1306DrawChar(page, column, *str++);
		column += 6;
	}
}

void ssd1306DrawInt(uint8_t page, uint8_t column, int32_t num) {

	/* here should be instructions for drawing positive and negative integers */
}

void ssd1306DrawFloat(uint8_t page, uint8_t column, float num, uint8_t decimals) {

	/* here should be instructions for drawing positive and negative floats */
}

