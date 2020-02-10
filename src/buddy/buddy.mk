BUDDY_DIR	:=	$(MALLOC_DIR)src/buddy/
BUDDY_SRC	:=	buddy_deoccupy.c\
				buddy_merge.c\
				buddy_occupy.c\

BUDDY_OBJ		=	$(addprefix $(MALLOC_OBJ_DIR), $(BUDDY_SRC:.c=.o))
MALLOC_SRC	+=	$(addprefix $(BUDDY_DIR), $(BUDDY_SRC:.c=.o))
MALLOC_OBJ	+=	$(BUDDY_OBJ)

$(BUDDY_OBJ): $(MALLOC_OBJ_DIR)%.o: $(BUDDY_DIR)%.c
	$(CC) $(MALLOC_CFLAGS) $(MALLOC_INCLUDE) $< -o $@
