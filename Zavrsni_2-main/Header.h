#ifndef HEADER_H
#define HEADER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_IME 50
#define MAX_PITANJE 100

extern int brojNIgraca;

typedef struct {
	int id;
	char ime[MAX_IME];
	char prezime[MAX_IME];
	int bodovi;
}IGRAC;

typedef struct {
	int id;
	char pitanje[MAX_PITANJE];
	char odgovorA[MAX_PITANJE];
	char odgovorB[MAX_PITANJE];
	char odgovorC[MAX_PITANJE];
	char Todgovor;
}PITANJA;

void upis(int brojBodova);
int ucitajPitanja(const char* filename, PITANJA pitanja[]);
void ispisiSveIgrace();
static int usporedi(const void* a, const void* b);
void ispisiTop5();
void obrisiIgracaPoID(int trazeniID);
IGRAC* pronadjiIgracaPoID(int id);
void azurirajBodove(IGRAC* igracZaAzuriranje, int noviBodovi);

#endif 