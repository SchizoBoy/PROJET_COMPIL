// Fonctions auxiliaires utilisées dans le programme YACC

#include <stdlib.h>
#include <stdio.h>
#include "../inc/fct_aux_yacc.h"

// fonction permettant de déterminer combien et quels formats simples se
// trouvent dans une chaine de caractère
void format(char *str){
  char *ptr = str;

  tab_format[0] = 0;

  while(*ptr != '\0'){
    if(*ptr == '%'){
      if(*(ptr+1) == 'd'
      || *(ptr+1) == 'f'
      || *(ptr+1) == 'b'
      || *(ptr+1) == 'c'
      || *(ptr+1) == 's'
      ){
        if(tab_format[0] == MAX_FORMAT){  // Vérification du max de formats
          fprintf(stderr, "Erreur, trop de formats.\n");
          exit(-1);
        }
        tab_format[0]++;
        switch(*(ptr+1)){   // On rempli le tableau de format avec les formats
          case 'd':
            tab_format[tab_format[0]] = 0;
            break;
          case 'f':
            tab_format[tab_format[0]] = 1;
            break;
          case 'b':
            tab_format[tab_format[0]] = 2;
            break;
          case 'c':
            tab_format[tab_format[0]] = 3;
            break;
          case 's':
            tab_format[tab_format[0]] = 4;
            break;
          default :
            fprintf(stderr, "Erreur format...");
            exit(-1);
        }
      }
    }
    ptr++;
  }
}

// Initialise la pile de région
void init_pile_region(){
  pile_region[0] = 1; // Une région dans la pile pour le moment
  pile_region[1] = 0; // Region 0
}

// Empile une region
void empiler_pile_region(int region){
  if(pile_region[0] == MAX_REGION){ // Vérification du nombre de régions
    fprintf(stderr, "Erreur, trop de formats.\n");
    exit(-1);
  }
  pile_region[0]++;
  pile_region[pile_region[0]] = region;
}

// Dépile une région
int depiler_pile_region(){
  pile_region[0]--;
  return pile_region[pile_region[0]+1];
}

// Retourne la tête de la pile des regions
int tete_pile_region(){
  return pile_region[pile_region[0]];
}

// Renvoie 1 si la région est dans la pile des régions, 0 sinon
int est_dans_pile_region(int region){
  int i;

  for(i = 1; i<pile_region[0]+1; i++){ //On regarde chaque élément de la pile
    if(pile_region[i] == region){ // On regarde si region en fait partie
      return 1;
    }
  }

  return 0;
}
