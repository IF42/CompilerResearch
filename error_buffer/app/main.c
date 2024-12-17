#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*
 * Chyby v kompilátoru je možné zaznamenávat a vypisovat různými způsoby.
 * První způsob je okamžitý výpis, kdy je chyba vypsána v místě vzniku, přičemž překlad zdrojového kódu je 
 * okamžitě přerušen, výhodou je jednoduchost realizace, ale nevýhoda je že není možné pokračovat v překladu pro získání 
 * všech chyb najednou
 * Další způsob je zotavení z chyby, kdy je chyba sice okamžitě vypsána, ale inforamce o chybě je zaznamenána a kompílátor
 * se pokusí zotavit z chyby a pokračovat v překladu, přičemž není i tak získat žádný výsledný spustitelný soubor, protože 
 * zdroj není validní.
 *
 * Chybový buffer je v principu pouze dynamické pole nebo jiná dynamická struktura, která umožňuje registrovat
 * nastalé chyby za běhu a následně jejich průchod a analýzu
 * 
 * Co je podstatné, tak je třeba do chybového záznamu zaznamenat všechny potřebné informace o nastalé události, ale i o
 * kontextu nastalé chyby
 * V podstatě nastalá událost nemusí být chyba, ale může se jednat o varování nebo jen nějakou logovací/debugovací
 * informaci
 *
 *
 * Co je podstatné je umístění chybového bufferu v rámci celého programu, případně může mít chybových bufferů více.
 * Nejdříve je třeba určit, které části mohou nějaké chyby či logové a debugovací informace vysílat. Tyto moduly je pak třeba
 * vzájemně propojit aby všechny měli k dispozici odkaz na chybový buffer
 * Další věc je jestli zda části programu mohou na chybový buffer přistupovat současně, tedy paralelně. V takovém případě je nutné
 * vyřešit synchronizaci přístupu a nebo definovat každé části vlastní chybový buffer, u kterých následně dojde k nějaké formě sloučení, 
 * nebo něco podobného
 *
 *
 * Nakonec je třeba vyřešit kde a jak budou nastalé chyby zpracovávány
 */
typedef struct {
    const char * msg;
}Err;


typedef struct {
    size_t capacity;
    size_t size;
    Err * arr;
}Buff;


Buff buff_init(void) {
    return (Buff) {0};
}


void err_assert(Buff * self, const char * msg) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->arr = realloc(self->arr, sizeof(Err) * self->capacity);
    }

    self->arr[self->size++] = (Err) {.msg = msg}; 
}


void buff_finalize(Buff * self) {
    if(self->arr != NULL) {
        free(self->arr);
    }
}


int main(void) {
    Buff buff = buff_init();

    err_assert(&buff, "This is error");
    err_assert(&buff, "This is another error");


    for(size_t i = 0; i < buff.size; i++) {
        printf("%s\n", buff.arr[i].msg);
    }


    buff_finalize(&buff);

    printf("Program exit..\n");
    return EXIT_SUCCESS;
}



