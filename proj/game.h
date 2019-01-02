#ifndef _GAME_H
#define _GAME_H

#include "vga.h"

typedef struct _keystatus {
    bool q;
    bool w;
    bool e; 
    bool r;
    bool t;
    bool y;
    bool u;
    bool i;
    bool o;
    bool p;
    bool a;
    bool s; 
    bool d;
    bool f;
    bool g;
    bool h;
    bool j;
    bool k;
    bool z;
    bool x;
    bool c;
    bool v;
    bool b;
    bool n;
    bool m;
    bool shift;
    bool caps;
    bool tab;
    bool esc;
    bool numb1;
    bool numb2;
    bool numb3;
    bool numb4;
    bool numb5;
    bool numb6;
    bool numb7;
    bool numb8;
    bool numb9;
    bool numb0;
} keystatus;

typedef enum _powerup{
	none, fast, slow, invincible
} powerup;

typedef struct _sprite{
	unsigned int width, height;
	char *map;
} sprite;

typedef struct _zombie{
	int x, y, speed_x, speed_y;
	unsigned int id;
	bool alive;
	sprite *img;
	struct _zombie* prev;
	struct _zombie* next;
} zombie;

typedef struct _enemies{
	unsigned int size;
	zombie* first;
	zombie* last;
} enemies;

typedef struct _hero{
	int x, y, speed_x, speed_y, score;
	sprite img;
	powerup status;
}hero;

enemies *alunos;
hero *souto;
keystatus ks;

int killZombie(unsigned int id){
	//printf("%d\n", id);
	if(alunos->last->id == id && alunos->last->alive == true){
		//printf("last %d\n", id);
		alunos->last->alive = false;
		return 0;
	}
	zombie * curr;
	for(curr = alunos->first; curr != alunos->last; curr = curr->next){
		printf("curr: %d, alive: %d, next: 0x%X\n", curr->id, curr->alive, curr->next);
		if(curr->alive != true) return 1;
		printf("It's alive!\n");
		if(curr->id == id){
			printf("Same id, prev: 0x%X, next: 0x%X\n", curr->prev, curr->next);
			curr->speed_x = 0;
			curr->speed_y = 0;
			curr->x = 3000;
			curr->y = 3000;
			if(curr->prev != NULL){
				curr->prev->next = curr->next;
				curr->prev = alunos->last;
			}
			curr->next->prev = curr->prev;
			curr->alive = false;
			curr->next = NULL;
			alunos->last->next = curr;
			alunos->last = curr;
			return 0;
		}
	}
	//printf("No such zombie!\n");
	return 1;
}

int gameInit(){
	souto = (hero*)malloc(sizeof(hero));
	alunos = (enemies*)malloc(sizeof(enemies));
	alunos->first = (zombie*)malloc(sizeof(zombie));
	alunos->size = 100;
	
	zombie *aux = alunos->first;
	
	sprite *zombies = (sprite *)malloc(sizeof(sprite));
	sprite heroes;
	
	/*heroes.map = read_xpm(souto_xpm, &(heroes.width), &(heroes.height));
	
	zombies.map = read_xpm(zombie_xpm, &(zombies.width), &(zombies.height));*/
	
	souto->img = heroes;
	souto->score = 0;
	souto->speed_x = 0;
	souto->speed_y = 0;
	souto->status = none;
	souto->x = 0;
	souto->y = 0;
	
	aux->id = 0;
	aux->alive = false;
	aux->img = zombies;
	aux->prev = NULL;
	aux->speed_x=0;
	aux->speed_y=0;
	aux->x=0;
	aux->y=0;
	
	for(unsigned int i=1; i < alunos->size; i++){
		zombie *newZ = (zombie*)malloc(sizeof(zombie));
		newZ->prev = aux;
		aux->next = newZ;
		aux = newZ;
		aux->id = i;
		aux->alive = false;
		aux->img = zombies;
		aux->speed_x=0;
		aux->speed_y=0;
		aux->x=0;
		aux->y=0;
	}
	
	alunos->last = aux;
	
	return 0;
}

float mod(float a){
	if(a < 0) return 0-a;
	else return a;
}

int colision(int x, int y){
	if(mod(souto->x - x) < 50 && mod(souto->y - y) < 33){
		return -1;
	}
	zombie *curr = alunos->first;
	for(unsigned int i=0; i < alunos->size; i++){
		if(curr->alive == false) break ;
		else{
			if(mod(curr->x - x) < 100 && mod(curr->y - y) < 92){
				return curr->id;
			}
		}
		curr = curr->next;
	}
	return 0;
}

int spawnZombie(){
	/*static int couve=0;
	printf("Spawning zombie! %d\n", couve++);*/
	if(alunos->last->alive == false){
		zombie *aux = alunos->last;
		aux->alive = true;
		alunos->last = aux->prev;
		alunos->last->next = NULL;
		aux->prev = NULL;
		aux->next = alunos->first;
		alunos->first->prev = aux;
		alunos->first = aux;
		aux->x = rand() % (2000-getHRes());
		aux->y = rand() % (2000-getVRes());
		
		int difX = mod(aux->x - souto->x);
		int difY = mod(aux->y - souto->y);
		
		if(difX < getHRes()/2 && difY < getVRes()/2){
			aux->x += getHRes();
			aux->y += getVRes();
		}
		
		while(/*colision(aux->x, aux->y) != 0 || */colision(aux->x, aux->y) != (int)aux->id){
			aux->x = rand() % (2000-getHRes());
			aux->y = rand() % (2000-getVRes());
			
			difX = mod(aux->x - souto->x);
			difY = mod(aux->y - souto->y);
			
			if(difX < getHRes()/2 && difY < getVRes()/2){
				aux->x += getHRes();
				aux->y += getVRes();
			}
		}
		return 0;
	}
	
	return 1;
}

#endif
