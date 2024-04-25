#if 0
typedef enum{
	COLOR_SENSOR_NONE,
	COLOR_SENSOR_RED,
	COLOR_SENSOR_GREEN,
	COLOR_SENSOR_BLUE,
	COLOR_SENSOR_WHITE,
	COLOR_SENSOR_BLACK,
	COLOR_SENSOR_NB
}color_e;


typedef struct{
	color_e color;
	bool_e received;
	Uint8 red;
	Uint8 green;
	Uint8 blue;
	Uint8 ambiant;
	Uint8 proximity;
}actExpander_colorSensor_s;

// Red
#define COLOR_SENSOR_RED__RED 75
#define COLOR_SENSOR_RED__GREEN 14
#define COLOR_SENSOR_RED__BLUE 25
#define COLOR_SENSOR_RED__RED_GREEN (absolute(COLOR_SENSOR_RED__RED - COLOR_SENSOR_RED__GREEN))
#define COLOR_SENSOR_RED__RED_BLUE (absolute(COLOR_SENSOR_RED__RED - COLOR_SENSOR_RED__BLUE))
#define COLOR_SENSOR_RED__GREEN_BLUE (absolute(COLOR_SENSOR_RED__GREEN - COLOR_SENSOR_RED__BLUE))

// Green
#define COLOR_SENSOR_GREEN__RED 13
#define COLOR_SENSOR_GREEN__GREEN 51
#define COLOR_SENSOR_GREEN__BLUE 43
#define COLOR_SENSOR_GREEN__RED_GREEN (absolute(COLOR_SENSOR_GREEN__RED - COLOR_SENSOR_GREEN__GREEN))
#define COLOR_SENSOR_GREEN__RED_BLUE (absolute(COLOR_SENSOR_GREEN__RED - COLOR_SENSOR_GREEN__BLUE))
#define COLOR_SENSOR_GREEN__GREEN_BLUE (absolute(COLOR_SENSOR_GREEN__GREEN - COLOR_SENSOR_GREEN__BLUE))

// Blue
#define COLOR_SENSOR_BLUE__RED 13
#define COLOR_SENSOR_BLUE__GREEN 38
#define COLOR_SENSOR_BLUE__BLUE 55
#define COLOR_SENSOR_BLUE__RED_GREEN (absolute(COLOR_SENSOR_BLUE__RED - COLOR_SENSOR_BLUE__GREEN))
#define COLOR_SENSOR_BLUE__RED_BLUE (absolute(COLOR_SENSOR_BLUE__RED - COLOR_SENSOR_BLUE__BLUE))
#define COLOR_SENSOR_BLUE__GREEN_BLUE (absolute(COLOR_SENSOR_BLUE__GREEN - COLOR_SENSOR_BLUE__BLUE))

// White
#define COLOR_SENSOR_WHITE__RED 45 //43
#define COLOR_SENSOR_WHITE__GREEN 32 //60
#define COLOR_SENSOR_WHITE__BLUE 38 //55
#define COLOR_SENSOR_WHITE__AMBIANT 255
#define COLOR_SENSOR_WHITE__RED_GREEN (absolute(COLOR_SENSOR_WHITE__RED - COLOR_SENSOR_WHITE__GREEN))
#define COLOR_SENSOR_WHITE__RED_BLUE (absolute(COLOR_SENSOR_WHITE__RED - COLOR_SENSOR_WHITE__BLUE))
#define COLOR_SENSOR_WHITE__GREEN_BLUE (absolute(COLOR_SENSOR_WHITE__GREEN - COLOR_SENSOR_WHITE__BLUE))

// Black
#define COLOR_SENSOR_BLACK__RED 76 //26
#define COLOR_SENSOR_BLACK__GREEN 21 //37
#define COLOR_SENSOR_BLACK__BLUE 32 //35
#define COLOR_SENSOR_BLACK__AMBIANT 150
#define COLOR_SENSOR_BLACK__RED_GREEN (absolute(COLOR_SENSOR_BLACK__RED - COLOR_SENSOR_BLACK__GREEN))
#define COLOR_SENSOR_BLACK__RED_BLUE (absolute(COLOR_SENSOR_BLACK__RED - COLOR_SENSOR_BLACK__BLUE))
#define COLOR_SENSOR_BLACK__GREEN_BLUE (absolute(COLOR_SENSOR_BLACK__GREEN - COLOR_SENSOR_BLACK__BLUE))

/**
 * Fonction de reconnaissance de couleurs à partir des composantes R, G et B fournies par le capteur couleur.
 * @param sensor_datas les données du capteur (composantes RGB)
 * @param search les couleurs à rechercher (i.e., les couleurs possibles) sous la forme d'un masque binaire
 */
static color_e analyse_color(volatile actExpander_colorSensor_s * sensor_datas, searchedColor_e search)
{
	Uint16 red, blue, green;
	Uint32 redFound, greenFound, blueFound,whiteFound,blackFound;
	color_e color;
	color = COLOR_SENSOR_NONE;
	if(sensor_datas->ambiant)
	{
		//Composantes RGB, ramenées en pourcentage de la mesure de lumière ambiante
		red = sensor_datas->red;
		green = sensor_datas->green;
		blue = sensor_datas->blue;

		// debug_printf("%d, %d, %d, %d\n", red, green, blue, sensors[i].ambiant);

		Sint32 colors_value[COLOR_SENSOR_NB] = {0};
		colorSensor_e colors_id[COLOR_SENSOR_NB] = {0};
		Uint8 nb = 0;

		if(search & SEARCH_COLOR_RED) {
			redFound = (absolute(red - COLOR_SENSOR_RED__RED) + absolute(green - COLOR_SENSOR_RED__GREEN) + absolute(blue - COLOR_SENSOR_RED__BLUE));
			colors_value[nb] = redFound;
			colors_id[nb] = COLOR_SENSOR_RED;
			nb++;
		}

		if(search & SEARCH_COLOR_GREEN) {
			greenFound = (absolute(red - COLOR_SENSOR_GREEN__RED) + absolute(green - COLOR_SENSOR_GREEN__GREEN) + absolute(blue - COLOR_SENSOR_GREEN__BLUE));
			colors_value[nb] = greenFound;
			colors_id[nb] = COLOR_SENSOR_GREEN;
			nb++;
			}
			
		if(search & SEARCH_COLOR_BLUE) {
			blueFound = (absolute(red - COLOR_SENSOR_BLUE__RED) + absolute(green - COLOR_SENSOR_BLUE__GREEN) + absolute(blue - COLOR_SENSOR_BLUE__BLUE));
			colors_value[nb] = blueFound;
			colors_id[nb] = COLOR_SENSOR_BLUE;
			nb++;
		}

		if(search & SEARCH_COLOR_WHITE) {
			if(sensor_datas->ambiant > 200){
				whiteFound = 0;
			}else{
				whiteFound = 255;
			}
			colors_value[nb] = whiteFound;
			colors_id[nb] = COLOR_SENSOR_WHITE;
			nb++;
		}
		if(search & SEARCH_COLOR_BLACK) {

			if(sensor_datas->ambiant <= 200){
				blackFound = 0;
			}else{
				blackFound = 255;
			}
			colors_value[nb] = blackFound;
			colors_id[nb] = COLOR_SENSOR_BLACK;
			nb++;
		}

		Uint8 indiceMin = searchMin(colors_value, nb);
		color = colors_id[indiceMin];
	}

	return color;
}

uint16_t searchMin(int32_t values[], uint16_t nbValue){
	uint8_t i, indiceMin = 0;
	for(i=0; i<nbValue; i++){
		if(values[i] < values[indiceMin]){
			indiceMin = i;
		}
	}
	return indiceMin;
}

#endif
