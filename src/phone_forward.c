/** @file
* Implementacja interfejsu phone_forward.h.
*
* @author Maria Wysogląd
* @date 2022
*/
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "phone_forward.h"

#define CORRECT 0 ///< Arbitralnie wybrana stała przekazująca informację o poprawności.
#define ERROR 1 ///< Arbitralnie wybrana stała przekazująca informację o niepoprawności.
#define END 2 ///< Arbitralnie wybrana stała przekazująca informację o końcu napisu.
#define ALPHABET_SIZE 10 ///< Cyfr od 0 do 9 jest 10.

/**
 * @brief To jest struktura przechowująca przekierowania numerów telefonów.
 * Przechowuję przekierowania w formie drzewa prefiksowego,
 * gdzie jeśli kolejną cyfrą w prefiksie pierwotnego drzewa prefiksowego jest
 * i, tablica children[i] trzyma przekierowanie kolejnej cyfry.
 */
struct PhoneForward {
    struct PhoneForward *children[ALPHABET_SIZE]; ///< Dalsze litery pierwotnego prefiksu.
    char *prefix; ///< Wskaźnik na nowy prefiks.
    struct PhoneForward *father; ///< Wskaźnik na poprzedni węzeł drzewa przekierowań.
};

/**
 * @brief To jest struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers {
    char **table_of_phone_numbers; ///< Wskaźnik na tablicę numerów telefonów.
    size_t size; ///< Rozmiar tablicy.
};

PhoneForward * phfwdNew(void) {
    PhoneForward *new_struct = malloc(sizeof(PhoneForward));
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

void phfwdDelete(PhoneForward *pf) {
    // Funkcja iteracyjnie usuwa dane poddrzewo, o korzeniu w pf.
    if (pf == NULL) {
        return;
    }

    PhoneForward *current = pf;
    PhoneForward *parent = NULL;
    PhoneForward *root = pf->father;

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

/**
 * @brief Sprawdza znaczenie znaku.
 * Funkcja sprawdza, czy dany znak jest cyfrą lub znakiem końca napisu,
 * czy też zawiera błędny inny znak.
 * @param[in] character - konkretny znak z napisu.
 * @return Daną stałą, która określa, czy dany znak kończy napis, jest cyfrą
 *         lub jest niepoprawny.
 */
static int if_correct(char character) {
    if (character == '\0') {
        return END;
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
 * @brief Sprawdza czy dane 2 napisy są takie same.
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
 * @brief Zmienia znak cyfry na odpowiadającą mu liczbę.
 * @param[in] n - dany znak.
 * @return Całkowitą liczbę odpowiadającą nadej cyfrze.
 */
static int conversion(char n) {
    return n - '0';
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
 * @brief Funkcja spełnia zadanie phfwdAdd, operując na niestałych num1 i num2.
 * @param [in, out] pf - wskaźnik na strukturę przechowującą przekierowania
 *                       numerów;
 * @param[in] num1 - wskaźnik na napis reprezentujący prefiks numerów
 *                   przekierowywanych;
 * @param[in] num2 - wskaźnik na napis reprezentujący prefiks numerów,
 *                   na które jest wykonywane przekierowanie.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się alokować pamięci.
 */
static bool phfwdAdd_help(PhoneForward *pf, char *num1, char *num2) {
    /* Na początku sprawdzam przypadki szczególne, oznaczające
    brak dalszej pracy na wskaźniku pf. */
    if (special_cases(num1, num2) == 0) {
        return 0;
    }

    PhoneForward *current_node = pf;
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
            PhoneForward *tmp = phfwdNew();
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

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    /* W funkcji phfwdAdd_help wykonujemy opis phfwdAdd, ale musimy zmienić
    dostępność napisów num1 i num2 (usuwanie stałości napisów). 
    Wobec tego phfwdAdd jest swego rodzaju funkcją pośredniczącą. */
    bool odp = phfwdAdd_help(pf, (char*)num1, (char*) num2);
    return odp;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    if ((num == NULL) || (pf == NULL) || (num[0] == '\0') ||
       (error((char*)num) == 1)) {
        return;
    }

    PhoneForward *current_node = pf;
    int i = 0;
    /* W <last_node> trzymamy wskaźnik na ostatni węzeł, którego nie
    chcemy usuwać, gdyż ma innego niż należący do prefiksu, lub ma 
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
    phfwdDelete(current_node);
}
   
/**
 * @brief Funkcja tworzy jednoelementowy węzeł PhoneNumbers.
 * @return Wskaźnik na zaalokowaną strukturę.
 */
static PhoneNumbers *phnum_new_one(void) {
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
 * @brief Okeśla PhoneNumbers zwracane przez phfwdGet w szczególnym przypadku.
 * Funkcja wyznacza wynik phfwdGet w przypadku, gdy napis nie ma przekierowania 
 * i tablica powinna zawierać wyjściowy numer.
 * @param[in, out] new - wskaźnik na strukturę;
 * @param[in] num - wskaźnik na wyjściowy napis;
 * @param[in] size - wielkość wyjściowego napisu.
 * @return Struktura zawierająca wyjściowy napis.
 */
static PhoneNumbers *same_number(PhoneNumbers *new, char *num, size_t size) {
    char *tmp2 = realloc(new->table_of_phone_numbers[0], (size_t) (size + 1));
    if (tmp2 == NULL) {
        return NULL;
    }
    new->table_of_phone_numbers[0] = tmp2;

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
static PhoneNumbers *new_number(PhoneNumbers *new, PhoneForward *last_prefix,
                                size_t previous_size, size_t prefix_size,
                                size_t to_subtract, char *num){
    size_t size = previous_size + prefix_size - to_subtract + 1;
    char *tmp3 = realloc(new->table_of_phone_numbers[0], (size_t) size);
    if (tmp3 == NULL) {
        return NULL;
    }
    new->table_of_phone_numbers[0] = tmp3;

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
    PhoneForward *current_node = (PhoneForward*)pf;
    PhoneForward *last_prefix = NULL;
    size_t z = 0;
    /* <last_prefix> trzyma ostatni węzeł mający prefiks, na można
    należy przekierować. */
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