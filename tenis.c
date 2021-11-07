#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct{
    char* nume_j;
    char* prenume_j;
    int scor_j;
}JUCATOR;

typedef struct{
    int nr_j;
	int scor_global;
	int scor_local;
	float scor_initial; 
    char* nume_tara;
    JUCATOR* jucator_tara;
}TARA;

typedef struct L{ //structura va fi folosita pentru LISTA si STIVA
    TARA* info;
    struct L* next, *prev;
}LISTA;

typedef struct{ // structura o folosesc pentru meciurile dintre jucatori
	char* nume_j1, * nume_j2;
	char* prenume_j1, * prenume_j2;
	int scor_j1, scor_j2;
}MECI;

typedef struct N{
	MECI* info_c;
	struct N* next;
}NOD;

typedef struct C {
	NOD* prim, * ultim;
}COADA;

typedef struct A {
	MECI* info_a;
	struct A* dreapta, * stanga;
}ARBORE;

//LISTA
//functie de adaugare elemente in lista
void CreareLista(LISTA** head, TARA* date)
{
    LISTA* headcopy = (*head);
    LISTA* nod = (LISTA*)malloc(sizeof(LISTA));
    LISTA* prim, *ultim;
    
	prim = nod; // setez a fi primul nod primul element adaugat dupa santinela (headcopy->next)
	ultim = headcopy->prev; // setez a fi ultimul nod ultimul element adaugat; care este inaintea santinelei (headcopy->prev)
    nod->info = date;

//adaugare de elemnt
    prim->next = headcopy; // se face legatura intre nodul adaugat si santinela
    prim->prev = ultim; // se face legatura intre noul nod adaugat si ultimul
    ultim->next = nod; // legatura intre eltimul nod adaugat si primul
    headcopy->prev = nod; // noul nod se adauga in spatele santinelei

    (*head) = headcopy;
}

void AfisareLista(LISTA* head, char* rez) // functie de afisare a tarilor (o folosesc pentru cerinta 1 si 2)
{
    FILE* file_rezultate;
    LISTA* headcopy = head;
    
    if((file_rezultate = fopen(rez, "wt")) == NULL){
        printf("\nFisierul 'rezultate' nu se poate crea!!!\n");
        exit(1);
    }

    if(head == NULL){
        fprintf(file_rezultate, "Lista goala");
    }
    while(headcopy->next != head){
    	fprintf(file_rezultate, "%s\n", headcopy->next->info->nume_tara);
        headcopy = headcopy->next;
    }
    
    fclose(file_rezultate);
}

void EliminareTara(LISTA** head, float min){ // se cauta pt stergere elem cu val min
	
	if((*head)->next == (*head)) return;
	LISTA* headcopy = (*head)->next;
	
	if(headcopy->info->scor_initial == min){ // cazul in care min este primul in lista
		(*head)->next = ((*head)->next)->next; 
		if((*head)->next != (*head)) ((*head)->next)->prev = (*head); 
		free(headcopy); 
		return;
	}
	
	do{ // cazul in care nu este primul in lista
		if(headcopy->info->scor_initial != min){
			headcopy = headcopy->next;
		}
		
		else{
			if(headcopy->next != (*head)) (headcopy->next)->prev = headcopy->prev;
			if(headcopy->prev != (*head)) (headcopy->prev)->next = headcopy->next;
			free(headcopy); // stergere
			return;
		}
	}while(headcopy->next != (*head)->next);
}


float MIN(LISTA* head) // functie de gasire a elementului min din lista
{
	LISTA* headcopy = head->next;
	float m;
	m = headcopy->info->scor_initial;
	
	while(headcopy != head){
		if(m > headcopy->info->scor_initial) 	m = headcopy->info->scor_initial;
		headcopy = headcopy->next;
	}
	return m;
}	

void StergereLista(LISTA** head) // functie de eliberare spetiu lista
{
	LISTA* aux, * headcopy = (*head)->next;
	aux = headcopy;

	while (headcopy != (*head)) {
		aux = headcopy->next;
		free(headcopy);
		headcopy = aux;
	}
	headcopy = NULL;
	(*head) = headcopy;
}

//STIVA
void push(LISTA** top, TARA* head) // se adauga elemente in stiva
{
	LISTA* nodS = (LISTA*)malloc(sizeof(LISTA));

	nodS->info = head;
	nodS->next = *top; 
	*top = nodS;
}

int VerificareS(LISTA* top) // se verifica daca stiva este goala
{
	return top == NULL;
}

TARA* pop(LISTA** top) //se scot elemente din stiva
{
	if (VerificareS(*top)) exit(1);

	LISTA* aux = (*top);
	TARA* ret = aux->info;

	*top = (*top)->next;
	free(aux);

	return ret;
}

void StergereStiva(LISTA** top) // se elibereaza memoria ocupata de stiva
{
	LISTA* aux;

	while ((*top) != NULL) {
		aux = *top;
		*top = (*top)->next;
		free(aux);
	}
}

//COADA
COADA* CreareCoada()
{
	COADA* q = (COADA*)malloc(sizeof(COADA));

	if (q == NULL) return NULL; 
	q->prim = q->ultim = NULL;

	return q;
}

void AddCoada(COADA* q, MECI* meci) // functie de adaugare a elementelor in coada
{
	NOD* nod = (NOD*)malloc(sizeof(NOD));

	nod->info_c = meci; 
	nod->next = NULL;

	if (q->ultim == NULL) q->ultim = nod;
	else {
		(q->ultim)->next = nod;
		q->ultim = nod;
	}

	if (q->prim == NULL) q->prim = q->ultim;
}

int VerificareC(COADA* q) //se verifica daca coada este goala
{
	return (q->prim == NULL);
}

MECI* Jucatori(COADA* q) // functie de scoatere a elementelor din coada
{
	NOD* aux;
	MECI* ret;
	if (VerificareC(q)) exit(1);

	aux = q->prim;
	ret = aux->info_c;
	q->prim = (q->prim)->next;
	free(aux);

	return ret;
}

void StergereCoada(COADA* q) // se elibereaza memoria ocupata de coada
{
	NOD* aux;

	while (!VerificareC(q)) {
		aux = q->prim;
		q->prim = q->prim->next;
		free(aux);
	}
	free(q);
}

// cerinta 3
// functia tine evidenta tuturor meciurilor jucate intre tari pana cand ramane tara castigatoare
void Clasament(LISTA* head, int nr_tari, char* rez, LISTA** clasament_tari)
{
	FILE* file_rezultate;
	if ((file_rezultate = fopen(rez, "a")) == NULL) {
		printf("\nFisierul 'rezultate' nu se poate deschide\n");
		exit(1);
	}


	LISTA* stiva = NULL; // s-a creat stiva initiala
	TARA* aux;
	LISTA* headcopy = head->next;

	LISTA* WINNER = NULL;
	LISTA* clasament = *clasament_tari; // in clasament voi retine ultimele 4 tari ramase in turneu (pentru cerinta 4)

	while (headcopy != head) {
		aux = headcopy->info; 
		push(&stiva, aux); // se adauga tarile in stiva
		headcopy = headcopy->next;
		headcopy->prev = NULL;
	}
	StergereLista(&head);

	int i, j, index = 1;
	TARA* temp1 = (TARA*)malloc(sizeof(TARA)), * temp2 = (TARA*)malloc(sizeof(TARA)); // in temp1 si temp2 se salveaza tarile care urmeaza sa se confrunte
	temp1->scor_local = 0; temp1->scor_global = 0;
	temp2->scor_local = 0; temp2->scor_global = 0;

	while (nr_tari != 1) // functia se executa pana cand in stiva WINNER ramane o singura tara
	{
		fprintf(file_rezultate, "\n====== ETAPA %d ======\n", index); // index tine evidenta etapelor

		while (!VerificareS(stiva))
		{
			TARA* temp1 = (TARA*)malloc(sizeof(TARA)), * temp2 = (TARA*)malloc(sizeof(TARA));

			temp1 = pop(&stiva);//in temp1 pun prima tara scoasa din stiva
			temp2 = pop(&stiva);// in temp2 a doua tara

			COADA* q;
			q = CreareCoada(); // am creat coada in care se vor pune meciurile dintre jucatori

			for (i = 0; i < temp1->nr_j; i++) {

				for (j = 0; j < temp2->nr_j; j++) {
					MECI* meci = (MECI*)malloc(sizeof(MECI)); // in meci sunt cei 2 jucatori care urmeaza sa se confrunte

					meci->nume_j1 = (char*)malloc((sizeof(char) * strlen(temp1->jucator_tara[i].nume_j)) + 1);
					strcpy(meci->nume_j1, temp1->jucator_tara[i].nume_j);
					meci->prenume_j1 = (char*)malloc((sizeof(char) * strlen(temp1->jucator_tara[i].prenume_j)) + 1);
					strcpy(meci->prenume_j1, temp1->jucator_tara[i].prenume_j);
					meci->scor_j1 = temp1->jucator_tara[i].scor_j;

					meci->nume_j2 = (char*)malloc((sizeof(char) * strlen(temp2->jucator_tara[j].nume_j)) + 1);
					strcpy(meci->nume_j2, temp2->jucator_tara[j].nume_j);
					meci->prenume_j2 = (char*)malloc((sizeof(char) * strlen(temp2->jucator_tara[j].prenume_j)) + 1);
					strcpy(meci->prenume_j2, temp2->jucator_tara[j].prenume_j);
					meci->scor_j2 = temp2->jucator_tara[j].scor_j;
					
					AddCoada(q, meci); // adaug in coada meciurile intre jucatori
				}
			}

			MECI* meci;
			
			fprintf(file_rezultate, "\n%s %d ----- %s %d\n", temp1->nume_tara, temp1->scor_global, temp2->nume_tara, temp2->scor_global);

			while (!VerificareC(q)) {

				temp1->scor_local = 0; temp2->scor_local = 0;

				for (i = 0; i < temp1->nr_j; i++) {
					for (j = 0; j < temp2->nr_j; j++) {

						meci = Jucatori(q); // scot meciurile din coada
						fprintf(file_rezultate, "%s %s %d vs %s %s %d\n", temp1->jucator_tara[i].nume_j, temp1->jucator_tara[i].prenume_j, temp1->jucator_tara[i].scor_j, temp2->jucator_tara[j].nume_j, temp2->jucator_tara[j].prenume_j, temp2->jucator_tara[j].scor_j);

						// compar scorurile personale ale jucatorilor si se punctez castigatorii
						if (temp1->jucator_tara[i].scor_j > temp2->jucator_tara[j].scor_j) {

							temp1->jucator_tara[i].scor_j = temp1->jucator_tara[i].scor_j + 5;
							temp1->scor_local = temp1->scor_local + 3;
						}
						if (temp1->jucator_tara[i].scor_j < temp2->jucator_tara[j].scor_j) {

							temp2->jucator_tara[j].scor_j = temp2->jucator_tara[j].scor_j + 5;
							temp2->scor_local = temp2->scor_local + 3;
						}
						if (temp1->jucator_tara[i].scor_j == temp2->jucator_tara[j].scor_j) {

							temp1->jucator_tara[i].scor_j = temp1->jucator_tara[i].scor_j + 2;
							temp1->scor_local = temp1->scor_local + 1;

							temp2->jucator_tara[j].scor_j = temp2->jucator_tara[j].scor_j + 2;
							temp2->scor_local = temp2->scor_local + 1;
						}
					}
				}

				// dupa ce toti jucatorii din cele doua tari s-au confruntat intre ei, scorul local obtinut de fiecare tara se adauga la cel global
				temp1->scor_global = temp1->scor_global + temp1->scor_local;
				temp2->scor_global = temp2->scor_global + temp2->scor_local;
				
				// compar scorurile locale si aflu tara castigatoare
				char copie[25];
				if (temp1->scor_local == temp2->scor_local) { // cazul scorurilor locale egale
					char copie1[25];
					int max1 = temp1->jucator_tara[0].scor_j;

					for (i = 0; i < temp1->nr_j; i++) { // gasesc scorul personal maxim al jucatorilor din temp1 (prima tara scoasa din stiva)

						if (max1 <= temp1->jucator_tara[i].scor_j) {

							max1 = temp1->jucator_tara[i].scor_j;
							strcpy(copie1, temp1->jucator_tara[i].nume_j);
						}
					}

					char copie2[25];
					int max2 = temp2->jucator_tara[0].scor_j;

					for (j = 0; j < temp2->nr_j; j++) { // gasesc scorul personal maxim al jucatorilor din temp2 (a doua tara scoasa din stiva)
						if (max2 <= temp2->jucator_tara[j].scor_j) {

							max2 = temp2->jucator_tara[j].scor_j;
							strcpy(copie2, temp2->jucator_tara[j].nume_j);
						}
					}

					// compar cele doua maxuri si adaug in stiva WINNER tara castigatorea
					if (max1 > max2) {
						push(&WINNER, temp1);
						if (nr_tari <= 4 && nr_tari > 1) push(&clasament, temp2); // adaug in stiva clasament ultimele 3 tari ramase in turneu (cerinta 4)
					}
					if (max1 < max2) {
						push(&WINNER, temp2);
						if (nr_tari <= 4 && nr_tari > 1) push(&clasament, temp1);
					}
					if (max1 == max2) { // cazul de egalitate
						push(&WINNER, temp1); // adaug prima tara scoasa din stiva initiala in stiva WINNER
						if (nr_tari <= 4 && nr_tari > 1) push(&clasament, temp2); // adaug in stiva clasament ultimele 3 tari ramase in turneu (cerinta 4)
					}

					/*if (max1 == max2) { // cazul de egalitate

						if (strcmp(copie1, copie2) > 0) { // compar numele jucatorilor (lexicografic)
							strcpy(copie, copie1);
							strcpy(copie1, copie2);
							strcpy(copie2, copie);
						}
						//in copie1 e tara castigatoare

						int ok = 1, x = temp1->nr_j;
						i = 0;

						while (ok == 1) { // verific care dintre cele doua tari este copie1 (tara castigatoare) si o adaug in stiva WINNER
							if ((strcmp(temp1->jucator_tara[i].nume_j, copie1)) == 0) {

								push(&WINNER, temp1);

								if(nr_tari <= 4 && nr_tari > 1) push(&clasament, temp2); // adaug in stiva clasament ultimele 3 tari ramase in turneu 

								if (x == 0) {
									push(&WINNER, temp2);
									if(nr_tari <= 4 && nr_tari > 1) push(&clasament, temp1); // adaug in stiva clasament ultimele 3 tari ramase in turneu 
								}
								x--;
								ok = 0;
							}
							else {
								i++;
								ok = 1;
							}
						}
					}*/
				}
				if (temp1->scor_local > temp2->scor_local) {
					push(&WINNER, temp1);
					if (nr_tari <= 4 && nr_tari > 1) push(&clasament, temp2); // adaug in stiva clasament ultimele 3 tari ramase in turneu 
				}
				if (temp1->scor_local < temp2->scor_local) {
					push(&WINNER, temp2);
					if (nr_tari <= 4 && nr_tari > 1) push(&clasament, temp1);
				}
			}
			
			free(meci);
			StergereCoada(q);
		}

		nr_tari = nr_tari / 2; // injumatatesc tarile pana ramane doar una

		if (VerificareS(stiva))
		{
			if (nr_tari != 1) { // daca sunt mai multe tari ramase in stiva WINNER, tarile vor fi scoase din WINNER si puse in stiva initiala

				fprintf(file_rezultate, "\n=== WINNER ===\n");

				while (!VerificareS(WINNER)) {
					fprintf(file_rezultate, "%s --- %d\n", WINNER->info->nume_tara, WINNER->info->scor_global);

					aux = pop(&WINNER);
					push(&stiva, aux);
				}
				StergereStiva(&WINNER);
				index++; // indexul creste => rezulta o noua etapa, si se va relua procedeul
			}
			else { // in stiva WINNER a ramas o singura tara, cea castigatoare
				fprintf(file_rezultate, "\n=== WINNER ===\n");

				while (!VerificareS(WINNER)) {
					fprintf(file_rezultate, "%s --- %d\n", WINNER->info->nume_tara, WINNER->info->scor_global);
					aux = pop(&WINNER);
					push(&clasament, aux); // adaug in stiva clasament ultima (a patra) tara ramasa in turneu 
					//WINNER = WINNER->next;
				}
				StergereStiva(&WINNER);
			}
		}
		*clasament_tari = clasament;
	}

	free(temp1); free(temp2); StergereStiva(&stiva);
	fclose(file_rezultate);
}

//BST
ARBORE* CreareNodNou(MECI* jucator) // functie care creaza un nou nod in arbore
{
	ARBORE* nodN = (ARBORE*)malloc(sizeof(ARBORE));

	nodN->info_a = jucator;
	nodN->dreapta = nodN->stanga = NULL;
	return nodN;
}

ARBORE* InserareElem(ARBORE* nod, MECI* jucator) // functie care adauga elemente in arbore (valorile mai mici ca radacina in stanga, iar cele mai mari in dreapta)
{
	if (nod == NULL) return CreareNodNou(jucator);

	if (jucator->scor_j1 < nod->info_a->scor_j1) nod->stanga = InserareElem(nod->stanga, jucator);
	else if (jucator->scor_j1 > nod->info_a->scor_j1) nod->dreapta = InserareElem(nod->dreapta, jucator);
	return nod;
}

ARBORE* CautareElem(ARBORE* root, char* cheie, int scor_cautat, int x, int c4, int c5) // functie de cautare element, daca il gasete il returneza
{
	if (root == NULL) return root;

	if (c4 == 1) { // caut dupa scorul personal al jucatorilor, daca gasesc un jucator cu acelasi scor il returnez
		if (root->info_a->scor_j1 == x) return root;

		if (root->info_a->scor_j1 < x) return CautareElem(root->dreapta, cheie, scor_cautat, x, c4, c5);
		else return CautareElem(root->stanga, cheie, scor_cautat, x, c4, c5);
	}

	if (c5 == 1) { // caut mai intai in arbore dupa nume
		if ((strcmp(root->info_a->nume_j1, cheie)) == 0) return root;

		if ((strcmp(root->info_a->nume_j1, cheie)) != 0) { // daca numle jucatorului din radacina difera de cel cautat
			if (root->info_a->scor_j1 < scor_cautat) return CautareElem(root->dreapta, cheie, scor_cautat, x, c4, c5); // compar scorul jucatorului din arbore cu jucatorul cautat, daca scorul din arbore este mai mic decat cel cautat cobor in subarborele drept, altfel in cel stang 
			else return CautareElem(root->stanga, cheie, scor_cautat, x, c4, c5);
		}
	}
}

int VerificareA(ARBORE* root) // functie de verificare arbore gol
{
	return (root == NULL);
}

void AfisareClasament(ARBORE* root, FILE* file_rezultate) // functie de afisare a clasamentului final al jucatorilor din arbore (de la cel mai mare la cel mai mic scor)
{
	if (root) {
		AfisareClasament(root->dreapta, file_rezultate);
		fprintf(file_rezultate, "%s %s %d\n", root->info_a->nume_j1, root->info_a->prenume_j1, root->info_a->scor_j1);
		AfisareClasament(root->stanga, file_rezultate);
	}
}

static int nr_elemente_interval = 0; // retin numarul jucatorilor care au scorul personal inclus intre cele doua scoruri personale
void Interval(ARBORE* root, int x, int y) // functie de aflare interval intre x si y, cate elemente sunt intre cele 2 val
{
	if (root == NULL) return;
	
	Interval(root->stanga, x, y);
	if (root->info_a->scor_j1 > x && root->info_a->scor_j1 < y) nr_elemente_interval++; // coboara pe arborele stang si drept pana cand radacina nu se mai gasete intre x si y
	Interval(root->dreapta, x, y);
}

void StergereArbore(ARBORE* root) // funtie de eliberare memorie ocupata de arbore
{
	if (root == NULL) return;

	StergereArbore(root->stanga);
	StergereArbore(root->dreapta);
	free(root);
}

ARBORE* MIN_arbore(ARBORE* root) // functie de gasire a minimului in arbore (se coboara spre cel mai din stanga nod din arbore)
{
	ARBORE* aux = root;

	while (aux->stanga != NULL)
		aux = aux->stanga;

	return aux;
}

ARBORE* StergereNodArbore(ARBORE* root, MECI* jucator) // functie de stergere a unui nod din arbore cu valoarea val_cautata
{
	if (root == NULL) return root;

	// caut nodul pe care trebuie sa il sterg
	if (jucator->scor_j1 < root->info_a->scor_j1) root->stanga = StergereNodArbore(root->stanga, jucator); // se cauta in subarborele drept si stang pana gasesc valoarea pe care o caut sau pana se termina arborele
	else if (jucator->scor_j1 > root->info_a->scor_j1) root->dreapta = StergereNodArbore(root->dreapta, jucator);

	else { // cand gasesc nodul
		if (root->stanga == NULL) {
			ARBORE* aux = root;
			root = root->dreapta;
			free(aux);
			return root;
		}
		else if (root->dreapta == NULL) {
			ARBORE* aux = root;
			root = root->stanga;
			free(aux);
			return root;
		}

		ARBORE* aux = MIN_arbore(root->dreapta); // daca are 2 copii; caut min in subarborele drept
		root->info_a = aux->info_a; /// si ii copiez valoarea in nodul sters
		root->dreapta = StergereNodArbore(root->dreapta, aux->info_a); // sterg nodul care contine min
	}
	return root;
}

int main(int argc, char* argv[])
{
    FILE* file_cerinte, *file_date;
    int c1, c2, c3, c4, c5; //pt cerinte
    TARA* date;
    
    LISTA* head = (LISTA*)malloc(sizeof(LISTA)); //creare nod santinela
    head->next = head->prev = head;

    if((file_cerinte = fopen(argv[1], "rt")) == NULL){
        printf("\nFisierul 'cerinte' nu se poate deschide!!!\n");
        exit(1);
    }

    fscanf(file_cerinte, "%d%d%d%d%d", &c1, &c2, &c3, &c4, &c5);
    fclose(file_cerinte);

    if((file_date = fopen(argv[2], "rt")) == NULL){
        printf("\nFisierul 'date' nu se poate deschide!!!\n");
        exit(1);
    }

    char copie1[30], copie2[30];
    int nr_jucatori, nr_tari, i, scor_pers;

    if(c1 == 1)
    {
        fscanf(file_date, "%d", &nr_tari); //citire nr tari

        for(i = 0; i < nr_tari; i++)
        {
            date = (TARA*)malloc(sizeof(TARA)); // alocare de memorie pt TARA

            fscanf(file_date, "%d", &nr_jucatori);
            date->nr_j = nr_jucatori; //citire nr jucatori

            fscanf(file_cerinte, "%s", copie1); //citire tara
            date->nume_tara = (char*)malloc((strlen(copie1)*sizeof(char))+1);
            strcpy(date->nume_tara, copie1);

            int j, suma = 0;
			float medie;
            date->jucator_tara = (JUCATOR*)malloc(nr_jucatori*sizeof(JUCATOR)); //alocare de memorie JUCATOR
			for(j = 0; j < nr_jucatori; j++)
            {
                fscanf(file_cerinte, "%s %s %d", copie1, copie2, &scor_pers);//citire jucator

                date->jucator_tara[j].nume_j = (char*)malloc((strlen(copie1)*sizeof(char))+1);
                strcpy(date->jucator_tara[j].nume_j, copie1);
                date->jucator_tara[j].prenume_j = (char*)malloc((strlen(copie2)*sizeof(char))+1);
                strcpy(date->jucator_tara[j].prenume_j, copie2);
                date->jucator_tara[j].scor_j = scor_pers;
                
                suma = suma + date->jucator_tara[j].scor_j;
            }
            medie = (float)suma/nr_jucatori;
            date->scor_initial = medie; // dupa ce am calculat media scorurilor jucatorilor, am pus-o in scor_initial
            
            CreareLista(&head, date); //creare lista
        }
        fclose(file_date);
    }
    
    if(c2 == 1)
    {
    	int k = 1, nr_final_tari = 0;
		float min;
    
    	while(k < nr_tari){ // aici aflu cate tari trebuie sa ramana dupa eliminare
    		k = k*2;
    		if(k < nr_tari) nr_final_tari = k;
		}
		
		while(nr_final_tari != nr_tari){

			min = MIN(head); // aflu care este cel mai mic scor initial, pana ajung la nr de tari dorit
			EliminareTara(&head, min); // si elimn tara
			nr_tari--;
		} 
	}
	AfisareLista(head, argv[3]);    

	LISTA* clasament_tari = NULL; // creez o stiva pentru a stoca ultimele 4 tari din turneu

	if (c3 == 1)
	{
		Clasament(head, nr_tari, argv[3], &clasament_tari); // in functie este rezolvarea cerintei 3
	}
	
	ARBORE* root = NULL;
	TARA* aux;
	MECI* jucator;

	if (c4 == 1)
	{
		COADA* q = CreareCoada(); // creez o coada pentru a stoca toti jucatorii celor 4 tari
		int nr_total_jucatori = 0;

		while (!VerificareS(clasament_tari))
		{
			aux = pop(&clasament_tari); // scot tara din stiva ca sa am acces la jucatori
			nr_total_jucatori = nr_total_jucatori + aux->nr_j; // aflu cati jucatori bag in coada

			for (i = 0; i < aux->nr_j; i++)
			{
				jucator = (MECI*)malloc(sizeof(MECI));

				jucator->nume_j1 = (char*)malloc((sizeof(char) * strlen(aux->jucator_tara[i].nume_j)) + 1);
				strcpy(jucator->nume_j1, aux->jucator_tara[i].nume_j);
				jucator->prenume_j1 = (char*)malloc((sizeof(char) * strlen(aux->jucator_tara[i].prenume_j)) + 1);
				strcpy(jucator->prenume_j1, aux->jucator_tara[i].prenume_j);
				jucator->scor_j1 = aux->jucator_tara[i].scor_j;

				AddCoada(q, jucator); // adaug jucatorii in coada
			}

		}

		int scor;
		char a[1] = "\0";
		int z; 

		while (!VerificareC(q))
		{
			for (i = 0; i < nr_total_jucatori; i++) {
				jucator = Jucatori(q); // scot pe rand jucatori din coada

				if (root == NULL) root = InserareElem(root, jucator); // daca arborele este gol adaug direct in arbore
				else {
					scor = jucator->scor_j1; // in scor pun valoarea pe care vreau sa o caut

					if (CautareElem(root, a, z, scor, 1, 0)) { // a si z sunt irelevante pt rezolvarea acestei cerinte, 1 - reprezinta cerinta 4, iar 0 - cerinta5
						char nume[30], nume1[30], nume2[30], prenume1[30], prenume2[30];
						int scor1, scor2, a1, a2;

						strcpy(nume1, jucator->nume_j1); strcpy(prenume1, jucator->prenume_j1); scor1 = jucator->scor_j1;
						
						ARBORE* arbore = (ARBORE*)malloc(sizeof(ARBORE));
						arbore = CautareElem(root, a, z, scor, 1, 0);
						strcpy(nume2, arbore->info_a->nume_j1); strcpy(prenume2, arbore->info_a->prenume_j1); scor2 = arbore->info_a->scor_j1;

						// se compara lexicografic pentru a vedea ce jucator intra in arbore si care nu
						if (strcmp(nume1, nume2) < 0) { // a eate 1 daca numele s-a sortat lexicografic, nu mai verifica si pronumele
							a1 = 1; a2 = 0;				// rezultatul final este copiat in nume2
							strcpy(nume, nume1);
							strcpy(nume1, nume2);
							strcpy(nume2, nume);
						}
						if (strcmp(nume1, nume2) == 0) { // caz de egalitate, se compara pronumele si se obtine a2 = 1
							a1 = 1; a2 = 1;				// rezultatul final este copiat in pronume2
							strcpy(nume, prenume1);
							strcpy(prenume1, prenume2);
							strcpy(prenume2, nume);
						}

						// in functie de a1 si a2 se adauga al doilea jucator gasit cu acelasi scor in arbore
						if (a1 == 1 && a2==0) {
							if ((strcmp(nume2, jucator->nume_j1)) == 0) { // verific daca numele jucatorului pe care il doresc adaugat in arbore coincide cu cel pe care l-am "sortat" lexicografic
								StergereNodArbore(root, jucator); // daca nume coincid steg nodul initial din arbore cu valoarea "scor"
								root = InserareElem(root, jucator); // adaug un nou nod cu numele jucatorului dorit
							}
						}
						if (a2 == 1 && a1 == 1) {
							if ((strcmp(prenume2, jucator->prenume_j1)) != 0) {
								StergereNodArbore(root, jucator);
								root = InserareElem(root, jucator);
							}
						}
					}
					// daca nu exista scoruri egale adug direct in arbore
					else root = InserareElem(root, jucator);
				}
			}
		}
		StergereCoada(q); 
		FILE* file_rezultate;
		if ((file_rezultate = fopen(argv[3], "a")) == NULL) {
			printf("\nFisierul 'rezultate' nu se poate deschide\n");
			exit(1);
		}
		
		fprintf(file_rezultate, "\n====== CLASAMENT JUCATORI ======\n");
		AfisareClasament(root, file_rezultate); // se afiseaza jucatorii de la cel mai mare scor la cel mai mic
		fclose(file_rezultate);
	}
	free(jucator); StergereStiva(&clasament_tari);

	if (c5 == 1)
	{
		FILE* file_cerinte;
		if ((file_cerinte = fopen(argv[1], "rt")) == NULL) {
			printf("\nFisierul 'cerinte' nu se poate deschide!!!\n");
			exit(1);
		}

		MECI* jucator = (MECI*)malloc(sizeof(MECI));
		char nume[30], nume1[30], nume2[30], prenume1[30], prenume2[30];
		int scor1, scor2;

		FILE* file_rezultate;
		if ((file_rezultate = fopen(argv[3], "a")) == NULL) {
			printf("\nFisierul 'rezultate' nu se poate deschide\n");
			exit(1);
		}
		
		//citesc din fisier cei 2 jucatori
		fscanf(file_cerinte, "%*[^\n]\n");
		fscanf(file_cerinte, " %s %s %d", nume1, prenume1, &scor1);
		jucator->nume_j1 = (char*)malloc((sizeof(char) * strlen(nume1)) + 1);
		strcpy(jucator->nume_j1, nume1);
		jucator->prenume_j1 = (char*)malloc((sizeof(char) * strlen(prenume1)) + 1);
		strcpy(jucator->prenume_j1, prenume1);
		jucator->scor_j1 = scor1;

		fscanf(file_cerinte, " %s %s %d", nume2, prenume2, &scor2);
		jucator->nume_j2 = (char*)malloc((sizeof(char) * strlen(nume2)) + 1);
		strcpy(jucator->nume_j2, nume2);
		jucator->prenume_j2 = (char*)malloc((sizeof(char) * strlen(prenume2)) + 1);
		strcpy(jucator->prenume_j2, prenume2);
		jucator->scor_j2 = scor2;

		fclose(file_cerinte);

		strcpy(nume1, jucator->nume_j1); // copiez numele lor pt a le folosi in cautarea lor in arbore
		strcpy(nume2, jucator->nume_j2);
		scor1 = jucator->scor_j1; // la fel si pt scoruri
		scor2 = jucator->scor_j2;

		int c1 = 0, c2 = 0; // ii folosesc pentru a verifica daca gasesc numele cautat in arbore sau nu
		int z;
		if (CautareElem(root, nume1, scor1, z, 0, 1)) c1 = 1; // caut numele celor 2 jucatori in arbore
		if (CautareElem(root, nume2, scor2, z, 0, 1)) c2 = 1;

		if (c1 == 1 && c2 == 1) { // daca gasesc ambele nume, aflu cati jucatori sunt intre cei 2 jucatori cititi din fisier
			if (scor1 < scor2) {
				Interval(root, scor1, scor2);
				fprintf(file_rezultate, "\n");
				fprintf(file_rezultate, "%d\n", nr_elemente_interval);
			}
			else {
				Interval(root, scor2, scor1);
				fprintf(file_rezultate, "\n");
				fprintf(file_rezultate, "%d\n", nr_elemente_interval);
			}
		}
		if (c1 == 0 && c2 == 1) { // daca nu gasesc un jucator afisez un measaj corespunzator
			fprintf(file_rezultate, "\n");
			fprintf(file_rezultate, "%s %s nu poate fi identificat!", jucator->nume_j1, jucator->prenume_j1);
		}
		if (c1 == 1 && c2 == 0) {
			fprintf(file_rezultate, "\n");
			fprintf(file_rezultate, "%s %s nu poate fi identificat!", jucator->nume_j2, jucator->prenume_j2);
		}
		
		fclose(file_rezultate);
	}
	free(jucator); StergereArbore(root);
	free(date->jucator_tara); free(date); // eliberez memoria ocupata de jucator si de tara

    return 0;
}
