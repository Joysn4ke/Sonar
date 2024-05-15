/*
 * ecran.c
 *
 *  Created on: 14 mars 2024
 *      Author: hippo
 */


#include "ecran.h"
#include "tft_ili9341/stm32f1_ili9341.h"

extern Quadrilateral static_quadrilateral;
extern Circle closeButton;



void Screen_init(void){
	ILI9341_Init();
	XPT2046_init();

	ILI9341_Fill(ILI9341_COLOR_WHITE);					 //Back Color
	ILI9341_Rotate(ILI9341_Orientation_Landscape_1);	 //Screen Orientation LandScape

	drawCloseButton();

	DrawMenu();

	print_screen_current_state_init(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10, closeButton.centerY);
}


void drawCloseButton(void) {
    ILI9341_DrawFilledCircle(closeButton.centerX, closeButton.centerY, closeButton.radius, ILI9341_COLOR_RED);

    // Dessiner une croix blanche pour indiquer la fermeture
    // Calculer les coordonnées pour centrer les lignes sur le centre du cercle
    int16_t line_length = 10; // Longueur de chaque ligne
    ILI9341_DrawLine(closeButton.centerX - line_length, closeButton.centerY - line_length, closeButton.centerX + line_length, closeButton.centerY + line_length, ILI9341_COLOR_WHITE);
    ILI9341_DrawLine(closeButton.centerX - line_length, closeButton.centerY + line_length, closeButton.centerX + line_length, closeButton.centerY - line_length, ILI9341_COLOR_WHITE);
}




bool_e isClickedOnRectangle(uint16_t click_x, uint16_t click_y, uint16_t rect_x1, uint16_t rect_y1, uint16_t rect_x2, uint16_t rect_y2) {
    // Vérifier si les coordonnées du clic sont à l'intérieur des limites du rectangle
    if ((click_x >= rect_x1 && click_x <= rect_x2) && (click_y >= rect_y1 && click_y <= rect_y2)) {
        return TRUE; // Le clic est à l'intérieur du rectangle
    } else {
        return FALSE; // Le clic n'est pas à l'intérieur du rectangle
    }
}




bool_e isClickedOnCircle(uint16_t click_x, uint16_t click_y, uint16_t circle_x, uint16_t circle_y, uint16_t radius) {
    //int16_t distance_squared = (click_x - circle_x) * (click_x - circle_x) + (click_y - circle_y) * (click_y - circle_y);	// Calculer la distance entre les coordonnées du clic et le centre du cercle
    float distance_squared = sqrt(pow(click_x - circle_x, 2) + pow(click_y - circle_y, 2));

    // Vérifier si la distance est inférieure ou égale au rayon du cercle au carré
    //if (distance_squared <= radius * radius) {
    if (distance_squared <= (float)radius ) {
        return TRUE; // Le clic est à l'intérieur du cercle
    } else {
        return FALSE; // Le clic n'est pas à l'intérieur du cercle
    }
}




void DrawQuadrilateral(Quadrilateral quadrilatere_local, uint16_t color){
	ILI9341_DrawRectangle(quadrilatere_local.x1, quadrilatere_local.y1, quadrilatere_local.x2, quadrilatere_local.y2, color);
}

void DrawQuadrilateralWithText(Quadrilateral quadrilatere_local, const char *string){
	ILI9341_DrawRectangle(quadrilatere_local.x1, quadrilatere_local.y1, quadrilatere_local.x2, quadrilatere_local.y2, ILI9341_COLOR_BLACK);

}


void DrawMenu(void){
	//Code temporaire
	DrawQuadrilateral(static_quadrilateral, ILI9341_COLOR_BLACK);		// Dessiner le quadrilatère
	//Code temporaire
}






bool_e scanning_enable(void) {

	//printf("PRINT SCANNING ENABLE \n");

	static uint16_t static_x,static_y;
	uint16_t x, y;

	if(XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
	{
		//ILI9341_DrawCircle(static_x, static_y, 15,ILI9341_COLOR_WHITE);
		//ILI9341_DrawCircle(x, y, 15, ILI9341_COLOR_BLUE);


		ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10, closeButton.centerY, "Current state : ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		ILI9341_DrawFilledRectangle(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10), closeButton.centerY, closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10) + string_length("Scanne environnement", &Font_7x10), closeButton.centerY + TEXT_HEIGHT_7_10, ILI9341_COLOR_WHITE);
		ILI9341_Puts(closeButton.centerX + closeButton.radius + TEXT_GAP_7_10 + string_length("Current state : ", &Font_7x10), closeButton.centerY, "Clicked", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

						static_x = x;
		static_y = y;


		if(isClickedOnRectangle(static_x, static_y, static_quadrilateral.x1, static_quadrilateral.y1, static_quadrilateral.x2, static_quadrilateral.y2)) {
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




void print_screen_current_state_init(uint16_t x_pos, uint16_t y_pos) {
	ILI9341_Puts(x_pos, 								y_pos, "Current state : ", &Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
	ILI9341_Puts(x_pos + TEXT_LENGTH_CURRENT_STATE_7_10, y_pos, "None", &Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
}



uint16_t string_length(const char *str, FontDef_t *font) {
    uint16_t length = 0;
    while (*str != '\0') {
        length += font->FontWidth;  // Ajoute la largeur d'un caractère
        str++;  				// Passe au caractère suivant
    }
    return length;
}
