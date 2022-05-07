#ifdef NDEBUG
#undef NDEBUG
#endif

#include "phone_forward.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define MAX_LEN 23

#define GREEN   "\033[0;32m"
#define RESET_COLOR "\033[0m"

void phfwdAddTests();

void phfwdGetTests();

int testId = 1;

void printTestPassed() {
    printf("Test %d: ", testId++);
    printf("%sPASSED%s\n", GREEN, RESET_COLOR);
}
void printGreeting() {
    printf("Testy rozpoczęte\n\n");
}

int main() {

    printGreeting();

    PhoneForward *pf;
    PhoneNumbers *pnum;


    /* phwdNew */
    printf("Testowanie phfwdNew\n");
    printf("%d", testId);
    pf = phfwdNew();
    printTestPassed();
    phfwdDelete(pf);


    /* phfwdDelete */
    printf("Testowanie phfwdDelete: \n");

    /* zwykłe usunięcie struktury */
    pf = phfwdNew();
    phfwdDelete(pf);
    printTestPassed();

    pf = NULL;
    phfwdDelete(pf);
    printTestPassed();

    phfwdAddTests();

    phfwdGetTests();

    /* phnumDelete */
    printf("Testowanie phnumDelete\n");
    phnumDelete(NULL);
    printTestPassed();


    /* phnumGet */
            printf("%d", testId);

    printf("Testowanie phnumGet\n");
            printf("%d", testId);

    assert(phnumGet(NULL, 0) == NULL);
        printf("%d", testId);

    printTestPassed();



    /* BIG COMPLEX EXAMPLE*/
    printf("Testowanie złożonego przykładu\n");
    pf = phfwdNew();

    assert(true == phfwdAdd(pf, "11", "113"));
    printTestPassed();

    pnum = phfwdGet(pf, "114");
    assert(strcmp(phnumGet(pnum, 0), "1134") == 0);
    phnumDelete(pnum);

    assert(phfwdAdd(pf, "119", "1"));
    printTestPassed();

    assert(phfwdAdd(pf, "113", "19"));
    printTestPassed();

    assert(phfwdAdd(pf, "1197", "191"));
    printTestPassed();

    pnum = phfwdGet(pf, "1198");
    assert(strcmp(phnumGet(pnum, 0), "18") == 0);
    phnumDelete(pnum);
    printTestPassed();

    pnum = phfwdGet(pf, "11");
    assert(strcmp(phnumGet(pnum, 0), "113") == 0);
    phnumDelete(pnum);
    printTestPassed();

    phfwdRemove(pf, "119");
    pnum = phfwdGet(pf, "1197");
    assert(strcmp(phnumGet(pnum, 0), "11397") == 0);
    phnumDelete(pnum);
    printTestPassed();

    phfwdDelete(pf);
}

void phfwdGetTests() {
    printf("Testowanie phfwdGet \n");

    char num[MAX_LEN + 1];
    PhoneForward *pf;
    PhoneNumbers *pnum;

    /* num nie jest numerem, powinno zwrócić pusty ciąg */
    pf = phfwdNew();
    pnum = phfwdGet(pf, "");
    assert(pnum != NULL && phnumGet(pnum, 0) == NULL);
    phnumDelete(pnum);
    phfwdDelete(pf);
    printTestPassed();

    /* pf == NULL */
    strcpy(num, "312");
    assert(NULL == phfwdGet(NULL, num));
    printTestPassed();

    /* pf == NULL, num == NULL i czy na koniec pusty ciąg */
    pnum = phfwdGet(NULL, NULL);
    assert(NULL == pnum);
    printTestPassed();

    /* pf == coś, num == NULL i czy na koniec pusty ciąg */
    pf = phfwdNew();
    pnum = phfwdGet(pf, NULL);
    assert(NULL != pnum);
    printTestPassed();
    assert(phnumGet(pnum, 0) == NULL);
    phfwdDelete(pf);
    phnumDelete(pnum);
    printTestPassed();

    /* proste przekierowanie*/
    pf = phfwdNew();
    phfwdAdd(pf, "123", "1");
    pnum = phfwdGet(pf, "123123");
    assert(strcmp("1123", phnumGet(pnum, 0)) == 0);
    phnumDelete(pnum);
    printTestPassed();
    /* bez przekierowania */
    pnum = phfwdGet(pf, "12089");
    assert(strcmp("12089", phnumGet(pnum, 0)) == 0);
    printTestPassed();
    /* czy tylko jeden numer był w pnum*/
    assert(phnumGet(pnum, 1) == NULL);
    phfwdDelete(pf);
    phnumDelete(pnum);
    printTestPassed();

}

void phfwdAddTests() {
    printf("Testowanie phfwdAdd \n");

    char num1[MAX_LEN + 1], num2[MAX_LEN + 1];
    PhoneForward *pf;

    /* takie same numery niepuste*/
    pf = phfwdNew();
    strcpy(num1, "23543");
    strcpy(num2, "23543");
    assert(false == phfwdAdd(pf, num1, num2));
        printf("%d", testId);

    printTestPassed();

    phfwdDelete(pf);

    /* takie same numery puste */
    pf = phfwdNew();
    strcpy(num1, "");
    strcpy(num2, "");
    assert(false == phfwdAdd(pf, num1, num2));
        printf("%d", testId);

    printTestPassed();
    phfwdDelete(pf);

    /* oba puste */
    pf = phfwdNew();
    strcpy(num1, "");
    strcpy(num2, "");
    assert(false == phfwdAdd(pf, num1, num2));
    printTestPassed();
    phfwdDelete(pf);

    /* pusty niepusty -> powinno byc false bo nie numer*/
    pf = phfwdNew();
    strcpy(num1, "");
    strcpy(num2, "01239876543");
    assert(false == phfwdAdd(pf, num1, num2));
    printTestPassed();
    phfwdDelete(pf);

    /* numer nie numer */
    pf = phfwdNew();
    strcpy(num1, "123");
    strcpy(num2, "92309-1");
    assert(false == phfwdAdd(pf, num1, num2));
        printf("%d", testId);

    printTestPassed();
    phfwdDelete(pf);

    /* pf == NULL */
    pf = NULL;
    strcpy(num1, "123");
    strcpy(num2, "3214");
    assert(false == phfwdAdd(pf, num1, num2));
    printTestPassed();
    phfwdDelete(pf);

    /* numer w nie numer */
    pf = phfwdNew();
    strcpy(num1, "123");
    strcpy(num2, "");
    assert(false == phfwdAdd(pf, num1, num2));
    printTestPassed();
    phfwdDelete(pf);

    /* prawidłowe */
    pf = phfwdNew();
    strcpy(num1, "123");
    strcpy(num2, "1234");
    assert(true == phfwdAdd(pf, num1, num2));
    printTestPassed();
    phfwdDelete(pf);

    /* prawidłowe */
    pf = phfwdNew();
    strcpy(num1, "9");
    strcpy(num2, "0");
    assert(true == phfwdAdd(pf, num1, num2));
    printTestPassed();
    phfwdDelete(pf);

    /* num1 = NULL */
    pf = phfwdNew();
    strcpy(num2, "0");
    assert(false == phfwdAdd(pf, NULL, num2));
    printTestPassed();
    phfwdDelete(pf);

    /* num2 = NULL */
    pf = phfwdNew();
    strcpy(num1, "0");
    assert(false == phfwdAdd(pf, num1, NULL));
    printTestPassed();
    phfwdDelete(pf);
}
/*
#ifdef NDEBUG
#undef NDEBUG
#endif

#include "phone_forward.h"
#include <assert.h>
#include <string.h>

#include <stdio.h>


#define MAX_LEN 23

int main() {
  char num1[MAX_LEN + 1], num2[MAX_LEN + 1];
  PhoneForward *pf;
  PhoneNumbers *pnum;

  pf = phfwdNew();
  strcpy(num1, "123");
  strcpy(num2, "9");
  assert(phfwdAdd(pf, num1, num2) == true);

 // assert(pf->children[1] != NULL); //my

  memset(num1, 0, sizeof num1);
  memset(num2, 0, sizeof num2);

  pnum = phfwdGet(pf, "1234");
  //for (int i = 0; i < 5; i++) {
  //  printf("x %d %c x", i, pnum->table_of_phone_numbers[0][i]);
 // }
  assert(strcmp(phnumGet(pnum, 0), "94") == 0);
  assert(phnumGet(pnum, 1) == NULL);
  
  phnumDelete(pnum);

  pnum = phfwdGet(pf, "12");
  assert(strcmp(phnumGet(pnum, 0), "12") == 0);
  phnumDelete(pnum);
  //printf("ok?");

  strcpy(num1, "123456");
  strcpy(num2, "777777");
  
  assert(phfwdAdd(pf, num1, num2) == true);

  pnum = phfwdGet(pf, "12345");
 // printf("ksfdkhjdkh %s\n",phnumGet(pnum, 0));
  assert(strcmp(phnumGet(pnum, 0), "945") == 0);
  phnumDelete(pnum);
  

  pnum = phfwdGet(pf, "123456");
  assert(strcmp(phnumGet(pnum, 0), "777777") == 0);
  phnumDelete(pnum);

  pnum = phfwdGet(pf, "997");
  assert(strcmp(phnumGet(pnum, 0), "997") == 0);
  phnumDelete(pnum);

  assert(phfwdAdd(pf, "431", "432") == true);
  assert(phfwdAdd(pf, "432", "433") == true);
  pnum = phfwdGet(pf, "431");
  assert(strcmp(phnumGet(pnum, 0), "432") == 0);
  phnumDelete(pnum);
  pnum = phfwdGet(pf, "432");
  assert(strcmp(phnumGet(pnum, 0), "433") == 0);
  phnumDelete(pnum);

 // pnum = phfwdReverse(pf, "432");
 // assert(strcmp(phnumGet(pnum, 0), "431") == 0);
 // assert(strcmp(phnumGet(pnum, 1), "432") == 0);
 // assert(phnumGet(pnum, 2) == NULL);
 // phnumDelete(pnum);

 // pnum = phfwdReverse(pf, "433");
 // assert(strcmp(phnumGet(pnum, 0), "432") == 0);
 // assert(strcmp(phnumGet(pnum, 1), "433") == 0);
 // assert(phnumGet(pnum, 2) == NULL);
 // phnumDelete(pnum);

  //pnum = phfwdReverse(pf, "987654321");
 // assert(strcmp(phnumGet(pnum, 0), "12387654321") == 0);
 // assert(strcmp(phnumGet(pnum, 1), "987654321") == 0);
  //assert(phnumGet(pnum, 2) == NULL);
  //phnumDelete(pnum);

  phfwdRemove(pf, "12");

  pnum = phfwdGet(pf, "123456");
  assert(strcmp(phnumGet(pnum, 0), "123456") == 0);
  phnumDelete(pnum);

 // pnum = phfwdReverse(pf, "987654321");
  //assert(strcmp(phnumGet(pnum, 0), "987654321") == 0);
  //assert(phnumGet(pnum, 1) == NULL);
  //phnumDelete(pnum);

  assert(phfwdAdd(pf, "567", "0") == true);
  assert(phfwdAdd(pf, "5678", "08") == true);

  //pnum = phfwdReverse(pf, "08");
 // assert(strcmp(phnumGet(pnum, 0), "08") == 0);
  //assert(strcmp(phnumGet(pnum, 1), "5678") == 0);
  //assert(phnumGet(pnum, 2) == NULL);
  //phnumDelete(pnum);

  assert(phfwdAdd(pf, "A", "1") == false);
  assert(phfwdAdd(pf, "1", "A") == false);

  phfwdRemove(pf, "");
  
  phfwdRemove(pf, NULL);

  pnum = phfwdGet(pf, "A");
  assert(phnumGet(pnum, 0) == NULL);
  phnumDelete(pnum);

  //pnum = phfwdReverse(pf, "A");
  //assert(phnumGet(pnum, 0) == NULL);
  //phnumDelete(pnum);

  phfwdAdd(pf, "12", "123");
  pnum = phfwdGet(pf, "123");
  assert(strcmp(phnumGet(pnum, 0), "1233") == 0);
  phnumDelete(pnum);

  phfwdAdd(pf, "2", "4");
  phfwdAdd(pf, "23", "4");
  //pnum = phfwdReverse(pf, "434");
  //assert(strcmp(phnumGet(pnum, 0), "2334") == 0);
 // assert(strcmp(phnumGet(pnum, 1), "234") == 0);
 // assert(strcmp(phnumGet(pnum, 2), "434") == 0);
 // assert(phnumGet(pnum, 3) == NULL);
  //phnumDelete(pnum);

  phfwdDelete(pf);
  pnum = NULL;
  phnumDelete(pnum);
  pf = NULL;
  phfwdDelete(pf);

  pf = phfwdNew();
  phfwdAdd(pf, "1234", "76");
  pnum = phfwdGet(pf, "1234581");
  assert(strcmp(phnumGet(pnum, 0), "76581") == 0);
  phnumDelete(pnum);
  pnum = phfwdGet(pf, "7581");
  assert(strcmp(phnumGet(pnum, 0), "7581") == 0);
  phnumDelete(pnum);
 // pnum = phfwdReverse(pf, "7581");
 // assert(strcmp(phnumGet(pnum, 0), "7581") == 0);
 // assert(phnumGet(pnum, 1) == NULL);
 // phnumDelete(pnum);
  phfwdDelete(pf);
 
}
*/