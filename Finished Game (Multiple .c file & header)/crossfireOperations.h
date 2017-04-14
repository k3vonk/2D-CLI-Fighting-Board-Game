#ifndef CROSSFIREOPERATIONS_H_
#define CROSSFIREOPERATIONS_H_



#endif /* CROSSFIREOPERATIONS_H_ */

#define BOARD_SIZE 7 
#define DIRECTION 4  //UP, DOWN, LEFT, RIGHT
#define ATK 3 //Melee, Ranged, Magic

/*
 * Definition of boolean types
 * This avoids using <stdbool.h>
 */
typedef int bool;
enum { false, true };

enum class{Elf, Human, Ogre, Wizard}; //Class
enum terrain{Level_Ground, Hill, City}; //Terrain to choose from

struct player{
	char name[40];
	enum class type;
	int life_points;
	int smartness, strength, magic_skills, luck, dexterity;
	int action; //1 = Move, 2 = Atk, 3 = Quit
	char *melee[6];
	char *ranged[6];
	char *magic[6];
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
void searchAttk(struct player p[],int currPlayer, int n, int choice);
void playerAttk(struct player p[], int n, int choice, int currPlayer);
int attkType(struct player p[], int currPlayer);
int termination(struct player p[], int n);
