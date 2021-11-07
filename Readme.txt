Tema 1: ATP Cup

In main:
Prima data am citit cerintele din fisier(c1, c2, ...c5)

Cerinta 1:
Am creat structurile necesare pentru a costrui TARA(nume, nr. jucatori...), JUCATOR(nume, ...) si pentru a crea LISTA, am citit din fisier informatiile necesare, am calculat media scorurilor jucatorilor (sorul initial pentru cerinta 2) si am creat lista dublu inlantuita circulara cu santinela.

Cerinta 2:
Am aflat mai intai nr de tari care trebuie sa ramana in turneu (cea mai mare putere a lui 2 posibila), apoi am eliminat tarile (cu funtia EliminareTara) cu cel mai mic scor initial (min se actualizeaza dupa fiecare eliminare) pana am ajuns la nr de tari dorit si am afisat tarile ramase in turneu dupa eliminare (pentru cerinta 1 am afisat toate tarile citite initial din fisier).

Cerinta 3: are toata rezolvarea in functia Clasament;
Functia Clasament:
Prima data am creat structurile necesare: pentru coada (COADA), pentru a retine meciurile (MECI), pentru stiva m-am folosita de structura listei (LISTA).
Am scos tarile din lista si le-am pus in stiva, dupa am eliberat spatiul ocupat de lista.
Am creat doua varibile de tipul TARA pentru a stoca succesiv 2 tari scoase din stiva si o variabila de tipul MECI pentru a pune meciurile dintre jucatori (fiecare jucator din prima tara joaca cu fiecare jucator din a doua tara), apoi creez o coada si bag meciurile in ea( cu functia AddCoada).
Scot cate un meci din coada, compar scorurile personale ale jucatorilor care se confrunta si dau/maresc/actualizez punctajul acestora si al tarii pe care o reprezint(scorul local) - fac asta pana cand coada meciurilor s-a golit (dupa o sterg si eliberez spatiul ocupt de variabila meci in care am stocat meciurile). Scorul local total obtinut in urma meciurilor jucate il adaug la scorul global.
In continuare compar scorurile locale ale celor 2 tari care se cofrunta si aflu tara castigatoare cu scorul mai mare si o adaug in stiva WINNER. Nr tarilor din stiva WINNER este jumatate din nr lor initial - fac asta pana cand stiva initiala este goala.
Afisez in fisier tarile din stiva WINNER, apoi le adaug din nou in stiva initiala (eliberez spatiul ocupat de stiva WINNER) si se vor repeta aceleasi procese de mai sus pana cand in stiva WINNER ramane o singura tara.
La sfarsit eliberez memoria ocupata de cele doua variabile de care m-am folosit pentru a stoca tarile si sterg stiva initiala.

Cerinta 4: 
M-am folosit de cerinta 3: am creat o stiva(clasament_tari) in care sa stochez ultimele 4 tari ramase in turneu, iar in functiile unde comparam scorurile locale am pus conditia: daca nr de tari este <= 4 si mai > 1 sa adaug tarile pierzatoare in stiva clasament (asta e pentru 3 tari, ultimma tara am adaugat-o in momentul cand in stiva WINNER a ramas o singura tara)
Apoi am scos din stiva cate o tara si am pus fiecare jucator intr-o variabila(jucator) pe care l-am bagat intr-o coada - fac asta pana cand stiva s-a golit. Am obtinut o coada cu toti jucatorii tarilor.
Scot din coada cate un jucator si il adaug in arbore astefel: caut in arbore scorul personal al jucatorului pe care doresc sa-l adaug, daca nu-l gasesc adaug jucatorul, altfel daca il gasesc: compar numele jucatorilor lexicografic iar castogatorul intra/ramane in arbore. In cazul in care castigatorul este jucatorul al carui scor l-am cautat in arbore, nodul cu acelasi scor existent in arbore il sterg si il adaug pe cel nou.
La sfarsit afisez jucatorii in ordine descrescatoare a scorurilor si eliberez memoria alocata dinamic.

Cerinta 5:
Citesc cei 2 jucatori din fisier si copiez informatiile despre ei in alte variabile (nume scor0.
Caut in arbore cei 2 jucatori. Daca ii gases pe amandoi afisez cati jucatori se afla intre ei cu ajutorul unei variabile pe care am declarat-o static, in afara functiei(nr_elemente_interval), daca gasesc doar un jucator afisez un mesaj corespunzator. La sfarsit eliberez memoria alocata dinamic.