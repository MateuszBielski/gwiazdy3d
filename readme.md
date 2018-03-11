Program przeznaczony do kompilacji w systemie Windows.
Z plików danych pochodzących z programu stellarium odczytuje parametry, 
na ich podstawie oblicza lokalizację gwiazd w przestrzeni 3D.
Punktem 0,0,0 układu odniesienia jest Ziemia.
Gwiazdy wyświetlane są jako okrągłe plamki.
Wielkość gwiazd jest różna  - wyliczana jest na podstawie absolutnej wielkości gwiazdowej.
Kolor gwiazd wyświetlany jako odwzorowanie typu widmowego: 
od niebieskich - najgorętszych, poprzez białe i żółte (jak Słońce) do czerwonych.

Aby łatwiej zorientować się co widać, zastosowano kilka ułatwień:
gwiazdy z przestrzeni można rzutować na sferę - widok jest wtedy tak jak z powierzchni ziemi
można wyświetlić linie łączące rzeczywiste położenie gwiazd z rzutem na sferę.

Funkcjonalność:
wybór i wyświetlenie dowolnych gwiazdozbiorów,
wyszukanie i wycentrowanie na gwiazdę według jej numeru z katalogu Hipparcosa.
przesuwanie, obracanie i skalowanie widoku.
Obliczanie odległości miedzy gwiazdami,
Wyświetlanie nazw ważniejszych gwiazd.

Optymalizacja:
liczba wczytywanych gwiazd przekracza 100 000. 
aby zachować płynność wyświetlania obrazu, na podstawie wczytanych parametrów, specjalny algorytm wybiera gwiazdy przeznaczone do renderowania.
Sortowanie gwiazd mające na celu pokazanie sąsiedztwa odbywa się w drugim wątku.
Przeładowanie programu, czyli ponowne wczytanie parametrów wyświelania również odbywa się w drugim wątku.

Niestety po migracji ze środowiska NetBeans do CodeLite w programie ujawnił się błąd, który nie pozwala na uruchomienie.