
# -------------------- config -------------------------

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
LINK_NAME := libft_malloc.so
TESTER_NAME = tester

# libs config
LIBFT_DIR := libs/libft
LIBFT := -L$(LIBFT_DIR) -lft
LIBMALLOC_DIR := .
LIBMALLOC := -L$(LIBMALLOC_DIR) -lft_malloc

# compiler config
CC := cc
CFLAGS = -std=gnu2x \
         -Wall -Wextra -Wconversion \
         -pedantic  \
         -pthread -Wno-gnu-binary-literal -fvisibility=hidden # -fsanitize=address #-Werror 
INCLUDES := -Iinc -I$(LIBFT_DIR)/inc -Isrc/utils/

# linker config
LDFLAGS = $(LIBMALLOC) $(LIBFT) \
           -Wl,-rpath,$(PWD)

# archive (library) config
AR := ar
ARFLAGS = rcs

# symlink config
LN := ln
LNFLAGS = -sf

# utils config
RM := rm -rf
MKDIR := mkdir -p

# -------------------- dependencies ---------------------

SDIR := src
SRC = $(shell find $(SDIR) -type f -name "*.c")

ODIR := build
OBJ = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRC))

TEST_DIR := test
TEST_SRC = $(shell find $(TEST_DIR) -type f -name "*.c")


# -------------------- public rules ---------------------
all: $(NAME)

$(NAME): $(OBJ) | libs
	$(CC) -shared $(CFLAGS) $(LIBFT) $(OBJ) -o $@
	$(LN) $(LNFLAGS) $@ $(LINK_NAME)

$(LINK_NAME): $(NAME)

clean: | clean_libs
	$(RM) $(ODIR)

fclean: | fclean_libs
	$(RM) $(ODIR)
	$(RM) $(NAME)
	$(RM) $(LINK_NAME)
	$(RM) $(TESTER_NAME)

re: fclean all

test: CFLAGS := $(filter-out -fvisibility=hidden, $(CFLAGS))
test: CFLAGS += -g
test: $(NAME)
	$(CC) $(CFLAGS) $(INCLUDES)  $(TEST_SRC) -o $(TESTER_NAME) $(LDFLAGS)

# TODO: Check on MacOS whether loading the libaries dynamically still works
# @export DYLD_LIBRARY_PATH=.:$(DYLD_LIBRARY_PATH)
# @export DYLD_INSERT_LIBRARIES="$(LINK_NAME)"
# @export DYLD_FORCE_FLAT_NAMESPACE=1
run: test
	@./$(TESTER_NAME)

debug: CFLAGS := $(filter-out -fvisibility=hidden, $(CFLAGS))
debug: CFLAGS += -g
debug: re test

.PHONY: all clean fclean re test run debug

# -------------------- libs rules -----------------------

libs:
	make -C $(LIBFT_DIR)

clean_libs:
	make clean -C $(LIBFT_DIR)

fclean_libs:
	make fclean -C $(LIBFT_DIR)

.PHONY: libs clean_libs fclean_libs

# -------------------- util rules -----------------------

# copy the directory tree from $(SDIR) to $(ODIR)
$(ODIR):
	$(MKDIR) $(patsubst $(SDIR)/%, $(ODIR)/% , $(shell find $(SDIR)/ -type d))

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR) libs
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ $(INCLUDES)
# PIC = Position Independent Code