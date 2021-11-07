Tema : Marele arhipelag Eao-Zinci


Prima data am creat graful g folosind liste de adiacenta.
Am alocat memorie pentru graf si pentru toate nodurile din graf; apoi citesc din fisier cate o insula si o adaug intr-un nod din graf (fiecare nod reprezinta capul fiecarei liste). Dupa ce am citit insulele, retin pozitia de unde am ramas in fisier ( de unde se dau conexiunile dintre insule: InsulaX - InsulaY) pentru a verifica toate cazurile: iau fiecare insula din graf si verific daca coincide cu insulele InsulaX citite din fisier, daca insulele coincid creez conexiunea intre insulele X-Y (folosind functia insertNode).
Am ales sa ma folosesc doar de nr insulelor pentru a afla mai repede pozitia insulelor in graf  (Insula1 => 1, pozitia 0):in functia convert_char_int: iau ultimele doua caractere din numele insulei ('dX' sau 'XX') si le convertesc in int. In functie de Y(sau X - graf neorientat) realizez conexiunea intre insule: verific mai intai daca lista contine doar un singur element (capul listei) sau nu, daca da - adaug insula care se afla pe pozitia y-1( sau x-1 - graf neorientat)la sfarsit, daca nu - caut pozitia unde trebuie adaugat.
Dupa ce am creat graful si am realizat conexiunile dintre insule trec la rezolvarea cerintelor:

- pentru partea de PROIECTARE:
la "conexiune": compar fiecare insula din graf cu insulaX, daca coincid, parcurg lista ce are ca si cap de lista insulaX si verific daca gasesc insulaY daca da afisez 'OK', daca nu 'NO'.
la "adaugare_zor": compar fiecare insula din graf cu insulaX, daca coincid, in functie Y realizez o noua conexiune intre insule x - y (functia insertNode)
la "anulare_zbor": am luat fiecare insula si am copratat-o cu Insulele X si Y, dupa ce le-am gasit sterg conexiunea (deleteNode)
la "max_resurse": iau fiecare resursa din fiecare insula si o pun intr-un vector(sir) si nr cate sunt, le ordonez lexicografic si dupa elimin dublurile si afisez nr de resrurse si resursele
la "max_cantitate":parcurg graful si gasesc cantitatea maxima a resursei, dupa il parcurg din nou si verific ce insula are maxim cantitate de resursa cautata si afisez insulele

- pentru partea de ANALIZA:
pentru cerintele "drum_zbor", "timp_zbor", "min_zbor" : folosesc algortimul Dijkstra pentru a determina drumul cel mai scurt dintre InsulaX-Insula Y(retin predecesorii), durata minima in conditii de distribuire instanta si in conditile adaugarii stationarii pe insulele intermediare pana la Y ( adug 15 minute pentru fiecare insula intermediara si scand 15 la insula destinatie)

-pentru partea de GESTIUNE:
am creat un nou graf folosind matrice de adiacenta si am citi din fisier, verific mai intai primele doua cazuri daca insulele respecta toleranta sau nu, iar pentru celelalte cazuri folosesc metoda backtracking - daca cerinta este valida creez solutia altfel parcurg graful pana ce gasesc nodul care sa respecte conditia


