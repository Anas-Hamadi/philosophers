NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -O3
RM = rm -f

SRCDIR = .
OBJDIR = obj

SOURCES = main.c \
		  init.c \
		  routine.c \
		  actions.c \
		  utils.c

OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
