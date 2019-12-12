/*
 * data_structures.c
 *
 *  Created on: Dec 10, 2019
 *      Author: zero
 */
#include "snake.h"

bean *b;
position *stone;
snake *snk;
uint8_t i;

/* Initializing the snake with specific length and color */
void snake_init(uint16_t init_len, uint16_t color) {
	snk = (snake *) malloc(sizeof(snake));
	snk->color = color;
	snk->head = snk->tail = NULL;
	snk->length = 0;
	direction *snk_dir = (direction *) malloc(sizeof(direction));
	snk_dir->ver = -1;
	snk_dir->hor = 0;
	snk->dir = snk_dir;

	while (snk->length <= init_len) {
		if (snk->head == NULL) {
			occupied_grid *original_grid = (occupied_grid*) malloc(
					sizeof(occupied_grid));
			position *original_pos = (position*) malloc(sizeof(position));
			original_pos->x = VERTICAL_GRID_NUMBER / 2;
			original_pos->y = HORIZONTAL_GRID_NUMBER / 2;
			original_grid->pos = original_pos;
			original_grid->prev = original_grid->next = NULL;
			snk->head = snk->tail = original_grid;
		} else {
			direction *dir = (direction *) malloc(sizeof(direction));
			dir->hor = 1;
			dir->ver = 0;
			snake_tail_lengthen(dir);
		}
		snk->length++;
	}
}

/* Add a segment of snake body in front of the head if the snake can eat the bean */
void snake_forward() {
	occupied_grid *next_head = (occupied_grid *) malloc(sizeof(occupied_grid));
	next_head->pos->x = snk->head->pos->x + snk->dir->ver;
	next_head->pos->y = snk->head->pos->y + snk->dir->hor;
	if (next_head->pos->x == b->pos->x && next_head->pos->y == b->pos->y) {
		next_head->next = snk->head;
		snk->head = next_head;
		snk->color = b->color;
		snk->length++;
		score++;
	}
}

/* Lengthen the snake by adding a segment to the tail */
void snake_tail_lengthen(direction *dir) {
	occupied_grid *temp = (occupied_grid*) malloc(sizeof(occupied_grid));
	position *temp_pos = (position*) malloc(sizeof(position));
	temp_pos->x = snk->tail->pos->x + dir->hor;
	temp_pos->y = snk->tail->pos->y + dir->ver;
	temp->pos = temp_pos;
	snk->tail->next = temp;
	temp->prev = snk->tail;
	temp->next = NULL;
	snk->tail = temp;
}

/* Judge if the snake bites itself */
uint8_t bite_self() {
	occupied_grid *body;
	body = snk->head->next;
	while (body != NULL) {
		if (snk->head->pos->x == body->pos->x
				&& snk->head->pos->y == body->pos->y) {
			end_game = 1;
			return 1;
		}
		body = body->next;
	}
	return 0;
}

/* Judge if the snake hits the wall */
uint8_t hit_wall() {
	position *head_pos = snk->head->pos;
	return head_pos->x < 0 || head_pos->x >= VERTICAL_GRID_NUMBER
			|| head_pos->y < 0 || head_pos->y >= HORIZONTAL_GRID_NUMBER;
}

/* Judge if the snake hits the stone */
uint8_t hit_stone() {
	position *head_pos = snk->head->pos;
	return head_pos->x == stone->x && head_pos->y == stone->y;
}

/* Get a random position out of snake's body */
position* random_pos() {
	position * temp_pos = (position *) malloc(sizeof(position));
	uint16_t x, y;
	occupied_grid *body;
	do {
		srand((unsigned) time(NULL));
		x = rand() % VERTICAL_GRID_NUMBER;
		y = rand() % HORIZONTAL_GRID_NUMBER;
		body = snk->head;
		while (body != NULL) {
			if (body->pos->x == x && body->pos->y == y) {
				break;
			}
			body = body->next;
		}
		if (b != NULL && stone != NULL && \
		    x != b->pos->x && x != stone->x && y != b->pos->y && y != stone->y) {
			break;
		}else if (b != NULL && stone == NULL && x != b->pos->x && y != b->pos->y){
			break;	
		}else if (b == NULL && stone != NULL && x != stone->x && y != stone->y){
			break;	
		}else if (b == NULL && stone == NULL){
			break;	
		}
		
	} while (1);
	temp_pos->x = x;
	temp_pos->y = y;
	return temp_pos;
}

/* Generate a bean */
void generate_bean() {
	uint8_t color_idx;
	position * temp_pos = random_pos();
	b->pos = temp_pos;
	srand((unsigned) time(NULL));
	color_idx = rand() % 14 + 1;	// color_idx is in [1,14]
	switch (color_idx) {
	case 1:
		b->color = WHITE;
		break;
	case 2:
		b->color = BLACK;
		break;
	case 3:
		b->color = BLUE;
		break;
	case 4:
		b->color = BRED;
		break;
	case 5:
		b->color = GRED;
		break;
	case 6:
		b->color = GBLUE;
		break;
	case 7:
		b->color = RED;
		break;
	case 8:
		b->color = MAGENTA;
		break;
	case 9:
		b->color = CYAN;
		break;
	case 10:
		b->color = YELLOW;
		break;
	case 11:
		b->color = BROWN;
		break;
	case 12:
		b->color = BRRED;
		break;
	case 13:
		b->color = GRAY;
		break;
	default:
		break;
	}
}

/* Generate a stone */
void generate_stone() {
	stone = random_pos();
}

/* Get the signal of keyboard */
uint8_t get_key();

/* Move snake's body */
void move();

/* Draw the back ground */
void draw_background(){
	LCD_Clear(0X2722);
	BACK_COLOR = LIGHTBLUE;
	LCD_Fill(0,280,240,320,LIGHTBLUE);

	POINT_COLOR=BLACK;
	LCD_DrawRectangle(0,320,240,280);
}

/* Draw the score */
void draw_score() {
	char num[8];
	char *content = "SCORE: ";
	itoa(score, num, 10);
	strcat(content, num);
	POINT_COLOR = RED;
	LCD_ShowString(20, 280, 200, 40, 24, (uint8_t*) content);
}

/* Draw one grid of snake body */
void draw_snake_body(occupied_grid *og){
	POINT_COLOR = snk->color;
	LCD_Fill(10*(og->pos->x),10*(og->pos->y),10*(og->pos->x)+10,10*(og->pos->y)+10,POINT_COLOR);
	 // LCD_Draw_Circle(10*(og->pos->x)+5,10*(og->pos->y)+5,5);
};

/* Draw the snake */
void draw_snake(){
	occupied_grid *tmp=snk->head;
	while(tmp->next){
	draw_snake_body(tmp);
	tmp=tmp->next;}

}


/* Draw the bean */
void draw_bean(){
	LCD_Draw_Circle(10*(b->pos->x)+5,10*(b->pos->y)+5,5);
	LCD_Draw_Circle(10*(b->pos->x)+5,10*(b->pos->y)+5,4);
	LCD_Draw_Circle(10*(b->pos->x)+5,10*(b->pos->y)+5,3);
	LCD_Draw_Circle(10*(b->pos->x)+5,10*(b->pos->y)+5,2);
	LCD_Draw_Circle(10*(b->pos->x)+5,10*(b->pos->y)+5,1);
}

/* Draw the stone */
void draw_stone(){

	LCD_Fill(10*(stone->x),10*(stone->y),10*(stone->x)+10,10*(stone->y)+10,BROWN);
	POINT_COLOR= BLACK;
	LCD_DrawRectangle(10*(stone->x),10*(stone->y),10*(stone->x)+10,10*(stone->y)+10);
}



/* Launch game */
void launch(){
	snake_init(3, GREEN);
	//generate_bean();
	generate_stone();
	draw_background();
	draw_score();
	draw_snake();
	draw_bean();
	draw_stone();
}

