#include "proprietes.cpp"
#include "bateau.h"
#include "coordonnees.h"
#include "plateau.h"
#include "cli.h"
#include "unittest.h"
#include <stdlib.h>

int getTaille(Bateau* b) {
	return b->taille;
}
void setTaille(Bateau* b, int x) {
	b->taille = x;
}
int getTouches(Bateau* b) {
	return b->compteTouche;
}
void setTouches(Bateau* b, int x) {
	b->compteTouche = x;
}
void incTouches(Bateau* b) {
	setTouches(b, getTouches(b) + 1);
}
Bateau* getSuivant(Bateau* b) {
	return b->suivant;
}
Bateau* getPrecedent(Bateau* b) {
	return b->precedent;
}
void setSuivant(Bateau* b, Bateau* d) {
	b->suivant = d;
}
void setPrecedent(Bateau* b, Bateau* d) {
	b->precedent = d;
}
Coordonnee* getCoordonnee(Bateau* b, int i) {
	return &b->liste[i];
}
void setCoordonnee(Bateau* b, int i, Coordonnee* x) {
	b->liste[i] = *x;
}
Bateau* getPremier(BateauMGR* m) {
	return m->premier;
}
Bateau* getDernier(BateauMGR* m) {
	return m->dernier;
}
void setPremier(BateauMGR* m, Bateau* b) {
	m->premier = b;
}
void setDernier(BateauMGR* m, Bateau* b) {
	m->dernier = b;
}
int getCompteur(BateauMGR* m, int i) {
	return m->compteurs[i];
}
void setCompteur(BateauMGR* m, int i, int x) {
	m->compteurs[i] = x;
}
void incCompteur(BateauMGR* m, int i) {
	setCompteur(m, i, getCompteur(m, i) + 1);
}

// #####################################

int totalBateauxAutorises() {
	int i, x = 0;
	for(i = 0; i < TAILLES_MAX; i++)
		x += TAILLES[i];
	return x;
}

bool bateauAutorise(BateauMGR* m, int taille) {
	return bateauxRestantsAutorises(m, taille) > 0;
}

int bateauxRestantsAutorises(BateauMGR* m, int taille) {
	return TAILLES[taille - 1] - getCompteur(m, taille - 1);
}

void afficherBateauxRestantsAutorises(BateauMGR* m) {
	int i, x;
	afficher("Vous pouvez placer :\n");
	for(i = 0; i < TAILLES_MAX; i++)
		if((x = bateauxRestantsAutorises(m, i)) > 0) {
			// printf("%d bateaux de : %d cases \n", TAILLES[i], i + 1);
			afficher(" - ", x);
			afficher(" bateaux de : ", i, " cases.\n");
		}
}

void allouerManager(BateauMGR* m){
	int i;
	for(i = 0; i < TAILLES_MAX; i++)
		setCompteur(m, i, 0);
	setPremier(m, NULL);
	setDernier(m, NULL);
}
void detruireManager(BateauMGR* m){
	Bateau *b, *suivant = getPremier(m);
	while((b = suivant) != NULL) {
		suivant = getSuivant(b);
		detruireBateau(b);
	}
	setPremier(m, NULL);
	setDernier(m, NULL);
}

static void detruireBateau(Bateau* b){
	setPrecedent(b, NULL);
	setSuivant(b, NULL);
	free(b);
}

static Bateau* creerBateau(Coordonnee liste[], int taille) {
	int i;
	Bateau* b;
	b = (Bateau*) malloc(sizeof(Bateau));
	for(i = 0; i < taille; i++) {
		initCoordonnee(getCoordonnee(b, i), getY(&liste[i]), getX(&liste[i]));
	}
	setTaille(b, taille);
	setPrecedent(b, NULL);
	setSuivant(b, NULL);
	setTouches(b, 0);
	return b;
}

Bateau* trouverBateau(BateauMGR* m, Coordonnee* x){
	Bateau *b, *suivant = getPremier(m);
	while((b = suivant) != NULL) {
		suivant = getSuivant(b);
		if(bateauContient(b, x))
			return b;
	}
	return NULL;
}

static bool bateauContient(Bateau* b, Coordonnee* x) {
	int i;
	for(i = 0; i < getTaille(b); i++)
		if(egal(getCoordonnee(b, i), x))
			return true;
	return false;
}

bool toucherBateau(Bateau* b) {
	incTouches(b);
	return (getTouches(b) == getTaille(b));
}

static void ajouterBateau(BateauMGR* m, Bateau* b) {
	if(getPremier(m) == NULL) {
		setPremier(m, b);
		setDernier(m, b);
	} else {
		Bateau* queue = getDernier(m);
		setSuivant(queue, b);
		setPrecedent(b, queue);
		setDernier(m, b);
	}
}

void enregistrerBateau(BateauMGR* m, Coordonnee liste[], int taille) {
	ajouterBateau(m, creerBateau(liste, taille));
	incCompteur(m, taille - 1);
}

bool enregistrerTir(BateauMGR* m, Coordonnee* tir) {
	Bateau* b;
	if((b = trouverBateau(m, tir)) != NULL) 
		return toucherBateau(b);
	return false;
}

void testsBateaux() {

	Coordonnee liste[10];
	initSuiteCoordonnees(liste, 2, "B0 B4");

	Bateau *b = creerBateau(liste, 3);
	Bateau *b2 = creerBateau(liste, 5);

	afficher("\n###############################");
	afficher("\n### *** FICHIER BATEAU *** ###\n");

	afficher("\n # FONCTION getTaille()\n");
	assertEquals(getTaille(b), 3, "Bateau de taille 3 ");
	assertEquals(getTaille(b2), 5, "Bateau de taille 5 ");

	afficher("\n # FONCTION setTaille()\n");
	setTaille(b, 3);
	assertEquals(getTaille(b), 3, "taille = 3");
	setTaille(b, 5);
	assertEquals(getTaille(b), 5, "taille = 5");

	afficher("\n # FONCTION getTouche()\n");
	BateauMGR MonBateauMGR;
	BateauMGR *mgr = &MonBateauMGR;
	Bateau *MonBateau = creerBateau(liste, 3);
	allouerManager(mgr);
	ajouterBateau(mgr, MonBateau);
	Coordonnee t;
	Coordonnee *tir = &t;
	initCoordonnee(tir,"B1");
	enregistrerTir(mgr, tir);
	assertEquals(getTouches(MonBateau), 1,"Bateau touche a la case 'B1' ");

	detruireManager(mgr);
}







