CC = gcc
CFLAGS = -O2 -Wall
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx -lm
INCLUDES = -I/usr/include/SDL
SRC = main.c
OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
EXEC = TileBlock3D

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

clean:
	rm -f $(OBJ) $(EXEC)
