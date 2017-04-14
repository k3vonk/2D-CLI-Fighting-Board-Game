#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "crossfireOperations.h"




/*
 * This function creates the board
 * Parameters:
 * 	p: player struct
 * 	upLeft: pointer of pointer to slot at position (0, 0)
 * 	upRight: pointer of pointer to slot at position (0, BOARD_SIZE -1)
 * 	downLeft: pointer of pointer to slot at position (BOARD_SIZE -1, 0)
 * 	upLeft: pointer of pointer to slot at position (BOARD_SIZE - 1, BOARD_SIZE -1)
 *	n: number of players
 */
void createBoard(struct player p[], struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight, int n){

	//The board is represented as a pointer of pointer to slots
	//This allocates in memory the space for the pointers to each row of the board
	struct slot ** board = malloc(BOARD_SIZE * sizeof(struct slot *));


	for(int i =0; i< BOARD_SIZE; i++){
		//This allocates in memory the space for the slots in each row of the board
		board[i] = malloc(BOARD_SIZE * sizeof(struct slot));

		//For each slot it sets up the row and column number
		for(int j=0;j < BOARD_SIZE; j++){
			board[i][j].row = i;
			board[i][j].column = j;
			board[i][j].type = rand()%3; //assigning terrain
		}
	}

	//It sets up the adjacent slots for the slots that are
	//not at the borders. These slots have 4 adjacent elements
	for(int i =1; i< BOARD_SIZE-1; i++){
		for(int j=1;j < BOARD_SIZE-1; j++){
			board[i][j].up = &board[i-1][j];
			board[i][j].right = &board[i][j+1];
			board[i][j].down = &board[i+1][j];
			board[i][j].left = &board[i][j-1];
		}
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last row, except the slots at the edges.
	//
	for(int j = 1; j < BOARD_SIZE -1; j++){
		//It sets up the adjacent slots for the slots that are in the first row.
		//These slots don't have an adjacent element on top of them
		// because they are on the first row of the board
		board[0][j].right = &board[0][j+1];
		board[0][j].left = &board[0][j-1];
		board[0][j].down = &board[1][j];
		board[0][j].up = NULL;

		//It sets up the adjacent slots for the slots that are in the last row.
		//These slots don't have an adjacent element on down them
		// because they are on the last row of the board
		board[BOARD_SIZE - 1][j].right = &board[BOARD_SIZE - 1][j+1];
		board[BOARD_SIZE -1][j].left = &board[BOARD_SIZE - 1][j-1];
		board[BOARD_SIZE - 1][j].up = &board[BOARD_SIZE - 2][j];
		board[BOARD_SIZE - 1][j].down = NULL;
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last column, except the slots at the edges.
	//
	for(int i = 1; i < BOARD_SIZE -1; i++){
		//It sets up the adjacent slots for the slots that are in the first column.
		//These slots don't have an adjacent element on the left
		// because they are on the first column of the board
		board[i][0].right = &board[i][1];
		board[i][0].up = &board[i-1][0];
		board[i][0].down = &board[i+1][0];
		board[i][0].left = NULL;

		//It sets up the adjacent slots for the slots that are in the last column.
		//These slots don't have an adjacent element on the right
		// because they are on the last column of the board
		board[i][BOARD_SIZE-1].left = &board[i][BOARD_SIZE-2];
		board[i][BOARD_SIZE -1].up = &board[i -1][BOARD_SIZE-1];
		board[i][BOARD_SIZE -1].down = &board[i+1][BOARD_SIZE -1];
		board[i][BOARD_SIZE -1].right = NULL;
	}


		//It sets up the adjacent slots for the slot at position (0,0).
		//This only has only 2 adjacent slots: right and down
		board[0][0].right = &board[0][1];
		board[0][0].down = &board[1][0];
		board[0][0].left = NULL;
		board[0][0].up = NULL;

		//It sets up the adjacent slots for the slot at position (0,BOARD_SIZE -1).
		//This only has only 2 adjacent slots: left and down
		board[0][BOARD_SIZE-1].left = &board[0][BOARD_SIZE-2];
		board[0][BOARD_SIZE -1].down = &board[1][BOARD_SIZE -1];
		board[0][BOARD_SIZE-1].right = NULL;
		board[0][BOARD_SIZE-1].up = NULL;

		//It sets up the adjacent slots for the slot at position (BOARD_SIZE -1 ,0).
		//This only has only 2 adjacent slots: up and right
		board[BOARD_SIZE -1][0].right = &board[BOARD_SIZE -1][1];
		board[BOARD_SIZE -1][0].up = &board[BOARD_SIZE -2][0];
		board[BOARD_SIZE -1][0].down = NULL;
		board[BOARD_SIZE -1][0].left = NULL;

		//It sets up the adjacent slots for the slot at position (boarSize -1 ,BOARD_SIZE-1).
		//This only has only 2 adjacent slots: up and left
		board[BOARD_SIZE - 1][BOARD_SIZE-1].up = &board[BOARD_SIZE-2][BOARD_SIZE-1];
		board[BOARD_SIZE - 1][BOARD_SIZE -1].left = &board[BOARD_SIZE -1][BOARD_SIZE -2];
		board[BOARD_SIZE - 1][BOARD_SIZE -1].right = NULL;
		board[BOARD_SIZE - 1][BOARD_SIZE -1].down = NULL;


	//assigns a pointer to slot at position (0, 0)
	*upLeft = &board[0][0];
	//assigns pointer of pointer to slot at position (0, BOARD_SIZE -1)
	*upRight = &board[0][BOARD_SIZE -1];
	//assigns pointer of pointer to slot at position ( BOARD_SIZE -1,)
	*downLeft = &board[BOARD_SIZE -1][0];
	//assigns pointer of pointer to slot at position (BOARD_SIZE -1, BOARD_SIZE -1)
	*downRight = &board[BOARD_SIZE -1][BOARD_SIZE -1];
	
	//Assigning players to random slots
	for(int i = 0; i < n; i++)
	{
		p[i].place = &board[rand()%7][rand()%7]; //random slot address to players
	}
	
	printf("\n\n");
	//Board Visual
	printf("------------BOARD SIZE------------\n");
	for(int i = 0; i < BOARD_SIZE; i++)
	{
		for(int j = 0; j < BOARD_SIZE; j++)
		{
			printf("[%d,%d]", board[i][j].row, board[i][j].column);
		}
		printf("\n");
	}
	
}

/*A function that depending on the players location it will increase or decrease their stats
 *Input:
 *				p[] 	= Player data array
 *				s[] 	= Slots data array
 *				player 	= The current player
 */
void terrain_mod(struct player p[],int currPlayer)
{
	
	//Depending on current players location type we modify their capabilities
	switch(p[currPlayer].place->type){
		case(0): //Level Ground slot no capabilities needed to be modified
			break;
		case(1): //Modify player capabilities, when on Hill slot
			if(p[currPlayer].dexterity < 50){
				p[currPlayer].strength -= 10;
			}
			else if(p[currPlayer].dexterity >= 60){
				p[currPlayer].strength += 10;
			}
			break;
		case(2): //Modify player capabilities, when on City slot
			if(p[currPlayer].smartness > 60){
				p[currPlayer].magic_skills += 10;
			}
			else if(p[currPlayer].smartness <= 50){
				p[currPlayer].dexterity -= 10;
			}
			break;
		default:
				break;
	}
	
	//Conditions that ensures that capabilties don't exceed 100 or fall below 0
	if(p[currPlayer].strength < 0){
		p[currPlayer].strength = 0;
	}
	else if(p[currPlayer].strength > 100){
		p[currPlayer].strength = 100;
	}
	
	if(p[currPlayer].magic_skills > 100){
		p[currPlayer].magic_skills = 100;
	}
	
	if(p[currPlayer].dexterity < 0){
		p[currPlayer].dexterity = 0;
	}
}

/*
 * The recursive function that traverses the board to find the slots at a predefined
 * distance from the current slot and place them in foundSlots.
 * Parameters:
 * 	reqDist: the required distance from the starting slot
 * 	currDist: the distance of the current slot from the starting slot
 * 	currSlot: a pointer to the current slot that is traversed
 * 	foundSlots: the array of slots that are at a required distance from the starting slot
 * 	count: pointer to an integer representing the number of slots that are found to be at a required distance from the starting slot
 * 	explored: matrix indicating for each slot at row x and column y has been traversed (true) or not (false)
 */
void findSlots(int reqDist, int currDist,  struct slot * currSlot, struct slot * foundSlots, int * count,  bool explored[BOARD_SIZE][BOARD_SIZE]){

	//The base case: the current slot is at the required distance from the starting slot
	if(currDist == reqDist){
		//the current slot was not explored
		if(explored[currSlot->row][currSlot->column] == false){
			//The next availbale position (indicated by count) at foundSlots points to the current slot
			*(foundSlots + *count) = *currSlot;
			//the counter is incremented
			(*count)++;
			//the matrix of the explored slots set to true the element at the row and column of the current slot
			explored[currSlot->row][currSlot->column] = true;
		}
	}
	//The recursive call: the current slot is at a distance that is less than the required distance (more slots still have to be traversed)
	else{
		//if the current slot has the up slot != NULL (i.e. the slot is not in the first row)
		if(currSlot->up != NULL){
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the up slot
			findSlots(reqDist, currDist +1,  currSlot->up, foundSlots, count, explored);
		}
		//if the current slot has the right slot != NULL (i.e. the slot is not in the last column)
		if(currSlot->right != NULL){
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the right slot
			findSlots(reqDist, currDist +1,  currSlot->right, foundSlots, count, explored);
		}
		//if the current slot has the down slot != NULL (i.e. the slot is not in the last row)
		if(currSlot->down != NULL){
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the down slot
			findSlots(reqDist, currDist +1,  currSlot->down, foundSlots, count, explored);
		}
		//if the current slot has the left slot != NULL (i.e. the slot is not in the first column)
		if(currSlot->left != NULL){
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the left slot
			findSlots(reqDist, currDist +1,  currSlot->left, foundSlots, count, explored);
		}

	}

}
