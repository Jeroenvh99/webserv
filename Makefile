NAME		:= webserv

SRC_DIR		:= ./source/
SRC_SUBDIRS	:= network/ network/ request/ http/
OBJ_DIR		:= ./object/
HDR_DIR		:= ./include/

SRC_FILES	:= main.cpp\
			network/except.cpp\
			network/Handle.cpp\
			network/Poller_ctor.cpp\
			network/Poller_method.cpp\
			network/Poller_Event.cpp \
			request/HttpRequest.cpp \
			http/Response.cpp \

OBJ_FILES	:= $(patsubst %.cpp,%.o,$(SRC_FILES))

CXX			:= clang++
CXXFLAGS	+= -Wall -Wextra -Werror -I$(HDR_DIR) -g
DEPFLAGS	:= -MMD $(@.o=.d) -MP
DEP_FILES	:= $(patsubst %.o,%.d,$(addprefix $(OBJ_DIR), $(OBJ_FILES)))

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(addprefix $(OBJ_DIR),$(OBJ_FILES))
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR) $(addprefix $(OBJ_DIR),$(SRC_SUBDIRS))
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

-include $(DEP_FILES)
