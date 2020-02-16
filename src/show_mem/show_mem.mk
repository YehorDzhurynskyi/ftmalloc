SHOW_MEM_DIR	:=	$(MALLOC_DIR)src/show_mem/
SHOW_MEM_SRC	:=	show_mem_chunk.c\
					show_mem_chunk_ex.c\
					show_mem_bin.c\

SHOW_MEM_OBJ	=	$(addprefix $(MALLOC_OBJ_DIR), $(SHOW_MEM_SRC:.c=.o))
MALLOC_SRC	+=	$(addprefix $(SHOW_MEM_DIR), $(SHOW_MEM_SRC:.c=.o))
MALLOC_OBJ	+=	$(SHOW_MEM_OBJ)

$(SHOW_MEM_OBJ): $(MALLOC_OBJ_DIR)%.o: $(SHOW_MEM_DIR)%.c
	$(CC) $(MALLOC_CFLAGS) $(MALLOC_INCLUDE) $< -o $@
