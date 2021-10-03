%{
#include<stdlib.h>
#include<stdio.h>

char *yytext;
int yylex();
int yyerror();

extern int nb_ligne;

int syntaxe_correcte = 1;

%}

%token PROG DEBUT FIN
%token POINT_VIRGULE DEUX_POINTS CROCHET_OUVRANT CROCHET_FERMANT OPAFF
%token EGAL DIFFERENT SUP SUP_EGAL INF INF_EGAL
%token PARENTHESE_OUVRANTE PARENTHESE_FERMANTE POINT_POINT VIRGULE POINT
%token TYPE STRUCT FSTRUCT TABLEAU
%token ENTIER REEL BOOLEEN CARACTERE CHAINE
%token CSTE_ENTIERE CSTE_REELLE CSTE_CHAINE CSTE_CARACTERE
%token VARIABLE IDF
%token SI ALORS SINON PROCEDURE FONCTION RETOURNE TANT_QUE FAIRE DE
%token VIDE
%token ET OU NON
%token PLUS MOINS MULT DIV MODULO
%token TRUE FALSE

%%
programme : PROG corps

corps : liste_declarations liste_instructions
      | liste_instructions
      ;

liste_declarations : declaration
                   | liste_declarations declaration
                   ;

liste_instructions : DEBUT suite_liste_inst FIN
                   ;

suite_liste_inst : instruction
                 | suite_liste_inst instruction
                 ;

declaration : declaration_type
            | declaration_variable POINT_VIRGULE
            | declaration_procedure
            | declaration_fonction
            ;

declaration_type : TYPE IDF DEUX_POINTS suite_declaration_type
                 ;

suite_declaration_type : STRUCT liste_champs FSTRUCT
                       | TABLEAU dimension DE nom_type
                       ;

dimension : CROCHET_OUVRANT liste_dimensions CROCHET_FERMANT
          ;

liste_dimensions : une_dimension
                 | liste_dimensions VIRGULE une_dimension
                 ;

une_dimension : expression POINT_POINT expression
              ;

liste_champs : un_champ
             | liste_champs POINT_VIRGULE un_champ
             ;

un_champ : IDF DEUX_POINTS nom_type
         ;

nom_type : type_simple
         | IDF
         ;

type_simple : ENTIER
            | REEL
            | BOOLEEN
            | CARACTERE
            | CHAINE CROCHET_OUVRANT CSTE_ENTIERE CROCHET_FERMANT
            ;

declaration_variable  : VARIABLE IDF DEUX_POINTS nom_type
                      ;

declaration_procedure : PROCEDURE IDF liste_parametres corps
                      ;

declaration_fonction  : FONCTION IDF liste_parametres RETOURNE type_simple corps
                      ;

liste_parametres : PARENTHESE_OUVRANTE liste_param PARENTHESE_FERMANTE
                 |
                 ;

liste_param : un_param
            | liste_param POINT_VIRGULE un_param
            ;

un_param : IDF DEUX_POINTS type_simple
         ;

instruction : affectation POINT_VIRGULE
            | condition
            | tant_que
            | appel POINT_VIRGULE
            | VIDE POINT_VIRGULE
            | RETOURNE resultat_retourne POINT_VIRGULE
            ;

resultat_retourne : un_arg
                  |
                  ;

appel : IDF liste_arguments
      ;

liste_arguments : PARENTHESE_OUVRANTE liste_args PARENTHESE_FERMANTE
                ;

liste_args : un_arg
           | liste_args VIRGULE un_arg
           |
           ;

un_arg : expression_booleenne
       ;

condition : SI expression_booleenne
            ALORS liste_instructions
            |
            SI expression_booleenne
            ALORS liste_instructions
            SINON liste_instructions
          ;

tant_que : TANT_QUE expression_booleenne FAIRE liste_instructions

affectation : variable OPAFF expression_booleenne
            ;

variable : IDF corps_variable
         ;

corps_variable : CROCHET_OUVRANT indice_expr_tab CROCHET_FERMANT corps_variable
               | POINT variable
               |
               ;

indice_expr_tab : e1_tab
                ;

e1_tab : e1_tab PLUS e2_tab
       | e1_tab MOINS e2_tab
       | e2_tab
       ;

e2_tab : e2_tab MULT e3_tab
       | e2_tab DIV e3_tab
       | e3_tab
       ;

e3_tab : PARENTHESE_OUVRANTE e1_tab PARENTHESE_FERMANTE
       | CSTE_ENTIERE
       | variable
       | appel
       ;

expression_booleenne : e1_bool
                     ;

e1_bool : e1_bool OU e2_bool
        | e2_bool
        ;

e2_bool : e2_bool ET e3_bool
        | e3_bool
        ;

e3_bool : NON e3_bool
        | PARENTHESE_OUVRANTE e1_bool PARENTHESE_FERMANTE
        | un_booleen
        ;

un_booleen : comparaison
           | expression
           | TRUE
           | FALSE
           ;

comparaison : expression operateur_comp expression
            ;

operateur_comp : EGAL
               | DIFFERENT
               | SUP
               | SUP_EGAL
               | INF
               | INF_EGAL
               ;

expression : e1
           ;

e1 : e1 PLUS e2
   | e1 MOINS e2
   | e2
   ;

e2 : e2 MULT e3
   | e2 DIV e3
   | e2 MODULO e3
   | e3
   ;

e3 : PARENTHESE_OUVRANTE e1 PARENTHESE_FERMANTE
   | CSTE_ENTIERE
   | CSTE_REELLE
   | CSTE_CARACTERE
   | CSTE_CHAINE
   | variable
   | appel
   ;
%%

int yyerror(){
  printf("\nErreur de syntaxe ligne %d : %s\n\n", nb_ligne, yytext);
  syntaxe_correcte = 0;
}

int main(){
  yyparse();

  if(syntaxe_correcte){
    printf("\nSYNTAXE CORRECTE\n\n");
  }
}
