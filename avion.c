#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <malloc.h>

typedef struct { // structura ce cotine date despre resursa
	char* nume;
	int cantitate;
}Resursa;

typedef struct { // structura ce contine date despre insula
	int index; // retin nr insulei
	char* nume;
	int nrResurse;
	Resursa* inventarResurse;
	int nrAvioane;
	int* avioane;
	int tolAvioane;
}Island;

struct ListNode { // structura pentru crearea listei
	Island* insula;
	int timp;
	struct ListNode* next;
};
typedef struct ListNode Node;

typedef struct { 
	int nr_insule, nr_conexiuni;
	Island** val; // stochez informatiile despre insula
	int** m; // vector pentru matrice
	Node** a; //vector pentru capetele de liste – lungime V 
} Graph;

int convert_char_int(char c[]) // functie de conversie char -> int 
{							// o folosesc pentru a afla indexul fiecarei insule (insland1 => index = 1)
	int k, x, nr = 0;
	char car[3];
	int n = strlen(c);

	for (k = n - 1; k >= n - 2; k--) { // salvez doar ultimele 2 caractere din sir (island1 salvez doar '1d')
		car[nr] = c[k];
		nr++;
	}
	char aux[1] = "\0"; int count = 0;
	for (k = 0; k < nr; k++) {
		if (car[k] >= '0' && car[k] <= '9') count++;
		if (count == nr) {
			k = 0;
			aux[k] = car[k];
			car[k] = car[k + 1];
			car[k + 1] = aux[k];
		}
	}
	x = atoi(car); // fac conversia in int

	return x;
}

void Add_node_in(Graph* g, Node* newnode, int x, int timp) // functie de inserare nod in interiorul listei
{
	Node* newN = (Node*)malloc(sizeof(Node));
	newN->insula = g->a[x]->insula;
	newN->timp = timp;
	newN->next = newnode->next;
	newnode->next = newN;
}

Node* Add_node_end(Graph* g, int x, int timp) { // functie de inserare nod la sfarsitul listei

	Node* newnode = (Node*)malloc(sizeof(Node));
	newnode->insula= g->a[x]->insula;
	newnode->timp = timp;
	newnode->next = NULL;
	return newnode;
}

Node* insertNode(Graph* g, Node* head, int x, int timp) // functie de inserare noduri in lista
{
	if (head->next != NULL) { // daca lista nu contine doar un element (head) verific cazurile de adaugare
		Node* santinela = (Node*)malloc(sizeof(Node)); // folosesc un nod santinela pentru a face legatura intre primul si ultimul nod din lista
		santinela->next = head;
		Node* newnode = santinela;
		
		if (head->next->insula->index == x) { head->next->timp = timp; return head; }
		if (head->insula->index < x) {

			while (head->insula->index < x) {

				if (head->next == NULL) {
					head->next = Add_node_end(g, x - 1, timp); return head; // se adauga nodul la sfarsit
				}
				if (head->next->insula->index == x) { head->next->timp = timp; return head; } // se schimba costul
				else {
					newnode = newnode->next; // se continua parcurgerea
					head = head->next;
				}
			}
			Add_node_in(g, newnode, x - 1, timp); // se adauga dupa nodul curent
			santinela= newnode; return santinela;
		}

		if (head->insula->index > x) { // daca indexul insulei este mare decat indexul citit

			newnode = newnode->next; // in newnode pun capul listei
			if(head->next->insula->index > x){
				Add_node_in(g, newnode, x - 1, timp); // se adauga dupa nodul curent
				santinela = newnode; return santinela;
			}
			else{
				while (head->next->insula->index < x && head->next->next != NULL) {
				//	if (head->next->insula->index == x) { head->next->timp = timp; return; } // se schimba costul
				//	else{
						newnode = newnode->next; // se continua parcurgerea
						head = head->next;
				//	}
				}
				if (head->next->next == NULL) {
					head->next->next = Add_node_end(g, x - 1, timp); return head; // se adauga la sfarsitul listei
				}
				else {
					Add_node_in(g, newnode, x - 1, timp); // se adauga dupa nodul curent
					santinela = newnode; return santinela;
				}
			}
		}
	}

	if (head->next == NULL) { // daca lista contine doar un nod (capul listei) aduci se adauga nodul la sfarsit
		head->next = Add_node_end(g, x - 1, timp);
		return  head;
	}
}

Graph* createL(FILE* file_date) // functie de creare graf - lista de adiacenta
{
	Island* date;
	Graph* g = (Graph*)malloc(sizeof(Graph));
	if (g == NULL) {
		printf("\nNo memory for this graph!!!\n");
		return NULL;
	}

	fscanf(file_date, "%d", &(g->nr_insule)); // citesc nr da insule (nr de noduri - graf)

	g->a = (Node**)malloc(g->nr_insule * sizeof(Node*)); // aloc memorie pentru nodurile din graf (fiecare varf reprezinta capul de lista)
	char copie1[10], copie2[10]; int i;

	int  j;
	for (i = 0; i < g->nr_insule; i++) {
		date = (Island*)malloc(sizeof(Island)); // aloc spatiu pentru insula

		fscanf(file_date, "%s", copie1); // citesc insula
		date->nume = (char*)malloc((strlen(copie1) * sizeof(char)) + 1);
		strcpy(date->nume, copie1);
		date->index = convert_char_int(copie1); // in date->index salvez nr Insulei (Island1 -> 1)

		fscanf(file_date, "%d", &(date->nrResurse)); // citesc nr de resurse
		date->inventarResurse = (Resursa*)malloc(date->nrResurse * sizeof(Resursa)); // aloc memorie pentru resursa
		for (j = 0; j < date->nrResurse; j++) { // citesc resursa si cantitatea
			fscanf(file_date, "%s %d", copie1, &(date->inventarResurse[j].cantitate));
			date->inventarResurse[j].nume = (char*)malloc((strlen(copie1) * sizeof(char)) + 1);
			strcpy(date->inventarResurse[j].nume, copie1);
		}
		Node* newnode = (Node*)malloc(sizeof(Node));
		newnode->insula = date;
		newnode->timp = 0;
		newnode->next = NULL;
		g->a[i] = newnode; // se creaza si adauga informatiile despre insula intr-un nod din graf
	}
	//g->nr_conexiuni = 0;

	int poz, timp, nr_conexiuni;
	poz = ftell(file_date); // retin pozitia din fisier de la nr de conexiuni
	for (i = 0; i < g->nr_insule; i++) {

		fseek(file_date, poz, SEEK_SET); // pentru fiecare nod verific toate conexiunile care se dau
		fscanf(file_date, "%d", &nr_conexiuni); // citesc nr de conexiuni

		Node* head = g->a[i]; // stochez adresa in head pentru a ca nu vreau sa o modific
		while (nr_conexiuni != 0) {
			fscanf(file_date, "%s - %s %d", copie1, copie2, &timp); // citesc conexiunile si timpul
			int X, Y;
			X = convert_char_int(copie1); // aflu indexul pt insulele tocmai citite
			Y = convert_char_int(copie2);
			
			if (g->a[i]->insula->index == X) head = insertNode(g, head, Y, timp); // adaug o insula in lista daca exista conexiune intre capul listei si ea
			if (g->a[i]->insula->index == Y) head = insertNode(g, head, X, timp);
			nr_conexiuni--;
		}
	}
	return g;
}

void parcurgere_lista(Node* head, int c, FILE* file_rezultate) // functie de parcurgere 
{
	while (head != NULL) {

		if (head->insula->index == c) {
			fprintf(file_rezultate, "OK\n");
			return;
		}
		head = head->next;
	}
	fprintf(file_rezultate, "NO\n");
}

void deleteNode(Node** head, int x) //functie de stergere nod pentru o pozitie data
{
	Node* headcopy = *head;
	Node* aux = *head;
	while (headcopy != NULL) {
		if (headcopy->insula->index != x) {
			aux = headcopy;
			headcopy = headcopy->next;
		}
		else {
			aux->next = headcopy->next;
			free(headcopy);
			return;
		}
	}
}

void deleteList(Node** head) { // functie de stergere a listei
	Node* headcopy;

	while (*head != NULL) {
		headcopy = (*head)->next;
		free(*head);
		*head = headcopy;
	}
	*head = NULL;
}

void ordonare_lexicografica(char sir[][20], int k) { // functie de ordonare lexicografica a cuvintelor din sir
	char temp[20];
	int i, j;
	for (i = 0; i < k; i++) {
		for (j = i + 1; j < k; j++) {
			if (strcmp(sir[i], sir[j]) > 0) {
				strcpy(temp, sir[i]);
				strcpy(sir[i], sir[j]);
				strcpy(sir[j], temp);
			}
		}
	}
}

void free_memory(Graph* g) // functie de eliberare a memoriei pentru graful reprezentat prin lista de adiacenta
{
	int i, j;

	for (i = 0; i < g->nr_insule; i++) {
		for (j = 0; j < g->a[i]->insula->nrResurse; j++) {
			free(g->a[i]->insula->inventarResurse[j].nume); // eliberez spatiul ocupat de nume resursa
		}
		free(g->a[i]->insula->nume);
		deleteList(&g->a[i]);
	}
	free(g->a);
	free(g);
}

void printPredecesor(FILE* file_rezultate, Graph* g, int* pred, int y) // functie de printare a predecesorilor
{
	if (pred[y] == -1) return;
	printPredecesor(file_rezultate, g, pred, pred[y]);
	fprintf(file_rezultate, "%s ", g->a[y]->insula->nume);
}

void afisare_drum_zbor(FILE* file_rezultate, Graph* g, int* pred, int y) // functie de afisare drum_zbor
{
	if (pred[y - 1] == -1) { // daca nu exista drum se afiseaza "NO"
		fprintf(file_rezultate, "NO"); return;
	}
	else printPredecesor(file_rezultate, g, pred, y - 1); // altfel se afiseaza insulele intermediare dintre sursa si destinatie
}

void afisare_timp_zbor(FILE* file_rezultate, int* dist, int y, char c1[]) // functie de afisare aduratei minime in conditii de distribuire instanta
{
	if (dist[y - 1] == INT_MAX) {
		fprintf(file_rezultate, "INF"); return; // daca nu pot sa determin timpul afisez "INF"
	}
	else {
		if(strcmp("min_zbor", c1) == 0) fprintf(file_rezultate, "%d", dist[y - 1] - 15); // afisez timpul min de la sursa la destinatie in cazul adaugarii de 15 min la stationarea pe fiecare insula intermediara (scad 15 deoarece, pentru insula destinatie nu se ia in calcul intarzierea)
		else fprintf(file_rezultate, "%d", dist[y - 1]); // altfel afisez timpul min de la sursa la destinatie
	}
}

int minDistance(int* dist, int* sps, int V) // functie de gasire a varfului care inca nu e in sps cu distanta min pana la sursa
{
	int v, min = INT_MAX, min_index;

	for (v = 0; v < V; v++)
		if (sps[v] == 0 && dist[v] <= min) {
			min = dist[v];
			min_index = v;
		}
	return min_index;
}

void Dijkstra(FILE* file_rezultate, Graph* g, int sursa, int y, char c1[]) // folosesc algortmul Dijkstra pentru gasirea celui mai scurt drum si pentru aflarea timpului min 
{
	int i, j, u;
	int* dist = (int*)malloc(g->nr_insule * sizeof(int));
	int* sps = (int*)malloc(g->nr_insule * sizeof(int));
	int* pred = (int*)malloc(g->nr_insule * sizeof(int));

	for (i = 0; i < g->nr_insule; i++) {
		dist[i] = INT_MAX;
		sps[i] = 0;
		pred[i] = -1;
	}

	dist[sursa - 1] = 0;
	for (j = 0; j < g->nr_insule - 1; j++) {
		u = minDistance(dist, sps, g->nr_insule);

		sps[u] = 1;
		Node* head = g->a[u];
		while (head) {
			if (sps[head->insula->index - 1] == 0 && dist[u] != INT_MAX && head->timp != 0 && dist[u] + head->timp < dist[head->insula->index - 1]) {
				if (strcmp("min_zbor", c1) == 0) dist[head->insula->index- 1] = dist[u] + head->timp + 15; // adaug 15 in cazul statonarii 
				else dist[head->insula->index - 1] = dist[u] + head->timp;
				
				pred[head->insula->index - 1]= u;
				
			}
			head = head->next;
		}
	}
	if (strcmp("drum_zbor", c1) == 0) afisare_drum_zbor(file_rezultate, g, pred, y);
	if ((strcmp("timp_zbor", c1) == 0) || (strcmp("min_zbor", c1) == 0)) afisare_timp_zbor(file_rezultate, dist, y, c1);
	free(dist); free(sps); free(pred);
}

Graph* createM(FILE* file_date) // creez graful pentru partea de GESTIUNE prin matrice de adiacenta
{
	int i, j, toleranta;
	Graph* graf = (Graph*)malloc(sizeof(Graph));
	if (graf == NULL) {
		printf("no memory graph\n");
		return NULL;
	}
	fscanf(file_date, "%d", &(graf->nr_insule)); // citesc datele despre insule din fisier
	
	fscanf(file_date, "%d", &toleranta);
	graf->val = (Island**)malloc(graf->nr_insule * sizeof(Island*));
	for (i = 0; i < graf->nr_insule; i++) {
		graf->val[i] = (Island*)malloc(sizeof(Island));
		graf->val[i]->tolAvioane = toleranta;
		fscanf(file_date, "%d", &(graf->val[i]->nrAvioane));
	}

	for (i = 0; i < graf->nr_insule; i++) {
		graf->val[i]->avioane = (int*)malloc(graf->val[i]->nrAvioane * sizeof(int));
		for (j = 0; j < graf->val[i]->nrAvioane; j++)
			fscanf(file_date, "%d", &(graf->val[i]->avioane[j]));
	}

	graf->m = (int**)malloc(graf->nr_insule * sizeof(int*));
	for (i = 0; i < graf->nr_insule; i++) graf->m[i] = (int*)calloc(graf->nr_insule, sizeof(int));
	if (graf->m == NULL) { 
		printf("no memory for this matrix\n"); 
		return NULL; 
	}

	for (i = 0; i < graf->nr_insule; i++) { // citesc conexiunile dintre insule
		for (j = 0; j < graf->nr_insule; j++) {
			fscanf(file_date, "%d", &(graf->m[i][j]));
			graf->m[j][i] = graf->m[i][j];
		}
	}
	return graf;
}

int valid(Graph* graf, int insula) // functie ce imi returneza conditia ce trebuie respectata
{
	return (graf->val[insula]->nrAvioane > graf->val[insula]->tolAvioane);
}

void afisare(Graph* graf, int i, int nr_avioane, int* avioane, FILE* file_rezultate) // functie de afisare a insulelor si a combinatiilor de avioane
{
	int j, k;

	while (nr_avioane != graf->val[i]->nrAvioane) { // printez solutiile pana cand nr de avione revine cel initial
		for (j = 0; j < nr_avioane; j++)
			fprintf(file_rezultate, "%d ", avioane[j]);
		fprintf(file_rezultate, "\n");
		if (j == nr_avioane && j != graf->val[i]->nrAvioane + 1) { // afisez solutiile intermediare
			for (k = 0; k < graf->val[i]->nrAvioane; k++)
				fprintf(file_rezultate, "%d ", avioane[k]);
			fprintf(file_rezultate, "%d", avioane[j-1]);
			fprintf(file_rezultate, "\n");
		}
		nr_avioane--;
	}
	if (nr_avioane == graf->val[i]->nrAvioane) { // afisez avionele la care nu s-a adugat niciun alt avion
		if (nr_avioane > graf->val[i]->tolAvioane) nr_avioane = graf->val[i]->tolAvioane; // pentru insula ce face distribuirile afisez avione pana la respectarea tolerantei
		for (j = 0; j < nr_avioane; j++)
			fprintf(file_rezultate, "%d ", avioane[j]);
		fprintf(file_rezultate, "\n");
	}
}

void solutie(Graph* graf, int insula, int surplus, int* avioane, FILE* file_rezultate) // functie de creare a solutiei
{
	int i, j, k = 0, nr_avioane;

	for (i = 0; i < graf->nr_insule; i++) { // parcurg graful ce are ca sursa insula de pe pozitia 'insula'
		fprintf(file_rezultate, "Island%d\n", i + 1);
		nr_avioane = graf->val[i]->nrAvioane; // salvez nr intial al insulelor aflate in conexiune directa cu sursa
		
		avioane = (int*)malloc(sizeof(int) * graf->val[i]->tolAvioane);
		if (graf->m[insula][i] == 1 && graf->val[i]->nrAvioane < graf->val[i]->tolAvioane) { // daca exista conexiune directa intre insule:: salvez in vectorul 'avione' avionele insulei cu conexiune directa si cele care sunt in plus pentru insula sursa 
			for (k = 0; k < graf->val[i]->nrAvioane; k++) avioane[k] = graf->val[i]->avioane[k];
			for (j = graf->val[insula]->nrAvioane - surplus; j < graf->val[insula]->nrAvioane; j++) {
				if (graf->val[i]->nrAvioane <= graf->val[i]->tolAvioane)
				avioane[k] = graf->val[insula]->avioane[j];
				k++;
				nr_avioane++; // aflu nr total de avioane dupa adaugara surplusului (pana cand se atinge toleranta)
			}
			afisare(graf, i, nr_avioane, avioane, file_rezultate); // afisez solutiile
		}
		else if (graf->m[insula][i] == 0) { // daca nu exista conexiune intre insule: salvez in vectorul "avioane" doar avioanele insulelor respective
			for (k = 0; k < graf->val[i]->nrAvioane; k++) avioane[k] = graf->val[i]->avioane[k];
			afisare(graf, i, nr_avioane, avioane, file_rezultate);
		}
	}
}

void backtracking(int insula, Graph* graf, int* avioane, FILE* file_rezultate) // functie backtracking de rezolvare a  cerintelor
{
	int i, surplus = 0;
	if (insula <= graf->nr_insule) {
		for (i = insula; i < insula + 1; i++) { 
			if (valid(graf, insula)) { // caut insula care respecta conditia
				surplus = graf->val[insula]->nrAvioane - graf->val[insula]->tolAvioane; // aflu cate avione are in plus insula gasita
				solutie(graf, insula, surplus, avioane, file_rezultate); // creez solutia
			}
			else backtracking(insula + 1, graf, avioane, file_rezultate); // parcurg graful pana ce gasesc gasesc o insula care sa respexte conditia
		}
	}
}

void free_memory_matrix(Graph* graf) // functie de eliberare a memoriei pentru matricea de adiacenta
{
	int i, j; 
	for(i = 0; i < graf->nr_insule; i++){
		//for (j = 0; j < graf->val[i]->nrAvioane; j++)
			//free(graf->val[i]->avioane[j]);
		free(graf->val[i]->avioane);
	}
	free(graf->val);
	free(graf->m);
	free(graf);
	
}

int main(int argc, char* argv[])
{
	FILE* file_date, *file_rezultate;
	if ((file_date = fopen(argv[1], "rt")) == NULL) {
		printf("\nFisierul 'date' nu se poate deschide!!!\n");
		exit(1);
	}
	if ((file_rezultate = fopen(argv[2], "wt")) == NULL) {
		printf("\nFisierul 'rezultate' nu se poate crea!!!\n");
		exit(1);
	}
	Graph* g = createL(file_date); // creeez graful folosid liste de adiacenta
	//printGraph(g);

	char c1[20], c2[10], c3[10]; // ma folosesc de c-uri pt a citi cerinta din fisier
	int i, j, poz;

	while (((fscanf(file_date, "%s", c1)) != EOF) && (fscanf(file_date, "%hhd", c1)) == 0) { // citesc pana la sfarsitul fisierului sau pana cand intalnesc prima linie cu o valoare intreaga

		if ((strcmp("conexiune", c1)) == 0) { // functie de aflare a conexiunilor dintre insulle
			fscanf(file_date, "%s  %s", c2, c3);
			int X, Y;
			X = convert_char_int(c2); // aflu indecsii pentru insulele tocmai citite
			Y = convert_char_int(c3);

			for (i = 0; i < g->nr_insule; i++) {
				Node* head = g->a[i];
				if (g->a[i]->insula->index == X) 
					parcurgere_lista(head, Y, file_rezultate); // parcurg lista si verific daca exitsa conexiune sau nu
			}
		}

		if ((strcmp("adauga_zbor", c1)) == 0) { // functie de adaugare zbor intre  X - Y
			int timp, X, Y;
			fscanf(file_date, "%s  %s %d", c2, c3, &timp);
			X = convert_char_int(c2);
			Y = convert_char_int(c3);

			for (i = 0; i < g->nr_insule; i++) { // cand gasesc insula X fac legatura cu insula Y
				Node* adr = g->a[i];

				if (g->a[i]->insula->index == X) adr = insertNode(g, adr, Y, timp); 
				if (g->a[i]->insula->index == Y) adr = insertNode(g, adr, X, timp);
			}
		}

		if ((strcmp("legatura", c1)) == 0) { // afisez legatura insulei X cu celelalte insule
			fscanf(file_date, "%s", c2);
			int X;
			X = convert_char_int(c2);

			for (i = 0; i < g->nr_insule; i++) {
				Node* head = g->a[i];
				if (g->a[i]->insula->index == X) { // dupa ce am gasit insula X afisez insulele cu care se afla in legatura
					if (head->next == NULL) break;
					while (head != NULL) {
						if (head->next == NULL) break;
						fprintf(file_rezultate, "%s ", head->next->insula->nume);
						head = head->next;
					}
				}
			}
			fprintf(file_rezultate, "\n");
		}

		if ((strcmp("anulare_zbor", c1)) == 0) { // functie de stergere de conexiuni intre insule
			int cost, X, Y;
			fscanf(file_date, "%s  %s %d", c2, c3, &cost);
			X = convert_char_int(c2);
			Y = convert_char_int(c3);

			for (i = 0; i < g->nr_insule; i++) {
				Node* head = g->a[i];

				if (g->a[i]->insula->index == X) { // cand gasesc insula X sterg insula Y
					for (j = 0; j < g->nr_insule; j++) {
						if (g->a[j]->insula->index == Y)
							deleteNode(&head, Y);
					}
				}

				if (g->a[i]->insula->index == Y) {
					for (j = 0; j < g->nr_insule; j++) {
						if (g->a[j]->insula->index == X)
							deleteNode(&head, X);
					}
				}
			}
		}

		if ((strcmp("max_resurse", c1)) == 0) { // functie de afisare a nr  maxim de resurse distincte, urmat de numele lor 

			char sir[50][20];  int nr_resurse = 0, l;

			for (i = 0; i < g->nr_insule; i++) {
				for (j = 0; j < g->a[i]->insula->nrResurse; j++) { // copiez intr-un sir toate resursele si aflu nr total
					strcpy(sir[nr_resurse], g->a[i]->insula->inventarResurse[j].nume);
					nr_resurse++;
				}
			}

			ordonare_lexicografica(sir, nr_resurse); // le ordonez lexocografic
			for (i = 0; i < nr_resurse; i++) { // sterg dublurile resurselor
				for (j = i + 1; j <= nr_resurse; j++) {
					if (strcmp(sir[i], sir[j]) == 0) {
						for (l = j; l < nr_resurse; l++)
							strcpy(sir[l], sir[l + 1]);
						nr_resurse--; j--;
					}
				}
			}

			fprintf(file_rezultate, "%d ", nr_resurse);
			for (i = 0; i < nr_resurse; i++) 
				fprintf(file_rezultate, "%s ", sir[i]); // afisez sirul final
			fprintf(file_rezultate, "\n");
		}

		if ((strcmp("max_cantitate", c1)) == 0) { // functie de afisare a insulei cu cantitatea cea mai mare de resursa disponibila
			fscanf(file_date, "%s", c2);
			int  max_cantitate = 0;

			for (i = 0; i < g->nr_insule; i++) {
				for (j = 0; j < g->a[i]->insula->nrResurse; j++) { // pacurg nodurile din graf si gasesc mai intai cantitatea maxima din resursa respectia
					if (strcmp(g->a[i]->insula->inventarResurse[j].nume, c2) == 0) {
						if (max_cantitate < g->a[i]->insula->inventarResurse[j].cantitate)
							max_cantitate = g->a[i]->insula->inventarResurse[j].cantitate;
					}
				}
			}

			for (i = 0; i < g->nr_insule; i++) { // parcurg din nou nodurile din graf si afisez insula cu cantitatea maxima
				for (j = 0; j < g->a[i]->insula->nrResurse; j++) {
					if (strcmp(g->a[i]->insula->inventarResurse[j].nume, c2) == 0) {
						if (max_cantitate == g->a[i]->insula->inventarResurse[j].cantitate)
							fprintf(file_rezultate, "%s ", g->a[i]->insula->nume);
					}
				}
			}
			fprintf(file_rezultate, "\n");
		}

		if (((strcmp("drum_zbor", c1)) == 0) || ((strcmp("timp_zbor", c1)) == 0) || ((strcmp("min_zbor", c1)) == 0)) {  // functie de gasire a drumului cel mai scurt si a timpului min cu ajutorul algoritmului Dijkstra
			fscanf(file_date, "%s %s", c2, c3);
			int X, Y;
			X = convert_char_int(c2);
			Y = convert_char_int(c3);
			for (i = 0; i < g->nr_insule; i++) {
				if (g->a[i]->insula->index== X) // dupa ce am gasit insula X (sursa) caut dumul cel mai scurt si timpul min
					Dijkstra(file_rezultate, g, X, Y, c1);

			}
			fprintf(file_rezultate,"\n");
		}
		poz = ftell(file_date); // retin pozitia pentru a citi date pentru partea de GESTIUNE
	}
	char c;
	fscanf(file_date, "%c", &c);
	if (!feof(file_date) && c!='\0') { // 
		fseek(file_date, poz, SEEK_SET);
		Graph* graf = createM(file_date); // creez un nou graf prin matrice de adiacenta
		int i, j, count_insule1 = 0, count_insule2 = 0;

		for (i = 0; i < graf->nr_insule; i++) { // verific daca insulele respecta toleranta
			if (graf->val[i]->nrAvioane <= graf->val[i]->tolAvioane) count_insule1++;
			if (graf->val[i]->nrAvioane > graf->val[i]->tolAvioane) count_insule2++;
					
		}

		if (count_insule2 == graf->nr_insule) fprintf(file_rezultate, "Stack overflow!\n"); // daca toate insulele depasesc toleranta afisez un mesaj corespunzator
		else if (count_insule1 == graf->nr_insule) { // daca toate insulele respecta toleranta afisez fiecare insula cu avioanele ei
			for (i = 0; i < graf->nr_insule; i++) {
				fprintf(file_rezultate, "Island%d\n", i + 1);
				for (j = 0; j < graf->val[i]->nrAvioane; j++)
					fprintf(file_rezultate, "%d ", graf->val[i]->avioane[j]);
				fprintf(file_rezultate, "\n");
			}
			fprintf(file_rezultate, "\n");
		}
		else{ // altfel folosesc metoda backtaking pentru rezolvarea celorlalte cazuri
			int* avioane = NULL;
			backtracking(0, graf, avioane, file_rezultate);
			free(avioane);
			fprintf(file_rezultate, "\n");
		}

		free_memory_matrix(graf);
	}

	fclose(file_rezultate);
	fclose(file_date);
	free_memory(g);
	return 0;
}