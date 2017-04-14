#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "crossfireOperations.h"


/*A function that searches the board for the players that are attackable
 *Input:
 *	p[]: Players data
 *	currPlayer: Current player
 *	n: number of players
 *	choice: melee,ranged,magic, searches enemies who are in the 3 different ranges
 */
void searchAttk(struct player p[],int currPlayer, int n, int choice)
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
					
					for(int i = 0; i < n-1; i++){ //Initilizing melee array
							p[currPlayer].melee[i] = '\0';
					}
					
					//Visual Presentation of Enemy Unit Range, (searches unit within melee range)
					printf("Melee Range:\n");
					findSlots(0, 0, currSlot, foundSlots, &count, explored); 
					findSlots(1, 0, currSlot, foundSlots, &count, explored);
				}
				else if(choice == 1){ //Find Distant Unit (Not Magic Attk)
					
					for(int i = 0; i < n-1; i++){ //Initilizing ranged array
							p[currPlayer].ranged[i] = '\0';
					}
					
					//Searches units within distant range
					printf("\nDistant Range:\n");
					findSlots(3, 0, currSlot, foundSlots, &count, explored);
					findSlots(4, 0, currSlot, foundSlots, &count, explored);
				}
				else{ //No need to find magic range, but reset its array
				
					for(int i = 0; i < n-1; i++){ //Initilizing magic array
							p[currPlayer].magic[i] = '\0';
					}
					printf("\nMagic Range:\n");
				}
				
				int value = 0;
				if(choice == 0){ //Units that are in melee range
					for(int j = 0; j < n; j++){
						for(int k=0; k<count; k++){
							
							//Check if its not the currplayer && slots correct
							if(p[j].place->row == foundSlots[k].row && p[j].place->column == foundSlots[k].column && p[j].name != p[currPlayer].name && p[j].action != 3 && p[j].life_points != 0){
								printf("(%d, %d)-> %s\n",foundSlots[k].row, foundSlots[k].column, p[j].name);
								p[currPlayer].melee[value]= p[j].name; //Store the name found into melee array
								value++; //Next slot in array
								break;
							}			
						}	
					}
					
					if(p[currPlayer].melee[0] == '\0'){ //Tell users melee array is empty, if no such conditions were met
						printf("Empty\n");
					}
				}	
				
				else if(choice == 1){ //Units that are in distant range
					
					for(int j = 0; j < n; j++){
						for(int k=0; k<count; k++){
							
							//Check if its not the currplayer && slots correct
							if(p[j].place->row == foundSlots[k].row && p[j].place->column == foundSlots[k].column && p[j].name != p[currPlayer].name && p[j].action != 3 && p[j].life_points != 0){
								printf("(%d, %d)-> %s\n",foundSlots[k].row, foundSlots[k].column, p[j].name);
								p[currPlayer].ranged[value] = p[j].name; //Store the name found into ranged array
								value++;
								break;
							}			
						}	
					}
					
					if(p[currPlayer].ranged[0] == '\0'){ //Tell users ranged array is empty, if no such conditions were met
						printf("Empty\n");
					}
				}
				
				else if(choice == 2){ //Units that are in magic range
					for(int j = 0; j < n; j++){
						if(p[j].name != p[currPlayer].name && (p[currPlayer].smartness + p[currPlayer].magic_skills) > 150 && p[j].action != 3 && p[j].life_points != 0){
							printf("(%d, %d)-> %s\n",p[j].place->row, p[j].place->column,p[j].name);
							p[currPlayer].magic[value] = p[j].name;
							value ++;
						}	
					}
					
					if(p[currPlayer].magic[0] == '\0'){ //Tell users magic array is empty
						printf("Empty\n");
					}
				}
			}
}


/*A function that prints out players stats
 *Input:
 *				p[] = Player data array
 *				n   = Number of players
 */
void stats(struct player p[], int n){
	
	printf("\n%-20s  Class \t%-40s\t%-24s\n", "Player Name","-------------------Capabilities-------------------", "[Current Slot]");

	//A loop that prints players stats
	for(int i = 0; i < n; i++)
	{
		//A switch that depends on what class players picked
		switch(p[i].type){
			case(0):
				printf("%d.%-20s%-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]  LP[%3d]\t->[%d,%d]\n",i+1,p[i].name, "Elf",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity, p[i].life_points, p[i].place->row, p[i].place->column);
				break;
			case(1):
				printf("%d.%-20s%-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]  LP[%3d]\t->[%d,%d]\n",i+1,p[i].name, "Human",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity, p[i].life_points, p[i].place->row, p[i].place->column);
				break;
			case(2):
				printf("%d.%-20s%-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]  LP[%3d]\t->[%d,%d]\n",i+1,p[i].name, "Ogre",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity, p[i].life_points, p[i].place->row, p[i].place->column);
				break;
			case(3):
				printf("%d.%-20s%-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]  LP[%3d]\t->[%d,%d]\n",i+1,p[i].name, "Wizard",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity, p[i].life_points, p[i].place->row, p[i].place->column);
				break;
			default:
			break;
		}
	}	
}

/*A function that checks if termination conditions are true or false
 *Input:
 *		p[]: players in game
 *		n: number of players
 */
int termination(struct player p[], int n){
	int status = 0; //default value showing no one left or died
	
	//Checks to see if the termination conditions are met
	for(int i = 0; i < n; i ++){
		
		if(p[i].action == 3){ //If players left the game
			status++; //Counts number of players that left
		}
		
		if(status == n - 1){//If there is only one player left
			status = n; //Change requirement to exit while loop
			
			for(int i = 0; i < n; i++){//Last player standing is winner
				if(p[i].action != 3){
					printf("*****************Congratulation %s Wins!*****************", p[i].name);
				}
			}
		}
		else if(status == n+1){//If all players left
			status = n;
			printf("All Players left the game.\n");
			printf("*****************Game Over*****************");
		}
	}
	
	return status;
}