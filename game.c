#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


enum class{Elf, Human, Ogre, Wizard};

struct player{
	char name[40];
	enum class type;
	int life_points;
	int smartness, strength, magic_skills, luck, dexterity;
	struct slot *place;
};

int play();
void name(struct player players[], int n);
void class_choice(struct player p[], int n);
void capabilities(struct player p[], int n);

int main(void)
{
	int num;
	num = play();
	struct player players[num]; //A struct array that contains players information
	name(players, num); //Calls function to store names
	class_choice(players, num);
	capabilities(players, num); //Setting a players capabilities
}

int play()
{
	int n; //Stores the value of players
	printf("\nEnter the number of players(max 6): ");
	scanf("%d", &n);
	
	//Error checking
	while(n < 1 || n > 6)
	{
		printf("Invalid input, please try again!\n");
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
	for(size_t i = 0; i < n; i++){
		printf("Enter the name of player %lu: ", i + 1);
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
	int p_class; //A value that is used to determine a player's class
	
	//A loop that asks user to pick a class
	for(size_t i = 0; i < n; i++)
	{
		//Class choice
		printf("\nEnter %s's class:\n", p[i].name);
		printf("(0)Elf  (1)Human  (2)Ogre  (3)Wizard\n");
		scanf("%d", &p_class);
		
		//Error Checking for class choice
		while(p_class < 0 || p_class > 3){
			printf("\nInvalid input, please try again!\n");
			printf("Enter %s's class:\n", p[i].name);
			printf("(0)Elf  (1)Human  (2)Ogre  (3)Wizard\n");
			scanf("%d", &p_class);
		}
		
		//Allocating the players their classes by storing an enum value to the .type
		switch(p_class){
			case(0):
				p[i].type = Elf; 
				break;
			case(1):
				p[i].type = Human;
				break;
			case(2):
				p[i].type = Ogre;
				break;
			case(3):
				p[i].type = Wizard;
				break;
			default:
				break;
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
	for(size_t i = 0; i < n; i++)
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
	}
}