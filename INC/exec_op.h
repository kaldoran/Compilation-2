/* ---------------------------------------------------------------------- */
/* Filename: exec_op.h                                                    */
/* Author: ABHAMON Ronan, BIGARD Florian, REYNAUD Nicolas                 */
/* Date: 2013-11-20 - 14:26:32                                            */
/*                                                                        */
/* ---------------------------------------------------------------------- */

#ifndef _EXEC_OP_
#define _EXEC_OP_

#include "exec_cast.h"

/** Obtenir le max entre 2 nombres. */
#define MAX(I, J) ((I) > (J) ? (I) : (J))

/* ---------------------------------------------------------------------- */
/* OPERATIONS                                                             */
/* ---------------------------------------------------------------------- */

/** Attribue le type à utiliser aux opérandes. */
/* Comment ça marche ? Les constantes SYMBOL_BASIC sont ordonnées
   de la sorte que string > float > int > char > bool, du coup il
   suffit de comparer les valeurs des constantes entre elles
   et de choisir la plus grande.

   Exemple : 45 + "23.95", 45 est de type int, "23.95" est de type string,
   le type string sera donc choisi, puisque la constante
   SYMBOL_BASIC_STRING > SYMBOL_BASIC_INT.

   Le résultat sera donc : "4523.95", soit la concaténation de deux chaines. */
#define OP_SET_TYPE(RES_A, RES_B)                                               \
  do {                                                                          \
    if(RES_A.type == SYMBOL_BASIC_STRING_UP && RES_B.type != RES_A.type) {      \
      CAST(RES_B, SYMBOL_BASIC_STRING);                                         \
    }                                                                           \
    else if(RES_B.type == SYMBOL_BASIC_STRING_UP && RES_A.type != RES_B.type) { \
      CAST(RES_A, SYMBOL_BASIC_STRING);                                         \
    }                                                                           \
    else                                                                        \
    {                                                                           \
      CAST(RES_A, MAX(RES_A.type, RES_B.type));                                 \
      CAST(RES_B, MAX(RES_A.type, RES_B.type));                                 \
    }                                                                           \
  } while(0)

/** Additionne 2 valeurs. */
#define OP_ADD(RES, RES_A, RES_B)                                                  \
  switch(RES.type = RES_A.type)                                                    \
  {                                                                                \
    case SYMBOL_BASIC_INT:                                                         \
      RES.value.i = RES_A.value.i + RES_B.value.i;                                 \
      break;                                                                       \
    case SYMBOL_BASIC_FLOAT:                                                       \
      RES.value.f = RES_A.value.f + RES_B.value.f;                                 \
      break;                                                                       \
    case SYMBOL_BASIC_BOOL:                                                        \
      RES.value.c = !!(RES_A.value.c + RES_B.value.c);                             \
      break;                                                                       \
    case SYMBOL_BASIC_CHAR:                                                        \
      RES.value.c = RES_A.value.c + RES_B.value.c;                                 \
      break;                                                                       \
    case SYMBOL_BASIC_STRING:                                                      \
    case SYMBOL_BASIC_STRING_UP:                                                   \
      if((RES.value.s = malloc((strlen(RES_A.value.s) + strlen(RES_A.value.s) + 1) \
                                * sizeof(char))) == NULL)                          \
        fatal_error("Concat error !");                                             \
                                                                                   \
      strcpy(RES.value.s, RES_A.value.s);                                          \
      strcpy(RES.value.s + strlen(RES_A.value.s), RES_B.value.s);                  \
      RES.type = SYMBOL_BASIC_STRING_UP;                                           \
                                                                                   \
      if(RES_A.type == SYMBOL_BASIC_STRING_UP) free(RES_A.value.s);                \
      if(RES_B.type == SYMBOL_BASIC_STRING_UP) free(RES_B.value.s);                \
                                                                                   \
      break;                                                                       \
  }

/** Soustrait 2 valeurs. */
#define OP_SUB(RES, RES_A, RES_B)                    \
  switch(RES.type = RES_A.type)                      \
  {                                                  \
    case SYMBOL_BASIC_INT:                           \
      RES.value.i = RES_A.value.i - RES_B.value.i;   \
      break;                                         \
    case SYMBOL_BASIC_FLOAT:                         \
      RES.value.f = RES_A.value.f - RES_B.value.f;   \
      break;                                         \
    case SYMBOL_BASIC_BOOL:                          \
      RES.value.c = !RES_A.value.c && RES_B.value.c; \
      break;                                         \
    case SYMBOL_BASIC_CHAR:                          \
      RES.value.c = RES_A.value.c - RES_B.value.c;   \
      break;                                         \
    case SYMBOL_BASIC_STRING:                        \
    case SYMBOL_BASIC_STRING_UP:                     \
      fatal_error("Exception in sub with string");   \
      break;                                         \
  }

/** Multiplie 2 valeurs. */
#define OP_MUL(RES, RES_A, RES_B)                   \
  switch(RES.type = RES_A.type)                     \
  {                                                 \
    case SYMBOL_BASIC_INT:                          \
      RES.value.i = RES_A.value.i * RES_B.value.i;  \
      break;                                        \
    case SYMBOL_BASIC_FLOAT:                        \
      RES.value.f = RES_A.value.f * RES_B.value.f;  \
      break;                                        \
    case SYMBOL_BASIC_BOOL:                         \
      RES.value.c = RES_A.value.c && RES_B.value.c; \
      break;                                        \
    case SYMBOL_BASIC_CHAR:                         \
      RES.value.c = RES_A.value.c * RES_B.value.c;  \
      break;                                        \
    case SYMBOL_BASIC_STRING:                       \
    case SYMBOL_BASIC_STRING_UP:                    \
      fatal_error("Exception in mul with string");  \
      break;                                        \
 }

/** Divise 2 valeurs. */
#define OP_DIV(RES, RES_A, RES_B)                                                     \
  switch(RES.type = RES_A.type)                                                       \
  {                                                                                   \
    case SYMBOL_BASIC_INT:                                                            \
      if(RES_B.value.i == 0)                                                          \
      {                                                                               \
        fprintf(stderr, "Division by 0 ! (%d / %d)\n", RES_A.value.i, RES_B.value.i); \
        longjmp(jmp, 1);                                                              \
      }                                                                               \
      RES.value.i = RES_A.value.i / RES_B.value.i;                                    \
      break;                                                                          \
    case SYMBOL_BASIC_FLOAT:                                                          \
      RES.value.f = RES_A.value.f / RES_B.value.f;                                    \
      break;                                                                          \
    case SYMBOL_BASIC_BOOL:                                                           \
      RES.value.c = 0;                                                                \
      break;                                                                          \
    case SYMBOL_BASIC_CHAR:                                                           \
      if(RES_B.value.c == 0)                                                          \
      {                                                                               \
        fprintf(stderr, "Division by 0 ! (%d / %d)\n", RES_A.value.c, RES_B.value.c); \
        longjmp(jmp, 1);                                                              \
      }                                                                               \
      RES.value.c = RES_A.value.c / RES_B.value.c;                                    \
      break;                                                                          \
    case SYMBOL_BASIC_STRING:                                                         \
    case SYMBOL_BASIC_STRING_UP:                                                      \
      fatal_error("Exception in div with string");                                    \
      break;                                                                          \
 }

/** Modulo de 2 valeurs. */
#define OP_MOD(RES, RES_A, RES_B)                            \
  switch(RES.type = RES_A.type)                              \
  {                                                          \
    case SYMBOL_BASIC_INT:                                   \
      RES.value.i = RES_A.value.i % RES_B.value.i;           \
      break;                                                 \
    case SYMBOL_BASIC_FLOAT:                                 \
      RES.value.f = (int)RES_A.value.f % (int)RES_B.value.f; \
      break;                                                 \
    case SYMBOL_BASIC_BOOL:                                  \
      RES.value.c = RES_A.value.c && !RES_B.value.c;         \
      break;                                                 \
    case SYMBOL_BASIC_CHAR:                                  \
      RES.value.c = RES_A.value.c + RES_B.value.c;           \
      break;                                                 \
    case SYMBOL_BASIC_STRING:                                \
    case SYMBOL_BASIC_STRING_UP:                             \
      fatal_error("Exception in mod with string");           \
      break;                                                 \
 }

#endif /* _EXEC_OP_ INCLUDED */
