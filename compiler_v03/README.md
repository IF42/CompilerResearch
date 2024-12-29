# Compiler_v3
V této verzi jsem přidal binární operace a generování souvisejících assemblerových instrukcí, optimalizace práce s pamětí pomocí paměťových alokátorů a možnost výstup do souboru ze všech fází překladu.
Upravil jsem výstup abstraktního syntaktického stromu do formy pretty-print


Bylo by dobré zobecnit struktury, které tvoří formu dynamické kolekce pro ukládání hodnot za běhu. 
Zatím jsem vše testoval na systému Windows, ale bylo by potřeba implementovat také generování assemblerového kódu i pro systém Linux


Bylo by dobré vylepšit mechanismus generování assemblerových instrukcí tím, že do instrukce přidám textovou reprezentaci instrukce.
Po fázi parsování již v lexeru není potřeba uchovávat bufferované tokeny, které zbytečně zabírají paměť a je možné je uvolnit, otázkou je jestli to provést třeba na konci parsování a nebo explicitně zavolat nějakou funkci na vyčištění Lexeru v funkci main

Left-associative operátor znamená, že pokud se stejný operátor vyskytne vícekrát ve výrazu bez závorek, výpočet se provádí zleva doprava. Jinými slovy, operátory stejné priority se vyhodnocují od levé strany k pravé.



