/****
 *
 * CriaArquivoBin.c: Programa que cria o arquivo bin�rio "CEPs.bin"
 *
 ****/

/*********************** Includes *************************/

#include <stdio.h>    /* Fun��es de Entrada/Sa�da */
#include <string.h>   /* Processamento de strings */
#include <stdlib.h>   /* exit() e strtod()        */
#include <time.h>     /* Medi��o de tempo         */

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

#define SEPARADORES "\t;\n" /* Poss�veis separadores de campos */

   /* Nome dos arquivos bin�rio */
#define NOME_ARQUIVO_BIN "CEPs.bin"

   /* Altere o string a seguir para incluir o diret�rio */
   /* em que se encontra o arquivo "CEPs.txt"           */
#define NOME_ARQUIVO_TXT "CEPs.txt"

   /* Se o arquivo "CEPs.txt" encontra-se no diret�rio */
   /* corrente, comente a linha anterior e remova o    */
   /* coment�rio da linha a seguir                     */
//#define NOME_ARQUIVO_TXT "CEPs.txt"

   /* M�ltiplos de bytes */
#define KIBIBYTE 1024             /* 1 Kibibyte */
#define MEBIBYTE (1024*1024)      /* 1 Mebibyte */
#define GIBIBYTE (1024*1024*1024) /* 1 Gibibyte */

/****************** Defini��es de Macros ******************/

   /* Macro utilizada em tratamento */
   /* de condi��es de exce��o       */
#define ASSEGURA(cond, msg) if (!(cond)) {\
                               fprintf(stderr, "\n%s\n", msg);\
                               exit(1); \
                            }

/****************** Defini��es de Tipos *******************/

   /* Tipo de uma estrutura que representa um registro de CEP */
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
           char statusLogradouro;
           char nomeSemAcento[MAX_NOME];
           char chaveDNE[TAM_DNE];
         } tRegistroCEP;

/***************** Defini��es de Fun��es ******************/

/****
 *
 * LeLinhaIlimitada(): L� uma linha de tamanho arbitr�rio num
 *                     stream de texto e armazena os caracteres
 *                     lidos num array alocado dinamicamente
 *
 * Par�metros:
 *      tam (sa�da) - se n�o for NULL, apontar� para uma vari�vel
 *                    que armazenar� o tamanho do string
 *                    constitu�do pelos caracteres da linha
 *      stream (entrada) - stream de texto no qual ser� feita a
 *                         leitura
 *
 * Retorno: Endere�o do array contendo a linha lida. NULL, se
 *          ocorrer erro ou o final do arquivo for atingido
 *          antes da leitura de qualquer caractere
 *
 * Observa��es:
 *       1. O stream deve estar associado a um arquivo de texto
 *          aberto em modo de texto que permita leitura
 *       2. O caractere '\n' n�o � inclu�do no string resultante
 *          da leitura
 *       3. O primeiro par�metro pode ser NULL. Nesse caso, o
 *          tamanho do string n�o ser� armazenado
 *
 ****/
static char *LeLinhaIlimitada(int *tam, FILE *stream)
{
   char *ar = NULL,   /* Ponteiro para um array alocado */
                      /* dinamicamente que conter� os   */
                      /* caracteres lidos               */
        *p; /* Usado em chamada de realloc() */
   int   tamanho = 0, /* Tamanho do array alocado */
         c,  /* Armazenar� cada caractere lido */
         i; /* �ndice do pr�ximo caractere */
            /* a ser inserido no array     */

      /* L� caracteres a partir da posi��o corrente do      */
      /* indicador de posi��o do arquivo e armazena-os num  */
      /* array. A leitura encerra quando '\n' � encontrado, */
      /* o final do arquivo � atingido ou ocorre erro.      */
   for (i = 0; ; ++i) {
         /* L� o pr�ximo caractere no arquivo */
      c = fgetc(stream);

         /* Se ocorreu erro de leitura, libera o  */
         /* bloco eventualmente alocado e retorna */
      if (ferror(stream)) {
         free(ar); /* Libera o bloco apontado por 'ar' */
         return NULL; /* Ocorreu erro de leitura */
      }

         /* Verifica se array est� completo. O maior valor  */
         /* que i poderia assumir deveria ser tamanho - 1.  */
         /* Mas, como ao final, o caractere '\0' dever� ser */
         /* inserido, limita-se o valor de i a tamanho - 2. */
      if (i > tamanho - 2) { /* Limite atingido */
            /* Tenta redimensionar o array */
         p = realloc(ar, tamanho + TAMANHO_BLOCO);

            /* Se o redimensionamento n�o foi     */
            /* poss�vel, libera o bloco e retorna */
         if (!p) {
            free(ar); /* Libera o bloco apontado por 'ar' */
            return NULL; /* Ocorreu erro de aloca��o */
         }

            /* Redimensionamento foi OK. Ent�o, faz-se */
            /* 'ar' apontar para o novo bloco.         */
         ar = p;

            /* O array aumentou de tamanho */
         tamanho = tamanho + TAMANHO_BLOCO;
      }

         /* Se final do arquivo for atingido ou o caractere */
         /* '\n' for lido,  encerra-se a leitura            */
      if (feof(stream) || c == '\n')
         break; /* Leitura encerrada */

      ar[i] = c; /* Acrescenta o �ltimo caractere lido ao array */
   }

      /* Se nenhum caractere foi lido, libera */
      /* o espa�o alocado e retorna NULL      */
   if (feof(stream) && !i) {
      free(ar); /* Libera o bloco apontado por 'ar' */
      return NULL; /* Nenhum caractere foi armazenado no array */
   }

      /* Insere o caractere terminal de string. Neste    */
      /* instante, deve haver espa�o para ele porque o   */
      /* array foi sempre redimensionado deixando um     */
      /* espa�o a mais para o onipresente caractere '\0' */
   ar[i] = '\0';

      /* Atualiza o valor apontado pelo par�metro */
      /* 'tam', se ele n�o for NULL               */
   if (tam)
         /* i � o �ndice do caractere terminal do */
         /* string e corresponde ao seu tamanho   */
      *tam = i;

            /*                                     */
            /* >>> NB: O tamanho do string n�o <<< */
            /* >>>     inclui o caractere '\0' <<< */
            /*                                     */

      /* Tenta ajustar o tamanho do array para n�o */
      /* haver desperd�cio de mem�ria. Como i � o  */
      /* tamanho do string, o tamanho do array que */
      /* o cont�m deve ser i + 1.                  */
   p = realloc(ar, i + 1);

      /*                                                 */
      /* Se a realoca��o foi bem sucedida, retorna-se p. */
      /* Caso contr�rio, 'ar' ainda aponta para um bloco */
      /* v�lido. Talvez, haja desperd�cio de mem�ria,    */
      /* mas, aqui, � melhor retornar 'ar' do que NULL.  */
      /*                                                 */
   return p ? p : ar;
}

/****
*
* ObtemTokens(): Separa um string em partes (tokens)
*
* Par�metros:
*     str (entrada/sa�da) - o string que ser� separado
*                           ou NULL para continuar uma
*                           separa��o anterior
*     sep (entrada) - string contendo os separadores de tokens
*
* Retorno: Endere�o do pr�ximo token do string, se este
*          existir, ou NULL, em caso contr�rio
*
* Observa��o: Esta fun��o N�O � funcionalmente equivalente �
*             fun��o strtok() da biblioteca padr�o de C
*
****/
static char *ObtemTokens(char *str, char const *sep)
{
   static char *proximoToken; /* Aponta para o pr�ximo */
                              /* token, se ele existir */
   char        *s, /* Apontar� para o string no qual a */
                   /* busca pelo token ser� efetuada   */
               *inicio = NULL; /* Guardar� o in�cio */
                               /* do token corrente */

      /* Se 'str' n�o for um ponteiro nulo, o pr�ximo  */
      /* token ser� obtido a partir do in�cio de 'str' */
   if (str)
      proximoToken = str;

      /* Se 'proximoToken' for um ponteiro nulo, o */
      /* string ora explorado n�o tem mais tokens  */
   if (!proximoToken)
      return NULL; /* N�o h� mais token nesse string */

      /* Obt�m o endere�o do primeiro separador */
      /* encontrado em 'proximoToken'           */
   s = strpbrk(proximoToken, sep);

      /* Verifica se foi encontrado algum separador */
      /* no string ora sendo explorado              */
   if (s) {
         /* Termina o token corrente na posi��o em */
         /* que se encontra o separador encontrado */
      *s = '\0';

         /* Guarda o in�cio do token corrente */
      inicio = proximoToken;

         /* O pr�ximo token come�ar� no primeiro */
         /* caractere ap�s o separador           */
      proximoToken = ++s;
   } else
      if (*proximoToken) {
            /* N�o foi encontrado nenhum separador, mas o string  */
            /* corrente n�o � vazio e seu endere�o ser� retornado */
         inicio = proximoToken;

         /* N�o foi encontrado nenhum separador, de modo que */
         /* n�o haver� mais nenhum token na pr�xima chamada  */
      proximoToken = NULL;
   }

   return inicio;
}

/****
 *
 * CopiaNStr(): Copia um determinado n�mero de caracteres de um
 *              string para um array de caracteres
 *
 * Par�metros:
 *     dest (sa�da) - o array que receber� os caracteres
 *     origem (entrada) - string doador de caracteres
 *     n  (entrada) - n�mero m�ximo de caracteres que ser�o copiados
 *
 * Retorno: O primeiro par�metro
 *
 * Observa��o: Esta fun��o � similar � strncpy(). A diferen�a �
 *             que, quando o segundo par�metro � NULL, um string
 *             vazio � copiado para o primeiro par�metro e o
 *             programa n�o � abortado.
 *
 ****/
char *CopiaNStr(char *dest, const char *origem, size_t n)
{
   return origem && *origem ? strncpy(dest, origem, n)
                            : strcpy(dest, "");
}

/****
 *
 * LinhaEmRegistro(): Converte uma linha do arquivo numa estrutura
 *                    do tipo tRegistroCEP
 *
 * Par�metros: regCEP (sa�da) - ponteiro para a estrutura
 *                             resultante da convers�o
 *             linha (entrada/sa�da) - linha que ser� convertida
 *
 * Retorno: Endere�o da estrutura que armazena o resultado
 *          da convers�o
 *
 * Observa��es:
 *     1. Para facilitar o entendimento, esta fun��o assume
 *        que o par�metro 'linha' realmente � um string no
 *        formato das linhas do arquivo. Portanto, os
 *        valores retornados por ObtemTokens() n�o s�o testados
 *        como deveriam.
 *
 *     2. O par�metro 'linha' � alterado por ObtemTokens().
 *
 *     3. A fun��o ObtemTokens() � usada em lugar de strtok()
 *        devido � presen�a de campos vazios no arquivo de
 *        texto, o que faz com que dois separadores apare�am
 *        juntos. Nesse caso, a fun��o strtok() consideraria
 *        os dois separadores como sendo apenas um.
 *
 ****/
static tRegistroCEP *LinhaEmRegistro(tRegistroCEP *regCEP, char *linha)
{
   char *str; /* Apontar� para cada token da linha */
   char *final; /* Final de uma convers�o num�rica */

      /* Obt�m o n�mero e acrescenta-o ao */
      /* respectivo campo da estrutura    */
   str = ObtemTokens(linha, SEPARADORES);
   ASSEGURA(str, "Token nulo: numero");
   regCEP->numero = strtol(str, &final, 10);
   ASSEGURA(!*final, "Erro de conversao numerica");

      /* Obt�m a UF e acrescenta-o ao  */
      /* respectivo campo da estrutura */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->UF, str, TAM_UF);

      /* Obt�m o n�mero de localidade e acrescenta-o */
      /* ao respectivo campo da estrutura            */
   str = ObtemTokens(NULL, SEPARADORES);
   ASSEGURA(str, "Token nulo: localidadeNumero");
   regCEP->localidadeNumero = (int) strtol(str, &final, 10);
   ASSEGURA(!*final || regCEP->localidadeNumero < 0,
            "Erro de conversao numerica");

      /* Obt�m o nome abreviado e acrescenta-o */
      /* ao respectivo campo da estrutura      */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->nomeAbr, str, MAX_NOME);

      /* Obt�m o nome e acrescenta-o ao */
      /* respectivo campo da estrutura  */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->nome, str, MAX_NOME);

      /* Obt�m o in�cio do bairro e acrescenta-o */
      /* ao respectivo campo da estrutura        */
   str = ObtemTokens(NULL, SEPARADORES);
   ASSEGURA(str, "Token nulo: bairroInicio");
   regCEP->bairroInicio = (int) strtol(str, &final, 10);
   ASSEGURA(!*final || regCEP->bairroInicio < 0,
            "Erro de conversao numerica");

      /* Obt�m o final do bairro e acrescenta-o */
      /* ao respectivo campo da estrutura       */
   str = ObtemTokens(NULL, SEPARADORES);
   ASSEGURA(str, "Token nulo: bairroFim");
   regCEP->bairroFim = (int) strtol(str, &final, 10);
   ASSEGURA(!*final || regCEP->bairroFim < 0,
            "Erro de conversao numerica");

      /* Obt�m o CEP e acrescenta-o ao */
      /* respectivo campo da estrutura */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->CEP, str, TAM_CEP);

      /* Obt�m o complemento e acrescenta-o */
      /* ao respectivo campo da estrutura   */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->complemento, str, MAX_COMP);

      /* Obt�m o tipo de logradouro e acrescenta-o */
      /* ao respectivo campo da estrutura          */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->tipoLogradouro, str, MAX_TIPO_LOG);

      /* Obt�m o status de logradouro e acrescenta-o */
      /* ao respectivo campo da estrutura            */
   str = ObtemTokens(NULL, SEPARADORES);
   regCEP->statusLogradouro = *str;

      /* Obt�m o nome sem acento e acrescenta-o */
      /* ao respectivo campo da estrutura       */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->nomeSemAcento, str, MAX_NOME);

      /* Obt�m a chave DNE e acrescenta-o */
      /* ao respectivo campo da estrutura */
   str = ObtemTokens(NULL, SEPARADORES);
   CopiaNStr(regCEP->chaveDNE, str, TAM_DNE);

   return regCEP;
}

/****
 *
 * CriaArquivoBin(): L� cada linha de um arquivo no formato
 *                   especificado, converte-a numa estrutura do
 *                   tipo tRegistroCEP e armazena a estrutura num arquivo
 *                   bin�rio
 *
 * Par�metros:
 *     streamTexto (entrada) - stream associado ao arquivo que ser�
 *                             lido
 *     streamBin (entrada) - stream associado ao arquivo que ser�
 *                           escrito
 *     linhasVazias (sa�da) - linhas do arquivo de entrada que n�o
 *                            s�o levadas em considera��o
 *
 * Retorno: N�mero de estruturas escritas no arquivo bin�rio ou um
 *          valor negativo, se ocorrer erro
 *
 * Observa��es:
 *     1. O stream de entrada deve estar aberto em modo de texto
 *        que permite leitura
 *     2. O stream de sa�da deve estar aberto em modo "wb"
 *
 ****/
int CriaArquivoBin( FILE *streamTexto, FILE *streamBin,
                    int *linhasVazias )
{
   char  *linha; /* Apontar� para cada linha lida */
   tRegistroCEP   umCEP; /* Dados de uma linha      */
                 /* convertida em estrutura */
   int    nRegistros = 0; /* N�mero de estruturas escritas */
                          /* no arquivo bin�rio            */

   *linhasVazias = 0; /* Ainda n�o foi lida nenhuma linha vazia */

      /* Garante leitura a partir do in�cio do arquivo */
   rewind(streamTexto);

      /* L� cada linha do arquivo, cria um registro do tipo tRegistroCEP */
      /* e armazena-o no arquivo bin�rio. O la�o encerra quando  */
      /* h� tentativa de leitura al�m do final do arquivo de     */
      /* entrada, ou ocorre erro de leitura/escrita.             */
   while (1) {
         /* L� uma linha no arquivo de entrada */
      linha = LeLinhaIlimitada(NULL, streamTexto);

         /* Verifica se o final do arquivo foi atingido */
      if (feof(streamTexto))
         break;

         /* Verifica se ocorreu erro de leitura */
      if (ferror(streamTexto))
         return -1; /* Opera��o mal sucedida */

         /* Verifica se a linha lida � vazia */
      if (!linha || !*linha || *linha == '\n') {
         ++*linhasVazias; /* Foi lida uma linha vazia */

         free(linha); /* A linha n�o cont�m dados relevantes */

         continue; /* Salta o restante do corpo do la�o */
      }

         /* Converte a linha lida em estrutura */
      LinhaEmRegistro(&umCEP, linha);

         /* Escreve a estrutura resultante da */
         /* convers�o no arquivo bin�rio      */
      fwrite(&umCEP, sizeof(umCEP), 1, streamBin);

         /* Verifica se ocorreu erro de escrita */
      if (ferror(streamBin))
         return -1; /* Opera��o mal sucedida */

      ++nRegistros; /* Mais um registro lido */

      free(linha); /* Libera o espa�o ocupado pela linha corrente */
   }

   return nRegistros; /* N�o ocorreu nenhum erro */
}

/****
 *
 * MedidaDeTempo(): Mede intervalos de tempo entre duas chamadas
 *                  consecutivas
 *
 * Par�metros:
 *     info (entrada) - informa��o que ser� apresentada na tela
 *
 * Retorno: Nada
 *
 * Observa��o: Esta fun��o deve ser chamada aos pares; i.e., na
 *             primeira chamada ela marca o in�cio da contagem de
 *             tempo e na segunda chamada ela apresenta o intervalo
 *             de tempo decorrido desde a �ltima chamada
 *
 ****/
void MedidaDeTempo(const char *info)
{
   static int    numeroDeChamadas = 0;
   static time_t t0; /* Instante inicial */
   time_t        tf; /* Instante final   */

   if (info) {
      printf("\n>>> %s\n", info);
      fflush(stdout);
   }

      /* Esta fun��o foi chamada mais uma vez */
   ++numeroDeChamadas;

      /* Se o n�mero da chamada for �mpar, esta */
      /* chamada � a primeira de um par         */
   if (numeroDeChamadas%2) { /* Calcula o instante inicial */
      t0 = time(NULL);
   } else { /* Calcula e apresenta o intervalo de   */
            /* tempo entre as duas �ltimas chamadas */
      tf = time(NULL);

      printf( "\n\t*** Tempo gasto na operacao: "
              "%5.2f segundos ***\n", difftime(tf, t0) );
   }
}

/****
 *
 * EscreveNumeroBytes(): Escreve um determinado n�mero de bytes
 *                       (valor) num stream de texto
 *
 * Par�metros:
 *     nBytes (entrada) - o valor a ser escrito
 *     stream (entrada) - o stream de texto
 *
 * Retorno: Nenhum
 *
 ****/
void EscreveNumeroBytes(int nBytes, FILE *stream)
{
   fprintf(stream, " %d bytes", nBytes);

   if (nBytes > GIBIBYTE)
      fprintf(stream, " (%.2f GiB)", (double) nBytes/GIBIBYTE);
   else if (nBytes > MEBIBYTE)
      fprintf(stream, " (%.2f MiB)", (double) nBytes/MEBIBYTE);
   else if (nBytes > KIBIBYTE)
      fprintf(stream, " (%.2f KiB)", (double) nBytes/KIBIBYTE);

   fprintf(stream, "\n");
}

/****
 *
 * main(): Executa a fun��o CriaArquivoBin()
 *
 * Par�metros: Nenhum
 *
 * Retorno: Zero
 *
 ****/
int main(void)
{
   int   nRegistros, /* N�mero de registros nos arquivos  */
         tamArquivo, /* Tamanho do arquivo bin�rio        */
         vazias;     /* Linhas que n�o ser�o consideradas */
   FILE *streamTexto, /* Associado ao arquivo de texto */
        *streamBin;   /* Associado ao arquivo bin�rio */

   printf("\n>>> Tamanho de um registro (tRegistroCEP): %d\n", sizeof(tRegistroCEP));

      /* Tenta abrir arquivo para leitura em modo texto */
   streamTexto = fopen(NOME_ARQUIVO_TXT, "r");

      /* Se o arquivo n�o pode ser aberto, */
      /* nada mais pode ser feito          */
   if (!streamTexto) {
      printf( "O arquivo %s nao pode ser aberto\n",
              NOME_ARQUIVO_TXT );
      return 1; /* Opera��o mal sucedida */
   }

      /* Tenta criar arquivo bin�rio. Se um arquivo com esse   */
      /* nome existir no diret�rio corrente ele ser� destru�do */
   streamBin = fopen(NOME_ARQUIVO_BIN, "wb");

      /* Se o arquivo n�o pode ser aberto, */
      /* nada mais pode ser feito.         */
   if (!streamBin) {
      printf("Arquivo %s nao pode ser aberto\n",NOME_ARQUIVO_BIN);

         /* Antes de retornar, fecha arquivo aberto */
      fclose(streamTexto);

      return 1; /* Opera��o mal sucedida */
   }

   MedidaDeTempo("Criando arquivo...");

      /* L� cada linha do arquivo de texto,   */
      /* converte-a em estrutura do tipo tRegistroCEP */
      /* e armazena-a no arquivo bin�rio.     */
   nRegistros = CriaArquivoBin(streamTexto, streamBin, &vazias);

      /* Verifica se ocorreu erro na cria��o do arquivo */
      /* bin�rio. Se for o caso, informa o usu�rio,     */
      /* fecha os arquivos e encerra o programa.        */
   if (nRegistros < 0) {
      printf( "\n\t>>> Ocorreu erro na criacao do arquivo"
              "\n\t>>> \"%s\"\n", NOME_ARQUIVO_BIN );

         /* Fecha os arquivos antes de encerrar */
      fclose(streamTexto);
      fclose(streamBin);

      return 1; /* Ocorreu erro */
   }

   MedidaDeTempo("...Arquivo criado com sucesso");

   tamArquivo = nRegistros*sizeof(tRegistroCEP);

      /* Apresenta o resultado da convers�o */
   printf( "\n>>> Foram armazenados %d registros no arquivo "
           "\"%s\"\n", nRegistros, NOME_ARQUIVO_BIN );

   printf( "\n>>> Tamanho do arquivo \"%s\":", NOME_ARQUIVO_BIN );
   EscreveNumeroBytes(tamArquivo, stdout);

   printf("\n>>> Linhas vazias: %d\n", vazias);

      /* Fecha os arquivos */
   fclose(streamTexto);
   fclose(streamBin);

   return 0; /* Tudo ocorreu bem */
}

/****************** Resultado do Programa *****************

>>> Tamanho de um registro (tRegistroCEP): 236

>>> Criando arquivo...

>>> ...Operacao encerrada. Arquivo criado com sucesso

        *** Tempo gasto na operacao:  5.00 segundos ***

>>> Foram armazenados 673580 registros no arquivo "CEPs.bin"

>>> Tamanho do arquivo "CEPs.bin": 158964880 bytes (151.60 MiB)

>>> Linhas vazias: 0

**********************************************************/

