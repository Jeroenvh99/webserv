NAME	= webserv

CXX			= c++
CXXFLAGS	= -Wall
LDFLAGS		= 
DEBUG_FLAGS	= -g # -fsanitize=address

SRC_DIR	= ./src
SRCS	= main.cpp \

OBJ_DIR	= ./.obj
OBJS	= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) 

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(NAME)

re: clean all
