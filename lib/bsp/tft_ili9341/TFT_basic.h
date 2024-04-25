/*
 * TFT_basic.h
 *
 *  Created on: 6 oct. 2020
 *      Author: guill
 */

#ifndef ABSTRACT_TFT_TFT_BASIC_H_
	#define ABSTRACT_TFT_TFT_BASIC_H_

	// include
	#include "macro_types.h"
	#include "tft_ili9341/stm32f1_ili9341.h"

	// const
	#define MAX_NB_POINTS		8				// nombre de points maximum par objet
	#define TFT_HEIGHT			ILI9341_HEIGHT	// hauteur de l'écran
	#define	TFT_WIDTH			ILI9341_WIDTH	// largeur de l'écran

	// énumération publique
	typedef enum {
		COLOR_WHITE 	= ILI9341_COLOR_WHITE,
		COLOR_BLACK 	= ILI9341_COLOR_BLACK,
		COLOR_RED 		= ILI9341_COLOR_RED,
		COLOR_GREEN 	= ILI9341_COLOR_GREEN,
		COLOR_GREEN2 	= ILI9341_COLOR_GREEN2,
		COLOR_BLUE 		= ILI9341_COLOR_BLUE,
		COLOR_BLUE2 	= ILI9341_COLOR_BLUE2,
		COLOR_YELLOW 	= ILI9341_COLOR_YELLOW,
		COLOR_ORANGE 	= ILI9341_COLOR_ORANGE,
		COLOR_CYAN 		= ILI9341_COLOR_CYAN,
		COLOR_MAGENTA 	= ILI9341_COLOR_MAGENTA,
		COLOR_GRAY		= ILI9341_COLOR_GRAY,
		COLOR_BROWN		= ILI9341_COLOR_BROWN
	}TFT_color_e;

	// structure public
	typedef uint16_t position; // permet de changer rapidement le type des positions (notamment pour l'usingned)
	typedef struct {
		position	y;
		position	x;
	}pos_s;

	typedef struct {
		pos_s 		points[MAX_NB_POINTS];
		uint8_t		nb_points;
		TFT_color_e	color;
		bool_e		filled;
	}TFT_object_s;

	typedef enum {
		FONT_7x10,
		FONT_11x18,
		FONT_16x26
	}text_size_e;

	typedef enum{
		TFT_PORTRAIT_BAS	=	ILI9341_Orientation_Portrait_1,
		TFT_PORTRAIT_HAUT	=	ILI9341_Orientation_Portrait_2,
		TFT_LANDSCAPE_LEFT	=	ILI9341_Orientation_Landscape_2,
		TFT_LANDSCAPE_RIGTH	=	ILI9341_Orientation_Landscape_1
	}TFT_orientation_e;


	// prototype de fonction public
	/**
	 * @brief	initialise l'utilisation du TFT
	 * @param	orientation	the orientation of the screen (direction refered to the pins)
	 */
	void TFT_init(TFT_orientation_e orientation);

	/**
	 * @brief					efface l'écran avec la couleur choisie (attention l'opération est longue !)
	 * @param	background_color	la couleur qui sera appliquer au fond lors de l'effacement
	 */
	void TFT_clear(TFT_color_e background_color);

	/**
	 * @brief			dessine l'objet
	 * @param	objet	pointeur sur l'objet à dessiner
	 */
	void TFT_draw_object(TFT_object_s *object);

	/**
	 * @brief						nettoye l'objet
	 * @param	objet				l'objet à nettoyer
	 * @param	background_color	la couleur du fond
	 */
	void TFT_clean_object(TFT_object_s *object, TFT_color_e background_color);

	/**
	 * @brief	test les fonctions de dessins bas level
	 */
	void TFT_test_basic(void);

	/**
	 * @brief	test l'affichage des triangles remplis
	 * @param	lunch drawn
	 * @pre		call regularly
	 */
	void TFT_test_triangles(bool_e drawn);

#endif /* ABSTRACT_TFT_TFT_BASIC_H_ */
