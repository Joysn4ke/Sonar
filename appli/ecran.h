/*
 * ecran.h
 *
 *  Created on: 14 mars 2024
 *      Author: hippo
 */

#include "tft_ili9341/stm32f1_ili9341.h"
#include "stm32f1_xpt2046.h"
#include <math.h>

#define PI 3.14159265

#ifndef ECRAN_H_
#define ECRAN_H_

/* Screen Dimension */
#define SCREEN_WIDTH			0x0140			//320(10) Pixels
#define SCREEN_LENGTH			0x00F0			//240(10) Pixels

#define SCREEN_CENTER_X     (SCREEN_WIDTH / 2)   // Centre de l'�cran en x
#define SCREEN_CENTER_Y     (SCREEN_LENGTH / 2)  // Centre de l'�cran en y
/* Screen Dimension */


#define TEXT_HEIGHT_11_18		0x0012			//18(10) Pixels
#define TEXT_GAP_11_18			0x000A			//10(10) Pixels

#define TEXT_HEIGHT_7_10		0x000A			//10(10) Pixels
#define TEXT_GAP_7_10			0x0007			//7(10) Pixels

#define TEXT_LENGTH_CURRENT_STATE_7_10	0x0070			//16*7=112(10) Pixels

#define DISPLAY_REFRESH_INTERVAL 250			// D�clarer une variable pour stocker l'intervalle de rafra�chissement de l'affichage (en millisecondes)

static uint32_t last_display_time = 0;			// D�clarer une variable statique pour stocker le temps du dernier affichage


typedef struct {
	uint16_t x1;
	uint16_t y1;
	uint16_t x2;
	uint16_t y2;
} Quadrilateral;

typedef struct {
	uint16_t centerX;
	uint16_t centerY;
	uint16_t radius;
} Circle;



//Formes �cran
static Circle closeButton = {30, 30, 20};



void Screen_init(void);


void printScreenCurrentStateInit(uint16_t x_pos, uint16_t y_pos);
// void printScreenCurrentState(uint16_t x_pos, uint16_t y_pos, uint16_t color);

void DrawCloseButton(void);
void DrawHalfCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void DrawMenu(void);
void HideMenu(void);

void DrawSonar(int16_t x0, int16_t y0, int16_t r, uint16_t color);



bool_e scanning_enable(void);

bool_e isClickedOnRectangle(uint16_t click_x, uint16_t click_y, uint16_t rect_x1, uint16_t rect_y1, uint16_t rect_x2, uint16_t rect_y2);
bool_e isClickedOnCircle(uint16_t click_x, uint16_t click_y, uint16_t circle_x, uint16_t circle_y, uint16_t radius);
void isClicked(void);


uint16_t string_length(const char *str, FontDef_t *font);

#endif /* ECRAN_H_ */


