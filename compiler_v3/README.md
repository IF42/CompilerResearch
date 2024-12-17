# Compiler_v2
V této vývojové iteraci jsem přidal dvě unární operace bitový doplněk (~) a znaménkovou negaci (-). Dále jsem přidal rozklad AST na IR a následně IR na ASM. ASM je následně v několika průchodech dplněn operandy odkazující na lokální proměnné na zásobníku, alokaci paměti zásobníku a převod instrukcí odkazující v obou operandech na paměť na dvě, kde jeden z operandů odkazuje na registr, protože instrukce nemohou pracovat s pamětí v obou operandech.

Narazil jsem na problém správy paměti kdy je složitější v jazyce C paměť spravovat - trekovat místa, která je třeba uvolnit a která jsou sdílena, je třeba použít sofistikovanější správu paměti. Zároveň potřebuji vytvořit hashovací tabulku pro definici tabulky symbolů, která mi řekne který pseudo-registr/proměnnou jsem již zprocesoval. Také je třeba blok ASM instrukcí definovat spíš jako spojový seznam, než jako lineární pole kvůli flexibilitě kdy je třeba vkládat nebo odstraňovat některé instrukce.


Potřebuji vytvořit sofistikovanější mechanismus průchodů fázemi překladu. Průchod je definován iterováním jednotlivých instrukcí v daném bloku.

Pro vytvoření sofistikovanějšího mechanismu průchodů a zároveň pro zjednodušení mechanismu správy paměti by bylo dobré v další iteraci vytvořit tabulku symbolů ve formě seznamu operadnů. Díky tomu je možné jednotlivé operandy recyklovat v odkazech jednotlivých instrukcí což sníží paměťovou náročnost ale i zjednodušší správu paměti díky kompikovaným vlastnickým právům paměti.






