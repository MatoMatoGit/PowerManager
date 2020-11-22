
#include "rgb_led.h"

#include "gpio_rgb_led.h"

#include <stdlib.h>


static void IRgbLedColorSet(RgbLedColor_t color);

static void IRgbLedOff(void);

static RgbLedColor_t CurrentColor;

static RgbLedMode_t CurrentMode;

static uint8_t On;

/* Init RGB LED, start at color = OFF. */
int8_t RgbLedInit(void)
{
	int8_t res = RGB_LED_ERR;

	CurrentColor = RGB_LED_COLOR_RED;
	GpioRgbLedInit();
	RgbLedModeSet(RGB_LED_MODE_OFF);
	res = RGB_LED_OK;

	return res;
}

/* Set RGB LED color. */
void RgbLedColorSet(RgbLedColor_t color)
{
	RgbLedMode_t mode = RgbLedModeGet();
	CurrentColor = color;

	/* Only turn the LED on right away if the current
	 * mode is ON. */
	if(mode == RGB_LED_MODE_ON) {
		IRgbLedColorSet(color);
	}
}

/* Get current RGB LED color. */
RgbLedColor_t RgbLedColorGet(void)
{
	return CurrentColor;
}

int8_t RgbLedModeSet(RgbLedMode_t mode)
{
	int8_t res = RGB_LED_ERR;

	switch(mode) {
		default:
		case RGB_LED_MODE_OFF: {
			On = 0;
			IRgbLedOff();
			res = RGB_LED_OK;
			break;
		}

		case RGB_LED_MODE_ON: {
			On = 1;
			IRgbLedColorSet(CurrentColor);
			res = RGB_LED_OK;
			break;
		}
	}

	if(res == RGB_LED_OK) {
		CurrentMode = mode;
	}

	return res;
}

RgbLedMode_t RgbLedModeGet(void)
{
	return CurrentMode;
}

/***** Internal functions *****/

static void IRgbLedOff(void)
{
#ifdef RGB_LED_CONFIG_CONTROL_INVERTED
	GpioRgbLedRedStateSet(1);
	GpioRgbLedGreenStateSet(1);
	GpioRgbLedBlueStateSet(1);
#else
	GpioRgbLedRedStateSet(0);
	GpioRgbLedGreenStateSet(0);
	GpioRgbLedBlueStateSet(0);
#endif
}

static void IRgbLedColorSet(RgbLedColor_t color)
{
	switch (color) {
		default:
		case RGB_LED_COLOR_RED: {
#ifdef RGB_LED_CONFIG_CONTROL_INVERTED
			GpioRgbLedRedStateSet(0);
			GpioRgbLedGreenStateSet(1);
			GpioRgbLedBlueStateSet(1);
#else
			GpioRgbLedRedStateSet(1);
			GpioRgbLedGreenStateSet(0);
			GpioRgbLedBlueStateSet(0);
#endif
			break;
		}

		case RGB_LED_COLOR_GREEN: {
#ifdef RGB_LED_CONFIG_CONTROL_INVERTED
			GpioRgbLedRedStateSet(1);
			GpioRgbLedGreenStateSet(0);
			GpioRgbLedBlueStateSet(1);
#else
			GpioRgbLedRedStateSet(0);
			GpioRgbLedGreenStateSet(1);
			GpioRgbLedBlueStateSet(0);
#endif
			break;
		}

		case RGB_LED_COLOR_BLUE: {
#ifdef RGB_LED_CONFIG_CONTROL_INVERTED
			GpioRgbLedRedStateSet(1);
			GpioRgbLedGreenStateSet(1);
			GpioRgbLedBlueStateSet(0);
#else
			GpioRgbLedRedStateSet(0);
			GpioRgbLedGreenStateSet(0);
			GpioRgbLedBlueStateSet(1);
#endif
			break;
		}

		case RGB_LED_COLOR_VIOLET: {
#ifdef RGB_LED_CONFIG_CONTROL_INVERTED
			GpioRgbLedRedStateSet(0);
			GpioRgbLedGreenStateSet(1);
			GpioRgbLedBlueStateSet(0);
#else
			GpioRgbLedRedStateSet(1);
			GpioRgbLedGreenStateSet(0);
			GpioRgbLedBlueStateSet(1);
#endif
			break;
		}

		case RGB_LED_COLOR_YELLOW: {
#ifdef RGB_LED_CONFIG_CONTROL_INVERTED
			GpioRgbLedRedStateSet(0);
			GpioRgbLedGreenStateSet(0);
			GpioRgbLedBlueStateSet(1);
#else
			GpioRgbLedRedStateSet(1);
			GpioRgbLedGreenStateSet(1);
			GpioRgbLedBlueStateSet(0);
#endif
			break;
		}

		case RGB_LED_COLOR_AQUA: {
#ifdef RGB_LED_CONFIG_CONTROL_INVERTED
			GpioRgbLedRedStateSet(1);
			GpioRgbLedGreenStateSet(0);
			GpioRgbLedBlueStateSet(0);
#else
			GpioRgbLedRedStateSet(0);
			GpioRgbLedGreenStateSet(1);
			GpioRgbLedBlueStateSet(1);
#endif
			break;
		}

		case RGB_LED_COLOR_WHITE: {
#ifdef RGB_LED_CONFIG_CONTROL_INVERTED
			GpioRgbLedRedStateSet(0);
			GpioRgbLedGreenStateSet(0);
			GpioRgbLedBlueStateSet(0);
#else
			GpioRgbLedRedStateSet(1);
			GpioRgbLedGreenStateSet(1);
			GpioRgbLedBlueStateSet(1);
#endif
 			break;
		}
		
	}
}
