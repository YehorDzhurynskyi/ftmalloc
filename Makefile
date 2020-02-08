#	libft
LIBFT			:=	libft.a
LIBFT_DIR		:=	./libft/
LIBFT_SRC		=
LIBFT_OBJ		=
LIBFT_OBJ_DIR	:=	./obj/libft/
LIBFT_INCLUDE	:=	-I$(LIBFT_DIR)include/ -I$(LIBFT_DIR)
LIBFT_CFLAGS	:=	-g3 -c -Wall -Wextra -Werror -DFTMALLOC_DEBUG -DFTMALLOC_THREADSAFE
#LIBFT_CFLAGS	:=	-O3 -c -Wall -Wextra -Werror -DFTMALLOC_THREADSAFE

#	malloc
MALLOC			:=	malloc.a
MALLOC_DIR		:= ./
MALLOC_SRC		=
MALLOC_OBJ		=
MALLOC_OBJ_DIR	:=	./obj/
MALLOC_INCLUDE	:=	$(LIBFT_INCLUDE) -I./include/
MALLOC_CFLAGS	:=	$(LIBFT_CFLAGS)

#	compilation
CC				:=	gcc

all: $(MALLOC) all_test

include ./src/src.mk
include ./libft/libft.mk
include ./test/test.mk

$(MALLOC): $(MALLOC_OBJ) $(LIBFT)
	ar rc $@ $^
	ranlib $@

all_test: test_case_malloc test_case_realloc test_case_mt test_case_mt_realloc

test_case_malloc: obj/test_case_malloc.o $(MALLOC) $(LIBFT)
	$(CC) $^ -o $@

test_case_realloc: obj/test_case_realloc.o $(MALLOC) $(LIBFT)
	$(CC) $^ -o $@

test_case_mt:  obj/test_case_mt.o $(MALLOC) $(LIBFT)
	$(CC) $^ -o $@

test_case_mt_realloc:  obj/test_case_mt_realloc.o $(MALLOC) $(LIBFT)
	$(CC) $^ -o $@

clean:
	rm -f $(MALLOC_OBJ)
	rm -f $(LIBFT_OBJ)
	rm -f $(TEST_OBJ)

fclean: clean
	rm -f $(LIBFT)
	rm -f $(MALLOC)
	rm -f test_case_malloc
	rm -f test_case_realloc
	rm -f test_case_mt
	rm -f test_case_mt_realloc

re: fclean all
