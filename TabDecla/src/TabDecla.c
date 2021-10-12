#include <stdio.h>
#include <stdlib.h>
#include "../../TabLexico/inc/TabLexico.h"
#include "../inc/TabDecla.h"
#define MAX_TAB_DECLA 3000


tabDecla TableDeclaration[MAX_TAB_DECLA];

/*Initialise la table des déclarations*/
void init_tab_decla(){
  int i;
  for(i=0; i<MAX_TAB_DECLA; i++){
    TableDeclaration[i].nature = -1;
    TableDeclaration[i].suivant = -1;
    TableDeclaration[i].num_region=-1;
    TableDeclaration[i].description = -1;
    TableDeclaration[i].exec=-1;
  }
}


/*-----------------------------------------------------------------------------
  Utilité fonction : Retourne le numéro dans la table des déclaration du lexème courant
  Paramètres : - num_lexico : numéro lexicographique du lexeme courant
              - nature
              - num_region
              - num_represention_type : (Variable ou paramatère) numéro de déclaration du type du lexème
               courant OU (procédure, fonction, structure, tableau) l'indice dans
               la table des représentation
              - nb_ligne : numéro de la ligne à laquelle on est.
 -----------------------------------------------------------------------------*/
int inserer_tab_declaration(int num_lexico, int nature, int num_region, int num_represention_type, int nb_ligne){
    int i;
    int num_decla;

    /*-------------------------------------------------------------------------
      On va déterminer si le lexème va dans la table primaire, ou dans la zone
      de débordement
    -------------------------------------------------------------------------*/



    if(TableDeclaration[num_lexico].nature == -1){ /*Si il n'y a jamais encore eu
                                                   de lexème identiques insérés
                                                   dans la table*/
      num_decla = num_lexico; /*On va donc insérer les informations du lexème
                              à l'indice num_lexico*/

    }else{ /*Sinon, on va cherche la premier case vide dans la zone de débordement*/
      /*Attention surchage possible..*/
      /*On vérifie que l'élément de la table primaire, qui est le même lexeme, n'est
      pas déclaré sous la même nature, et dans la même région*/
      if((TableDeclaration[num_lexico].num_region == num_region) && (TableDeclaration[num_lexico].nature == nature)){
        switch (nature) {
          case TYPE_STRUCT:
            printf("Problème de sémantique ligne %d : structure de même nom déjà défini dans cette région\n", nb_ligne);
            break;
          case TYPE_TAB:
            printf("Problème de sémantique ligne %d : tableau de même nom déjà défini dans cette région\n", nb_ligne);
            break;
          case VAR:
            printf("Problème de sémantique ligne %d : variable de même nom déjà défini dans cette région\n", nb_ligne);
            break;
          case PARAMETRE:
            printf("Problème de sémantique ligne %d : paramatère de la fonction de même nom déjà défini dans cette région\n", nb_ligne);
            break;
          case PROC:
            printf("Problème de sémantique ligne %d : procédure de même nom déjà défini dans cette région\n", nb_ligne);
            break;
          case FCT:
            printf("Problème de sémantique ligne %d : fonction de même nom déjà défini dans cette région\n", nb_ligne);
            break;
          default:
            exit(-1);
            break;
          }
      }


      i = MAX_TAB_LEX;  /*Donne le premier indice de la zone de débordement*/
      while(TableDeclaration[i].nature != -1){ /*Tant que la case i n'est pas libre*/
        /*Gestion de la surcharge possible*/
        /*On vérifie que l'élément i, qui est le même lexème, n'est pas déclaré
        dans la même région ET à la même nature*/


        if((TableDeclaration[i].num_region == num_region) && (TableDeclaration[i].nature == nature)){
          switch (nature) {
            case TYPE_STRUCT:
              printf("Problème de sémantique ligne %d : structure de même nom déjà défini dans cette région\n", nb_ligne);
              break;
            case TYPE_TAB:
              printf("Problème de sémantique ligne %d : tableau de même nom déjà défini dans cette région\n", nb_ligne);
              break;
            case VAR:
              printf("Problème de sémantique ligne %d : variable de même nom déjà défini dans cette région\n", nb_ligne);
              break;
            case PARAMETRE:
              printf("Problème de sémantique ligne %d : paramatère de la fonction de même nom déjà défini dans cette région\n", nb_ligne);
              break;
            case PROC:
              printf("Problème de sémantique ligne %d : procédure de même nom déjà défini dans cette région\n", nb_ligne);
              break;
            case FCT:
              printf("Problème de sémantique ligne %d : fonction de même nom déjà défini dans cette région\n", nb_ligne);
              break;
            default:
              exit(-1);
              break;
            }
        }
        i++;
      }
      num_decla = i; /*Première case libre trouvé dans la zone de débordement*/

      /*-----------------------------------------------------------------------
        On remet à jour le chainage
        ----------------------------------------------------------------------*/
      i = num_lexico;
      while(TableDeclaration[i].suivant != -1){ /*On regarde si il y a un suivant*/
        i = TableDeclaration[i].suivant;
      }
      TableDeclaration[i].suivant = num_decla;
    }

    /*-------------------------------------------------------------------------
      On insère le numéro de la région où se trouve le lexème courant
    --------------------------------------------------------------------------*/
    TableDeclaration[num_decla].num_region = num_region;

    /*-------------------------------------------------------------------------
       On insère la nature du lexème courant
       -----------------------------------------------------------------------*/
    switch (nature) {
      case TYPE_STRUCT:
        TableDeclaration[num_decla].nature = 1;
        break;
      case TYPE_TAB:
        TableDeclaration[num_decla].nature = 2;
        break;
      case VAR:
        TableDeclaration[num_decla].nature = 3;
        break;
      case PARAMETRE:
        TableDeclaration[num_decla].nature = 4;
        break;
      case PROC:
        TableDeclaration[num_decla].nature = 5;
        break;
      case FCT:
        TableDeclaration[num_decla].nature = 6;
        break;
      default:
        printf("Problème de nature du lexeme dans la table des déclarations\n");
        exit(-1);
        break;
      }

    /*------------------------------------------------------------------------
       On détermine les valeurs des champs descriptions et exec suivant la
       nature
       ----------------------------------------------------------------------*/

    if((nature == TYPE_STRUCT) || (nature == TYPE_TAB)){
      TableDeclaration[num_decla].description = num_represention_type;
      TableDeclaration[num_decla].exec = -1;  /*laisse vide pour le moment*/

    }else if((nature == VAR) || (nature == PARAMETRE)){
      TableDeclaration[num_decla].description = num_represention_type;
      TableDeclaration[num_decla].exec = -1;  /*laisse vide pour le moment*/

    }else if((nature == PROC) || (nature == FCT)){
      TableDeclaration[num_decla].description = num_represention_type;
      TableDeclaration[num_decla].exec = -1;  /*laisse vide pour le moment*/

    }else{
      printf("Problème dans la nature du lexeme dans la table des déclarations\n");
      exit(-1);
    }
    return num_decla;
}

/*Affiche la table des déclarations*/
 void afficher_tab_declaration(){
   int i = 0;
   printf("\n-----------------TABLE DES DECLARATIONS----------------------\n");
   printf("  Nature    |  Indice du suivant   |  Région   |   Description   |  Exécution  \n" );
   while(i<30){
     switch (TableDeclaration[i].nature) {
       case TYPE_STRUCT:
         printf("TYPE_STRUCT |         %d           |     %d     |        %d        |  %d  \n", TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case TYPE_TAB:
         printf("  TYPE_TAB  |          %d          |     %d     |        %d        |  %d  \n", TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case VAR:
         printf("  VARIABLE  |          %d          |     %d     |        %d        |  %d  \n", TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case PARAMETRE:
         printf(" PARAMETRE  |          %d          |     %d     |         %d         |  %d  \n", TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case PROC:
         printf("  PROCEDURE |          %d          |     %d     |         %d        |  %d \n", TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case FCT:
         printf("  FONCTION  |          %d          |     %d     |        %d        |  %d  \n", TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       default:
        printf("           |                      |            |                   |     \n");
         break;
       }
       i++;

   }
}