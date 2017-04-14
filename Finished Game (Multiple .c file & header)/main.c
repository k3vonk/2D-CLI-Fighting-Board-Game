#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "crossfireOperations.h"


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
	
	int point = -1; //Defines what type of attack to do
	int game = 0; //To decide if game is to be ended
	int round = 0; //Counts the number of rounds
	
	//Initialization of players action
	for(int i = 0; i < num; i++)
		{
			players[i].action = -1;
		}
		
		
	do{
		round++;
		printf("\n\n**********ROUND: %d**********\n", round);
	for(int i = 0; i < num; i++)
	{
		//Boundary for user to pick an action
		if((players[i].action) != 3){
		act: action(players,i); //Everyone alive chooses an action
			
			switch(players[i].action){
				case(1): //Move
					movement(players, i); //Player moves with choice
					stats(players, num); //Visual representation of capabilities of Players
				break;
				case(2): //Attack
					//Prints out a list of players on specific attack range
					printf("\nCurrent players targetability: \n");
					searchAttk(players,i, num, 0); //Melee range
					searchAttk(players,i, num, 1); //Distant range
					searchAttk(players,i, num, 2); //Magic range
					
					//If there is no one to attack then choose another action
					point = attkType(players, i);
					if(point == -1){
						goto act;
					}
					
					
					playerAttk(players, num, point, i); //Attack players
					stats(players, num); //Visual representation of capabilities of Players
				break;
				case(3): //Quit
					printf("\n%s has left the game.\n", players[i].name);
				break;
				default:
				break;
			}
		}
	}
		game = termination(players, num); //game value determines if there are more rounds or not
	}while(game != num );
	
	return 0;
}