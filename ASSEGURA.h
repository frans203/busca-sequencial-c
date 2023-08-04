#ifndef _ASSEGURA_H_
#define _ASSEGURA_H_

#include <stdio.h>  /* fprintf() e stderr */
#include <stdlib.h> /* exit()             */

   /* Macro utilizada em tratamento */
   /* de condi��es de exce��o       */
#define ASSEGURA(condicao, msg) if (!(condicao)) {\
                                   fprintf(stderr, "\n%s\n", msg);\
                                   exit(1); \
                                }

#endif
