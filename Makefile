
# -------------------- config -------------------------

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
LINK_NAME := libft_malloc.so
TESTER_NAME = tester

# libs config
# LIBFT = -L./src/libft -lft
LIBFT := 
LIBMALLOC := -L. -lft_malloc

# compiler config
CC := cc
CFLAGS = -std=gnu2x \
         -Wall -Wextra -Wconversion \
         -pedantic -Wno-gnu-binary-literal \
         -pthread #-Werror
INC_DIR := inc

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
all: test

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -shared $(LIBFT) $(OBJ) -o $@
	$(LN) $(LNFLAGS) $@ $(LINK_NAME)

$(LINK_NAME): $(NAME)

clean:
	$(RM) $(ODIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LINK_NAME)
	$(RM) $(TESTER_NAME)

re: fclean all

test: $(NAME)
	$(CC) $(CFLAGS) -I $(INC_DIR) $(TEST_SRC) $(LIBFT) $(LIBMALLOC) -o $(TESTER_NAME)

run_test: test
	@export DYLD_LIBRARY_PATH=.:$(DYLD_LIBRARY_PATH)
	@export DYLD_INSERT_LIBRARIES="$(LINK_NAME)"
	@export DYLD_FORCE_FLAT_NAMESPACE=1
	@./tester


.PHONY: all clean fclean re test run_tests



# -------------------- util rules -----------------------

# copy the directory tree from $(SDIR) to $(ODIR)
$(ODIR):
	$(MKDIR) $(patsubst $(SDIR)/%, $(ODIR)/% , $(shell find $(SDIR)/ -type d))

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INC_DIR)
# PIC = Position Independent Code