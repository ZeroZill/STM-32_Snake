/*
 * data_structures.h
 *
 *  Created on: Dec 10, 2019
 *      Author: zero
 */

#ifndef SRC_DATA_STRUCTURES_H_
#define SRC_DATA_STRUCTURES_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lcd.h"

/*
 * The area of game is 240*280(from up-left to down-right), and one grid is 10*10(from 0-9)
 */
#define VERTICAL_GRID_NUMBER 24
#define HORIZONTAL_GRID_NUMBER 28

static uint16_t score = 0;				// Score
static uint8_t end_game = 0;			// If the game is over.
static uint16_t time_interval = 1000;	// Time interval for moving


/* The position tuple */
typedef struct position {
	uint16_t x;
	uint16_t y;
} position;

/* The direction of movement */
typedef struct direction {
	uint16_t ver;	// vertical value, {1:right , -1:left, 0:neither}
	uint16_t hor;	// Horizontal value, {1:down , -1:up, 0:neither}
} direction;

/* Store bean */
typedef struct bean {
	uint16_t color;
	position *pos;
} bean;

/* A list node that contains the position of current single node */
typedef struct occupied_grid {
	struct occupied_grid *prev;
	struct occupied_grid *next;
	position *pos;
} occupied_grid;

/* A linked list is used to store the occupied grids of the snake */
typedef struct snake {
	occupied_grid *head;
	occupied_grid *tail;
	uint16_t length;
	uint16_t color;
	direction *dir;
} snake;

/* Initializing the snake with specific length and color */
void snake_init(uint16_t init_len, uint16_t color);

/* Add a segment of snake body in front of the head if the snake can eat the bean */
void snake_forward();

/* Lengthen the snake by adding a segment to the tail */
void snake_tail_lengthen(direction *dir);

/* Judge if the snake bites itself */
uint8_t bite_self();

/* Judge if the snake hits the wall */
uint8_t hit_wall();

/* Judge if the snake hits the stone */
uint8_t hit_stone();

/* Get a random position out of snake's body */
position* random_pos();

/* Generate a bean */
void generate_bean();

/* Generate a stone */
void generate_stone();

/* Get the signal of keyboard */
uint8_t get_key();

/* Move snake's body */
void move();

/* Draw the back ground */
void draw_background();

/* Draw the score */
void draw_score();

/* Draw one grid of snake body */
void draw_snake_body(occupied_grid *og);

/* Draw the snake */
void draw_snake();

/* Draw the bean */
void draw_bean();

/* Draw the stone */
void draw_stone();

/* Print all staffs, including snake, bean and stone */
void draw_all();


/* Launch game */
void launch();
#endif /* SRC_DATA_STRUCTURES_H_ */
