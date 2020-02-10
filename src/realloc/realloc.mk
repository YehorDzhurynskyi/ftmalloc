REALLOC_DIR	:=	$(MALLOC_DIR)src/realloc/
REALLOC_SRC	:=	realloc_try_grow.c\
				realloc_try_shrink.c\

REALLOC_OBJ	=	$(addprefix $(MALLOC_OBJ_DIR), $(REALLOC_SRC:.c=.o))
MALLOC_SRC	+=	$(addprefix $(REALLOC_DIR), $(REALLOC_SRC:.c=.o))
MALLOC_OBJ	+=	$(REALLOC_OBJ)

$(REALLOC_OBJ): $(MALLOC_OBJ_DIR)%.o: $(REALLOC_DIR)%.c
	$(CC) $(MALLOC_CFLAGS) $(MALLOC_INCLUDE) $< -o $@
