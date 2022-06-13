#include <stdio.h>
#include <stdlib.h>					//bibliotecas funciones estandar
#include <unistd.h>					//bibliotecas de sistema Unix
#include <pthread.h>				//bibliotecas de threads
#include <signal.h>
#include <ncurses.h> 				//bibliotecas manejo de pantalla


typedef struct caballo_ {			//se define la estructura de caballos
	int x,y;						//y sus atributos 
	char* nombre;					
} caballo_t;


void gen_caballo(caballo_t* caballo) {		//funcion generadora de caballos
	int x = caballo->x, y=caballo->y;
	char* graphic = "♞";       
    fprintf(stderr, "\033[m\033[%d;%dH%s", y, x,graphic );
}


int parar;									//variable de stop a la carrera
int EnSusMarcasListosFuera;					//variable de inicio a la carrera
caballo_t* ganador;


pthread_mutex_t pantalla = PTHREAD_MUTEX_INITIALIZER;  //variable pantalla tipo mutex para sincronizar el acceso a la pantalla
pthread_cond_t condicion = PTHREAD_COND_INITIALIZER;	//variable de condicion 



void* carrera(void*param) {								//funcion que realiza la carrera

	caballo_t* caballo = (caballo_t*) param;			
	while(!parar) {								//condicion para iniciar la carrera		
		caballo->x = 0;
		#ifdef mutex
		pthread_mutex_lock(&pantalla);			//bloquea la pantalla (seccion critica)		
		#endif 
		while(EnSusMarcasListosFuera ==0) pthread_cond_wait(&condicion,&pantalla); 
		
		pthread_mutex_unlock(&pantalla);		//desbloquea la pantalla (seccion critica)
		
		
		while(ganador == NULL) {				//mientras no haya ganador, avanza el caballo
		
		   caballo->x++;
		   usleep(100000 + 2500  *(rand() % 10));  
		   gen_caballo(caballo);
		   if(caballo->x >40) { ganador = caballo; pthread_cond_broadcast(&condicion); } //condicion de victoria
		}
		#ifdef mutex
		pthread_mutex_lock(&pantalla);			//bloquea la pantalla (seccion critica)
		#endif 	
		while(EnSusMarcasListosFuera ==1) pthread_cond_wait(&condicion,&pantalla);  // se espera por un cambio que de sentido a la condicion 
		#ifdef mutex
		pthread_mutex_unlock(&pantalla);		//desbloquea la pantalla (seccion critica)
		#endif	
		
	}
	return NULL;
}


void handlectrlc(int sig) {
	parar = 1;
}


int main(void) {

	initscr();										    //inicializa la pantalla para ncurses
	curs_set(0);									    //elimina el cursor
	erase();											//limpia pantalla
	refresh();										    //refresca los cambion en la pantalla
	srand(time(NULL));						            //inicializa semilla de la funcion random

	signal(SIGINT, handlectrlc);
	
	
	
	#define NcaballoS (7)	//se define la cantidad de caballos (se puede modificar [2-7])
	pthread_t tids[NcaballoS];
	caballo_t caballos[NcaballoS];
	char *nombres[] = {"Caballo 1","Caballo 2","Caballo 3","Caballo 4" ,"Caballo 5","Caballo 6","Caballo 7"};
	
	
    for(int i =0; i < NcaballoS; i++) {
    	
		caballos[i].y = 1+ i*2;
		caballos[i].nombre = nombres[i%7];
     	pthread_create(tids+i, NULL, carrera,caballos + i);
     	}
	
	
	while(!parar) {											//inicio de la carrera
		printf("\033[16;1HPreparados...");
		sleep(1);
		puts("listos...");
		sleep(1);
		puts("fuera...");
		fprintf(stderr , "\033[2J" );
		ganador = NULL;											//se establece que no hay ganador, para iniciar la carrera
		EnSusMarcasListosFuera = 1;
		pthread_cond_broadcast(&condicion);
		#ifdef mutex
		pthread_mutex_lock(&pantalla);			//bloquea la pantalla (seccion critica)
		#endif 		
		while(ganador == NULL) pthread_cond_wait(&condicion,&pantalla);
		#ifdef mutex
		pthread_mutex_unlock(&pantalla);		//desbloquea la pantalla (seccion critica)
		#endif
	    printf("\033[16;1H%s es el ganador!\n", ganador->nombre); 			//se nombra el ganador
	    
	
		EnSusMarcasListosFuera = 0;
		pthread_cond_broadcast(&condicion);
		
		getch(); 										//cierra la pantalla de ncurses
		endwin();										//espera que se ingrese cualquier tecla para salir										
	}
												
}
