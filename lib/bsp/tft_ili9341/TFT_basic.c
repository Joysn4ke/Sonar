/**
 * @file	TFT_basic.c
 * @brief	définition des fonctions de dessin généraliser
 * @author	Théo GUILLEMAUD
 */
#include "TFT_basic.h"


// const
typedef struct {
	double	m;
	int16_t p;
	bool_e no_x;	// true si l'equation est du type x=c
}droite_s;


// prototype de fonction privé
/**
 * @brief			dessine un triangle rempli
 * @param	point_1	premier point
 * @param	point_2	second point
 * @param	point_3	troisième point
 */
static void PRIVATE_TFT_draw_filled_triangle(pos_s *point_1, pos_s *point_2, pos_s *point_3, TFT_color_e color);

/**
 * @brief			dessine l'objet avec des lignes
 * @param object	l'objet à dessiner
 */
static void PRIVATE_TFT_draw_object_line(TFT_object_s *object);

/**
 * @brief			dessine l'objet avec des lignes
 * @param object	l'objet à dessiner
 */
static void PRIVATE_TFT_draw_object_fill(TFT_object_s *object);

/**
 * @brief	creer une equation de droite
 * @param	A	le point A
 * @param	B	le point B
 */
static droite_s PRIVATE_TFT_find_straigth_line(pos_s *A, pos_s *B);

/**
 * @brief	trouve le point milieu entre les 3 points selon le x
 */
static void PRIVATE_TFT_find_middle(pos_s *A, pos_s *B, pos_s *C, pos_s** middle, pos_s** left, pos_s** rigth);

/***
 * @brief	trouve la droite où ne passe pas le sommet
 */
static droite_s* PRIVATE_TFT_find_base(droite_s *d1, droite_s *d2, droite_s *d3, pos_s *sommet);

/**
 * @breif	trouve la droite qui ne passe pas par le sommet
 */
static droite_s* PRIVATE_TFT_find_base(droite_s *d1, droite_s *d2, droite_s *d3, pos_s *sommet);

/**
 * @brief	calcul le y pour un x et une droite donné
 */
static position PRIVATE_TFT_compute_straigth(droite_s *d, position x);

// fonction
void TFT_init(TFT_orientation_e orientation){
	ILI9341_Init();
	ILI9341_Rotate((ILI9341_Orientation_t) orientation);
}

void TFT_clear(TFT_color_e background_color){
	ILI9341_Fill((uint16_t) background_color);
}

void TFT_draw_object(TFT_object_s *object){
	//init

	//code
	if( object->nb_points == 0){
		return;
	}else{
		if( object->filled ){
			PRIVATE_TFT_draw_object_fill(object);
		}else{
			PRIVATE_TFT_draw_object_line(object);
		}
	}
}

void TFT_clean_object(TFT_object_s *object, TFT_color_e background_color){
	//init
	TFT_object_s temp_object = *object;
	temp_object.filled = TRUE;
	temp_object.color = background_color;

	//code
	TFT_draw_object(&temp_object);
}

void TFT_test_basic(void){
	//init
	TFT_object_s object_1;
		object_1.color = COLOR_BLACK;
		object_1.filled = TRUE;
		object_1.points[0].x = 10;
		object_1.points[0].y = 10;
		object_1.points[1].x = 120;
		object_1.points[1].y = 20;
		object_1.points[2].x = 240;
		object_1.points[2].y = 140;
		object_1.points[3].x = 80;
		object_1.points[3].y = 200;
		object_1.points[4].x = 20;
		object_1.points[4].y = 120;
		object_1.nb_points = 5;

	TFT_object_s object_2;
		object_2.color = COLOR_BLUE;
		object_2.filled = FALSE;
		object_2.points[0].x = 180;
		object_2.points[0].y = 30;
		object_2.points[1].x = 300;
		object_2.points[1].y = 10;
		object_2.points[2].x = 200;
		object_2.points[2].y = 200;
		object_2.points[3].x = 150;
		object_2.points[3].y = 220;
		object_2.points[4].x = 50;
		object_2.points[4].y = 170;
		object_2.nb_points = 5;

	TFT_object_s line_1;
		line_1.color = COLOR_BLACK;
		line_1.filled= FALSE;
		line_1.nb_points = 2;
		line_1.points[0] = (pos_s){0, 0};
		line_1.points[1].y = ILI9341_WIDTH;
		line_1.points[1].x = ILI9341_HEIGHT;

	TFT_object_s line_2;
		line_2.color = COLOR_BLACK;
		line_2.filled= FALSE;
		line_2.nb_points = 2;
		line_2.points[0].y = 0;
		line_2.points[0].x = ILI9341_HEIGHT;
		line_2.points[1].y = ILI9341_WIDTH;
		line_2.points[1].x = 0;

	//code
	TFT_draw_object(&object_1);
	TFT_draw_object(&object_2);
	TFT_draw_object(&line_1);
	TFT_draw_object(&line_2);

}
/*
void TFT_test_triangles(bool_e draw){
	static bool_e init = FALSE;
	static TFT_object_s triangle;
	static TFT_object_s point;

	static const uint8_t size_square = 5;

	if(!init){
		init = TRUE;
	//	RAND_init();

		triangle.color = COLOR_BLACK;
		triangle.filled = TRUE;
		triangle.nb_points = 3;

		point.color = COLOR_BLUE;
		point.filled = TRUE;
		point.nb_points = 4;
	}

	// catch event
	//RAND_catch_event();

	if(draw){
		TFT_clear(COLOR_WHITE);

		pos_s A = (pos_s){RAND_get()%TFT_WIDTH, RAND_get()%TFT_HEIGHT};
		pos_s B = (pos_s){RAND_get()%TFT_WIDTH, RAND_get()%TFT_HEIGHT};
		pos_s C = (pos_s){RAND_get()%TFT_WIDTH, RAND_get()%TFT_HEIGHT};

		// triangle
		triangle.points[0] = A;
		triangle.points[1] = B;
		triangle.points[2] = C;

		// point bleu
		pos_s *points[] = {&A, &B, &C};

		for(uint8_t i = 0; i<3; i++){
			position y_moins = (points[i]->y - size_square < 0)? 0 : points[i]->y - size_square;
			position x_moins = (points[i]->x - size_square < 0)? 0 : points[i]->x - size_square;

			position y_plus = (points[i]->y + size_square > TFT_WIDTH)? TFT_WIDTH : points[i]->y + size_square;
			position x_plus = (points[i]->x + size_square > TFT_HEIGHT)? TFT_HEIGHT : points[i]->x + size_square;

			point.points[0].y = (position) y_moins;
			point.points[0].x = (position) x_moins;

			point.points[1].y = (position) y_moins;
			point.points[1].x = (position) x_plus;

			point.points[2].y = (position) y_plus;
			point.points[2].x = (position) x_plus;

			point.points[3].y = (position) y_plus;
			point.points[3].x = (position) x_moins;
//
			TFT_draw_object(&point);

//			ILI9341_DrawFilledRectangle(
//				(points[i]->x-size_square < 0)?0:points[i]->x-size_square,
//				(points[i]->y-size_square < 0)?0:points[i]->y-size_square,
//				(points[i]->x+size_square >= TFT_HEIGHT)?TFT_HEIGHT:points[i]->x+size_square,
//				(points[i]->y+size_square >= TFT_WIDTH)? TFT_WIDTH: points[i]->y+size_square,
//				ILI9341_COLOR_CYAN
//			);
		}

		TFT_draw_object(&triangle);

		char x_str[15];
		sprintf(x_str, "x:%.3d|%.3d|%.3d", A.x, B.x, C.x);
		ILI9341_Puts(0, 0, x_str, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

		char y_str[15];
		sprintf(y_str, "y:%.3d|%.3d|%.3d", A.y, B.y, C.y);
		ILI9341_Puts(0, 18, y_str, &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
	}
}
*/

void PRIVATE_TFT_draw_filled_triangle(pos_s *A, pos_s *B, pos_s *C, TFT_color_e color){
	// init
	droite_s dAB = PRIVATE_TFT_find_straigth_line(A, B);
	droite_s dBC = PRIVATE_TFT_find_straigth_line(B, C);
	droite_s dCA = PRIVATE_TFT_find_straigth_line(C, A);

	pos_s *point_milieu = NULL, *point_left = NULL, *point_rigth = NULL;
	PRIVATE_TFT_find_middle(A, B, C, &point_milieu, &point_left, &point_rigth);
	droite_s* base = PRIVATE_TFT_find_base(&dAB, &dBC, &dCA, point_milieu);
	droite_s* rigth = PRIVATE_TFT_find_base(&dAB, &dBC, &dCA, point_left); // on change de point de vue
	droite_s* left = PRIVATE_TFT_find_base(&dAB, &dBC, &dCA, point_rigth); // et on rechange de point de vue

	// creation d'une ligne temporaire
	TFT_object_s line;
	line.color = color;
	line.filled = FALSE;
	line.nb_points = 2;

	//code
		// left
	if(left->no_x){
		line.points[0] = *point_left;
		line.points[1] = *point_milieu;
		PRIVATE_TFT_draw_object_line(&line);
	}else{
		for(position i = point_left->x; i < point_milieu->x; i++){
			line.points[0] = (pos_s){PRIVATE_TFT_compute_straigth(left, i), i}; // droite left
			line.points[1] = (pos_s){PRIVATE_TFT_compute_straigth(base, i), i}; // droite base
			PRIVATE_TFT_draw_object_line(&line);
		}
	}
		// rigth and middle
	if(rigth->no_x){
		line.points[0] = *point_rigth;
		line.points[1] = *point_milieu;
		PRIVATE_TFT_draw_object_line(&line);
	}else{
		for(position i = point_milieu->x; i <= point_rigth->x; i++){
			line.points[0] = (pos_s){PRIVATE_TFT_compute_straigth(rigth, i), i}; // droite rigth
			line.points[1] = (pos_s){PRIVATE_TFT_compute_straigth(base, i), i}; // droite base
			PRIVATE_TFT_draw_object_line(&line);
		}
	}

	// finish with borders
	pos_s *points[3] = {A, B, C};
	for(uint8_t i =0; i<3; i++){
		line.points[0] = *points[i];
		line.points[1] = *points[(i+1)%3];
		PRIVATE_TFT_draw_object_line(&line);
	}
}

droite_s PRIVATE_TFT_find_straigth_line(pos_s *A, pos_s *B){
	// init
	droite_s droite;

	// code
	if(A->x == B->x){
		droite.no_x = TRUE;
		droite.m = 0;
		droite.p = 0;
	}else{
		droite.m = (double)(B->y - A->y)/(double)(B->x - A->x);		// m = (yB-yA)/(xB-xA)
		droite.p = (int16_t) (A->y - (droite.m * A->x));			// p = yA - m*xA
		droite.no_x = FALSE;
	}

	// end
	return droite;
}

position PRIVATE_TFT_compute_straigth(droite_s *d, position x){
	position temp = (position) (d->m * x + d->p);
	return temp;
}

void PRIVATE_TFT_find_middle(pos_s *A, pos_s *B, pos_s *C, pos_s** middle, pos_s** left, pos_s** rigth){
	// init
	position min = MIN(MIN(A->x, B->x), C->x);
	position max = MAX(MAX(A->x, B->x), C->x);

	pos_s *points[] = {A, B, C};

	// code
	for(int i=0; i<3; i++){
		if(points[i]->x == min && *left == NULL){
			*left = points[i];
		}else if(points[i]->x == max && *rigth == NULL){
			*rigth = points[i];
		}else{
			*middle = points[i];
		}

	}
}

droite_s* PRIVATE_TFT_find_base(droite_s *d1, droite_s *d2, droite_s *d3, pos_s *sommet){
	position y1 = PRIVATE_TFT_compute_straigth(d1, sommet->x);
	position y2 = PRIVATE_TFT_compute_straigth(d2, sommet->x);
	position y3 = PRIVATE_TFT_compute_straigth(d3, sommet->x);

	// check
	if(y1 == 0 && d1->no_x == TRUE && y2 != y3){
		y1 = sommet->y;
	}
	if(y2 == 0 && d2->no_x == TRUE && y1 != y3){
		y2 = sommet->y;
	}
	if(y3 == 0 && d3->no_x == TRUE && y1 != y2){
		y3 = sommet->y;
	}

	if(absolute(y2 - sommet->y) <= 1 && absolute(y3 - sommet->y) <= 1){
		return d1;
	}
	if(absolute(y1 - sommet->y) <= 1 && absolute(y3 - sommet->y) <= 1){
		return d2;
	}
	if(absolute(y1 - sommet->y) <= 1 && absolute(y2 - sommet->y) <= 1){
		return d3;
	}

	// là c'est la merde
	return NULL;
}

void PRIVATE_TFT_draw_object_line(TFT_object_s *object){
	// init

	//code
	if( object->nb_points < 2 ){
		ILI9341_DrawPixel((uint16_t) object->points[0].x, (uint16_t) object->points[0].y, object->color);
	}else{
		for(int i = 0; i < object->nb_points; i++){
//			ASSERT_T(i == object->nb_points, "L'indice a dépassé la taille du tableau de points d'objet");
			ILI9341_DrawLine(
					(uint16_t) object->points[i].x,
					(uint16_t) object->points[i].y,
					(uint16_t) object->points[(i+1)%object->nb_points].x,
					(uint16_t) object->points[(i+1)%object->nb_points].y,
					object->color
			);
		}
	}
}

void PRIVATE_TFT_draw_object_fill(TFT_object_s *object){
	// init

	//code
	if( object->nb_points < 3 ){
		PRIVATE_TFT_draw_object_line(object);
	}else{
		for(int i = 1; i < object->nb_points - 1; i++){
//			ASSERT_T(i == object->nb_points, "L'indice a dépassé la taille du tableau de points d'objet");
			PRIVATE_TFT_draw_filled_triangle(
					&object->points[0],
					&object->points[i],
					&object->points[i+1],
					object->color
			);
		}
	}
}
