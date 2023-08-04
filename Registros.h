#ifndef _Registros_H_
#define _Registros_H_

/***************** Constantes Simbólicas ******************/

#define MAX_NOME  40 /* Número máximo de caracteres num nome */
#define TAM_UF     2 /* Número de dígitos num UF */
#define TAM_CEP    8 /* Número de dígitos num CEP */
#define MAX_COMP  60 /* Número máximo de caracteres */
                     /* num complemento             */
#define MAX_TIPO_LOG 10 /* Número máximo de caracteres */
                        /* num tipo de logradouro      */
#define TAM_DNE   16 /* Número de dígitos numa chave DNE */

   /* Tamanho do acréscimo do bloco usado para conter */
   /* uma linha a cada chamada de realloc()           */
#define TAMANHO_BLOCO 256

/****************** Definições de Tipos *******************/

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

/******************* Alusões de Funções *******************/

extern void ExibeRegistro(const tRegistroCEP *registro);
extern tRegistroCEP *LeRegistro(tRegistroCEP *regCEP);
extern tRegistroCEP *SubstituiRegistro( tRegistroCEP *novo,
                                        const tRegistroCEP *antigo );
extern int TamanhoDeArquivo(FILE *stream);
extern int NumeroDeRegistros( FILE *stream, int tamRegistro );

#endif
