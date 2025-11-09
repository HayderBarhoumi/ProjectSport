# Makefile pour le projet Gestion des Entraîneurs

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-2.0` -Wall -g
LDFLAGS = `pkg-config --libs gtk+-2.0`

# Liste des fichiers objets
OBJS = main.o interface.o support.o callbacks.o entraineur.o

# Nom de l'exécutable
TARGET = prog

# Règle par défaut
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Règles de compilation individuelles
main.o: main.c interface.h support.h callbacks.h
	$(CC) $(CFLAGS) -c main.c

interface.o: interface.c interface.h support.h callbacks.h
	$(CC) $(CFLAGS) -c interface.c

support.o: support.c support.h
	$(CC) $(CFLAGS) -c support.c

callbacks.o: callbacks.c callbacks.h interface.h support.h entraineur.h
	$(CC) $(CFLAGS) -c callbacks.c

entraineur.o: entraineur.c entraineur.h
	$(CC) $(CFLAGS) -c entraineur.c

# Nettoyage
clean:
	rm -f $(OBJS) $(TARGET) *~

# Exécution
run: $(TARGET)
	./$(TARGET)

# Aide
help:
	@echo "Utilisation du Makefile:"
	@echo "  make         - Compile le projet"
	@echo "  make clean   - Supprime les fichiers objets et l'exécutable"
	@echo "  make run     - Compile et exécute le programme"
	@echo "  make help    - Affiche cette aide"

.PHONY: all clean run help
