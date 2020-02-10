#	libft
LIBFT			:=	libft.a
LIBFT_DIR		:=	./libft/
LIBFT_SRC		=
LIBFT_OBJ		=
LIBFT_OBJ_DIR	:=	./obj/libft/
LIBFT_INCLUDE	:=	-I$(LIBFT_DIR)include/ -I$(LIBFT_DIR)
LIBFT_CFLAGS	:=	-g3 -c -Wall -Wextra -Werror -DFTMALLOC_DEBUG -DFTMALLOC_THREADSAFE -DFTMALLOC_POSIX_API

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

#	malloc
MALLOC			:=	libft_malloc_$(HOSTTYPE).so
MALLOC_LNK		:=	libft_malloc.so
MALLOC_DIR		:= ./
MALLOC_SRC		=
MALLOC_OBJ		=
MALLOC_OBJ_DIR	:=	./obj/
MALLOC_INCLUDE	:=	$(LIBFT_INCLUDE) -I./include/ -I./src/
MALLOC_CFLAGS	:=	$(LIBFT_CFLAGS)

#	compilation
CC				:=	gcc

all: $(MALLOC_LNK)

include ./libft/libft.mk
include ./src/src.mk
include ./src/bin/bin.mk
include ./src/buddy/buddy.mk
include ./src/chunk/chunk.mk
include ./src/mem/mem.mk
include ./src/realloc/realloc.mk
include ./src/show_mem/show_mem.mk

$(MALLOC): $(MALLOC_OBJ) $(LIBFT)
	$(CC) -dynamiclib -o $@ $^

$(MALLOC_LNK): | $(MALLOC)
	ln -s $(MALLOC) $@

clean:
	rm -f $(MALLOC_OBJ)
	rm -f $(LIBFT_OBJ)

fclean: clean
	rm -f $(LIBFT)
	rm -f $(MALLOC)
	rm -f $(MALLOC_LNK)

re: fclean all
