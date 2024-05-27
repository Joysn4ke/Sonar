/*
 * ecran.c
 *
 *  Created on: 14 mars 2024
 *      Author: hippo
 */


#include "ecran.h"
#include "tft_ili9341/stm32f1_ili9341.h"

extern Circle closeButton;

int16_t pointsAngle[15][2];



bool_e isClickedOnRectangle(uint16_t click_x, uint16_t click_y, uint16_t rect_x1, uint16_t rect_y1, uint16_t rect_x2, uint16_t rect_y2) {
    // V�rifier si les coordonn�es du clic sont � l'int�rieur des limites du rectangle
    if ((click_x >= rect_x1 && click_x <= rect_x2) && (click_y >= rect_y1 && click_y <= rect_y2)) {
        return TRUE; // Le clic est � l'int�rieur du rectangle
    } else {
        return FALSE; // Le clic n'est pas � l'int�rieur du rectangle
    }
}



bool_e isClickedOnCircle(uint16_t click_x, uint16_t click_y, uint16_t circle_x, uint16_t circle_y, uint16_t radius) {
    float distance_squared = sqrt(pow(click_x - circle_x, 2) + pow(click_y - circle_y, 2));

    // V�rifier si la distance est inf�rieure ou �gale au rayon du cercle au carr�
    if (distance_squared <= (float)radius ) {
        return TRUE; // Le clic est � l'int�rieur du cercle
    } else {
        return FALSE; // Le clic n'est pas � l'int�rieur du cercle
    }
}


bool_e scanning_enable(void) {
	static uint16_t static_x,static_y;
	uint16_t x, y;

	uint16_t x1 = SCREEN_WIDTH/4;
	uint16_t y1 = SCREEN_LENGTH/4;
	uint16_t x2 = SCREEN_WIDTH - SCREEN_WIDTH/4;
	uint16_t y2 = SCREEN_LENGTH/4 + 25;

	uint16_t delta1 = 2;
	uint16_t delta2 = y2 - y1 + 15;

	if(XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
	{
		//ILI9341_DrawCircle(static_x, static_y, 15,ILI9341_COLOR_BLACK);
		//ILI9341_DrawCircle(x, y, 15, ILI9341_COLOR_BLUE);

		static_x = x;
		static_y = y;

		if(isClickedOnRectangle(static_x, static_y, x1, y1, x2, y2)) {
			return TRUE;
		}
		else{
			return FALSE;
		}
	}
	else{
		return FALSE;
	}
}




void Screen_init(void){
	ILI9341_Init();
	XPT2046_init();

	ILI9341_Fill(ILI9341_COLOR_BLACK);					 //Back Color
	ILI9341_Rotate(ILI9341_Orientation_Landscape_1);	 //Screen Orientation LandScape

	DrawCloseButton();

	DrawMenu();

	printScreenCurrentStateInit(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10, closeButton.centerY);
}


void printScreenCurrentStateInit(uint16_t x_pos, uint16_t y_pos) {
	ILI9341_Puts(x_pos, 								y_pos, "Current state : ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLACK);
	ILI9341_Puts(x_pos + TEXT_LENGTH_CURRENT_STATE_7_10, y_pos, "Init", &Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
}



void DrawCloseButton(void) {
    ILI9341_DrawFilledCircle(closeButton.centerX, closeButton.centerY, closeButton.radius, ILI9341_COLOR_RED);

    // Dessiner une croix blanche pour indiquer la fermeture
    // Calculer les coordonn�es pour centrer les lignes sur le centre du cercle
    int16_t line_length = 10; // Longueur de chaque ligne
    ILI9341_DrawLine(closeButton.centerX - line_length, closeButton.centerY - line_length, closeButton.centerX + line_length, closeButton.centerY + line_length, ILI9341_COLOR_WHITE);
    ILI9341_DrawLine(closeButton.centerX - line_length, closeButton.centerY + line_length, closeButton.centerX + line_length, closeButton.centerY - line_length, ILI9341_COLOR_WHITE);
}



void DrawMenu(void){
	// uint16_t x1 = 70;
	// uint16_t y1 = 70;
	// uint16_t x2 = 230;
	// uint16_t y2 = 110;

	uint16_t x1 = SCREEN_WIDTH/4;
	uint16_t y1 = SCREEN_LENGTH/4;
	uint16_t x2 = SCREEN_WIDTH - SCREEN_WIDTH/4;
	uint16_t y2 = SCREEN_LENGTH/4 + 25;

	uint16_t delta1 = 2;
	uint16_t delta2 = y2 - y1 + 15;

	uint16_t width = 0;
    uint16_t height = 0;
    
    ILI9341_GetStringSize("Scanning & Display", &Font_7x10, &width, &height);

	ILI9341_INT_Fill(x1, y1, x2, y2, ILI9341_COLOR_BLUE);
	ILI9341_INT_Fill(x1 + delta1, y1 + delta1, x2 - delta1, y2 - delta1, ILI9341_COLOR_GREEN);
	ILI9341_Puts(x1 + delta1 * 4, y1 + delta1 * 4, "Scanning & Display", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_GREEN);


	ILI9341_GetStringSize("Light Mode", &Font_7x10, &width, &height);

	ILI9341_INT_Fill(x1, y1 + delta2, x2, y2 + delta2, ILI9341_COLOR_BLUE);
	ILI9341_INT_Fill(x1 + delta1, y1 + delta1 + delta2, x2 - delta1, y2 - delta1 + delta2, ILI9341_COLOR_GREEN);
	ILI9341_Puts(x1 + delta1 * 4, y1 + delta1 * 4 + delta2, "Light Mode", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_GREEN);
}



void HideMenu(void){
	// uint16_t x1 = 70;
	// uint16_t y1 = 70;
	// uint16_t x2 = 230;
	// uint16_t y2 = 110;

	uint16_t x1 = SCREEN_WIDTH/4;
	uint16_t y1 = SCREEN_LENGTH/4;
	uint16_t x2 = SCREEN_WIDTH - SCREEN_WIDTH/4;
	uint16_t y2 = SCREEN_LENGTH/4 + 25;

	uint16_t delta1 = 2;
	uint16_t delta2 = y2 - y1 + 15;

	ILI9341_INT_Fill(x1, y1, x2, y2, ILI9341_COLOR_BLACK);
	ILI9341_INT_Fill(x1 + delta1, y1 + delta1, x2 - delta1, y2 - delta1, ILI9341_COLOR_BLACK);

	ILI9341_INT_Fill(x1, y1 + delta2, x2, y2 + delta2, ILI9341_COLOR_BLACK);
	ILI9341_INT_Fill(x1 + delta1, y1 + delta1 + delta2, x2 - delta1, y2 - delta1 + delta2, ILI9341_COLOR_BLACK);
}




/**
 * @brief  Draws half a circle on LCD
 * @param  x0: X coordinate of center circle point
 * @param  y0: Y coordinate of center circle point
 * @param  r: Circle radius
 * @param  color: Circle color
 * @retval None
 */
void DrawHalfCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    ILI9341_DrawPixel(x0, y0 + r, color);
    ILI9341_DrawPixel(x0, y0 - r, color);
    ILI9341_DrawPixel(x0 + r, y0, color);
    ILI9341_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        // ILI9341_DrawPixel(x0 + x, y0 + y, color);
        // ILI9341_DrawPixel(x0 - x, y0 + y, color);
        ILI9341_DrawPixel(x0 + x, y0 - y, color);
        ILI9341_DrawPixel(x0 - x, y0 - y, color);

        // ILI9341_DrawPixel(x0 + y, y0 + x, color);
        // ILI9341_DrawPixel(x0 - y, y0 + x, color);
        ILI9341_DrawPixel(x0 + y, y0 - x, color);
        ILI9341_DrawPixel(x0 - y, y0 - x, color);
    }
}




void DrawSonar(int16_t centerX, int16_t centerY, int16_t radius, uint16_t color) {
    // Dessiner le demi-cercle
    //for (int i = 0; i <= 180; i++) {
	for (int i = 15; i <= 165; i++) {
		int16_t x = centerX - round(radius / 1.75 * cos(M_PI * i / 180));
		int16_t y = centerY - round(radius / 1.75 * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		x = centerX - round(radius / 2.75 * cos(M_PI * i / 180));
		y = centerY - round(radius / 2.75 * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		x = centerX - round(radius / 1.25 * cos(M_PI * i / 180));
		y = centerY - round(radius / 1.25 * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

        x = centerX - round(radius * cos(M_PI * i / 180));
        y = centerY - round(radius * sin(M_PI * i / 180));
        ILI9341_DrawPixel(x, y, color);

        if(i % 15 == 0){
        	ILI9341_DrawLine(centerX, centerY, x, y, color);
			char angle[10];
			sprintf(angle, "%d'", i);

			// Adjust the position for the text to be outside the half-circle
			uint16_t text_x = x;
			uint16_t text_y = y;

			// Determine the position based on the angle
			if (i < 45) {
				text_x -= TEXT_GAP_7_10 * 3.1;        // Center horizontally
				text_y -= TEXT_HEIGHT_7_10 / 1.3;         // Move up to ensure it is outside the circle
			} else if (i < 90) {
				text_x -= TEXT_GAP_7_10 * 2;        // Center horizontally
				text_y -= TEXT_HEIGHT_7_10 * 1.5;   // Move up to ensure it is outside the circle
			} else if (i == 90) {
				text_x -= TEXT_GAP_7_10 / 2;        // Center horizontally
				text_y -= TEXT_HEIGHT_7_10 * 2;     // Move up
			} else if (i < 135) {
				text_x -= TEXT_GAP_7_10 / 2;        // Center horizontally
				text_y -= TEXT_HEIGHT_7_10 * 2;     // Move up
			} else {
				text_x += TEXT_GAP_7_10;        // Center horizontally
				text_y -= TEXT_HEIGHT_7_10;     // Move up to ensure it is outside the circle
			}

			/*
			if(color == ILI9341_COLOR_BLACK){
				ILI9341_Puts(text_x, text_y, angle, &Font_7x10, color, ILI9341_COLOR_WHITE);
			} else if (color == ILI9341_COLOR_WHITE) {
				ILI9341_Puts(text_x, text_y, angle, &Font_7x10, color, ILI9341_COLOR_BLACK);
			}
			*/

			ILI9341_Puts(text_x, text_y, angle, &Font_7x10, color, ILI9341_COLOR_BLACK);

			pointsAngle[i / 15][0] = text_x;
			pointsAngle[i / 15][1] = text_y;
        }
    }
}



void HideSonar(int16_t centerX, int16_t centerY, int16_t radius, uint16_t color) {
    // Dessiner le demi-cercle
	DrawHalfCircle(centerX, centerY, radius / 1.25, color);
	DrawHalfCircle(centerX, centerY, radius / 1.75, color);
	DrawHalfCircle(centerX, centerY, radius / 2.75, color);
	for (int i = 15; i <= 165; i++) {
		int x = centerX - round(radius / 1.75 * cos(M_PI * i / 180));
		int y = centerY - round(radius / 1.75 * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		x = centerX - round(radius / 2.75 * cos(M_PI * i / 180));
		y = centerY - round(radius / 2.75 * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		x = centerX - round(radius / 1.25 * cos(M_PI * i / 180));
		y = centerY - round(radius / 1.25 * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		x = centerX - round(radius * cos(M_PI * i / 180));
		y = centerY - round(radius * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		if(i % 15 == 0){
        	ILI9341_DrawLine(centerX, centerY, x, y, color);
        	char angle[10];
			sprintf(angle, "%d'", i);

			ILI9341_Puts(pointsAngle[i / 15][0], pointsAngle[i / 15][1], angle, &Font_7x10, color, color);
        }
    }
}

void DrawScanning(int16_t centerX, int16_t centerY, int16_t radius, uint16_t color, uint16_t position, uint16_t previousPosition) {
	int16_t tempX;
	int16_t tempY;


	tempX = centerX - round(radius * cos(M_PI * previousPosition / 180));
	tempY = centerY - round(radius * sin(M_PI * previousPosition / 180));
	ILI9341_DrawLine(centerX, centerY, tempX, tempY, ILI9341_COLOR_BLACK);

	if(position >= 15 && position <= 165){
		tempX = centerX - round(radius * cos(M_PI * position / 180));
		tempY = centerY - round(radius * sin(M_PI * position / 180));
		ILI9341_DrawLine(centerX, centerY, tempX, tempY, color);
	}

	for (int i = position - 2; i <= position + 2; i++) {
		int x = centerX - round(radius / 1.75 * cos(M_PI * i / 180));
		int y = centerY - round(radius / 1.75 * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		x = centerX - round(radius / 2.75 * cos(M_PI * i / 180));
		y = centerY - round(radius / 2.75 * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		x = centerX - round(radius / 1.25 * cos(M_PI * i / 180));
		y = centerY - round(radius / 1.25 * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		x = centerX - round(radius * cos(M_PI * i / 180));
		y = centerY - round(radius * sin(M_PI * i / 180));
		ILI9341_DrawPixel(x, y, color);

		if(i % 15 == 0){
			ILI9341_DrawLine(centerX, centerY, x, y, color);
		}
	}
}


void DrawStateChoixMenu(void){
	ILI9341_Puts(x, closeButton.centerY, "Current state : ", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	ILI9341_DrawFilledRectangle((uint16_t)(x + strLenghtState), closeButton.centerY, (uint16_t)(x + strLenghtState + strLenghtScanEnv), y, ILI9341_COLOR_BLACK);
	ILI9341_Puts((uint16_t)(x + strLenghtState), closeButton.centerY, "Choix menu", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_BLACK);
}


void DrawStateScan(void){
	ILI9341_Puts(x, closeButton.centerY, "Current state : ", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	ILI9341_DrawFilledRectangle((uint16_t)(x + strLenghtState), closeButton.centerY, (uint16_t)(x + strLenghtState + strLenghtScanEnv), y, ILI9341_COLOR_BLACK);
	ILI9341_Puts((uint16_t)(x + strLenghtState), closeButton.centerY, "Scanne environnement", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_BLACK);
}

uint16_t string_length(const char *str, FontDef_t *font) {
    uint16_t length = 0;
    while (*str != '\0') {
        length += font->FontWidth;  // Ajoute la largeur d'un caract�re
        str++;  				// Passe au caract�re suivant
    }
    return length;
}
