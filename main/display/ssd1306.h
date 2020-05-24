/*
 * ssd1306.h
 *
 * ssd1306 128x64 OLED display commands.
 * 
 */

#ifndef SSD1306_H
#define SSD1306_H

/*
 * Fundamental command table.
 */
#define SET_CONTRAST_CONTROL 0x81   // double byte command contrast level 1 to 256
#define ENTIRE_DISPLAY_ON_RESUME_TO_RAM 0xA4  // Set entire display ON; use contents of RAM
#define ENTIRE_DISPLAY_ON_IGNORE_RAM  // Set entire display ON; ignore RAM contents

#endif /*SSD1306_H*/
