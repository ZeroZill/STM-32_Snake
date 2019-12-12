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

unsigned time_bean;
unsigned time_stone;
unsigned time_snake;

/* Initializing the snake with specific length and color */
void snake_init(int init_len, uint16_t color) {
	snake snk_t;
	snk = &snk_t;

	snk->color = color;
	snk->length = 0;
	snk->dir->ver = 0;
	snk->dir->hor = -1;

	while (snk->length < init_len) {
		if (snk->length == 0) {
			occupied_grid temp_og;
			position temp_p;
			occupied_grid *original_grid = &temp_og;
			position *original_pos = &temp_p;
			original_pos->x = HORIZONTAL_GRID_NUMBER / 2;
			original_pos->y = VERTICAL_GRID_NUMBER / 2;
			original_grid->pos = original_pos;
			snk->head = snk->tail = original_grid;
		} else {
			direction temp_d;
			direction *dir = &temp_d;
			dir->ver = 0;
			dir->hor = 1;
			snake_tail_lengthen(dir);
		}
		snk->length++;
	}
	time_snake = milisecond;
}

/* Add a segment of snake body in front of the head if the snake can eat the bean */
void snake_forward() {
	occupied_grid temp_og;
	occupied_grid *next_head = &temp_og;
	next_head->pos->x = (snk->head->pos->x + snk->dir->hor)
			% HORIZONTAL_GRID_NUMBER;
	next_head->pos->y = (snk->head->pos->y + snk->dir->ver)
			% VERTICAL_GRID_NUMBER;
//	if (next_head->pos->x >= HORIZONTAL_GRID_NUMBER){
//		next_head->pos->x = next_head->pos->x - HORIZONTAL_GRID_NUMBER;
//	}

	next_head->next = snk->head;
	snk->head = next_head;
	if (next_head->pos->x == b->pos->x && next_head->pos->y == b->pos->y) {
		snk->color = b->color;
		snk->length++;
		score++;
		time_interval = 25 * (40 - score / 3);
		green_blink = 1;
	} else {
		occupied_grid *temp = snk->tail->prev;
		snk->tail = snk->tail->prev;
		free(temp->pos);
		free(temp);
	}
}

/* Lengthen the snake by adding a segment to the tail */
void snake_tail_lengthen(direction *dir) {
	occupied_grid temp_og;
	position temp_p;
	occupied_grid *temp = &temp_og;
	position *temp_pos = &temp_p;
	temp_pos->x = snk->tail->pos->x + dir->hor;
	temp_pos->y = snk->tail->pos->y + dir->ver;
	temp->pos = temp_pos;
	snk->tail->next = temp;
	temp->prev = snk->tail;
	temp->next = NULL;
	snk->tail = temp;
}

/* Judge if the snake bites itself */
int bite_self() {
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
int hit_wall() {
	position *head_pos = snk->head->pos;
	return head_pos->x < 0 || head_pos->x >= HORIZONTAL_GRID_NUMBER
			|| head_pos->y < 0 || head_pos->y >= VERTICAL_GRID_NUMBER;
}

/* Judge if the snake hits the stone */
int hit_stone() {
	position *head_pos = snk->head->pos;
	return head_pos->x == stone->x && head_pos->y == stone->y;
}

/* Get a random position out of snake's body */
position* random_pos() {
	position temp_p;
	position * temp_pos = &temp_p;
	uint16_t x, y;
	occupied_grid *body;

	char msg[20];
	sprintf(msg, "%d\r\n", milisecond);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	srand(milisecond);
	do {
		x = rand() % HORIZONTAL_GRID_NUMBER;
		y = rand() % VERTICAL_GRID_NUMBER;
		body = snk->head;
		while (body != NULL) {
			if (body->pos->x == x && body->pos->y == y) {
				break;
			}
			body = body->next;
		}
		if (b != NULL && stone != NULL && x != b->pos->x && x != stone->x
				&& y != b->pos->y && y != stone->y) {
			break;
		} else if (b != NULL && stone == NULL && x != b->pos->x
				&& y != b->pos->y) {
			break;
		} else if (b == NULL && stone != NULL && x != stone->x
				&& y != stone->y) {
			break;
		} else if (b == NULL && stone == NULL) {
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
	color_idx = rand() % 13 + 1;	// color_idx is in [1,14]
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
	time_bean = milisecond;
}

/* Generate a stone */
void generate_stone() {
	stone = random_pos();
	time_stone = milisecond;
}

///* Get the signal of keyboard */
//uint8_t get_key() {
//	//TODO: to be implemented
//
//}

/* Move snake's body */
void move() {
	if (milisecond - time_snake > time_interval) {
		snake_forward();
		time_snake = milisecond;
	}
	if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET) {
		if (snk->dir->hor == 1) {
			snk->dir->hor = 0;
			snk->dir->ver = 1;
		} else if (snk->dir->hor == -1) {
			snk->dir->hor = 0;
			snk->dir->ver = -1;
		} else if (snk->dir->ver == 1) {
			snk->dir->hor = -1;
			snk->dir->ver = 0;
		} else if (snk->dir->ver == -1) {
			snk->dir->hor = 1;
			snk->dir->ver = 0;
		}

		snake_forward();
		time_snake = milisecond;
	}
	if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET) {
		if (snk->dir->hor == 1) {
			snk->dir->hor = 0;
			snk->dir->ver = -1;
		} else if (snk->dir->hor == -1) {
			snk->dir->hor = 0;
			snk->dir->ver = 1;
		} else if (snk->dir->ver == 1) {
			snk->dir->hor = 1;
			snk->dir->ver = 0;
		} else if (snk->dir->ver == -1) {
			snk->dir->hor = -1;
			snk->dir->ver = 0;
		}

		snake_forward();
		time_snake = milisecond;
	}

}

/* Draw the back ground */
void draw_background() {
	LCD_Clear(0X2722);
	BACK_COLOR = LIGHTBLUE;
	LCD_Fill(0, 280, 240, 320, LIGHTBLUE);

	POINT_COLOR = BLACK;
	LCD_DrawRectangle(0, 320, 240, 280);
}

/* Draw the score */
void draw_score() {
	char num[8];
	char *content = "SCORE: ";
	itoa(score, num, 10);
	POINT_COLOR = RED;
	LCD_ShowString(20, 290, 200, 30, 24, (uint8_t*) content);
	LCD_ShowString(120, 290, 200, 30, 24, (uint8_t*) num);
}

/* Draw one grid of snake body */
void draw_snake_body(occupied_grid *og) {
	POINT_COLOR = snk->color;
	LCD_Fill(10 * (og->pos->x), 10 * (og->pos->y), 10 * (og->pos->x) + 10,
			10 * (og->pos->y) + 10, POINT_COLOR);
}
;

/* Draw the snake */
void draw_snake() {
	occupied_grid *tmp = snk->head;

	while (tmp->next != NULL) {
		draw_snake_body(tmp);
		tmp = tmp->next;
	}
}

/* Draw the bean */
void draw_bean() {
	LCD_Draw_Circle(10 * (b->pos->x) + 5, 10 * (b->pos->y) + 5, 5);
	LCD_Draw_Circle(10 * (b->pos->x) + 5, 10 * (b->pos->y) + 5, 4);
	LCD_Draw_Circle(10 * (b->pos->x) + 5, 10 * (b->pos->y) + 5, 3);
	LCD_Draw_Circle(10 * (b->pos->x) + 5, 10 * (b->pos->y) + 5, 2);
	LCD_Draw_Circle(10 * (b->pos->x) + 5, 10 * (b->pos->y) + 5, 1);
}

/* Draw the stone */
void draw_stone() {
	LCD_Fill(10 * (stone->x), 10 * (stone->y), 10 * (stone->x) + 10,
			10 * (stone->y) + 10, BROWN);
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(10 * (stone->x), 10 * (stone->y), 10 * (stone->x) + 10,
			10 * (stone->y) + 10);
}

/* Launch game */
void launch() {
	// TODO: Need to be implemented.
	draw_background();
	score = 1;		// debug
	draw_score();	//debug

	snake_init(3, WHITE);
	draw_snake();
	score = 2;
draw_score();

	generate_bean();
//
//	generate_stone();
//	while (end_game == 0) {
//		HAL_Delay(100);
//		draw_background();
//		draw_score();
//		draw_snake();
//		if (milisecond - time_bean > 3000) {
//			generate_bean();
//		}
//		draw_bean();
//
//		if (milisecond - time_stone > 3000) {
//			generate_stone();
//		}
//		draw_stone();
//
//		move();
//	}
	red_blink = 1;
}

