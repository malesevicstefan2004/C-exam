# Dokumentacija – vCard Generator

Ova dokumentacija objašnjava kako funkcionišu ključni koncepti C/C++ koji su
korišteni u ovom programu. Pisana je kao vodič za učenje.

---

## 1. Struktura projekta

Program je podijeljen u tri logičke cjeline (moduli):

```
main.cpp      – ulazna tačka programa, parsiranje opcija
list/         – implementacija vezane liste (reusable modul)
vcard/        – gradnja vCard stringa i ispisivanje pomoći
```

Svaki modul ima odvojene direktorijume za zaglavlja (`include/`) i za
izvorni kod (`src/`). To je zahtjev profesora i standardna praksa u C/C++:
zaglavlje (.h) opisuje **šta** postoji, a izvorni fajl (.cpp) opisuje **kako**
to funkcioniše.

---

## 2. Šta je header fajl (.h)?

Header fajl je deklaracija — govori kompajleru da nešto postoji, ali ga ne
implementira. Primjer iz ovog projekta (`list.h`):

```cpp
struct VcardNode {
    int        order;   // redni broj za sortiranje
    char      *content; // pokazivač na string u heapu
    VcardNode *next;    // pokazivač na sljedeći čvor
};

void list_insert(VcardNode **head, int order, const char *content);
void list_free(VcardNode **head);
void list_print(const VcardNode *head, FILE *out);
```

Ovo je **samo obećanje** kompajleru. Pravi kod je u `list.cpp`.

### Header guard (#ifndef)

Svaki header fajl počinje zaštitom od višestrukog uključivanja:

```cpp
#ifndef _LIST_H_
#define _LIST_H_
// ... sadržaj headera ...
#endif /* _LIST_H_ */
```

Bez ovoga, ako dva fajla uključe isti header, kompajler bi vidio duple
definicije i vratio grešku. `#ifndef` to sprečava — drugi put kad je header
već uključen, kompajler preskače sadržaj.

---

## 3. Pokazivači (Pointers)

Pokazivač je varijabla koja čuva **adresu** u memoriji, a ne samu vrijednost.

```cpp
int broj = 42;
int *ptr = &broj;   // ptr čuva adresu varijable 'broj'

printf("%d\n", *ptr); // *ptr čita vrijednost na toj adresi → 42
*ptr = 99;            // mijenja vrijednost indirektno → broj je sada 99
```

U ovom programu pokazivači se koriste za:

- Čvorove vezane liste (`VcardNode *`)
- Stringove (`char *content`)
- Prosljeđivanje argumenata da bi funkcija mogla mijenjati original

### Pokazivač na pokazivač (`**`)

Funkcija `list_insert` prima `VcardNode **head`. Zašto dupla zvjezdica?

Jer funkcija mora moći promijeniti **sam head pointer** (npr. kad ubacuje
prvi čvor, ili čvor na početak liste). Kad prosljedimo `&head`, funkcija
dobija adresu od headera i može ga mijenjati:

```cpp
list_insert(&head, ORDER_NAME, "N:Doe;John;;;");
//           ↑ adresa headera, ne vrijednost
```

Unutar funkcije: `*head = node;` mijenja originalni head u pozivajućem kodu.

---

## 4. Heap memorija (dinamička alokacija)

Postoje dva mjesta gdje program čuva podatke:

| Stack (stek) | Heap (gomila) |
|---|---|
| automatski se oslobađa kad funkcija završi | ostaje dok se ne oslobodi ručno |
| ograničena veličina | praktično neograničena |
| lokalne varijable | `new` / `malloc` |

U ovom programu svaki čvor liste mora živjeti **duže od funkcije** koja ga
kreira, pa se alocira na heapu:

```cpp
VcardNode *node  = new VcardNode;          // novi čvor na heapu
node->content    = new char[strlen(s) + 1]; // string na heapu
strcpy(node->content, s);                  // kopiraj tekst u heap
```

#### Obavezno oslobađanje memorije

Sve što je alociraro s `new` mora biti oslobođeno s `delete`. To rade
`list_free`:

```cpp
void list_free(VcardNode **head) {
    VcardNode *cur = *head;
    while (cur != nullptr) {
        VcardNode *next = cur->next;
        delete[] cur->content; // oslobodi string (niz)
        delete cur;            // oslobodi čvor
        cur = next;
    }
    *head = nullptr;
}
```

Ako se zaboravi `delete`, nastaje **memory leak** — operativni sistem
ne može povratiti tu memoriju dok program ne završi.

---

## 5. Vezana lista (Linked List)

Vezana lista je podatkovna struktura gdje svaki element (čvor) čuva podatak
i pokazivač na sljedeći čvor:

```
[BEGIN:VCARD | order=0] → [VERSION:3.0 | order=0] → [N:Doe;John | order=1] → nullptr
```

Prednosti u odnosu na niz:
- Lako ubacivanje u sredinu bez pomjeranja ostalih elemenata
- Veličina ne mora biti poznata unaprijed

### Sortirano ubacivanje

`list_insert` prolazi kroz listu dok ne nađe čvor čiji je `order` **veći**
od novog, i ubacuje ispred njega:

```cpp
VcardNode **pos = head;
while (*pos != nullptr && (*pos)->order <= order) {
    pos = &(*pos)->next;  // napreduj kroz listu
}
node->next = *pos;  // novi čvor pokazuje na ostatak
*pos       = node;  // prethodni element pokazuje na novi čvor
```

Time se automatski postiže redoslijed: BEGIN/VERSION → N/FN → ORG → TEL →
EMAIL → REV/END, bez obzira kojim redoslijedom korisnik zada opcije.

---

## 6. Parsiranje opcija – getopt_long

`getopt_long` je standardna LibC funkcija za čitanje opcija komandne linije.

```cpp
static const struct option long_options[] = {
    { "help",      no_argument,       nullptr, 'h' },
    { "firstname", required_argument, nullptr, 'f' },
    { "surname",   required_argument, nullptr, 's' },
    // ...
    { nullptr, 0, nullptr, 0 }  // kraj tabele
};

int opt;
while ((opt = getopt_long(argc, argv, "hf:s:p:m:", long_options, nullptr)) != -1) {
    switch (opt) {
        case 'h': /* --help */ break;
        case 'f': /* --firstname, vrijednost je u optarg */ break;
        // ...
    }
}
```

- `"hf:s:p:m:"` — kratke opcije: `h` bez argumenta, `f:` zahtijeva argument
- `optarg` — globalna varijabla koja sadrži vrijednost opcije (npr. ime)
- `optind` — indeks prvog non-option argumenta (npr. naziv fajla)

---

## 7. Strukture (struct)

Struktura grupira više polja različitih tipova u jedan tip:

```cpp
struct VcardNode {
    int        order;    // redoslijed (0–5)
    char      *content;  // tekst linije (heap)
    VcardNode *next;     // sljedeći čvor
};
```

Pristup poljima se vrši s `.` (za objekt) ili `->` (za pokazivač):

```cpp
VcardNode node;
node.order = ORDER_NAME;       // direktan pristup

VcardNode *ptr = &node;
ptr->order = ORDER_NAME;       // pristup kroz pokazivač
```

---

## 8. Enum (nabrojani tip)

`enum` definiše skup imenovanim konstantama. Koristi se za redoslijed vCard
polja da kod bude čitljiv:

```cpp
typedef enum {
    ORDER_HEADER = 0,  // BEGIN, VERSION
    ORDER_NAME   = 1,  // N, FN
    ORDER_ORG    = 2,  // ORG
    ORDER_TEL    = 3,  // TEL
    ORDER_EMAIL  = 4,  // EMAIL
    ORDER_FOOTER = 5   // REV, END
} VcardOrder;
```

Bez enuma bismo koristili magične brojeve (0, 1, 2...) što je teže razumjeti.

---

## 9. Static funkcije

Funkcija označena s `static` u .cpp fajlu vidljiva je **samo unutar tog
fajla** — ne može je pozvati nikoji drugi modul:

```cpp
static void build_rev_line(char *buf, const size_t size) {
    // Ova funkcija je interna za vcard.cpp
    const time_t    now = time(nullptr);
    const struct tm *t  = gmtime(&now);
    strftime(buf, size, "REV:%Y%m%dT%H%M%SZ", t);
}
```

Ovo je dobra praksa — eksportuj samo ono što je zaista potrebno drugima.

---

## 10. Kako se program kompajlira

CMake automatski generiše Makefile na osnovu `CMakeLists.txt` fajlova:

```bash
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Redoslijed kompajliranja:
1. `list.cpp` → `list.cpp.o` (object fajl)
2. `vcard.cpp` → `vcard.cpp.o`
3. `main.cpp` → `main.cpp.o`
4. Linker spaja sve `.o` fajlove → `bin/vcard` (izvršna datoteka)

CMake koristi `target_include_directories` da kompajler zna gdje su header
fajlovi — zato u kodu pišemo `#include <list.h>`, a ne `#include "../../list/include/list.h"`.

---

## 11. Primjer kompletnog toka programa

```
vcard -f Thomas -s Staudacher --org TIA -m t@dhbw.de
```

1. `main()` poziva `getopt_long` u petlji
2. `-f Thomas` → `opts.firstnames[0] = "Thomas"`, `firstname_count = 1`
3. `-s Staudacher` → `opts.surname = "Staudacher"`
4. `--org TIA` → `opts.orgs[0] = "TIA"`, `org_count = 1`
5. `-m t@dhbw.de` → `opts.emails[0] = "t@dhbw.de"`, `email_count = 1`
6. `build_vcard(&opts)` kreira listu:
   - `list_insert` ubacuje "BEGIN:VCARD" (order=0)
   - `list_insert` ubacuje "VERSION:3.0" (order=0, ide iza BEGIN)
   - `list_insert` ubacuje "N:Staudacher;Thomas;;;" (order=1)
   - `list_insert` ubacuje "FN:Thomas Staudacher" (order=1)
   - `list_insert` ubacuje "ORG:TIA" (order=2)
   - `list_insert` ubacuje "EMAIL;TYPE=PREF,INTERNET:t@dhbw.de" (order=4)
   - `list_insert` ubacuje "REV:20260522T..." (order=5)
   - `list_insert` ubacuje "END:VCARD" (order=5, ide iza REV)
7. `list_print` ispisuje sve čvorove redom
8. `list_free` oslobađa svu heap memoriju
9. Program završava s `EXIT_SUCCESS`
