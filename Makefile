
# -------------------- config -------------------------

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
LINK_NAME := libft_malloc.so
TESTER_NAME = tester

# libs config
LIBFT = -L./src/libft -lft
LIBMALLOC = -L. -lft_malloc

# compiler config
CC = cc
CFLAGS = -Wall -Wextra -Wpedantic -Wconversion #-Werror
INC_DIR = inc

# archive (library) config
AR = ar
ARFLAGS = rcs

# symlink config
LN = ln
LNFLAGS = -s

# utils config
RM = rm -rf
MKDIR = mkdir -p

# -------------------- dependencies ---------------------

SDIR = src
SRC = $(shell find $(SDIR) -type f -name "*.c")

ODIR = build
OBJ = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRC))

TEST_DIR = test
TEST_SRC = $(shell find $(TEST_DIR) -type f -name "*.c")


# -------------------- public rules ---------------------
$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)
	$(LN) $(LNFLAGS) $@ $(LINK_NAME)

all: test

clean:
	$(RM) $(ODIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LINK_NAME)
	$(RM) $(TESTER_NAME)

re: fclean all

test: $(NAME)
	$(CC) $(CFLAGS) -I $(INC_DIR) $(TEST_SRC) $(LIBFT) $(LIBMALLOC) -o $(TESTER_NAME)



# -------------------- util rules -----------------------

# copy the directory tree from $(SDIR) to $(ODIR)
$(ODIR):
	$(MKDIR) $(patsubst $(SDIR)/%, $(ODIR)/% , $(shell find $(SDIR)/ -type d))

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)