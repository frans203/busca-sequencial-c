# Compilador utilizado
COMP = gcc

# Linker utilizado
LINKER = gcc

# Op��es de compila��o
OPCOES_COMP = -c -std=c99 -Wall -g

# Op��es de liga��o
OPCOES_LINK = -lm

# Arquivos-objeto
OBJETOS = Tabela.o Leitura.o Registros.o main.o

# Nome do arquivo execut�vel
EXEC = ExercicioBuscaSequencial1.exe

$(EXEC): $(OBJETOS)
	$(LINKER) $(OPCOES_LINK) $(OBJETOS) -o $(EXEC)

Leitura.o: Leitura.c Leitura.h
	$(COMP) $(OPCOES_COMP) Leitura.c -o Leitura.o

Tabela.o: Tabela.c Tabela.h
	$(COMP) $(OPCOES_COMP) Tabela.c -o Tabela.o

Registros.o: Registros.c Registros.h
	$(COMP) $(OPCOES_COMP) Registros.c -o Registros.o

main.o: main.c Tabela.h
	$(COMP) $(OPCOES_COMP) main.c -o main.o
