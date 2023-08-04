#ifndef _Tabela_H_
#define _Tabela_H_

#include "Registros.h" /* Interface do m�dulo Registros */

/****************** Defini��es de Tipos *******************/

typedef char tCEP[TAM_CEP + 1]; /* Tipo de chave */

typedef struct {
           tCEP chave; /* CEP */
           int  valor; /* �ndice do CEP no arquivo */
        } tCEP_Ind;

   /* Tipo de n� e tipo de ponteiro para n� */
   /* de uma lista simplesmente encadeada   */
typedef struct rotNoLSE {
           tCEP_Ind         conteudo;
           struct rotNoLSE *proximo;
        } tNoListaSE, *tListaSE;

/************************* Alus�es ************************/

extern void IniciaListaSE(tListaSE *lista);
extern int ComprimentoListaSE(tListaSE lista);
extern void InsereLSE( tListaSE *lista, const tCEP_Ind *conteudo );
extern int RemoveLSE(tListaSE *lista, tCEP chave);
extern int EstaVaziaLSE(tListaSE lista);
extern tCEP_Ind *BuscaComTransposicaoLSE( tListaSE *lista, tCEP chave );
extern tCEP_Ind *BuscaComMovimentoLSE( tListaSE *lista, tCEP chave );
extern void DestroiListaSE(tListaSE *lista);
extern tCEP_Ind *ProximoListaSE(tListaSE lista);

/******************* Falta Implementar *********************/

extern tCEP_Ind *BuscaPisoLSE(tListaSE lista, tCEP chave);
extern tListaSE BuscaIntervaloLSE(tListaSE lista, tCEP chave1, tCEP chave2);
extern tCEP_Ind *BuscaEInsereLSE(tListaSE *lista, const tCEP_Ind *elemento);
extern tListaSE BuscaDuplicadasLSE(tListaSE lista);

#endif
