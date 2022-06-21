/** @file
* Implementacja interfejsu phone_forward.h.
*
* @author Maria Wysogląd
* @date 2022
*/
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "phone_forward.h"

#define CORRECT 0 ///< Arbitralnie wybrana stała przekazująca informację o poprawności.
#define ERROR 1 ///< Arbitralnie wybrana stała przekazująca informację o niepoprawności.
#define END 2 ///< Arbitralnie wybrana stała przekazująca informację o końcu napisu.
#define ALPHABET_SIZE 12 ///< Cyfr od 0 do 9 jest 10, -plus cyfry 10 i 11 reprezentowane jako * i #.
#define TEN '*' ///< Stała odpowiadająca znakowi '*' = 10.
#define ELEVEN '#' ///< Stała odpowiadająca znakowi '#' = 11.

/**
 * @brief Zmienia znak cyfry na odpowiadającą mu liczbę.
 * @param[in] n - dany znak.
 * @return Całkowita liczba odpowiadająca danej cyfrze.
 */
static int conversion(char n) {
    if (n == TEN) {
        return 10;
    }
    else if (n == ELEVEN) {
        return 11;
    }
    
    return n - '0';
}

/**
 * @brief To jest komparator napisów.
 * Porównuje ze sobą napisy pod kątem porządku leksykograficznego.
 * Służy do porównywania słów w tworzonych przez nas tablicach, dlatego
 * wiemy, że te słowa są poprawne.
 * @param[in] a - pierwszy numer (napis),
 * @param[in] b - drugi numer (napis).
 * @return Zwraca 1, gdy znak drugiego numeru jest większy, -1 gdy jest 
 *         odwrotnie lub 0, gdy napisy są równe.
 */
static int compare(char* a, char*b) {
    if ((a != NULL) && (b != NULL)) {
        size_t i = 0;
        while ((a[i] != '\0') && (b[i] != '\0')) {
            if (conversion(b[i]) > conversion(a[i])) {
                return 1;
            }
            else if (conversion(a[i]) > conversion(b[i])) {
                return -1;
            }
            else {
                i++;
            }
        }

        if (a[i] != '\0') {
            return -1;
        }
        else if (b[i] != '\0') {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

/**
 * @brief To jest struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers {
    char **table_of_phone_numbers; ///< Wskaźnik na tablicę numerów telefonów.
    size_t size; ///< Rozmiar tablicy.
};

/**
 * @brief To jest struktura przechowująca przekierowania numerów telefonów.
 * Przechowuję przekierowania w formie drzewa prefiksowego,
 * gdzie jeśli kolejną cyfrą w prefiksie pierwotnego drzewa prefiksowego jest
 * i, tablica children[i] trzyma przekierowanie kolejnej cyfry.
 */
struct PhoneFWD {
    struct PhoneFWD *children[ALPHABET_SIZE]; ///< Dalsze litery pierwotnego prefiksu.
    char *prefix; ///< Wskaźnik na nowy prefiks.
    struct PhoneFWD *father; ///< Wskaźnik na poprzedni węzeł drzewa przekierowań.
};
/**
 * Tworzy typ PhoneFWD.
 */
typedef struct PhoneFWD PhoneFWD;

/**
 * @brief To jest struktura przechowująca odwrócone drzewo numerów telefonów.
 * Przechowuję przekierowania w formie drzewa prefiksowego z tym, że tym razem
 * to znaki przekierowania są w formie węzłów, a prefiksy, które przekierowują
 * trzymane są w tablicy PhoneNumbers.
 */
struct PhoneReversed {
    struct PhoneReversed *children[ALPHABET_SIZE]; ///< Dalsze litery przekierowania.
    struct PhoneNumbers *table_of_prefixes; ///< Wskaźnik na tablicę prefiksów.
    struct PhoneReversed *father; ///< Wskaźnik na poprzedni węzeł drzewa odwróconego.
};
/**
 * Tworzy typ PhoneReversed.
 */
typedef struct PhoneReversed PhoneReversed;

/**
 * @brief To jest struktura przechowująca drzewo prefiksów i przekierowań.
 */
struct PhoneForward {
    PhoneFWD *new_tree; ///< Drzewo prefiksów.
    PhoneReversed *reversed_tree; ///< Odwrócone drzewo przekierowań.
};

/**
 * @brief Tworzy zaalokowaną strukturę PhoneFWD.
 * @return Zwraca zaalokowaną strukturę PhoneFWD.
 */
static PhoneFWD * phfwdNew_help(void) {
    PhoneFWD *new_struct = malloc(sizeof(PhoneFWD));
    // Sprawdzam, czy alokowanie pamięci działa.
    if (new_struct != NULL) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            new_struct->children[i] = NULL;
        }

        new_struct->prefix = NULL;
        new_struct->father = NULL;
    }

    return new_struct;
}

/**
 * @brief Tworzy zaalokowaną strukturę PhoneReversed.
 * @return Zwraca zaalokowaną strukturę PhoneReversed.
 */
static PhoneReversed * phfwd_rev_New_help(void) {
    PhoneReversed *new_struct = malloc(sizeof(PhoneReversed));
    // Sprawdzam, czy alokowanie pamięci działa.
    if (new_struct != NULL) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            new_struct->children[i] = NULL;
        }

        new_struct->table_of_prefixes = NULL;
        new_struct->father = NULL;
    }

    return new_struct;
}

PhoneForward * phfwdNew(void) {
    PhoneForward *new_struct = malloc(sizeof(PhoneForward));
    if (new_struct != NULL) {
        new_struct->new_tree = phfwdNew_help();
        new_struct->reversed_tree = phfwd_rev_New_help();
    }

    return new_struct;
}

/**
 * @brief Funkcja tworzy jednoelementowy węzeł PhoneNumbers.
 * @return Wskaźnik na zaalokowaną strukturę.
 */
static PhoneNumbers * phnum_new_one(void) {
    PhoneNumbers *new = malloc(sizeof(PhoneNumbers));
    if (new != NULL) {
        new->size = 1;
        new->table_of_phone_numbers = malloc(sizeof(char*));

        if (new->table_of_phone_numbers != NULL) {
            new->table_of_phone_numbers[0] = NULL;
        }
    }

    return new;
}

/**
 * @brief Funkcja usuwa drzewo prefiksowe przekierowań.
 * @param[in] pf - wskaźnik na drzewo.
 */
static void phfwdDelete_help(PhoneFWD *pf) {
    // Funkcja iteracyjnie usuwa dane poddrzewo, o korzeniu w pf.
    if (pf == NULL) {
        return;
    }

    PhoneFWD *current = pf;
    PhoneFWD *parent = NULL;
    PhoneFWD *root = pf->father;

    while (current != root) {
        bool if_continue = 1;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (current->children[i] != NULL) {
                current = current->children[i];
                if_continue = 0;
                break;
            }
        }

        if (if_continue) {
            parent = current->father;
            if (parent != NULL) {
                for (int i = 0; i < ALPHABET_SIZE; i++) {
                    if (parent->children[i] == current) {
                        parent->children[i] = NULL;
                        break;
                    }
                }
            }

            free(current->prefix);
            free(current);
            current = parent;
        }
    }  
}

void phnumDelete(PhoneNumbers *pnum) {
    if (pnum != NULL) {
        if (pnum->table_of_phone_numbers != NULL) {
            for (size_t i = 0; i < (pnum->size); i++) {
                if (pnum->table_of_phone_numbers[i] != NULL) {
                    free(pnum->table_of_phone_numbers[i]);
                }
            }
            free(pnum->table_of_phone_numbers);
        }
        free(pnum);
    }
}

/**
 * @brief Funkcja usuwa odwrócone drzewo prefiksowe.
 * @param[in] pf - wskaźnik na odwrócone drzewo.
 */
static void phfwdDelete_rev_help(PhoneReversed *pf) {
    // Funkcja iteracyjnie usuwa dane poddrzewo, o korzeniu w pf.
    if (pf == NULL) {
        return;
    }

    PhoneReversed *current = pf;
    PhoneReversed *parent = NULL;
    PhoneReversed *root = pf->father;

    while (current != root) {
        bool if_continue = 1;

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (current->children[i] != NULL) {
                current = current->children[i];
                if_continue = 0;
                break;
            }
        }

        if (if_continue) {
            parent = current->father;
            if (parent != NULL) {
                for (int i = 0; i < ALPHABET_SIZE; i++) {
                    if (parent->children[i] == current) {
                        parent->children[i] = NULL;
                        break;
                    }
                }
            }

            if (current != NULL) {
                if (current->table_of_prefixes != NULL) {
                    if (current->table_of_prefixes->table_of_phone_numbers
                        != NULL) {
                        for (size_t i = 0;
                             i < (current->table_of_prefixes->size); i++) {
                            if 
                            (current->table_of_prefixes->table_of_phone_numbers
                            [i] != NULL) {
                                free
                                (current->table_of_prefixes->table_of_phone_numbers[i]);
                            }
                        }
                        free(current->table_of_prefixes->table_of_phone_numbers);
                    }
                    free(current->table_of_prefixes);
                }

                if (current->children != NULL) {
                    free(current->children);
                }

                current = parent;
            }
        }
    }  
}

void phfwdDelete(PhoneForward *pf) {
    if (pf != NULL) {
        phfwdDelete_help(pf->new_tree);
        phfwdDelete_rev_help(pf->reversed_tree);
        free(pf);
    }
}

/**
 * @brief Sprawdza znaczenie znaku.
 * Funkcja sprawdza, czy dany znak jest cyfrą lub znakiem końca napisu,
 * czy też zawiera błędny inny znak.
 * @param[in] character - konkretny znak z napisu.
 * @return Dana stała, która określa, czy dany znak kończy napis, jest cyfrą
 *         lub jest niepoprawny.
 */
static int if_correct(char character) {
    if (character == '\0') {
        return END;
    }

    if ((character == TEN) || (character == ELEVEN)) {
        return CORRECT;
    }

    if ((character > '9') || (character < '0')) {
        return ERROR;
    }
    else {
        return CORRECT;
    }
}

/**
 * @brief Sprawdza poprawność danego napisu.
 * Funkcja sprawdza, czy podany napis jest poprawny, czyli czy
 * nie zawiera znaku różnego od cyfry i znaku '\0'.
 * @param[in] num - wskaźnik na dany napis.
 * @return Wartość @p true, jeśli dany napis jest nieprawidłowy.
 *         Wartość @p false, jeśli dany napis jest prawidłowy.
 */
static bool error(char *num) {
    size_t i = 0;
    while (if_correct(num[i]) != END) {
        if (if_correct(num[i]) == ERROR) {
            return true;
        }
        i++;
    }
    
    return false;
}

/**
 * @brief Sprawdza, czy dane 2 napisy są takie same.
 * @param[in] num1 - wskaźnik na pierwszy napis;
 * @param[in] num2 - wskaźnik na drugi napis.
 * @return Wartość @p true, gdy napisy są takie same. 
 *         Wartość @p false, gdy napisy są różne.
 */
static bool check_if_same (char *num1, char *num2) {
    size_t i = 0;
    while ((num1[i] != '\0') && (num2[i] != '\0')) {
        if (num1[i] != num2[i]) {
            return false;
        }
        i++;
    }

    if (num1[i] != num2[i]) {
        return false;
    }
    
    return true;
}



/**
 * @brief Sprawdza obecność przypadków szczególnych w napisach.
 * Funkcja sprawdza, czy dane 2 napisy są takie same, czy któryś z nich
 * jest pusty lub niepoprawny. W takim przypadku przekazuję tę informację dalej.
 * @param[in] num1 - wskaźnik na dany napis;
 * @param[in] num2 - wskaźnik na dany napis.
 * @return Wartość @p true, gdy nie zachodzą specjalne przypadki,
 *         wartość @p false, gdy zachodzi któryś przypadek.
 */
static bool special_cases(char *num1, char *num2) {
    if ((num1 == NULL) || (num2 == NULL) ||
        (num1[0] == '\0') || (num2[0] == '\0')) {
        return false;
    }

    if (error(num1) || error(num2)) {
        return false;
    }

    if (check_if_same(num1, num2) == 1) {
        return false;
    }

    return true;
}

/**
 * @brief Funkcja znajduje prefiks, który trzeba usunąć w konkretnym węźle.
 * @param[in, out] pf - wskaźnik na drzewo odwrócone;
 * @param[in] num - usuwany napis.
 */
static void remove_cell(PhoneReversed *pf, char *num) {
    if (pf->table_of_prefixes != NULL && 
        pf->table_of_prefixes->table_of_phone_numbers != NULL) {
        size_t count_rubbish = 0;
        size_t index = pf->table_of_prefixes->size - 1;

        for (size_t i = pf->table_of_prefixes->size ; i > 0; i--) {
            size_t j = 0;
            /* Szukamy różnycy pomiędzy num i napisem, jeśli num jest
            zawiera się w napisie, usuwamy napis. */
            while ((if_correct(num[j])!= END) && 
                  (if_correct
                  (pf->table_of_prefixes->table_of_phone_numbers[i - 1][j]
                  != END)) && 
                  (num[j] == pf->table_of_prefixes->table_of_phone_numbers[i - 1][j])) {
                j++;
            }

            if ((i == 1) && (index == 0) && (if_correct(num[j]) == END)) {
                for (size_t ii = pf->table_of_prefixes->size; ii > 0; ii--) {
                    free(pf->table_of_prefixes->table_of_phone_numbers[ii - 1]);
                         pf->table_of_prefixes->size = 0;
                }
                return;
            }
            else if (if_correct(num[j]) == END) {
                char* tmp = pf->table_of_prefixes->table_of_phone_numbers[index];
                pf->table_of_prefixes->table_of_phone_numbers[index] = pf->table_of_prefixes->table_of_phone_numbers[i - 1];
                pf->table_of_prefixes->table_of_phone_numbers[i - 1] = tmp;
            
                index--;
                count_rubbish++;
            }
        }

        size_t size = pf->table_of_prefixes->size - count_rubbish;
        if (size != 0) {
            for (size_t ii = pf->table_of_prefixes->size; ii > size; ii--) {
                free(pf->table_of_prefixes->table_of_phone_numbers[ii - 1]);
            }
            pf->table_of_prefixes->table_of_phone_numbers =
            realloc(pf->table_of_prefixes->table_of_phone_numbers, size * 
                   (sizeof(char*)));
            pf->table_of_prefixes->size = size;
        }
        else {
            for (size_t ii = pf->table_of_prefixes->size; ii > 0; ii--) {
                free(pf->table_of_prefixes->table_of_phone_numbers[ii - 1]);
            }
            pf->table_of_prefixes->size = size;
        }
    }
}

/**
 * @brief Funkcja znajduje konkretny węzeł, z którego trzeba usunąć przy nadpisaniu. przekierowania z drzewa odwróconego.
 * @param[in, out] pf - wskaźnik na strukturę z odwróconym drzewem;
 * @param[in] where - napis, za pomocą którego znajdujemy węzeł;
 * @param[in] what_to_remove_cell - usuwany napis.
 */
static void remove_cell_certain (PhoneForward *pf, char *where,
                                 char *what_to_remove_cell) {
    if (pf != NULL) {
        if (where != NULL && where[0] != '\0') {
            PhoneReversed *current = pf->reversed_tree;
            size_t i = 0;
            while ((where[i] != '\0') &&
                  (current->children[conversion(where[i])] != NULL )) {
                current = current->children[conversion(where[i])];
                i++;
            }

            if (where[i] == '\0') {
                remove_cell(current, what_to_remove_cell);
            }
        }
    }
}

/**
 * @brief Funkcja spełnia zadanie phfwdAdd dla drzewa nieodwróconego.
 * @param [in, out] pf - wskaźnik na strukturę przechowującą przekierowania
 *                       numerów;
 * @param[in] num1 - wskaźnik na napis reprezentujący prefiks numerów
 *                   przekierowywanych;
 * @param[in] num2 - wskaźnik na napis reprezentujący prefiks numerów,
 *                   na które jest wykonywane przekierowanie;
 * @param pf1 - wskaźnik na strukturę PhoneForward przekazywaną dalej 
 *              w przypadku nadpisania.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się alokować pamięci.
 */
static bool phfwdAdd_help(PhoneFWD *pf, char *num1, char *num2,
                          PhoneForward *pf1) {
    /* Na początku sprawdzam przypadki szczególne, oznaczające
    brak dalszej pracy na wskaźniku pf. */
    PhoneFWD *current_node = pf;
    if (pf == NULL) {
        return 0;
    }

    size_t i = 0;
    /* Szukamy, czy w dzieciach jest już dana cyfra, jak nie, tworzymy nowego 
    syna i jego dalej przeszukujemy */
    while (if_correct(num1[i]) == CORRECT) {
        if (current_node->children[conversion(num1[i])] != NULL) {
            current_node = current_node->children[conversion(num1[i])];
        }
        else {
            PhoneFWD *tmp = phfwdNew_help();
            if (tmp == NULL) {
                return 0;
            }
            tmp->father = current_node;
            current_node->children[conversion(num1[i])] = tmp;
            current_node = current_node->children[conversion(num1[i])];
        }
        i++;
    }

    // Na koniec zapisujemy napis, na który przekierowujemy.
    size_t z = 0;
    while (if_correct(num2[z]) == CORRECT) {
        z++;
    }

    if (current_node->prefix != NULL) {
        remove_cell_certain(pf1, current_node->prefix, num1);
    }

    free(current_node->prefix);
    current_node->prefix = malloc(sizeof(char)*(z + 1));
    if (current_node->prefix == NULL) {
        return 0;
    }

    for (size_t j = 0; j < z + 1; j++) {
        current_node->prefix[j] = num2[j];
    }

    return 1;
}

/**
 * @brief Funkcja spełnia zadanie phfwdAdd dla drzewa odwróconego.
 * @param [in, out] pf - wskaźnik na strukturę przechowującą prefiksy.
 * @param[in] num1 - wskaźnik na napis reprezentujący prefiks numerów
 *                   na które jest wykonywane przekierowanie;
 * @param[in] num2 - wskaźnik na napis reprezentujący prefiks numerów,
 *                   przekierowywanych;
 *                   
 * @return Wartość @p true, jeśli prefiksy zostały dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się alokować pamięci.
 */
static bool phfwdAdd_rev_help(PhoneReversed *pf, char *num1, char *num2) {
    PhoneReversed *current_node = pf;
    if (pf == NULL) {
        return 0;
    }

    size_t i = 0;
    /* Szukamy, czy w dzieciach jest już dana cyfra, jak nie, tworzymy nowego 
    syna i jego dalej przeszukujemy */
    while (if_correct(num1[i]) == CORRECT) {
        if (current_node->children[conversion(num1[i])] != NULL) {
            current_node = current_node->children[conversion(num1[i])];
        }
        else {
            PhoneReversed *tmp = phfwd_rev_New_help();
            if (tmp == NULL) {
                return 0;
            }
            tmp->father = current_node;
            current_node->children[conversion(num1[i])] = tmp;
            current_node = current_node->children[conversion(num1[i])];
        }
        i++;
    }

    // Na koniec zapisujemy napis, na który przekierowujemy.
    size_t z = 0;
    while (if_correct(num2[z]) == CORRECT) {
        z++;
    }

    size_t size = 1;
    if (current_node->table_of_prefixes == NULL) {
        current_node->table_of_prefixes = phnum_new_one();
        if (current_node->table_of_prefixes == NULL) {
            return 0;
        }

    }
    else {
        size = current_node->table_of_prefixes->size + 1;
        current_node->table_of_prefixes->size = size;
        current_node->table_of_prefixes->table_of_phone_numbers =
        realloc(current_node->table_of_prefixes->table_of_phone_numbers,
                size * (sizeof(char*)));
        if (current_node->table_of_prefixes->table_of_phone_numbers == NULL) {
            return NULL;
        }
    }

    char *tmp = malloc((z + 1) * sizeof(char));
    if (tmp == NULL) {
        return NULL;
    }
    current_node->table_of_prefixes->table_of_phone_numbers[size-1] = tmp;

    for (size_t j = 0; j < (z + 1); j++) {
        current_node->table_of_prefixes->table_of_phone_numbers[size-1][j] = 
        num2[j];
    }

    return 1;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    /* W funkcji phfwdAdd_help wykonujemy opis phfwdAdd, ale musimy zmienić
    dostępność napisów num1 i num2 (usuwanie stałości napisów). 
    Wobec tego phfwdAdd jest swego rodzaju funkcją pośredniczącą. */
    if (pf == NULL) {
        return 0;
    }

    if (special_cases((char*)num1, (char*)num2) == 0) {
        return 0;
    }

    // Musimy przekazywać wskaźnik na oba drzewa, by móc usunąć nadpisane przekierowanie z drzewa odwróconego.
    bool odp = phfwdAdd_help(pf->new_tree, (char*)num1, (char*) num2, pf);
    if (odp == 0) {
        return odp;
    }
    else {
        odp = phfwdAdd_rev_help(pf->reversed_tree, (char*)num2, (char*)num1);
    }

    return odp;
}

/**
 * @brief Funkcja usuwająca przekierowanie z drzewa nieodwróconego.
 * @param[in] pf - wskaźnik na drzewo.
 * @param[in] num - wskaźnik na usuwany numer.
 */
static void phfwdRemove_help(PhoneFWD *pf, char const *num) {
    PhoneFWD *current_node = pf;
    int i = 0;
    /* W <last_node> trzymamy wskaźnik na ostatni węzeł, którego nie
    chcemy usuwać, gdyż ma syna innego niż należący do prefiksu, lub ma 
    przekierowanie. */
    while ((if_correct(num[i]) != END) &&
          (current_node->children[conversion(num[i])] != NULL)) {
        current_node = current_node->children[conversion(num[i])];
        i++;
    }

    if ((if_correct(num[i]) != END)) {
        return;
    }

    // Na koniec usuwamy całe poddrzewo poniżej ostatniego elementu prefiksu.
    phfwdDelete_help(current_node);
}

/**
 * @brief Funkcja oblicza rozmiar napisu i sprawdza jego poprawność.
 * @param[in] num - wskaźnik na napis.
 * @param[in, out] correct - indykator poprawności napisu.
 * @return Rozmiar napisu.
 */
static size_t count_size (char *num, bool *correct) {
    size_t size = 0;
    while (if_correct(num[size]) == CORRECT) {
        size++;
    }

    if (if_correct(num[size]) == END) {
        *correct = 1;
    }
    else {
        *correct = 0;
    }

    return size;
}

/**
 * @brief Funkcja usuwająca przekierowanie z drzewa nieodwróconego.
 * Funkcja usuwa tylko elementy z tablic PhoneNumbers, zostawia nienaruszoną
 * strukturę ogólną drzewa.
 * @param[in] pf - wskaźnik na drzewo.
 * @param[in] num - wskaźnik na usuwany numer.
 */
static void phfwdRemove_rev_help(PhoneReversed *pf, char const *num) {
    remove_cell(pf, (char*) num);

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (pf->children[i] != NULL) {
            phfwdRemove_rev_help(pf->children[i], num);
        }
    }
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    if ((pf != NULL) && (num != NULL) && (num[0] != '\0')
        && (error((char*)num) != 1)) {
        phfwdRemove_help(pf->new_tree, num);
        phfwdRemove_rev_help(pf->reversed_tree, num);
    }
}

/**
 * @brief Wyznacza długość pozostałej części słowa.
 * @param[in] num - wskaźnik na dany napis;
 * @param[in] i - indeks od którego zaczynamy wyliczanie długości części słowa.
 * @return Długość słowa.
 */
static size_t word_length(char *num, size_t i) {
    while (if_correct(num[i]) == CORRECT) {
        i++;
    }

    return i;
}

/**
 * @brief Funkcja sortuje bąbelkowo tablice.
 * @param[in, out] pf - wskaźnik na strukturę z tablicą.
 */
static void bubble_sort(PhoneNumbers *pf) {
    bool ok = 0;
    while (ok == 0) {
        ok = 1;
        for (size_t i = 1; i < pf->size; i++) {
            if ((pf->table_of_phone_numbers[i - 1] != NULL) &&
                (pf->table_of_phone_numbers[i] != NULL) &&
                (compare(pf->table_of_phone_numbers[i - 1],
                pf->table_of_phone_numbers[i]) == -1)) {
                char*tmp = pf->table_of_phone_numbers[i - 1];
                pf->table_of_phone_numbers[i - 1] = pf->table_of_phone_numbers[i];
                pf->table_of_phone_numbers[i] = tmp;
                ok = 0;
            }
        }
    }
}

/**
 * @brief To jest funkcja alokująca PhoneNumbers o zadanej wielkości.
 * @param[in] size - rozmiar tablicy.
 * @return Zaalokowana tablica
 */
static PhoneNumbers * wider_allocation(size_t size) {
    PhoneNumbers *answer = malloc(sizeof(PhoneNumbers));
    if (answer != NULL) {
        answer->size = size;
        answer->table_of_phone_numbers = malloc(size * sizeof(char*));
        if (answer->table_of_phone_numbers != NULL) {
            for (size_t i = 0; i < size; i++) {
                answer->table_of_phone_numbers[i] = malloc(sizeof(char));
            }
        }
    }

    return answer;
}

/**
 * @brief Funkcja zlicza liczbę wszystkich prefiksów w drzewie odwróconym.
 * @param[in] current - wskaźnik na odwrócone;
 * @param[in] num - słowo, będące swoistą "mapą drzewa";
 * @param[in] max_size - największa głębokość drzewa.
 * @return Szukana liczba.
 */
static size_t count_how_many_cells(PhoneReversed *current, char const *num,
                                   size_t *max_size) {
    size_t count_cells = 0;
    size_t i = 0;

    while ((current != NULL) && (i < *max_size)) {
        current = current->children[conversion(num[i])];

        if ((current != NULL) && (current->table_of_prefixes != NULL)) {
            count_cells += current->table_of_prefixes->size;
        }
        i++;
    }

    return count_cells;
}

/**
 * @brief Funkcja przepisuje napis na drugi.
 * @param[in] answer - PhoneNumbers zawierające tabicę;
 * @param[in] num - matryca kopiowania;
 * @param[in] count_cells - lokalizacja pierwszej tablicy;
 * @param[in] numsize - wielkość napisu;
 * @param[in] start - punkt, od którego zaczynamy przepisywanie.
 */
static void string_copy(PhoneNumbers *answer, char *num, size_t count_cells,
                        size_t numsize, size_t start) {
    for (size_t ii = start; ii < numsize + 1; ii++) {
        answer->table_of_phone_numbers[count_cells][ii] = num[ii];
    }
}


/**
 * @brief Funkcja zliczająca powtarzające się elementy.
 * @param[in] answer - wskaźnik na strukturę z tablicą.
 */
static size_t rubbish_count(PhoneNumbers* answer) {
    size_t rubbish = 0;

    for (size_t z = 0; z < answer->size - 1; z++) {
        size_t zz = 0;

        while ((if_correct(answer->table_of_phone_numbers[z][zz]) != END) && 
               (if_correct(answer->table_of_phone_numbers[z + 1][zz]) != END) &&
                (answer->table_of_phone_numbers[z + 1][zz] == 
                answer->table_of_phone_numbers[z][zz])) {
            zz++;
        }

        if (answer->table_of_phone_numbers[z + 1][zz] == 
        answer->table_of_phone_numbers[z][zz]) {
            rubbish++;
        }
    }

    return rubbish;
}

/**
 * @brief Funkcja tworzy nowe, wynikowe PhoneNumbers.
 * Funkcja tworzy nową strukturę, nie zawierającą powtórzeń, zakładająć, 
 * że otrzyma posortowaną tablicę.
 * @param[in] answer - pierwotna struktura z powtórzeniami;
 * @param[in] rubbish - liczba elementów do wyrzucenia.
 * @return Wynikowa struktura.
 */
static PhoneNumbers * without_repetition(PhoneNumbers *answer, size_t rubbish) {
    size_t i = 0;
    bool nothing = 0; // Zmienna jest śmietnikiem dla funkcji count_size.

    PhoneNumbers *new_answer = wider_allocation(answer->size - rubbish);
    if (new_answer == NULL) {
        return phnum_new_one();
    }

    /* Gdy występują obok siebie dwa takie same elementy, przepisujemy tylko
    jeden z nich. */
    for (size_t z = 0; z < answer->size - 1; z++) {
        size_t zz = 0;
        while ((if_correct(answer->table_of_phone_numbers[z][zz]) != END) && 
               (if_correct(answer->table_of_phone_numbers[z + 1][zz]) != END) &&
                (answer->table_of_phone_numbers[z + 1][zz] == 
                answer->table_of_phone_numbers[z][zz])) {
            zz++;
        }

        if (answer->table_of_phone_numbers[z + 1][zz]
            != answer->table_of_phone_numbers[z][zz]) {
            size_t stringsize = count_size(answer->table_of_phone_numbers[z],
                                           &nothing) + 1;
            new_answer->table_of_phone_numbers[i] = realloc
            (new_answer->table_of_phone_numbers[i], stringsize);
            if (new_answer->table_of_phone_numbers[i] == NULL) {
                return NULL;
            }

            string_copy(new_answer, answer->table_of_phone_numbers[z], i, 
                        stringsize - 1, 0);
            i++;
        }
    }

    /* Na koniec dopisujemy ostatni element, którego nie mieliśmy jak wpisać
    po porównaniu. */
    if (i != new_answer->size) {
        size_t stringsize = count_size(answer->table_of_phone_numbers
                                       [answer->size - 1], &nothing) + 1;
        new_answer->table_of_phone_numbers[i] = 
        realloc(new_answer->table_of_phone_numbers[i], stringsize);
        if (new_answer->table_of_phone_numbers[i] == NULL) {
            return NULL;
        }

        string_copy(new_answer, answer->table_of_phone_numbers[answer->size-1], 
                    i, stringsize - 1, 0);
    }

    phnumDelete(answer);
    return new_answer;
}

/**
 * @brief Funkcja faktycznie wykonująca operację phfwdReverse.
 * @param[in] pf - wskaźnik na strukturę;
 * @param[in] num - napis, którego szukamy;
 * @param[in] max_size - maksymalny rozmiar tablicy.
 * @return Wynikowe PhoneNumbers.
 */
static PhoneNumbers * relreverse(const PhoneForward *pf, char const *num,
                                size_t max_size) {
    PhoneReversed *current = pf->reversed_tree;
    size_t count_cells = count_how_many_cells(current, num, &max_size) + 1;
    // Dodajemy przestrzeń na ten sam numer.
    PhoneNumbers *answer = wider_allocation(count_cells);

    size_t i = 0;
    count_cells = 0;
    bool nothing = 0; // Zmienna jest śmietnikiem dla funkcji count_size.
    size_t numsize = count_size((char*)num, &nothing);
    answer->table_of_phone_numbers[count_cells] = 
    realloc(answer->table_of_phone_numbers[count_cells], numsize + 1);
    if (answer->table_of_phone_numbers[count_cells] == NULL) {
        return NULL;
    }

    string_copy(answer, (char*) num, count_cells, numsize, 0);
    count_cells++;
    current = pf->reversed_tree;

    /* Główna pętla funkcji. Przechodzimy po tablicach, sklejamy je ze sobą
    i dodajemy adekwatne końcówki (z num). */    
    while ((current != NULL) && (i < max_size)) {
            current = current->children[conversion(num[i])];
        if ((current != NULL) && (current->table_of_prefixes != NULL)) {
            for (size_t z = 0; z < current->table_of_prefixes->size; z++) {
                size_t string_size = count_size
                (current->table_of_prefixes->table_of_phone_numbers[z],
                &nothing);

                answer->table_of_phone_numbers[count_cells] =
                realloc (answer->table_of_phone_numbers[count_cells], 
                         string_size + numsize - i);
                if (answer->table_of_phone_numbers[count_cells] == NULL) {
                    return NULL;
                }

                string_copy(answer, 
                            current->table_of_prefixes->table_of_phone_numbers[z], 
                            count_cells, string_size, 0);

                size_t jj = i + 1;
                for (size_t ii = string_size; ii < (numsize - i + string_size); ii++) {
                    answer->table_of_phone_numbers[count_cells][ii] = num[jj];
                    jj++;
                }
                count_cells++;
            }
        }
        i++;
    }

    bubble_sort(answer);
    size_t rubbish = rubbish_count(answer);
    if (rubbish != 0) {
       return without_repetition(answer, rubbish);
    }

    return answer;
}

PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num) {
    // Przechodzimy drzewo pierwszy raz i zliczamy prefiksty.
    if (pf == NULL) {
        return NULL;
    }
    if ((num == NULL) || (error((char*) num) == 1) || (num[0] == '\0')) {
        return phnum_new_one();
    }

    PhoneReversed *current = pf->reversed_tree;
    bool correct = 0;

    size_t max_size = count_size((char*)num, &correct);

    if (correct && current != NULL) {
       return relreverse(pf, (char*) num, max_size);
    }
    else {
        return phnum_new_one();
    }
}


/**
 * @brief Okeśla PhoneNumbers zwracane przez phfwdGet w szczególnym przypadku.
 * Funkcja wyznacza wynik phfwdGet w przypadku, gdy napis nie ma przekierowania 
 * i tablica powinna zawierać wyjściowy numer.
 * @param[in, out] new - wskaźnik na strukturę;
 * @param[in] num - wskaźnik na wyjściowy napis;
 * @param[in] size - wielkość wyjściowego napisu.
 * @return Struktura zawierająca wyjściowy napis.
 */
static PhoneNumbers *same_number(PhoneNumbers *new, char *num, size_t size) {
    char *tmp = realloc(new->table_of_phone_numbers[0], (size_t) (size + 1));
    if (tmp == NULL) {
        return NULL;
    }
    new->table_of_phone_numbers[0] = tmp;

    for (size_t i = 0; i < (size + 1); i++) {
        new->table_of_phone_numbers[0][i] = num[i];
    }

    return new;
}

/**
 * @brief Okeśla PhoneNumbers zwracane przez phfwdGet w ogólnym przypadku.
 * @param[in, out] new - wskaźnik na zwracaną strukturę;
 * @param[in] last_prefix - wskaźnik na węzeł z ostatnim przekierowaniem;
 * @param[in] previous_size - rozmiar pierwotnego numeru;
 * @param[in] prefix_size - rozmiar napisu reprezentującego nowy prefiks;
 * @param[in] to_subtract - rozmiar części numeru, która jest podmieniana
 *                          na nowy prefiks;
 * @param[in] num - wskaźnik na wejściowy numer.
 * @return Struktura zawierająca nowy, przekierowany numer.
 */
static PhoneNumbers * new_number(PhoneNumbers *new, PhoneFWD *last_prefix,
                                size_t previous_size, size_t prefix_size,
                                size_t to_subtract, char *num){
    size_t size = previous_size + prefix_size - to_subtract + 1;
    char *tmp = realloc(new->table_of_phone_numbers[0], (size_t) size);
    if (tmp == NULL) {
        return NULL;
    }
    new->table_of_phone_numbers[0] = tmp;

    size_t i = 0;
    while (i < prefix_size) {
        new->table_of_phone_numbers[0][i] = last_prefix->prefix[i];
        i++;
    }

    while (i < size - 1) {
        new->table_of_phone_numbers[0][i] = num[to_subtract];
        to_subtract++;
        i++;
    }

    new->table_of_phone_numbers[0][i] = '\0';

    return new;
}

PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num) {
    if (pf == NULL) {
        return NULL;
    }

    PhoneNumbers *new = phnum_new_one();
    if ((new == NULL) || (new->table_of_phone_numbers == NULL)) {
        return NULL;
    }

    if ((num == NULL) || (num[0] == '\0') || (error((char*)num) == 1)) {
        return new;
    }

    size_t j = 0;
    size_t i = 0;
    PhoneFWD *current_node = (PhoneFWD*)pf->new_tree;
    PhoneFWD *last_prefix = NULL;
    size_t z = 0;
    while ((if_correct(num[i]) == CORRECT) && 
           (current_node->children[conversion(num[i])] != NULL)) {
        if (current_node->prefix != NULL) {
            last_prefix = current_node;
            z = i;
        }
        current_node = current_node->children[conversion(num[i])];
        i++;
    }

    if (current_node->prefix != NULL) {
        last_prefix = current_node;
        z = i;
    }

    // Jeśli nie ma prefiksu, numer zwraca sam siebie.
    if (last_prefix == NULL) {
       return same_number(new, (char*)num, word_length((char*)num, i));
    }

    return new_number(new, last_prefix, word_length((char*)num, i),
                      word_length(last_prefix->prefix, j), z, (char*)num);
}



char const * phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (pnum != NULL) {
        if ((pnum->table_of_phone_numbers != NULL) && (idx < (pnum->size))) {
            return pnum->table_of_phone_numbers[idx];
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}

/**
 * @brief Funkcja sprawdza, czy elementy x w tablicy są postaci phfwdGet(x) = num.
 * Funkcja przystosowuje tablicę wygenerowaną przez phfwdReverse do warunku
 * phfwdGet(x) = num. Funkcja testuje to sprawdzając wywołanie funkcji phfwdGet
 * na każdym elemencie tablicy i jeśli element spełnia warunek phfwdGet(x) =
 * num, element x zostaje przeniesiony do tablicy wynikowej.
 * @param pf - wskaźnik na strukturę drzew;
 * @param answer - PhoneNumbers wygenerowane przez phfwdReverse;
 * @param num - dany przekierowany numer;
 * @return PhoneNumbers zawierające przeciwobraz funkcji phfwdGet.
 */
static PhoneNumbers * check_by_get (PhoneForward const *pf,
                                    PhoneNumbers *answer, char const *num) {
    size_t rubbish = 0;
    bool nothing = 0;

    for (size_t i = 0; i < answer->size; i++) {
        PhoneNumbers *pnum = phfwdGet(pf, answer->table_of_phone_numbers[i]);
        if (!check_if_same((char*)phnumGet(pnum, 0), (char*)num)) {
            rubbish++;
        }
        phnumDelete(pnum);
    }

    PhoneNumbers *new_answer = wider_allocation(answer->size - rubbish);
    if (new_answer == NULL) {
        return phnum_new_one();
    }

    size_t j = 0;
    for (size_t i = 0; i < answer->size; i++) {
        PhoneNumbers *pnum = phfwdGet(pf, answer->table_of_phone_numbers[i]);
        if (check_if_same((char*)phnumGet(pnum, 0), (char*)num)) {
            size_t size = count_size(answer->table_of_phone_numbers[i], &nothing);
            new_answer->table_of_phone_numbers[j] =
            realloc(new_answer->table_of_phone_numbers[j],
                    (size + 1) * sizeof(char));
            string_copy(new_answer, answer->table_of_phone_numbers[i], j, size, 0);
            if (new_answer->table_of_phone_numbers[j] == NULL) {
                return NULL;
            }
            j++;
        }
        phnumDelete(pnum);
    }

    phnumDelete(answer);
    return new_answer;
}

PhoneNumbers * phfwdGetReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL) {
        return NULL;
    }
    if ((num == NULL) || (num[0] == '\0') || error((char*)num)) {
        return phnum_new_one();
    }
    PhoneNumbers *answer = phfwdReverse(pf, num);
    return check_by_get(pf, answer, num);
}