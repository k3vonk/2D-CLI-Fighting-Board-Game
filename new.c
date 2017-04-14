#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define BOARD_SIZE 7 
#define DIRECTION 4  //UP, DOWN, LEFT, RIGHT

/*
 * Definition of boolean types
 * This avoids using <stdbool.h>
 */
typedef int bool;
enum { false, true };

enum range{None,Melee,Distant, Magic}; //Type of attack
enum class{Elf, Human, Ogre, Wizard}; //Class
enum terrain{Level_Ground, Hill, City}; //Terrain to choose from

struct player{
	char name[40];
	enum class type;
	enum range target;
	int life_points;
	int smartness, strength, magic_skills, luck, dexterity;
	int action; //1 = Move, 2 = Atk, 3 = Quit
	struct slot *place;
};

struct slot{
	int row;
	int column;
	enum terrain type;
	struct slot *left;
	struct slot *right;
	struct slot *up;
	struct slot *down;
};

//Function Prototype
int play();
void name(struct player players[], int n);
void class_choice(struct player p[], int n);
void capabilities(struct player p[], int n);
void createBoard(struct player p[], struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight, int n);
void stats(struct player p[], int n);
void action(struct player p[], int currPlayer);
void movement(struct player p[], int currPlayer);
void terrain_mod(struct player p[],int currPlayer);
void findSlots(int reqDist, int currDist,  struct slot * currSlot, struct slot * foundSlots, int * count,  bool explored[BOARD_SIZE][BOARD_SIZE]);
void searchAttk(struct player p[],int currPlayer, struct slot *upLeft, struct slot *upRight, struct slot *downLeft, struct slot *downRight, int n, int choice);

int main(void)
{
	srand(time(NULL));
	int num = play(); //Number of players in game
	
	struct player players[num]; //A struct array that contains players information
	struct slot *upLeft; //pointer to slot (0,0)
	struct slot *upRight; //pointer to slot (0,BOARD_SIZE -1)
	struct slot *downLeft; //pointer to slot (BOARD_SIZE - 1, 0)
	struct slot *downRight; //pointer to slot (BOARD_SIZE - 1, boardSize -1)
	
	name(players, num); //Sets names to each user
	class_choice(players, num); //Sets class to each user
	capabilities(players, num); //Setting a players capabilities
	createBoard(players, &upLeft, &upRight, &downLeft, &downRight, num); //Creating Board and assigning players to board
	stats(players, num); //Visual representation of capabilities of Players
	
	for(int i = 0; i < num; i++)
	{
		action(players,i); //Everyone alive chooses an action
		
		switch(players[i].action){
			case(1):
				movement(players, i); //Player moves with choice
			break;
			case(2):
			//Visual Presentation of current player's slot
			printf("--------Current Slot-------\n");
			printf("[%d,%d] -> %s\n", players[i].place->row, players[i].place->column, players[i].name);
			
			//Visual Presentation of Enemy Unit Range
			printf("\n\nMelee Range:\n");
			searchAttk(players,i, upLeft, upRight, downLeft, downRight, num, 0);
			printf("\n\nDistant Range:\n");
			searchAttk(players,i, upLeft, upRight, downLeft, downRight, num, 1);
			printf("\n\nMagic Range: \n");
			searchAttk(players,i, upLeft, upRight, downLeft, downRight, num, 2);
			break;
			case(3):
			break;
		}
	}
}

/*A function that gets the number of players, playing the game*/
int play()
{
	int n; //Stores the value of players
	printf("\nEnter the number of players(min 2,max 6): ");
	scanf("%d", &n);
	
	//Error checking
	while(n < 2 || n > 6)
	{
		printf("\nInvalid input, please try again!\n");
		printf("Enter the number of players(max 6): ");
		scanf("%d", &n);
	}

	return n; //Returns number of players to main
}

/*Function to ask user to choose names
 *Input: 
 *			p[] = struct array of players
 *			n   = number of players
 */
void name(struct player p[], int n)
{
	//Loop through players asking and storing their name into the array
	for(int i = 0; i < n; i++){
		printf("Enter the name of player %d: ", i + 1);
		scanf("%s", p[i].name);
	}
}

/*A function to allow user to choose player classes
 *Input:
 *			p[] = Player data array
 *			n   = number of players
 */
void class_choice(struct player p[], int n)
{
	
	//A loop that asks user to pick a class
	for(int i = 0; i < n; i++)
	{
		//Class choice
		printf("\nEnter %s's class:\n", p[i].name);
		printf("(0)Elf  (1)Human  (2)Ogre  (3)Wizard\n");
		scanf("%d", &p[i].type);
		
		//Error Checking for class choice
		while(p[i].type < 0 || p[i].type > 3){
			printf("\nInvalid input, please try again!\n");
			printf("Enter %s's class:\n", p[i].name);
			printf("(0)Elf  (1)Human  (2)Ogre  (3)Wizard\n");
			scanf("%d", &p[i].type);
		}	
	}
}

/*Setting players capabilities based on class
 *Input:
 *				p[] = Player data array
 *				n   = Number of players
 */
void capabilities(struct player p[], int n)
{
	//Loops through the players and setting each players capabilities
	for(int i = 0; i < n; i++)
	{
		//Depending on the .type value a player will get a set of capabilities
		switch(p[i].type){
			case(0): //Elf capabilities
				p[i].life_points = 100;
				p[i].smartness = 70 + rand() % 31; 
				p[i].strength = 1 + rand() % 50;
				p[i].magic_skills = 51 + rand() % 29;
				p[i].luck = 60 + rand() % 41;
				p[i].dexterity = 1 + rand() % 100;
				break;
			case(1): //Human capabilities
				do{
					p[i].life_points = 100;
					p[i].smartness = 1 + rand() % 100; 
					p[i].strength = 1 + rand() % 100;
					p[i].magic_skills = 1 + rand() % 100;
					p[i].luck = 1 + rand() % 100;
					p[i].dexterity = 1 + rand() % 100;
				}while((p[i].smartness + p[i].strength + p[i].magic_skills + p[i].luck + p[i].dexterity) > 299); //Condition that his capabilities excluding life points needs to be < 300
				break;
			case(2): //Ogre capabilities
				do{
					p[i].life_points = 100;
					p[i].smartness = rand() % 21;
					p[i].strength = 80 + rand() % 21;
					p[i].magic_skills = 0;
					p[i].luck = rand() % 51;
					p[i].dexterity = 80 + rand() % 21;
				}while((p[i].luck + p[i].smartness > 50)); //Conditions that ogre's luck and smartness must be less than or equal to 50
				break;
			case(3): //Wizard capabilities
				p[i].life_points = 100;
				p[i].smartness = 90 + rand() % 11; 
				p[i].strength = 1 + rand() % 20;
				p[i].magic_skills = 80 + rand() % 21;
				p[i].luck = 50 + rand() % 51;
				p[i].dexterity = 1 + rand() % 100;
				break;
			default:
				break;
		}
		
		p[i].action == -1; //Default value such that it doesn't show garbage value in another function
	}
}

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
	printf("------Player Position------\n");
	for(int i = 0; i < n; i++)
	{
		p[i].place = &board[rand()%7][rand()%7]; //random slot address to players
		printf("[%d,%d] -> %s\n", p[i].place->row, p[i].place->column, p[i].name);
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

/*A function that prints out players stats
 *Input:
 *				p[] = Player data array
 *				n   = Number of players
 */
void stats(struct player p[], int n){
	
	printf("\n%-10s  Class \t%-40s\n", "Player Name","---------------Capabilities---------------");
	
	//A loop that prints players stats
	for(int i = 0; i < n; i++)
	{
		//A switch that depends on what class players picked
		switch(p[i].type){
			case(0):
				printf("%d.%-10s %-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]\n",i+1,p[i].name, "Elf",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity);
				break;
			case(1):
				printf("%d.%-10s %-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]\n",i+1,p[i].name, "Human",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity);
				break;
			case(2):
				printf("%d.%-10s %-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]\n",i+1,p[i].name, "Ogre",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity);
				break;
			case(3):
				printf("%d.%-10s %-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]\n",i+1,p[i].name, "Wizard",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity);
				break;
			default:
			break;
		}
	}	
}

/*A function that allows players to choose an action
 *Input:
 *	p: Player's data
 *	currPlayer: Current player being asked 
 */
void action(struct player p[], int currPlayer){
	//If player isn't dead 
	if(p[currPlayer].life_points != 0 || p[currPlayer].action != 3){
	//A loop that asks each user what course of action they will take
		printf("\n%s choose an action:\n", p[currPlayer].name);
		printf("(1)Move to an adacent slot - 1 step\n(2)Attack\n(3)Quit the game\n");
		scanf("%d", &p[currPlayer].action);
	
		//Error checking
		while(p[currPlayer].action < 1 || p[currPlayer].action > 3)
		{
			printf("Invalid input, please try again!\n");
			printf("\n%s choose an action:\n", p[currPlayer].name);
			printf("(1)Move to an adacent slot - 1 step\n(2)Attack\n(3)Quit the game\n");
			scanf("%d", &p[currPlayer].action);
		}
	}
	else{
		p[currPlayer].action = 3; //Quit or dead 
	}
	
}	


void movement(struct player p[], int currPlayer){
		bool move[DIRECTION]; //Signifies which direction is possible to move to
		int point = -1; //Initialization
		
		//Initialization of move
		for(int i = 0; i < DIRECTION; i++)
		{
			move[i] = false; //[0] = UP, [1] = DOWN, [2] = LEFT, [3] = RIGHT
		}		
		
		
				printf("\n\nCurrent Position:[%d,%d] -> %s \n", p[currPlayer].place->row, p[currPlayer].place->column, p[currPlayer].name);
				printf("Available slots:\n");
				//Check if adjacent slot is available
				if(p[currPlayer].place->up != NULL){ //UP
					printf("          UP[%d,%d]\n", p[currPlayer].place->up->row, p[currPlayer].place->up->column);
					move[0] = true;
				}
				else{
					printf("         UP[NULL]\n");
				}
				
				if(p[currPlayer].place->left != NULL){ //LEFT
					printf("LEFT[%d,%d]", p[currPlayer].place->left->row, p[currPlayer].place->left->column);
					move[2] = true;
				}
				else{
					printf("LEFT[NULL]");
				}
				
				if(p[currPlayer].place->right != NULL){ //RIGHT
					printf("         RIGHT[%d,%d]\n", p[currPlayer].place->right->row, p[currPlayer].place->right->column);
					move[3] = true;
				}
				else{
					printf("         RIGHT[NULL]\n");
				}
				
				if(p[currPlayer].place->down != NULL){ //DOWN
					printf("         DOWN[%d,%d]\n", p[currPlayer].place->down->row, p[currPlayer].place->down->column);
					move[1] = true;
				}
				else{
					printf("         DOWN[NULL]\n");
				}
			
			//Loop to ask user for direction they want to go in
			do{
				printf("\n\nEnter your choice of movement:\n");
				printf("(0)Up (1)Down (2)Left (3)Right\n");
				scanf("%d", &point);
				
				//Error Messages
				if(point < 0 || point > 3){
					printf("Invalid action, %s retry!\n", p[currPlayer].name);
				}	
				if(move[point] == false){
						printf("Slot was unavailable\n");
					}
			}while(move[point] == false || (point < 0 || point > 3));
				
				
			    //Moving player to the selected slot
				if(point == 0){ //UP
					p[currPlayer].place = p[currPlayer].place->up;
				}
				else if(point == 1){ //DOWN
					p[currPlayer].place = p[currPlayer].place->down;
				}
				else if(point == 2){ //LEFT
					p[currPlayer].place = p[currPlayer].place ->left;
				}
				else if(point == 3){ //RIGHT
					p[currPlayer].place = p[currPlayer].place -> right;
				}
				
				terrain_mod(p,currPlayer); //After moving slot modify players stats
				
				printf("---------NEW SLOT---------\n");
				printf("[%d,%d] -> %s\n", p[currPlayer].place->row, p[currPlayer].place->column, p[currPlayer].name);		
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

void searchAttk(struct player p[],int currPlayer, struct slot *upLeft, struct slot *upRight, struct slot *downLeft, struct slot *downRight, int n, int choice)
{
		struct slot *currSlot = NULL; //Initialization
		struct slot *foundSlots = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(struct slot)); //Check which slots are players
		bool explored[BOARD_SIZE][BOARD_SIZE];
		int count = 0; //The number of slots that are found
		
		//Initializing explored
		for(int i=0; i<BOARD_SIZE; i++){
			for(int j=0; j<BOARD_SIZE;j++){
				explored[i][j] = false;
			}
		}
	
		currSlot = p[currPlayer].place; //The slot we are searching from
		
	//If slot isn't empty
			if(currSlot!= NULL){
				//Find Melee Unit
				if(choice == 0){
					findSlots(0, 0, currSlot, foundSlots, &count, explored);
					findSlots(1, 0, currSlot, foundSlots, &count, explored);
				}
				else if(choice == 1){ //Find Distant Unit (Not Magic Attk)
					findSlots(3, 0, currSlot, foundSlots, &count, explored);
					findSlots(4, 0, currSlot, foundSlots, &count, explored);
				}
				else{ //Do nothing for Magic find
					
				}
				
				
				if(choice == 0){
					for(int j = 0; j < n; j++){
						for(int k=0; k<count; k++){
							if(p[j].place->row == foundSlots[k].row && p[j].place->column == foundSlots[k].column && p[j].name != p[currPlayer].name){
								printf("(%d, %d)-> ",foundSlots[k].row, foundSlots[k].column);
								printf("%s", p[j].name);
								p[j].target = 1;
								break;
							}			
							else{
								p[j].target = 0;
							}
						}	
					}
				}	
				
				else if(choice == 1){
					
					for(int j = 0; j < n; j++){
						for(int k=0; k<count; k++){
							if(p[j].place->row == foundSlots[k].row && p[j].place->column == foundSlots[k].column && p[j].name != p[currPlayer].name){
								printf("(%d, %d)-> ",foundSlots[k].row, foundSlots[k].column);
								printf("%s", p[j].name);
								p[j].target = 2;
								break;
							}			
							else{
								p[j].target = 0;
							}
						}	
					}
				}
				
				else if(choice == 2){
					for(int j = 0; j < n; j++){
						if(p[j].name != p[currPlayer].name){
							printf("(%d, %d)-> %s\n",p[j].place->row, p[j].place->column,p[j].name);
						}		
					}
				}
				
			}
}
