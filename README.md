**Title: PROJECT\_SPEC.md**

Ten dokument pozwala zrozumieć pełen kontekst, reguły gry, architekturę komunikacyjną oraz logiczny podział na warstwy.

## ---

**Specyfikacja Projektowa: System Arbitrażowy „Arena Betonu”**

## **1\. Opis Ogólny Projektu**

Projekt to system do przeprowadzania turniejowych pojedynków sztucznej inteligencji (botów) w autorskiej grze logiczno-strategicznej. System składa się z dwóch niezależnych programów zewnętrznych (Gracz A i Gracz B) oraz **Arbitra (Silnika Gry)**. Arbiter zarządza procesami botów, weryfikuje zasady, wizualizuje stan rozgrywki w czasie rzeczywistym i trwale zapisuje historię pojedynków.

**Stos technologiczny:**

> * **Arbiter i Wizualizacja:** Dart / Flutter (Aplikacja desktopowa na systemy Windows/macOS/Linux).  
> * **Baza Danych:** SQLite (za pomocą sqflite\_common\_ffi).  
> * **Boty (Programy walczące):** Zewnętrzne pliki wykonywalne lub skrypty (np. Python, C++, Java, .exe, JavaScript, BASH, PowerShell) lub użytkownik, człowiek.

## ---

**2\. Mechanika i Zasady Gry**

> * **Plansza:** Siatka o domyślnych wymiarach zmienną \`DlugoscA=10\` **i** zmienną **\`**SzerokoscB=10\`**, tj \`\`\`DlugoscA x SzerokoscB\`\`\` pól** ( adresowanie pól dla planszy o wymiarze 10x10 pól zapisywane są w postaci zakresu liczb/indeksów od 0 do 9 dla \`**DlugoscA**\` i 0 do 9 dla osi \`**SzerokoscB**\`) reprezentuje najmniejszy możliwe wymiary planszy. Stosunek \`**DlugoscA\`** do \`**SzerokoscB**\` nie może przekroczyć współczynnika określonego w zmiennej \`**WSPOLCZYNNIK\_PROPORCJI\_WYMIAROW**\`, gdzie jego wartość jest liczbą, dodatnią, zmiennoprzecinkową \[z istotnym tylko 1 miejscem po przecinku\], parametryzowalną o wartości minimalnej 1.0 i wartości maksymalnej określonej zmienną \`MAX\_WSPOLCZYNNIK\_PROPORCJI\_WYMIAROW\` domyślnie ustawioną na wartość równa “3.0” ( jest liczbą, dodatnią, zmiennoprzecinkową \[z istotnym tylko 1 miejscem po przecinku\], max 10.0). Wymiar planszy można określić przed rozgrywką w panelu ustawień dla danej rozgrywki (dostępne w postaci klasycznego, rozwijanego menu), w której biorą udział uczestnicy (dwaj gracze). Arbiter informuje w pliku logu, w jego pierwszym wierszu, o rozmiarze planszy i zapisuje ID sesji (uuid \-v1 bazujący na czasie) przypisany do danej rozgrywki..  
  1. Wartość zmiennej \`MAX\_MOVE\_RANGE\` \= to wartośći calkowita (zaokrąglona w dół) określona wzorem: sqrt( ( 30% \* DlugoscA )^2 \+ ( 30% \* SzerokoscB )^2 )  
> * **Uczestnicy** (Ilość uczestników parametryzowana jest zmienna \`ILOSC\_GRACZY\`):  
  Gracze poruszający się po tej samej przestrzeni, domyślnie \`ILOSC\_GRACZY=2\`.  
  Maksymalna ilość graczy w grze zależna jest od wielkości/rozmiaru planszy.  
  Dla planszy 10x10, wzór na maksymalna ilość graczy to:


	Warunki gry:  
A.) 	{  
Plansza: 10x10,  
długość ruchu MAX\_MOVE\_RANGE: 3,   
gamers: 3  
}  
Pytanie: czy grac moze 3 graczy aka \`gamers=3\`  
	  
		Przestrzeń swobody: (10\*10)/(MAX\_MOVE\_RANGE+1) \= 25  
> > Przestrzeń ruchów: ( 10 \* 10 ) \- \`Przestrzeń swobody\` \= 100 \- 25 \= 75  
> > 

1. Tyle pol pokonać może jeden gracz w (zasada\_10ruchow\_minimum:=10) ruchach dla MAX\_MOVE\_RANGE, tj 30  
2. Gracz nigdy nie gra sam więc w (zasada\_10ruchow\_minimum:=10) jest (\`gamers\`-1) graczy i (\`gamers\` \-1) razy tyle pól do pokonania, czyli 2 \* \#1 \= 60 pol  
3. \`“75 pol to przestrzeń ruchow”\` \- \`“pol z \#2”\` \= 75 \- 60 \= 15  
4. \`15 z \#3\` \+ \`Przestrzen swobody\` \= 15 \+ 25 \= 40 pol  
5. Czy \`40 pol z \#4\` to wiecej niz \`\#1 ilosc pol pokonywanych w 10 ruchach 1 gracza\` ?

   

   cd \#5, czy \`40 z \#4\` \> \`30 z \#1\` ?

   Tak: 40 \> 30 \- TAK może grać \`gamers==3\` graczy

	Warunki gry:  
		B.) 	{  
Plansza: 10x10,  
długość ruchu MAX\_MOVE\_RANGE: 3,   
gamers: 4  
}  
Pytanie: czy grac moze 4 graczy aka \`gamers=4\`  
		  
		Przestrzeń swobody: (10\*10)/(MAX\_MOVE\_RANGE+1) \= 25  
> > Przestrzeń ruchów: ( 10 \* 10 ) \- \`Przestrzeń swobody\` \= 100 \- 25 \= 75  
> > 

1. Tyle pol pokonać może jeden gracz w (zasada\_10ruchow\_minimum:=10) ruchach dla MAX\_MOVE\_RANGE, tj 30  
2. Gracz nigdy nie gra sam więc w (zasada\_10ruchow\_minimum:=10) jest (\`gamers\`-1) graczy i (\`gamers\` \-1) razy tyle pól do pokonania, czyli 3 \* \`pol z \#1\` \= 90 pol  
3. \`“75 pol to przestrzeń ruchow”\` \- \`“pol z \#2”\` \= 75 \- 90 \= \-15  
4. \`-15 z \#3\` \+ \`Przestrzen swobody\` \= \-15 \+ 25 \= 10 pol  
5. Czy \`10 pol z \#4\` to wiecej niz \`\#1 ilosc pol pokonywanych w 10 ruchach 1 gracza\` ?

   cd \#5, czy \`10 z \#4\` \>  \`30 z \#1\` ?

   Nie:  10 \!\> 30 \- NIE może grać \`gamers==4\` graczy w tej grze

   

Warunki gry:  
		C.) 	{  
Plansza: 12x12,  
długość ruchu MAX\_MOVE\_RANGE: 3,   
gamers: 4  
}  
Pytanie: czy grac moze 4 graczy aka \`gamers=4\`  
		  
		Przestrzeń swobody: (12\*12)/(MAX\_MOVE\_RANGE+1) \= 144/4 \= 32  
> > Przestrzeń ruchów: ( 12 \* 12 ) \- \`Przestrzeń swobody\` \= 144 \- 32 \= 112  
> > 

1. Tyle pol pokonać może jeden gracz w (zasada\_10ruchow\_minimum:=10) ruchach dla MAX\_MOVE\_RANGE, tj 30  
2. Gracz nigdy nie gra sam więc w (zasada\_10ruchow\_minimum:=10) jest (\`gamers\`-1) graczy i (\`gamers\` \-1) razy tyle pól do pokonania, czyli 3 \* \`pol z \#1\` \= 90 pol  
3. \`“112 pol to przestrzeń ruchow”\` \- \`“pol z \#2”\` \= 112 \- 90 \= 22  
4. \`22 z \#3\` \+ \`Przestrzen swobody\` \= 22 \+ 32 \= 52 pol  
5. Czy \`52 pol z \#4\` to wiecej niz \`\#1 ilosc pol pokonywanych w 10 ruchach 1 gracza\` ?

   cd \#5, czy \`52 pol z \#4\` \>  \`30 pol z \#1\` ?

   Tak:  52 \> 30 \- TAK może grać \`gamers==4\` graczy w tej grze

   

   

   

   * **Gracz A** zaczyna z **pozycji domyślnej**.

   Pozycję tą określa się w następujący sposób:

   Należy określić geometryczny środka całej planszy i przyporządkować mu adres z obszaru planszy. Zapisujemy ten adres w postaci współrzędnych \`Xgeozero\`, \`Ygeozero\` na planszy. Adres **pozycji domyślnej gracza A** o współrzędnych \`XbaseA\`, \`YbaseA\`, jest także adresem startowym gracza A. Otrzymuje się go obliczając jego składowe \`XbaseA\` i \`YbaseA\` wzorami/regułą zapisaną następująco:

   \`\`\`

   XbaseA \= Xgeozero \- ABSOLUTE( FLOOR(0.5 \* MAX\_MOVE\_RANGE) \- (1 \* (RANDOM modulo 2)))

   YbaseA \= Ygeozero \- ABSOLUTE( FLOOR(0.5 \* MAX\_MOVE\_RANGE) \- (1 \* (RANDOM modulo 2)))

   \`\`\`

 


   * **Gracz B** zaczyna z **pozycji domyślnej**.

   Pozycję tą określa się w następujący sposób:

   Należy określić geometryczny środka całej planszy i przyporządkować mu adres z obszaru planszy. Zapisujemy ten adres w postaci współrzędnych \`Xgeozero\`, \`Ygeozero\` na planszy. Adres **pozycji domyślnej gracza B** o współrzędnych \`XbaseB\`, \`YbaseB\`, jest także adresem startowym gracza B. Otrzymuje się go obliczając jego składowe \`XbaseB\` i \`YbaseB\` wzorami/regułą zapisaną następująco:

\`\`\`  
XbaseB \= Xgeozero \+ ABSOLUTE( FLOOR(0.5 \* MAX\_MOVE\_RANGE) \- (1 \* (RANDOM modulo 2)))

YbaseB \= Ygeozero \+ ABSOLUTE( FLOOR(0.5 \* MAX\_MOVE\_RANGE) \- (1 \* (RANDOM modulo 2)))  
\`\`\`  
> >   
	Mechanizm wyznaczania pozycji startowej graczy, gdy graczy jest więcej niż 2:

1. Wyznaczyć pozycję domyślną graczy A i B wg reguł powyżej  
2. Stworzyć \`**zbiór\_graczy\_ustawionych**\` do którego należy dodać gracza A i gracza B.  
3. Dla każdego gracza nie znajdującego się w \`**zbiór\_graczy\_ustawionych\`** wykonaj następujące kroki aby ustalić jego adres **pozycji domyślnej:**  
4. Losowo wybierz ze \`**zbiór\_graczy\_ustawionych\`** gracza. Odczytaj jego adres pozycji domyślnej i zapisz jego współrzędne odpowiednio do zmiennych \`Xgamma\`, \`Ygamma\`. Zmienne \`Xgamma\`, \`Ygamma\`  użyjesz do obliczenia **pozycji domyślnej** **gracza Ni** (nieustawionego)**,** według następujących wzorów, gdzie \`XbaseNi\` i \`YbaseNi\` to współrzędne obliczanego adresu domyślnego nieustawionego gracza:

   

\`\`\`  
XbaseNi \= Xgamma \+ ( ((RANDOM modulo 2)==0 ? \-1:1) \* ABSOLUTE(FLOOR(0.5 \* MAX\_MOVE\_RANGE) \- (1 \* (RANDOM modulo 2))))  
> >   
> >   
YbaseNi \= Ygamma \+ ( ((RANDOM modulo 2)==0 ? \-1:1) \* ABSOLUTE(FLOOR(0.5 \* MAX\_MOVE\_RANGE) \- (1 \* (RANDOM modulo 2))))  
	\`\`\`

5. Należy sprawdzić czy adres o współrzędnych \`XbaseNi\`, \`YbaseNi\` znajduje się poza obszarem (gry). Jeśli jest poza obszarem gry wróć do instrukcji \#4 w celu wyliczenia innego adresu domyślnego, w przeciwnym razie kontynuuj do punktu następnego.  
6. Należy sprawdzić czy adres o współrzędnych \`XbaseNi\`, \`YbaseNi\` odpowiada współrzędnym adresu domyślnego jednego z graczy znajdującego się w zbiorze graczy ustawionych aka \`**zbior\_graczy\_ustawionych\`**. Jeśli adres jest unikalny i żaden inny gracz ze zbioru nie ma takiego adresu, przejdź do następnego punktu.  
7. Przypisać obliczony adres \`XbaseNi\`, \`YbaseNi\` do gracza jako jego adres domyślny i dodaj go do zbioru \`**zbior\_graczy\_ustawionych\`.**  
8. Jeśli istnieją jeśli istnieją gracze bez ustalonego adresu domyślnego przejdź do \#3.   
9. Zakończ ustalanie adresów domyślnych graczy gdyż wszyscy gracze, mają ustawiony adres domyślny i znajdują się w \`**zbior\_graczy\_ustawionych\`**

			  
> > 

> * **Fazy Tury Gracza:** Ruch i wylanie betonu są niepodzielne. Każda tura gracza **musi** składać się z obu tych kroków:  
  1. **Ruch:** Gracz przemieszcza się w pionie i/lub poziomie o **maksymalnie wartosc okreslona w zmiennej** MAX\_MOVE\_RANGE **jednostki** (możliwy jest ruch z zakresu 0, 1, 2 … MAX\_MOVE\_RANGE) w płaszczyźnie horyzontalnej i/lub wertykalnej.  
     * Gracz może poruszać się ruchem złożonym z ruchu horyzontalnego i wertykalnego, czyli po polach (kwadratach) które stykają się całym jednym bokiem.   
     * Gracz nie może przesunąć się ruchem skośnym, tj do pola które styka się z jego polem położenia tylko rogiem (punkt styku 2 boków kwadratu, który zajmuje) i traktować ten ruch jako ruch na odległość **Jednej Jednostki** (1 pola)\*  
       \* Gracz może przemieścić się do takiego pola sąsiadującego z jego polem położenia (stykającymi się rokiem) ruchem złożonym, tj, musi wykonać ruch o wartości 1 jednostki  wertykalnie i 1 jednostki horyzontalnie, wykonując tym samym ruch na odległość 2 jednostek. Ruch jest tylko wtedy dozwolony, gdy pola którymi się przemieszcza nie są zajęte. Gracz nie może przeskoczyć ani stanąć na polu zajętym przez beton lub drugiego gracza – cała ścieżka ruchu musi być pusta. W innym przypadku gracz dokonuje ruchu niedozwolonego i przegrywa swoją rozgrywkę (przestaje brać udział w grze \- móc się przemieszczać i wylewać beton, przyjmuje status przegranego).  
  2. **Betonowanie (Zapora):** Na koniec ruchu gracz musi wlać/wylać beton na **dokładnie jedno wolne pole**, które bezpośrednio sąsiaduje z jego docelową pozycją (pionowo, poziomo lub na ukos –  dowolne wolne sąsiadujące 1 pole). Beton staje się trwałą przeszkodą, która ma uniemożliwić przeciwnikowi możliwość ruchu, ucieczki.  
> * **Zasada Kończąca (Warunek Przegranej):** Gra kończy się, gdy gracz nie jest w stanie wykonać prawidłowego ruchu (jest zablokowany). W efekcie zmuszony jest wlać beton na pole, na którym sam się znajduje, co łamie nadrzędną zasadę gry (zakaz stania na przeszkodzie) i powoduje natychmiastową przegraną.

## ---

**3\. Protokół Komunikacji (Format Danych)**

Komunikacja odbywa się za pomocą standardowych potoków systemowych (stdin / stdout). Komunikaty są **jednoliniowymi ciągami tekstowymi**.

## Arbiter nadaje na czas danej rozgrywki każdemu graczowi uuid (-v4) identyfikującego go unikalnie w danej rozgrywce. To UUID jest pierwszym argumentem z ciągu 5 wartości; która arbiter musi przesłać do każdego z graczy informując każdego gracza o pozycji jego przeciwników na planszy i miejscu w którym kończąc swój ruch, wylali beton (w postaci adresu o współrzędnych Xb Yb). 

Przyklad: 

1) Gdy graczy jest dwoje (ILOSC\_GRACZY=2), arbiter przesyła do gracza A jedna linie danych w postaci 1 sekwencji pięciu argumentów o niezmiennej kolejności:  
   	\`\`\`  
   { UUID-graczaB, X, Y, Xb, Yb }\\n  
   \`\`\`

	Gdzie:  
		UUID-graczaB \- UUID przeciwnika, gracza B  
		X, Y \- adres, położenie UUID-przeciwnika w momencie wylewania betonu   
		Xb, Yb \- \- adres na planszy/mapie określający miejsce wylewania betonu

Transmisja sekwencji konczona jest ZAWSZE znakiem nowej linia w komunikacji stdio.

2) Gdy graczy jest troje (ILOSC\_GRACZY=3), arbiter przesyła do gracza A jedną linie danych w postaci 2 (dwóch) sekwencji składających się z pięciu argumentów każda, o określonej kolejności każdego z argumentów:  
   \`\`\`  
   {UUID-graczaB, X, Y, Xb, Yb}, {UUID-graczaC, X, Y, Xb, Yb}\\n  
   \`\`\` 

	Gdzie:  
		Sekwencja pierwsza:  
		UUID-graczaB \- UUID przeciwnika, gracza B  
		X, Y \- adres, położenie UUID-graczaB w momencie wylewania betonu   
		Xb, Yb \- \- adres na planszy/mapie określający miejsce wylewania betonu  
	  
		Sekwencja druga:  
		UUID-graczaC \- UUID przeciwnika, gracza C  
		X, Y \- adres, położenie UUID-graczaC w momencie wylewania betonu   
		Xb, Yb \- \- adres na planszy/mapie określający miejsce wylewania betonu

## 	Transmisja sekwencji konczona jest ZAWSZE znakiem nowej linia w komunikacji stdio.

## **Strumień Arbiter ➡️ Bot (Wysłanie stanu)**

Arbiter wysyła do bota, którego jest tura, dokładnie jedną linię informującą go w danej sekwencji, o **ostatnim ruchu danego przeciwnika** w formacie:  
\`\`\`  
{ UUID-przeciwnika, X, Y, Xb, Yb}\\n  
\`\`\`  
*(Gdzie X, Y to nowa pozycja przeciwnika, a Xb, Yb to współrzędne wylanego przez niego betonu)*.

> * **Przypadek Startowy:** W pierwszej turze meczu Gracz A nie ma przeciwnika, który wykonałby ruch. Arbiter wysyła do Gracza A sekwencję inicjalizującą postaci:

> \`\`\`  
> {UUID-przeciwnika, X, Y, \-1 \-1}\\n  
> \`\`\`

* Przypadek danych dla gracza który nie jest już aktywny (przegrał) a w grze pozostało jeszcze minimum 2 graczy.  
  Sekwencja argumentów przesyłana do grających/aktywnych graczy informująca ich nieruchomym/martwym/nieaktywnym/przegranym graczu ma postać:  
  \`\`\`

{ UUID-przegranego-gracza, X, Y, NaN, NaN }  
\`\`\`

	Arbiter czeka aby każdy z graczy zakończył swój ruchu (ruch gracza kończy się wylaniem/rozlaniem przez niego betonu) i dopiero wtedy wysyła do każdego z grających jeszcze graczy strumień danych opisany w protokole komunikacji. 

## **Strumień Bot ➡️ Arbiter (Odpowiedź bota)**

Bot w czasie swojej tury musi obliczyć ruch i wypisać na swoje standardowe wyjście dokładnie jedną linię w formacie:  
\`\`\`  
{ X, Y, Xb, Yb,}\\n  
\`\`\`  
*(Gdzie X, Y to jego nowa pozycja, a Xb, Yb to współrzędne wylewanego betonu)*. **Wymagane jest wymuszenie opróżnienia bufora wyjściowego (Flush).**

## ---

**4\. Reguły Zarządzania i Klasyfikacja Błędów przez Arbitra**

Arbiter musi ściśle przestrzegać reguł sprawdzania poprawności i kategoryzować koniec meczu zgodnie z poniższą specyfikacją błędów:

> * **Status Legalny (brak):** Gra toczy się dalej.  
> * **Kod błędu \#1 (Zabetonowany/Zablokowany):** Gracz nie ma możliwości ruchu i wlał beton na pole, na którym sam stoi.  
> * **Kod błędu \#2 (Nieprawidłowy ruch mechaniczny):**  
  * **\#2.a:** Gracz wszedł na pole zajęte przez beton lub drugiego gracza, bądź przeszkoda znajdowała się na trajektorii jego ruchu.  
  * **\#2.b:** Ruch niezgodny z matematycznymi zasadami: ruch o więcej niż MAX\_MOVE\_RANGE pól, ruch na ukos, wskazanie pola betonowania nie sąsiadującego z nową pozycją bota.  
> * **Kod błędu \#3 (Timeout):** Bot przekroczył dozwolony czas na odpowiedź (limit parametryzowalny, okreslony zmienna `zmienna:BOT_MAX_RESP_TIME`, równy domyślnie **1 sekundę**). Liczy się binarna weryfikacja czasu przez Arbitra.  
> * **Kod błędu \#4 (Błąd oprogramowania/formatowania):**  
  * Nadesłana linia jest pusta lub zawiera nieprawidłową liczbę danych.  
  * Dane nie są liczbami całkowitymi lub współrzędne wykraczają poza indeksy planszy 0-9.  
  * Wystąpił błąd krytyczny aplikacji bota (Crash, Access Violation, Null Pointer Exception), co Arbiter wykrywa poprzez sprawdzenie kodu wyjścia procesu (exitCode).

## **Reguła Specjalna: Obustronny Walkower**

Jeżeli **zarówno Gracz A, jak i Gracz B** wykonają ruch kwalifikujący się pod błędy **\#2, \#3 lub \#4 w swojej pierwszej turze**, mecz nie ma zwycięzcy. Arbiter ogłasza **obustronny walkower**. Jeśli tylko jeden gracz popełni błąd w 1\. turze, a drugi zagra poprawnie – ten drugi wygrywa.

## ---

**5\. Wytyczne dla Architektury Kodu (Szablon w VS Code)**

Dla Agenta AI, kod zaprezentowany w konwersacji stanowi **szablon strukturalny**. Przy generowaniu ostatecznych plików należy zachować następujący podział odpowiedzialności:

## **A. Warstwa Logiczna (GameState & WynikMeczu)**

> * Klasa GameState zarządza dwuwymiarową macierzą List\<List\<Pole\>\> oraz obiektami Pozycja.  
> * Metoda walidująca nie może modyfikować stanu gry przed upewnieniem się, że cały ruch jest legalny. Powinna zwracać odpowiedni enum KodKonca.

## **B. Warstwa Procesowa i Asynchroniczna (GameController)**

> * Uruchamianie botów przez Process.start z pełną obsługą strumieni asynchronicznych i transformacją danych (utf8.decoder, LineSplitter).  
> * Wykorzystanie .first.timeout(Duration(seconds: 1)) do wychwytywania błędu **\#3**.  
> * Zastosowanie opóźnień Future.delayed wyłącznie w celach płynności animacji dla interfejsu użytkownika.

## **C. Warstwa Trwałości Danych (DatabaseHelper & EksporterLogow)**

> * **Baza danych:** Tabela mecze przechowująca klucze: id, data\_meczu (ISO string), logi\_botow (surowy ciąg wszystkich linii wysłanych przez boty oddzielonych \\n) oraz podsumowanie.  
> * **Eksport:** Moduł operujący na File generujący czysty plik .txt.  
  * *Struktura pliku tekstowego:* Nagłówek ➡️ Sekwencja surowych ruchów z botów ➡️ Ostatnia linia zawierająca podsumowanie i precyzyjny kod błędu (np. ZAKONCZENIE: Wygrywa Gracz A. Gracz B zlamal zasade \#3...).

## **D. Warstwa UI (MaterialApp / GridView)**

> * Wizualizacja oparta na reaktywnym wzorcu (np. ListenableBuilder powiązany z GameController).  
> * Plansza renderowana jako dynamiczny GridView.builder (100 elementów) z obsługą kolorów stanów (puste \= zielony, beton \= szary, Gracz A \= czerwony, Gracz B \= niebieski).

## ---

**🤖 Instrukcja dla Agenta AI w VS Code**

*"Użyj powyższych reguł do wygenerowania kompletnej aplikacji Flutter. Zaimplementuj pełną obsługę błędów zgodnie z punktami \#1 do \#4, stwórz bazę danych SQLite oraz połącz widok GridView z kontrolerem procesów. Pamiętaj o asynchroniczności potoków IO w Dart oraz o wymaganiu poprawności formatu logu końcowego."*

Czy dla Agenta AI w VS Code potrzebujesz również **przykładowego, poprawnie napisanego bota w C++ lub Javie** realizującego ten protokół, aby mógł posłużyć jako zestaw testowy (mock) do weryfikacji wydajności i obsługi timeoutów?