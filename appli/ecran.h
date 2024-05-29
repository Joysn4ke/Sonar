/*
 * ecran.h
 *
 *  Created on: 14 mars 2024
 *      Author: hippo
 */

#include "tft_ili9341/stm32f1_ili9341.h"
#include "stm32f1_xpt2046.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PI 3.14159265

#ifndef ECRAN_H_
#define ECRAN_H_

/* Screen Dimension */
#define SCREEN_WIDTH			0x0140			//320(10) Pixels
#define SCREEN_LENGTH			0x00F0			//240(10) Pixels

#define SCREEN_CENTER_X     (SCREEN_WIDTH / 2)   // Centre de l'ï¿½cran en x
#define SCREEN_CENTER_Y     (SCREEN_LENGTH / 2)  // Centre de l'ï¿½cran en y
/* Screen Dimension */


#define TEXT_HEIGHT_11_18		0x0012			//18(10) Pixels
#define TEXT_GAP_11_18			0x000A			//10(10) Pixels

#define TEXT_HEIGHT_7_10		0x000A			//10(10) Pixels
#define TEXT_GAP_7_10			0x0007			//7(10) Pixels

#define TEXT_LENGTH_CURRENT_STATE_7_10	0x0070			//16*7=112(10) Pixels

#define DISPLAY_REFRESH_INTERVAL 250			// Dï¿½clarer une variable pour stocker l'intervalle de rafraï¿½chissement de l'affichage (en millisecondes)

#define NB_TAB_ANGLES 		15

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

typedef struct {
    uint32_t time;
    uint16_t x;
    uint16_t y;
} TabTarget;


static uint16_t strLenghtState = 112;					//uint16_t strLenghtState = string_length("Current state : ", &Font_7x10);
static uint16_t strLenghtPosMot = 126; 					//uint16_t strLenghtPosMot = string_length("Position moteur : ", &Font_7x10);
static uint16_t strLenghtDistance = 74;				//uint16_t strLenghtDistance = string_length("Distance : ", &Font_7x10);
static uint16_t strLenghtScanEnv = 140;					//uint16_t strLenghtScanEnv = string_length("Scanne environnement", &Font_7x10);


static uint16_t xOrigin = 30 + 20 + TEXT_GAP_7_10;		//static uint16_t x = closeButton.centerX + closeButton.radius + TEXT_GAP_7_10;
static uint16_t yOrigin = 30 + TEXT_HEIGHT_7_10;			//static uint16_t y = closeButton.centerY + TEXT_HEIGHT_7_10;
static uint16_t yOrigin2 = 30 + TEXT_HEIGHT_7_10 * 2;
static uint16_t yOrigin4 = 30 + TEXT_HEIGHT_7_10 * 4;

static uint32_t last_display_time = 0;			// Dï¿½clarer une variable statique pour stocker le temps du dernier affichage

static uint16_t eraseX = 0;
static uint16_t eraseY = 0;


static TabTarget *array = NULL;
static size_t size = 0;
static size_t capacity = 10;

uint16_t pointsAngle[NB_TAB_ANGLES][2];
uint16_t pointsAngleText[NB_TAB_ANGLES][2];


//Formes écran
static Circle closeButton = {30, 30, 20};


void addEntry(TabTarget **array, size_t *size, size_t *capacity, uint32_t time, int16_t x, int16_t y);
void removeOldEntries(TabTarget *array, size_t *size, uint32_t currentTime);
void removeOldEntriesForced(TabTarget **array, size_t *size);

void Screen_init(void);

void printScreenCurrentStateInit(uint16_t x_pos, uint16_t y_pos);

void DrawCloseButton(void);
void DrawHalfCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void DrawMenu(void);
void HideMenu(void);

void PrintStateChoixMenu(void);
void PrintStateScan(void);
void PrintStatePause(void);

void PrintDistance(uint16_t distance);

void DrawSonar(int16_t centerX, int16_t centerY, int16_t radius, uint16_t color);
void HideSonar(int16_t centerX, int16_t centerY, int16_t radius, uint16_t color);

void DrawScanning(int16_t centerX, int16_t centerY, int16_t radius, uint16_t color, uint16_t position, uint16_t previousPosition);

void DrawTarget(int16_t centerX, int16_t centerY, int16_t radius, int16_t position);

bool_e scanning_enable(void);

bool_e isClickedOnRectangle(uint16_t click_x, uint16_t click_y, uint16_t rect_x1, uint16_t rect_y1, uint16_t rect_x2, uint16_t rect_y2);
bool_e isClickedOnCircle(uint16_t click_x, uint16_t click_y, uint16_t circle_x, uint16_t circle_y, uint16_t radius);
void isClicked(void);


uint16_t string_length(const char *str, FontDef_t *font);

#endif /* ECRAN_H_ */


