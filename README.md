# Minimal Storage Simulator (C++)

## Opis
Projekt symuluje **blokowy system storage** w C++.
- Plik `disk.bin` działa jak wirtualny dysk SSD/RAID.
- Wbudowany **LRU cache** dla przyspieszenia operacji.
- Zlicza **read/write**, **cache hit/miss** oraz średni czas operacji w milisekundach.
- Interaktywny CLI umożliwia testowanie operacji na dysku.

## Komendy CLI
```
read <block>           # odczyt bloku
write <block> <text>   # zapis tekstu do bloku
stats                  # wyświetlenie statystyk
exit                   # zakończenie programu
```

## Kompilacja
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Uruchomienie
```bash
./storage_simulator_cpp
```

## Przykład użycia
```
> write 10 HelloWorld
written
> read 10
[block 10] data: 'HelloWorld'
> stats
reads: 1, writes: 1
cache hits: 1, cache misses: 0
avg read ms: 0.0032
avg write ms: 1.1602
```

## Uwagi
- Rozmiar bloku = 512 B  
- Rozmiar cache = 16 bloków  
- Plik `disk.bin` tworzony automatycznie przy pierwszym uruchomieniu

