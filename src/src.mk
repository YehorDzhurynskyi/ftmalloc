SRC_DIR		:=	$(MALLOC_DIR)src/
SRC_SRC		:=	ftmalloc_malloc.c\
				ftmalloc_realloc.c\
				ftmalloc_free.c\
				ftmalloc_bin.c\
				ftmalloc_buddy.c\
				ftmalloc_chunk.c\
				ftmalloc_init.c\
				ftmalloc_reserve_mem.c\
				ftmalloc_show_mem.c\

SRC_OBJ		=	$(addprefix $(MALLOC_OBJ_DIR), $(SRC_SRC:.c=.o))
MALLOC_SRC	+=	$(addprefix $(SRC_DIR), $(SRC_SRC:.c=.o))
MALLOC_OBJ	+=	$(SRC_OBJ)

$(SRC_OBJ): $(MALLOC_OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(MALLOC_CFLAGS) $(MALLOC_INCLUDE) $< -o $@
