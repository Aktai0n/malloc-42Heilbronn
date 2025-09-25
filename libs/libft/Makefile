
# -------------------- config -------------------------

NAME := libft.a


# compiler config
CC := cc
CFLAGS = -std=c99 -O2 \
         -Wall -Wextra -Wconversion -Werror \
         -pedantic  \
         -fPIC
# ifeq ($(PLATFORM),Darwin)
    # CFLAGS += -fvisibility=hidden
# endif
INC_DIR := inc

# archive (library) config
AR := ar
ARFLAGS = rcs


# utils config
RM := rm -rf
MKDIR := mkdir -p

# -------------------- dependencies ---------------------

SDIR := src
SRC = $(shell find $(SDIR) -type f -name "*.c")

ODIR := build
OBJ = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRC))

# -------------------- public rules ---------------------

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $(NAME) $(OBJ)

clean:
	$(RM) $(ODIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

# -------------------- util rules -----------------------

# copy the directory tree from $(SDIR) to $(ODIR)
$(ODIR):
	$(MKDIR) $(patsubst $(SDIR)/%, $(ODIR)/% , $(shell find $(SDIR)/ -type d))

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)