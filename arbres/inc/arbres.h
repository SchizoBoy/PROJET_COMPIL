// Prototypes des fonctions relatives aux arbres
// Auteurs : Réalisé une fois par tous les membres du groupe

#ifndef ARBRES_H_
#define ARBRES_H_

typedef struct arbre{
  int numlex;
  int numdecl;
  int nature;
  int entier;
  double reel;
  struct arbre *fils_gauche;
  struct arbre *frere_droit;
}* arbre;

typedef struct tab_arbre{
  int numlex;
  int numdecl;
  int nature;
  int entier;
  double reel;
}tab_arbre;

// Crée un arbre vide
arbre creer_arbre_vide();

// Renvoie 1 si l'arbre est vide, 0 sinon
int est_vide(arbre a);

// Crée un noeud avec ses caractéristiques
arbre creer_noeud(int numlex, int numdecl, int nature, int val_e, double val_r);

// Concaténation du pere et du fils
arbre concat_pere_fils(arbre pere, arbre fils);

// Concaténation du pere et du frere
arbre concat_pere_frere(arbre pere, arbre fils);

// Permet d'afficher un arbre de dans le teminal
void afficher_arbre(arbre a);

// Permet de sauver un arbre sous la forme d'un tableau de ses noeuds (parcours
// préfixé)
void sauver_arbre(FILE *fic, arbre a);

// Permet de charger un arbre à partir d'un tableau contenant ses noeuds
// (parcours préfixé)
arbre charger_arbre(tab_arbre *t);

#endif
