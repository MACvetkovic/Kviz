#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Header.h"

#define POKRENI_KVIZ 1
#define TABLICA 2
#define ISPISI_IGRACE 3
#define OBRISI_IGRACA 4
#define AZURIRAJ_IGRACA 5
#define IZLAZ 6

int main() {
	int Odabir_opcije,z=0,bodovi=0,id,novibodovi;
	srand(time(NULL));
	PITANJA* pitanja = malloc(100 * sizeof(PITANJA));
	if (pitanja == NULL) {
		printf("Greška: pokazivač je NULL!\n");
		return 0;
	}

	do {
		printf("====================");
		printf("Odaberite jednu od ponudenih opcija:");
		printf("====================\n");
		printf("\t\tOpcija 1: Pokreni kviz\n");
		printf("\t\tOpcija 2: Tablica\n");
		printf("\t\tOpcija 3: Ispis svih igraca\n");
		printf("\t\tOpcija 4: Obrisi igraca\n");
		printf("\t\tOpcija 5: Azuriraj rezultat\n");
		printf("\t\tOpcija 6: Zavrsetak programa\n");
		printf("======================================\======================================\n");

		scanf("%d", &Odabir_opcije);


		switch (Odabir_opcije) {
		case POKRENI_KVIZ:
			ucitajPitanja("pitanja.txt", pitanja);
			for (int i = 0; i < 10; i++) {
				printf("\nPitanje %d: %s\n", 1+i, (pitanja + i)->pitanje);
				printf("A) %s\n", (pitanja + i)->odgovorA);
				printf("B) %s\n", (pitanja + i)->odgovorB);
				printf("C) %s\n", (pitanja + i)->odgovorC);

				char odgovor;
				printf("Vas odgovor (A, B, C): ");
				scanf(" %c", &odgovor);

				odgovor = toupper(odgovor);

				if (odgovor == (pitanja + i)->Todgovor) {
					printf("Tocno!\n");
					bodovi++;
				}
				else {
					printf("Netocno! Tocan odgovor je: %c\n", (pitanja + i)->Todgovor);
				}
			}
			upis(bodovi);
			bodovi = 0;
			break;
		case TABLICA:
			ispisiTop5();
			break;
		case ISPISI_IGRACE:
			ispisiSveIgrace();
			break;
		case OBRISI_IGRACA:
			printf("Unesite id igraca kojeg zelite obrisati:");
			scanf("%d", &id);
			obrisiIgracaPoID(id);
			break;
		case AZURIRAJ_IGRACA:
			printf("Unesite ID igraca kojeg zelite azurirati: ");
			scanf("%d", &id);

			IGRAC* trazeni = pronadjiIgracaPoID(id);
			if (trazeni) {
				printf("Unesite nove bodove: ");
				do{
				scanf("%d", &novibodovi);
				} while (novibodovi < 0 || novibodovi>12);
				azurirajBodove(trazeni, novibodovi);
				free(trazeni);
			}
			else {
				printf("Igrac s ID %d nije pronaden.\n", id);
			}
			break;
		case IZLAZ:
			free(pitanja);
			pitanja = NULL;
			z = 1;
			break;
		default:
			printf("Nepoznata opcija, pokusajte ponovo.\n");
			break;
		}
	} while (z == 0);
	return 0;
}