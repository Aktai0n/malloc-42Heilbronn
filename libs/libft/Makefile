
# -------------------- config -------------------------

NAME := libft.a

PLATFORM := $(shell uname -s)

# compiler config
CC := cc
CFLAGS = -std=c99 -O2 \
         -Wall -Wextra -Wconversion -Werror \
         -pedantic  \
         -fPIC
INC_DIR := inc

# toolchain detection
PREDEFS := $(shell echo | $(CC) -dM -E -)
ifeq ($(findstring __clang__,$(PREDEFS)),__clang__)
    TOOLCHAIN := clang
else ifeq ($(findstring __GNUC__,$(PREDEFS)),__GNUC__)
    TOOLCHAIN := gnuc
else
    TOOLCHAIN := 
endif


ifeq ($(TOOLCHAIN), clang)
    CFLAGS += -fvisibility=hidden
endif


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