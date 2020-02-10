SRC_DIR		:=	$(MALLOC_DIR)src/
SRC_SRC		:=	ftcalloc.c\
				ftfree.c\
				ftmalloc_check_heap_fully.c\
				ftmalloc_check_heap_relaxed.c\
				ftmalloc_common.c\
				ftmalloc_posix.c\
				ftmalloc_show_mem.c\
				ftmalloc.c\
				ftrealloc.c\

SRC_OBJ		=	$(addprefix $(MALLOC_OBJ_DIR), $(SRC_SRC:.c=.o))
MALLOC_SRC	+=	$(addprefix $(SRC_DIR), $(SRC_SRC:.c=.o))
MALLOC_OBJ	+=	$(SRC_OBJ)

$(SRC_OBJ): $(MALLOC_OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(MALLOC_CFLAGS) $(MALLOC_INCLUDE) $< -o $@
