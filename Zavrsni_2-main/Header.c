#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"

int brojNIgraca=0;

void upis(int bodovi) {
	FILE* fp = fopen("igraci.bin", "ab+");
	if (!fp) {
		perror("Ne mogu otvoriti datoteku");
		return;
	}

	IGRAC* igrac = malloc(sizeof(IGRAC));
	if (!igrac) {
		printf("Nema dovoljno memorije!\n");
		fclose(fp);
		return;
	}

	igrac->id = ++brojNIgraca;
	printf("Unesi ime: ");
	scanf("%49s", igrac->ime);
	printf("Unesi prezime: ");
	scanf("%49s", igrac->prezime);
	igrac->bodovi = bodovi;

	fwrite(igrac, sizeof(IGRAC), 1, fp);
	fclose(fp);
	free(igrac);
}

int ucitajPitanja(const char* filename, PITANJA* pitanja) {
	if (!filename || !pitanja) {
		printf("Neispravan pokazivač!\n");
		return 0;
	}
	
	FILE* file = fopen(filename, "r");
	if (!file) {
		perror("Ne mogu otvoriti datoteku");
		return 0;
	}

	char linija[100];
	int count = 0;

	while (fgets(linija, sizeof(linija), file) && count < 100) {
		pitanja[count].id = atoi(linija);

		fgets((pitanja + count)->pitanje, sizeof((pitanja + count)->pitanje), file);
		fgets((pitanja + count)->odgovorA, sizeof((pitanja + count)->odgovorA), file);
		fgets((pitanja + count)->odgovorB, sizeof((pitanja + count)->odgovorB), file);
		fgets((pitanja + count)->odgovorC, sizeof((pitanja + count)->odgovorC), file);
		fgets(linija, sizeof(linija), file);


		(pitanja + count)->pitanje[strcspn((pitanja + count)->pitanje, "\n")] = 0;
		(pitanja + count)->odgovorA[strcspn((pitanja + count)->odgovorA, "\n")] = 0;
		(pitanja + count)->odgovorB[strcspn((pitanja + count)->odgovorB, "\n")] = 0;
		(pitanja + count)->odgovorC[strcspn((pitanja + count)->odgovorC, "\n")] = 0;
		(pitanja + count)->Todgovor = linija[0];

		count++;
	}

	srand(time(NULL));

	for (int i = count - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		PITANJA temp = pitanja[i];
		pitanja[i] = pitanja[j];
		pitanja[j] = temp;
	}


	fclose(file);
	return count;
}

void ispisiSveIgraceRekurzivno(FILE* fp) {
	IGRAC igrac;
	if (fread(&igrac, sizeof(IGRAC), 1, fp) != 1) {
		return;
	}

	printf("%d. %s %s - Bodovi: %d\n", igrac.id, igrac.ime, igrac.prezime, igrac.bodovi);

	ispisiSveIgraceRekurzivno(fp);
}

void ispisiSveIgrace() {
	FILE* fp = fopen("igraci.bin", "rb");
	if (!fp) {
		perror("Ne mogu otvoriti datoteku igraci.bin");
		return;
	}

	printf("\nLista igraca:\n");
	printf("Ukupni broj novih igraca: %d\n\n", brojNIgraca);
	ispisiSveIgraceRekurzivno(fp);

	fclose(fp);
}

static int usporedi(const void* a, const void* b) {
	IGRAC* ia = (IGRAC*)a;
	IGRAC* ib = (IGRAC*)b;
	return ib->bodovi - ia->bodovi;
}

void ispisiTop5() {
	FILE* fp = fopen("igraci.bin", "rb");
	if (!fp) {
		perror("Ne mogu otvoriti datoteku igraci.bin");
		return;
	}

	
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	int brojIgraca = size / sizeof(IGRAC);
	rewind(fp);

	if (brojIgraca == 0) {
		printf("Nema zapisanih igraca.\n\n\n");
		fclose(fp);
		return;
	}

	IGRAC* igraci = malloc(size);
	if (!igraci) {
		printf("Nema dovoljno memorije.\n");
		fclose(fp);
		return;
	}

	fread(igraci, sizeof(IGRAC), brojIgraca, fp);
	fclose(fp);

	
	qsort(igraci, brojIgraca, sizeof(IGRAC), usporedi);

	
	int limit = brojIgraca < 5 ? brojIgraca : 5;
	printf("Top %d igraca po bodovima:\n", limit);
	for (int i = 0; i < limit; i++) {
		printf("%d. %s %s - %d bodova\n", i + 1, (igraci+i)->ime, (igraci+i)->prezime, (igraci+i)->bodovi);
	}

	free(igraci);
	igraci = NULL;
}

void obrisiIgracaPoID(int trazeniID) {
	FILE* fp = fopen("igraci.bin", "rb");
	if (!fp) {
		perror("Ne mogu otvoriti datoteku za citanje");
		return;
	}

	FILE* temp = fopen("temp.bin", "wb");
	if (!temp) {
		perror("Ne mogu otvoriti privremenu datoteku");
		fclose(fp);
		return;
	}

	IGRAC igrac;
	int pronadjen = 0;

	while (fread(&igrac, sizeof(IGRAC), 1, fp) == 1) {
		if (igrac.id != trazeniID) {
			fwrite(&igrac, sizeof(IGRAC), 1, temp);
		}
		else {
			pronadjen = 1;
		}
	}

	fclose(fp);
	fclose(temp);

	if (pronadjen) {
		remove("igraci.bin");
		rename("temp.bin", "igraci.bin");
		printf("Igrac s ID-om %d je obrisan.\n", trazeniID);
		brojNIgraca--;
	}
	else {
		remove("temp.bin");
		printf("Igrac s ID-om %d nije pronaden.\n", trazeniID);
	}
}

IGRAC* pronadjiIgracaPoID(int id) {
	FILE* fp = fopen("igraci.bin", "rb");
	if (!fp) {
		perror("Greska pri otvaranju datoteke");
		return NULL;
	}

	IGRAC* igrac = malloc(sizeof(IGRAC));
	if (!igrac) {
		printf("Nema dovoljno memorije!\n");
		fclose(fp);
		return NULL;
	}

	while (fread(igrac, sizeof(IGRAC), 1, fp) == 1) {
		if (igrac->id == id) {
			fclose(fp);
			return igrac;
		}
	}

	fclose(fp);
	free(igrac);
	return NULL;
}

void azurirajBodove(IGRAC* igracZaAzuriranje, int noviBodovi) {
	if (!igracZaAzuriranje) {
		printf("Igrac nije pronađen.\n");
		return;
	}

	FILE* fp = fopen("igraci.bin", "rb");
	FILE* temp = fopen("temp.bin", "wb");
	if (!fp || !temp) {
		perror("Greška pri otvaranju datoteke");
		if (fp) fclose(fp);
		if (temp) fclose(temp);
		return;
	}

	IGRAC igrac;

	while (fread(&igrac, sizeof(IGRAC), 1, fp) == 1) {
		if (igrac.id == igracZaAzuriranje->id) {
			igrac.bodovi = noviBodovi;
		}
		fwrite(&igrac, sizeof(IGRAC), 1, temp);
	}

	fclose(fp);
	fclose(temp);

	remove("igraci.bin");
	rename("temp.bin", "igraci.bin");

	igracZaAzuriranje->bodovi = noviBodovi;
	printf("Igrac %s %s (ID: %d) sada ima %d bodova.\n",
		igracZaAzuriranje->ime,
		igracZaAzuriranje->prezime,
		igracZaAzuriranje->id,
		igracZaAzuriranje->bodovi);
}

