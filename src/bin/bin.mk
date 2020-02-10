BIN_DIR		:=	$(MALLOC_DIR)src/bin/
BIN_SRC		:=	bin_adj.c\
				bin_init.c\
				bin_mem.c\
				bin_of.c\
				bin_verify.c\

BIN_OBJ		=	$(addprefix $(MALLOC_OBJ_DIR), $(BIN_SRC:.c=.o))
MALLOC_SRC	+=	$(addprefix $(BIN_DIR), $(BIN_SRC:.c=.o))
MALLOC_OBJ	+=	$(BIN_OBJ)

$(BIN_OBJ): $(MALLOC_OBJ_DIR)%.o: $(BIN_DIR)%.c
	$(CC) $(MALLOC_CFLAGS) $(MALLOC_INCLUDE) $< -o $@
