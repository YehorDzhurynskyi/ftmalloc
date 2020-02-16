MEM_DIR	:=	$(MALLOC_DIR)src/mem/
MEM_SRC	:=	mem_allocate.c\
			mem_deallocate.c\
			mem_find.c\
			mem_reserve.c\
			mem_inpool.c\

MEM_OBJ	=	$(addprefix $(MALLOC_OBJ_DIR), $(MEM_SRC:.c=.o))
MALLOC_SRC	+=	$(addprefix $(MEM_DIR), $(MEM_SRC:.c=.o))
MALLOC_OBJ	+=	$(MEM_OBJ)

$(MEM_OBJ): $(MALLOC_OBJ_DIR)%.o: $(MEM_DIR)%.c
	$(CC) $(MALLOC_CFLAGS) $(MALLOC_INCLUDE) $< -o $@
