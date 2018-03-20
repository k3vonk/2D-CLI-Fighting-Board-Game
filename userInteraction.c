#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "crossfireOperations.h"


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
		printf("(1)Move to an adjacent slot - 1 step\n(2)Attack\n(3)Quit the game\n");
		scanf("%d", &p[currPlayer].action);
	
		//Error checking
		while(p[currPlayer].action < 1 || p[currPlayer].action > 3)
		{
			printf("Invalid input, please try again!\n");
			printf("\n%s choose an action:\n", p[currPlayer].name);
			printf("(1)Move to an adjacent slot - 1 step\n(2)Attack\n(3)Quit the game\n");
			scanf("%d", &p[currPlayer].action);
		}
	}
	else{
		p[currPlayer].action = 3; //Quit or dead 
	}
	
}	

/*A function that asks user what position they want to move to
 *Input: 
 *		p[]: players data
 *		currPlayer: current player
 */
void movement(struct player p[], int currPlayer){
		bool move[DIRECTION]; //Signifies which direction is possible to move to
		int point = -1; //Initialization & point = what direction they want to go to
		
		//Initialization of move
		for(int i = 0; i < DIRECTION; i++)
		{
			move[i] = false; //[0] = UP, [1] = DOWN, [2] = LEFT, [3] = RIGHT
		}		
		
		
				printf("\n\nCurrent Position:[%d,%d] -> %s \n", p[currPlayer].place->row, p[currPlayer].place->column, p[currPlayer].name);
				printf("Available slots:\n\n");
				
				
				//Check if adjacent slot is available - Visual representation
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
}


/*A function to ask user what type of attack they want to perform
 *Input:
 *	p[]:Players data
 *	currPlayer: current player
 */
int attkType(struct player p[], int currPlayer){
	
	bool attack[ATK]; //Used to check if attack is valid or not
	int choice = -1; //Default value of choice

	//Initialization of bool attack
	for(int i = 0; i < ATK; i++){
		attack[i] = false; //[0] = Melee, [1] = Ranged, [2] = Magic
	}	
				
	if(p[currPlayer].melee[0] != '\0'){// melee array isn't empty - CAN MELEE ATTACK
		attack[0] = true;
	}
	if(p[currPlayer].ranged[0] != '\0'){// Ranged array isn't empty - CAN DISTANT ATTACK
		attack[1] = true;
	}
	if(p[currPlayer].magic[0] != '\0'){// Magic array isn't empty - CAN RANGE ATTACK
		attack[2] = true;
	}
	if(p[currPlayer].melee[0] == '\0' && p[currPlayer].ranged[0] == '\0' && p[currPlayer].magic[0] == '\0'){//If cannot attack then choose another option
		printf("%s cannot attack\n", p[currPlayer].name);
		return choice; //Choose another option
	}
	
	//A loop that asks user for what type of attack they want to do
	do{
		printf("\n\nEnter %s's choice of attack:\n", p[currPlayer].name);
		printf("(0)Melee (1)Distant (2)Magic\n");
		scanf("%d", &choice);
				
		//Error Messages
		if(choice < 0 || choice > 2){
			printf("Invalid action, %s retry.\n", p[currPlayer].name);
		}	
		if(attack[choice] == false){
			printf("Attack type was unavailable.\n");
		}
	}while(attack[choice] == false || (choice < 0 || choice > 2)); //If conditions not met then repeat question
				
	return choice; //choice = the type of attack chosen
}

/*A function that allows the user to attack a specific person
 *Input:
 *		p[]: players that are in game
 *		n: number of players playing
 *		choice: the type of attack
 *		currPlayer: Current player
 */
void playerAttk(struct player p[], int n, int choice, int currPlayer){
	char enemy[40]; //Input name given by user to be attacked
	bool unit = false; //There are no units to attack with the name
	name: printf("\n\nEnter enemy unit's name: ");
	scanf("%s", enemy);
	
	//A check to see if user input target name is correct
	if(choice == 0){ //Melee target
		for(int i = 0; i < n-1; i++){
			if(strcmp(p[currPlayer].melee[i],enemy) == 0){
				unit = true;
				break;
			}
		}
	}
	else if(choice == 1){ //Distant target
		for(int i = 0; i < n-1; i++){
			if(strcmp(p[currPlayer].ranged[i],enemy) == 0){
				unit = true;
				break;
			}
		}
	}
	else if(choice == 2){ //Magic target
		for(int i = 0; i < n-1; i++){
			if(strcmp(p[currPlayer].magic[i],enemy) == 0){
				unit = true;
				break;
			}
		}
	}
	
	//If input name is wrong, then ask again
	if(unit == false){
		printf("Invalid name given!\n");
		printf("%s:\n", p[currPlayer].name);
		goto name;
	}

	
	int j = 0; //j: value to search through players
	double damage; //Calculation of damage to a player
		switch(choice){
			case(0): //Melee attack
				for(j; j < n; j++){ 
					if(strcmp(p[j].name,enemy) == 0){ //Looks for player with the name and get his data to attack him
						if(p[j].strength <= 70){ //Enemy attacked
							damage = (0.5)*p[currPlayer].strength;
							printf("%s attacks %s for %.2lf damage\n",p[currPlayer].name,p[j].name, damage);
							p[j].life_points = p[j].life_points - (0.5)*p[currPlayer].strength;
							break;
						}
						else if(p[j].strength > 70){ //Current player attacked
							damage = (0.3)*p[currPlayer].strength;
							printf("%s attacks %s for %.2lf damage\n",p[j].name,p[currPlayer].name, damage);
							p[currPlayer].life_points = p[currPlayer].life_points - (0.3)*p[j].strength;
							break;
						}
					}
				}
			break;
			case(1): //Distant attack
				for(j; j < n; j++){
					if(strcmp(p[j].name,enemy) == 0){ //Looks for player with the name and get his data to attack him
						if(p[j].dexterity >= p[currPlayer].dexterity){ //No one hurt basically a miss
							printf("%s took no damage.\n", p[j].name);
							break;
						}
						else if(p[currPlayer].dexterity > p[j].dexterity){ //Enemy attacked from a distance
							damage = (0.3)*p[currPlayer].strength;
							printf("%s attacks %s for %.2lf damage\n",p[currPlayer].name,p[j].name, damage);
							p[j].life_points = p[j].life_points - (0.3)*p[currPlayer].strength;
							break;
						}
					}
				}
			break;
			case(2): //Magic attack
				for(j; j < n; j++){
					if(strcmp(p[j].name,enemy) == 0){ //Looks for players with the name and get his data to attack him
					
						//Magic damage to enemy
						damage = ((0.5)*p[currPlayer].magic_skills) + ((0.2)*p[currPlayer].smartness);
						printf("%s attacks %s for %.2lf damage\n",p[currPlayer].name,p[j].name, damage);
						p[j].life_points = p[j].life_points - ((0.5)*p[currPlayer].magic_skills) + ((0.2)*p[currPlayer].smartness);
						break;
					}
				}
			break;
			default:
			break;
		}
		
		//Conditions to set health to zero if they go below 0 and action = 3 to show that player died and has to quit the game
		if(p[j].life_points <= 0){
			p[j].life_points = 0;
			p[j].action = 3;
		}
		if(p[currPlayer].life_points <= 0){
			p[currPlayer].life_points = 0;
			p[currPlayer].action = 3;
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
		//Depending on the player's class, they will get a set of capabilities
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
		
		//Initializing array size for melee,ranged,magic and making them empty for every player
		for(int j=0;j < n-1;j++){
			p[i].melee[j] = malloc(40*sizeof(char)); //Can store 40 char
			p[i].ranged[j] = malloc(40*sizeof(char));
			p[i].magic[j] = malloc(40*sizeof(char));
			
			p[i].melee[j] = '\0'; //Storing NULL
			p[i].ranged[j] = '\0';
			p[i].magic[j] = '\0';
		}
	}
	
}