CHUNK_DIR	:=	$(MALLOC_DIR)src/chunk/
CHUNK_SRC	:=	chunk_adj.c\
				chunk_arrange.c\
				chunk_bin_of.c\
				chunk_freed.c\
				chunk_in_use.c\
				chunk_mem.c\
				chunk_prev_in_use.c\
				chunk_size.c\
				chunk_verify.c\

CHUNK_OBJ	=	$(addprefix $(MALLOC_OBJ_DIR), $(CHUNK_SRC:.c=.o))
MALLOC_SRC	+=	$(addprefix $(CHUNK_DIR), $(CHUNK_SRC:.c=.o))
MALLOC_OBJ	+=	$(CHUNK_OBJ)

$(CHUNK_OBJ): $(MALLOC_OBJ_DIR)%.o: $(CHUNK_DIR)%.c
	$(CC) $(MALLOC_CFLAGS) $(MALLOC_INCLUDE) $< -o $@
