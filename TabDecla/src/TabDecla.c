#include <stdio.h>
#include <stdlib.h>
#include "../../TabLexico/inc/TabLexico.h"
#include "../../TabRepresentation/inc/TabRepresentation.h"
#include "../inc/TabDecla.h"
#include "../../inc/fct_aux_yacc.h"

tabDecla TableDeclaration[MAX_TAB_DECLA];
int indice_tab_debordement = MAX_TAB_LEX;

/*Initialise la table des déclarations*/
void init_tab_decla(){
  int i;

  /*Initialisation des types de bases*/
  TableDeclaration[0].nature = TYPE_BASE;
  TableDeclaration[0].suivant = -1;
  TableDeclaration[0].num_region = 0;
  TableDeclaration[0].description = -1;
  TableDeclaration[0].exec = 1;

  TableDeclaration[1].nature = TYPE_BASE;
  TableDeclaration[1].suivant = -1;
  TableDeclaration[1].num_region = 0;
  TableDeclaration[1].description = -1;
  TableDeclaration[1].exec = 1;

  TableDeclaration[2].nature = TYPE_BASE;
  TableDeclaration[2].suivant = -1;
  TableDeclaration[2].num_region = 0;
  TableDeclaration[2].description = -1;
  TableDeclaration[2].exec = 1;

  TableDeclaration[3].nature = TYPE_BASE;
  TableDeclaration[3].suivant = -1;
  TableDeclaration[3].num_region = 0;
  TableDeclaration[3].description = -1;
  TableDeclaration[3].exec = 1;

  for(i=4; i<MAX_TAB_DECLA; i++){
    TableDeclaration[i].nature = -1;
    TableDeclaration[i].suivant = -1;
    TableDeclaration[i].num_region= -1;
    TableDeclaration[i].description = -1;
    TableDeclaration[i].exec= -1;
  }
}

/*-----------------------------------------------------------------------------
  Utilité fonction : Retourne le numéro dans la table des déclaration du lexème
                    courant
  Paramètres : - num_lexico : numéro lexicographique du lexeme courant
              - nature
              - num_region_engendree : (Procédure ou fonction) numéro de la
              région engendrée, -1 pour les autres nature.
              - num_region
              - num_represention_type : (Variable ou paramatère) numéro de
                déclaration du type du lexème
               courant OU (procédure, fonction, structure, tableau) l'indice dans
               la table des représentation
              - nb_ligne : numéro de la ligne à laquelle on est.
 -----------------------------------------------------------------------------*/
int inserer_tab_declaration(int num_lexico, int nature,
                           int num_region, int num_represention_type, int nb_ligne){
    int i;
    int num_declaration;
    int taille;
    int nb_dim;
    int nb_champs;

    /*-------------------------------------------------------------------------
      On va déterminer si le lexème va dans la table primaire, ou dans la zone
      de débordement
    -------------------------------------------------------------------------*/
    if(TableDeclaration[num_lexico].nature == -1){ /*Si il n'y a jamais encore eu
                                                   de lexème identiques insérés
                                                   dans la table*/
      num_declaration = num_lexico; /*On va donc insérer les informations du lexème
                              à l'indice num_lexico*/

    }else{ /*Sinon, on va dans la zone de débordement*/

      /*Attention surchage possible..*/
      /*On vérifie qu'il n'y ai pas d'element portant le même lexème et étant
      de même type dans la même région */
        if(nature == TYPE_STRUCT || nature == TYPE_TAB){ //Pour les types
          if(num_decla(num_lexico, TYPE_STRUCT, num_region) != -1 || num_decla(num_lexico, TYPE_TAB, num_region) != -1){
            return -1;
          }
        }else if(nature == PARAMETRE || nature == VAR){ //Pour les variables et paramètres
          if(num_decla(num_lexico, PARAMETRE, num_region) != -1 || num_decla(num_lexico, VAR, num_region) != -1){
            return -1;
          }
        }else{ //Pour le reste
          if(num_decla(num_lexico, nature, num_region) != -1){
            return -1;
          }
        }

        num_declaration = indice_tab_debordement;
        indice_tab_debordement += 1;


      /*-----------------------------------------------------------------------
        On remet à jour le chainage
        ----------------------------------------------------------------------*/
      i = num_lexico;
      while(TableDeclaration[i].suivant != -1){ /*On regarde si il y a un suivant*/
        i = TableDeclaration[i].suivant;
      }
      TableDeclaration[i].suivant = num_declaration;

    }
    /*-------------------------------------------------------------------------
      On insère le numéro de la région où se trouve le lexème courant
    --------------------------------------------------------------------------*/
    TableDeclaration[num_declaration].num_region = num_region;

    /*-------------------------------------------------------------------------
       On insère la nature du lexème courant
       -----------------------------------------------------------------------*/
    switch (nature) {
      case TYPE_STRUCT:
        TableDeclaration[num_declaration].nature = 1;

        /*On calcule la taille de la structure*/
        nb_champs = valeur_tab_representation(num_represention_type);
        taille = 0;

        for(i=1; i<nb_champs*3; i = i+3){
            /*On ajoute la taille du type de chaque champs*/
            taille = taille + valeur_exec_tab_decla((valeur_tab_representation(num_represention_type + i)));
        }

        break;
      case TYPE_TAB:
        TableDeclaration[num_declaration].nature = 2;

        /*On calcule la taille du tableau*/
        taille = valeur_exec_tab_decla(valeur_tab_representation(num_represention_type)); //Taille du type des éléments du tableau
        nb_dim = valeur_tab_representation(num_represention_type + 1);

        for(i=2; i<nb_dim*2 + 1; i= i+2){
          taille = taille*(valeur_tab_representation(num_represention_type +i +1) - valeur_tab_representation(num_represention_type + i ) +1 );
        }

        break;
      case VAR:
        TableDeclaration[num_declaration].nature = 3;
        break;
      case PARAMETRE:
        TableDeclaration[num_declaration].nature = 4;
        break;
      case PROC:
        TableDeclaration[num_declaration].nature = 5;
        break;
      case FCT:
        TableDeclaration[num_declaration].nature = 6;
        break;
      default:
        fprintf(
          stderr,
          "Problème dans la nature du lexeme dans la table des déclarations."
        );
        exit(-1);
        break;
      }

    /*------------------------------------------------------------------------
       On détermine les valeurs des champs descriptions et exec suivant la
       nature
       ----------------------------------------------------------------------*/

    if((nature == TYPE_STRUCT) || (nature == TYPE_TAB)){
      TableDeclaration[num_declaration].description = num_represention_type;
      TableDeclaration[num_declaration].exec = taille;  /*Taille de la structure, ou du tableau*/

    }else if((nature == VAR) || (nature == PARAMETRE)){
      TableDeclaration[num_declaration].description = num_represention_type;
      TableDeclaration[num_declaration].exec = -1;  /*laisse vide pour le moment*/

    }else if((nature == PROC) || (nature == FCT)){
      TableDeclaration[num_declaration].description = num_represention_type;
      TableDeclaration[num_declaration].exec = -1;  /*laisse vide pour le moment*/

    }else{
      fprintf(
        stderr,
        "Problème dans la nature du lexeme dans la table des déclarations."
      );
      exit(-1);
    }
    return num_declaration;
}

/*----------------------------------------------------------------------------
 Utilité : Rempli le champs exec pour une fonction/procédure avec le num_region_engendree
  Paramètre :  num_region_engendree : numéro de la région engendrée par la
              prodédure/fonction en question.
 ----------------------------------------------------------------------------- */
 void inserer_exec_tab_decla(int num_decla, int num_region_engendree){
   TableDeclaration[num_decla].exec = num_region_engendree;
 }



/*----------------------------------------------------------------------------
 Utilité : Retourne le champs décription à l'indice donnée dans la table des
          déclarations.
  Paramètre : - indice : indice en question.
 ----------------------------------------------------------------------------- */
 int valeur_description_tab_decla(int indice){
   return TableDeclaration[indice].description;
 }

/*Affiche la table des déclarations*/
 void afficher_tab_declaration(){
   int i = 0;
   printf("\n######################################   TABLE DES DECLARATIONS   ##############################################\n");
   printf("/--------------------------+------------+-------------+------------+-----------+-----------------+-------------\\\n");
   printf("|          Lexeme          |   Indice   |   Nature    |  Suivant   |  Région   |   Description   |  Exécution  |\n" );
   printf("+--------------------------+------------+-------------+------------+-----------+-----------------+-------------+\n" );
   while(i<50){
     switch (TableDeclaration[i].nature) {
       case TYPE_BASE :
         printf("|    %-21s |     %-6d |  TYPE_BASE  |     %-6d |     %-5d |        %-8d |      %-6d |\n", lexeme(i),i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case TYPE_STRUCT:
         printf("|    %-21s |     %-6d | TYPE_STRUCT |     %-6d |     %-5d |        %-8d |      %-6d |\n", lexeme(i),i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case TYPE_TAB:
         printf("|    %-21s |     %-6d |  TYPE_TAB   |     %-6d |     %-5d |        %-8d |      %-6d |\n",lexeme(i),i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case VAR:
         printf("|    %-21s |     %-6d |  VARIABLE   |     %-6d |     %-5d |        %-8d |      %-6d |\n",lexeme(i),i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case PARAMETRE:
         printf("|    %-21s |     %-6d |  PARAMETRE  |     %-6d |     %-5d |        %-8d |      %-6d |\n",lexeme(i),i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case PROC:
         printf("|    %-21s |     %-6d |  PROCEDURE  |     %-6d |     %-5d |        %-8d |      %-6d |\n",lexeme(i),i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case FCT:
         printf("|    %-21s |     %-6d |  FONCTION   |     %-6d |     %-5d |        %-8d |      %-6d |\n",lexeme(i),i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       default:
         printf("|                          |            |             |            |           |                 |             |\n");
         break;
       }
       i++;

   }
   printf("\\--------------------------+------------+-------------+------------+-----------+-----------------+-------------/\n" );

   printf("\n########################################   ZONE DE DEBORDEMENT   ###############################################\n");
   printf("/--------------------------+------------+-------------+------------+-----------+-----------------+-------------\\\n");
   printf("|                          |   Indice   |   Nature    |  Suivant   |  Région   |   Description   |  Exécution  |\n" );
   printf("+--------------------------+------------+-------------+------------+-----------+-----------------+-------------+\n" );
   i = 500;
   while(i<520){
     switch (TableDeclaration[i].nature) {
       case TYPE_STRUCT:
         printf("|                          |    %-7d | TYPE_STRUCT |     %-6d |     %-5d |        %-8d |      %-6d |\n", i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case TYPE_TAB:
         printf("|                          |    %-7d |  TYPE_TAB   |     %-6d |     %-5d |        %-8d |      %-6d |\n", i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case VAR:
         printf("|                          |    %-7d |  VARIABLE   |     %-6d |     %-5d |        %-8d |      %-6d |\n", i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case PARAMETRE:
         printf("|                          |    %-7d |  PARAMETRE  |     %-6d |     %-5d |        %-8d |      %-6d |\n", i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case PROC:
         printf("|                          |    %-7d |  PROCEDURE  |     %-6d |     %-5d |        %-8d |      %-6d |\n", i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       case FCT:
         printf("|                          |    %-7d |  FONCTION   |     %-6d |     %-5d |        %-8d |      %-6d |\n", i, TableDeclaration[i].suivant,TableDeclaration[i].num_region, TableDeclaration[i].description, TableDeclaration[i].exec);
         break;
       default:
         printf("|                          |            |             |            |           |                 |             |\n");
         break;
       }
       i++;
   }
   printf("\\--------------------------+------------+-------------+------------+-----------+-----------------+-------------/\n" );
}

/*----------------------------------------------------------------------------
  Utilité : Renvoie le numéro de déclaration du lexème si il est déclaré, -1
  sinon.
  Paramatères : - num_lexico : numéro du lexème en question.
                - nature : nature du lexème en question (si c'est une procédure,
               fonction, ...)
               - region_particuliere : deux cas :
                  * egal à -1, dans ce cas on cherche le numéro de déclaration
                    du lexème (si il est déclaré) dans n'importe quelle région
                    présente dans la pile des régions.
                  * égal à la région courante , dans ce cas, on regarde si le
                  le lexème est déjà déclaré dans cette région particuliere;

  ----------------------------------------------------------------------------*/
int num_decla(int num_lexico, int nature, int region_particuliere){
  int num_decla = -1;
  int derniere_region;/*Region la plus proche de la région courante*/
  int chainage = num_lexico; /*Début du chainage*/

  if(region_particuliere == -1){
    derniere_region = -1; /*On part de rien*/
  }else{
    derniere_region = region_particuliere;
  }

  /*Tant qu'il existe une déclaration de même numéro lexicographique*/
  while((chainage != -1) && (TableDeclaration[chainage].nature != -1)){
    /*On vérifie si les natures des déclarations sont les mêmes*/
    if(TableDeclaration[chainage].nature == nature){
      /*On vérifie ensuite si la région de la déclaration en court d'examen est
      dans la pile des régions*/
      if(est_dans_pile_region(TableDeclaration[chainage].num_region) == 1){ /*si oui*/
        /*On sauvage ce numéro de déclaration si derniere_region est plus petit
        que*/
        if(TableDeclaration[chainage].num_region >= derniere_region){
          derniere_region = TableDeclaration[chainage].num_region;
          num_decla = chainage;
        }
      }
    }
    chainage = TableDeclaration[chainage].suivant;
  }

  return num_decla;
}

// Donne le numéro de déclaration d'une variable
// En fonction de sont numéro lexicographique, sa nature et sa région
// Renvoie le susdit numero ou -1 si cette variable n'existe pas
int lex2decl(int numlex, int nature, int region){
  int chaine = numlex;

  while (
    chaine != -1 &&
    !(TableDeclaration[chaine].nature == nature &&
    TableDeclaration[chaine].num_region == region)
  ) {
    chaine = TableDeclaration[chaine].suivant;
  }

  return chaine;
}

// Donne le numéro lexicagraphique d'une variable
// En fonction de sont numéro de déclaration
// Renvoie le susdit numero ou -1 si cette variable n'existe pas
int decl2lex(int num_decla){
  int i;
  int suivant;

  if(num_decla < MAX_TAB_LEX){
    return num_decla;
  }

  suivant = num_decla;

  for(i=0; i< MAX_TAB_DECLA; i++){
    if(TableDeclaration[i].suivant == suivant){
      return decl2lex(i);
    }
  }
  return -12;
}

/*----------------------------------------------------------------------------
 Utilité : Renvoie le nom de la région num_reg
  Paramètre : - num_reg : le numéro de la région en question
 ----------------------------------------------------------------------------- */
 char * tab_decla_region(int num_reg){
   int i =0;
   if(num_reg == 0){
     return "programme principal";
   }
   
   while(i != MAX_TAB_DECLA){
     if(TableDeclaration[i].nature == FCT || TableDeclaration[i].nature == PROC){
       if(TableDeclaration[i].exec == num_reg){
         return lexeme(decl2lex(i));
       }
     }
     i++;
   }
   return "programme principal";
 }




/*----------------------------------------------------------------------------
 Utilité : Renvoie le numéro de région d'une certaine déclaration
  Paramètre : - num_decla : numéro de déclaration en question
 ----------------------------------------------------------------------------- */
int region(int num_decla){
  return TableDeclaration[num_decla].num_region;
}

/*----------------------------------------------------------------------------
 Utilité : Renvoie la nature d'une certaine déclaration
  Paramètre : - num_decla : numéro de déclaration en question
 ----------------------------------------------------------------------------- */
int nature(int num_decla){
  return TableDeclaration[num_decla].nature;
}

/*----------------------------------------------------------------------------
 Utilité : Renvoie le champs execution d'une certaine déclaration
  Paramètre : - num_decla : numéro de déclaration en question
 ----------------------------------------------------------------------------- */
int valeur_exec_tab_decla(int num_decla){
  return TableDeclaration[num_decla].exec;
}

/*----------------------------------------------------------------------------
 Utilité :  Renvoie le numéro de déclaration d'une variable
  Paramètre : - numlex : numéro léxicographique du lexeme
 ----------------------------------------------------------------------------- */
int num_decla_variable(int numlex){
  // On recherche à quoi correspond la variable
  int num_decla_param = num_decla(numlex,PARAMETRE,-1);
  int num_decla_var = num_decla(numlex,VAR,-1);

  if(num_decla_var == -1 && num_decla_param == -1){
    return -1;
  }
  else if(num_decla_var > num_decla_param){
    return num_decla_var;           // VARIABLE SIMPLE
  }
  else{
    return num_decla_param;         // PARAMETRE
  }
  return -1;
}

/*----------------------------------------------------------------------------
 Utilité :  Renvoie le numéro de déclaration d'un type
  Paramètre : - numlex : numéro léxicographique du lexeme
 ----------------------------------------------------------------------------- */
int num_decla_type(int numlex){
  // On recherche à quoi correspond le type
  int num_decla_type_s = num_decla(numlex,TYPE_STRUCT,-1);
  int num_decla_type_t = num_decla(numlex,TYPE_TAB,-1);

  if(numlex < 4 && numlex !=-1){
    return numlex; // Type de base
  }else{
    if(num_decla_type_s == -1 && num_decla_type_t == -1){
      return -1;
    }else if(num_decla_type_s == -1){
      return num_decla_type_t;
    }else if(num_decla_type_t == -1){
      return num_decla_type_s;
    }else {
      if(region(num_decla_type_s) > region(num_decla_type_t)){
        return num_decla_type_s;           // Type structure
      }
      else{
        return num_decla_type_t;         // Type tableau
      }
  }

}
return -1;
}

/*----------------------------------------------------------------------------
 Utilité :  Renvoie 1 si le type correspondant à ce numéro lexico est un type
 de base, 0 sinon.
  Paramètre : - numlex : numéro léxicographique du rype
 ----------------------------------------------------------------------------- */
int type_base(int num_lexico){
  if(num_lexico < 4 && num_lexico != -1){
    return 1;
  }else{
    return 0;
  }
}


// Charge la table des déclarations à partir du texte intermédiaire
void charger_table_decla(FILE *fic){
  int i = 0, retour = 0;

  // table decla (zone normale)
  do{
    retour = fscanf(
      fic,
      "%d|%d|%d|%d|%d|",
      &TableDeclaration[i].nature,
      &TableDeclaration[i].suivant,
      &TableDeclaration[i].num_region,
      &TableDeclaration[i].description,
      &TableDeclaration[i].exec
    );
    i++;
  }while(retour != -1 && TableDeclaration[i-1].nature != -1);

  i = 500;
  // table decla (zone débordement)
  do{
    retour = fscanf(
      fic,
      "%d|%d|%d|%d|%d|",
      &TableDeclaration[i].nature,
      &TableDeclaration[i].suivant,
      &TableDeclaration[i].num_region,
      &TableDeclaration[i].description,
      &TableDeclaration[i].exec
    );
    i++;
  }while(retour != -1 && TableDeclaration[i-1].nature != -1);

  for(i = 0; i < MAX_TAB_DECLA; i++){
    if(TableDeclaration[i].nature == -99){
      TableDeclaration[i].nature = -1;
    }
  }
}

// Renvoie le numéro de déclaration de la fct/proc représentant une région
// donnée, -1 sinon
int num_decl_reg(int num_region){
 int i = 0, num_decl = -1;
 while(i < MAX_TAB_DECLA && num_decl == -1){
   if(valeur_exec_tab_decla(i) == num_region
   && (nature(i) == PROC || nature(i) == FCT)){
     num_decl = i;
   }
   i++;
 }
 return num_decl;
}
