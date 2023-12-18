CC = gcc
CFLAGS = -Wall -Wextra

SERVERDIR = server
CLIENTDIR = client
OUTDIR = bin

SERVER_SRCS = $(shell find $(SERVERDIR) -name '*.c')

build:
	$(CC) $(CFLAGS) $(SERVER_SRCS) -o $(OUTDIR)/server
	$(CC) $(CLIENTDIR)/main.c -o $(OUTDIR)/Alethia

run:
	./$(OUTDIR)/server

install:
	sudo cp ./$(OUTDIR)/Alethia /bin