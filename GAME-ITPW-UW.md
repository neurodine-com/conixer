# Title: **ConcrateMixers**

Ten dokument opisuje gre turowa, wzorowana na legendarnym Internetowy Turniej Programów Walczących (ITPW) organizowany w przeszłości przez Koło Naukowe Informatyków Uniwersytetu Warszawskiego. Dokument ten ma na celu spisanie informacji, ktore pozwolą stworzyć nowoczesna grę w środowisku Flutter w jezyku DART, bazującą na idei jednej z gier ITPW, tj gry "Betoniarek".  

## Czym w istocie były "programy walczace" we wspomnianym konkursie ITPW?

"Programy walczace" to programy (kompilowalne/skrypty powłok) pisane w dowolnym jezyku, ktore kommunikowaly sie z procesem Arbitra poprzez standardowe IO. Arbiter przekazywał graczom na przemian (w turach) ruchy ich porzeciwników i odzwierciedla je na planszy. Arbiter dodatkowo mógł wizualizować proces rozgrywki w czasie rzeczywistym.  

## Co było celem w konkursowej grze "Betoniarek" ITPW?

Celem gry bylo wyłonienie zwyciescy i jego zwycieskiego programu walczacego. Uczestnicy pisali programy, które walczyły w kontrolowanym środowisku Arbitra, przeciwko programom innych uczestnikow.
Program walczacy uczestnika, miał za zadanie sterować ruchami betoniarki na planszy 10x10, tak aby sukcesywnie ograniczać możliwosci ruchów betoniarki przeciwnika. Wygrywał ten gracz/program, ktory zablokował betorniarke przeciwnika - unimozliwił jej ruchy po planszy.

## Jak Ogranizatorzy ITPW realizowali rozgrywki konkursowe programow walczacych w grze "Betoniarek"?

Uczestnicy konkursu udostepniali (poprzez email,sftp) kod zrodlowy swoich programow ogranizatorom. Organizatorzy, kompilowali programy lokalnie (jesli bylo to konieczne) i uruchamiali iteracje gier przy użyciu procesu Arbitra, tak aby kazdy nadeslany program walczyl z programami pozostałych uczestnikow konkursu.  
Konkurs ITPW w grze "Betoniarek" wygrywal ten uczestnik, który nadesłał program/kod źrodłowy, który "walcząc" przeciwko programom innych uczestników konkursu, miał najwyższy stosunek gier wygranych w stosunku do przegranych.

## Co robił proces Arbitra?

Proces arbitra Tworzyl grę. Budował plansze 10x10 pól, na której grało 2 graczy, tj. 2 betoniarki.  
Arbiter gwarantował niezmienność zasad gry w calym konkursie, pomiedzy wszystkimi programami walczacymi.  
Odczytywal dane wyjsciowe z programow walczacych, sprawdzal je, wizualizowal rozgrywke na planszy.  
Okreslal zwyciezce gry i przegranego.  
Zapisywał cały log gry do pozniejszej weryfikacji.

Arbiter weryfikował czy:

1. Gra postepuje zgodnie z zasadami
   * czy gracz przemieszcza sie po pustych polach i w dozwolonym zakresie
   * czy gracz wylewa beton/betonuje pole, ktore jest puste
   * czy gracz przesyla prawidlowo sformatowane dane
   * czy gracz przeslal odpowiedz/dane w przeznaczonym do tego czasie (nie dluzszym niz 1 sek)
   * czy w wyniku ruchow graczy i postepującym ubywaniu pustych pól na planszy, jest wciaz mozliwa dalsza gra, tj. czy przeciwnicy moga przemieszczac sie po planszy i wylewac beton na puste pola.  

2. Zaszły warunki określajace zwycięzce gry  
Zwyciezca zostawał ten, który zabetonował przeciwnika. Zablokował możliwość ruchów betoniarki przeciwnika i ten w wyniku braku możliwości ruchu i wolnych pól, zmuszony był wylać beton na samgo siebie (pole na ktorym sam stał).

## Kim był gracz w grze Betoniarek  

Graczem w grze, na planszy była betoniarka, która była sterowana logika zaimplementowana w "programie walczacym" lub sterowana przy uzyciu myszki/klawiatury w iterakcja czlowieka z GUI Arbitra - plansza. Sterowanie betoniarka polegalo na wyslaniu na wyjscie komunikacyjne programu walczacego dwóch par koordynatow. Pierwsza para określała pole na planszy do którego betoniarka sie przesuwa, a druga para pole na które wylewa beton.  
W grze w której jedna z betoniarek sterował człowiek (interakacja z GUI Arbitra - plansza rozgrywki), betoniarka przemieszczała sie na wskazane myszka/kursorem/klawiatura pole a nastepnie wylawała beton na kolejno wskazane pole. Gdy betoniarka sterował czlowiek, timeout ruchu nie obowiązywał.  

Gdy w grze uczestniczyły betoniarki sterowane logika mająca swoje źródło w programach komunikujacych sie z IO Arbitra, mozliwe było przyspieszenie rozgrywki (pominiecie/przyspieszenie animacji przemieszczających sie betoniarek). Przycisk "przespieszenia" działa na wzór przycisku zwiekszania predkosci odtwarzania filmow.  

## Sytuacje w których Arbiter kończył gre z powodu błędów programów walczących

Arbiter okreslał koniec gry i tym samym zwyciezce gdy:
* gracz wykonał nieprawidlowy ruch - zwycięzcą zostawał jego przeciwnik
  * przemiesci sie na zajete pole
  * przemieszczajac sie po polach planszy które nie były puste:
    * były zabetonowane
    * stał na nich przeciwnik
* gracz wysyłał błędne koordynaty, np. poza obszarem planszy
* arbiter oczekiwał dluzej niz 1 sekunde na dane wyjsciowe z programu walczacego, tj, program walczący przekroczyl dozwolony limit czasu na odpowiedz wynoszacy 1 sekunde  

## Jakim zasadom podlegał ruch poplanszy w grze Betoniarek ITPW

Arbiter przeslał graczom na zmiane (w turach) pare punktów: nowe położenie przeciwnika; pole na którym przeciwnik wylewał beton. Program walczący musiał zapamietywac zmiany na planszy by wiedziec ktore pola sa wolne, a ktore zajete. Program walczacy obliczał sobie tylko znaną logiką, pole na ktore sie chce przesunac (wykonać swój ruch) i pole, na ktore chce wylac beton.  
Ruch i wylanie betonu są niepodzielne. Każdy ruch gracza (takze o dlugosci ZERO pól) musi zakończyć sie wylaniem betonu.  

* Gracz przemieszcza się w pionie i/lub poziomie o maksymalnie wartosc 2 (dwóch) pól. Możliwy jest ruch od 0 pól do 2 pól, w poziomie i/lub pionie, nigdy na ukos. Ruch o długości 0 pól oznacza, że gracz sie NIE przemieszcza, lub inaczej: przemieszcza sie o dystans równy ZERO pól.  

* Gracz może poruszać się ruchem złożonym, tj. pionowo i poziomow ale NIGDY na ukos. Ruch złożony to ruch po pustych polach planszy, takich które stykają się całym swoim bokiem, nie wierzchołkiem.  

* Po wykonaniu ruchu gracz musi wylać beton na dokładnie jedno wolne pole, z którym bezpośrednio sąsiaduje. Pole z betonem aka zabtonowane pole staje się trwałą przeszkodą, którą unimożliwia każdemu z graczy możliwość porzemieszczania się przez to pole.
