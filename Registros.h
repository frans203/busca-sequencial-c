#ifndef _Registros_H_
#define _Registros_H_

/***************** Constantes Simb�licas ******************/

#define MAX_NOME  40 /* N�mero m�ximo de caracteres num nome */
#define TAM_UF     2 /* N�mero de d�gitos num UF */
#define TAM_CEP    8 /* N�mero de d�gitos num CEP */
#define MAX_COMP  60 /* N�mero m�ximo de caracteres */
                     /* num complemento             */
#define MAX_TIPO_LOG 10 /* N�mero m�ximo de caracteres */
                        /* num tipo de logradouro      */
#define TAM_DNE   16 /* N�mero de d�gitos numa chave DNE */

   /* Tamanho do acr�scimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

/****************** Defini��es de Tipos *******************/

typedef struct {
           int  numero;
           char UF[TAM_UF];
           int  localidadeNumero;
           char nomeAbr[MAX_NOME];
           char nome[MAX_NOME];
           int  bairroInicio;
           int  bairroFim;
           char CEP[TAM_CEP];
           char complemento[MAX_COMP];
           char tipoLogradouro[MAX_TIPO_LOG];
           char statusTipoLogradouro;
           char nomeSemAcento[MAX_NOME];
           char chaveDNE[TAM_DNE];
         } tRegistroCEP;

/******************* Alus�es de Fun��es *******************/

extern void ExibeRegistro(const tRegistroCEP *registro);
extern tRegistroCEP *LeRegistro(tRegistroCEP *regCEP);
extern tRegistroCEP *SubstituiRegistro( tRegistroCEP *novo,
                                        const tRegistroCEP *antigo );
extern int TamanhoDeArquivo(FILE *stream);
extern int NumeroDeRegistros( FILE *stream, int tamRegistro );

#endif
