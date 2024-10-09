NAME		:= webserv
SRC_DIR		:= ./source/
SRC_SUBDIRS	:= network/ cgi/ http/ http/parse job/ logging/ route/ config/
OBJ_DIR		:= ./object/
INCLUDE_DIR	:= ./include/

SRC_FILES	:= Environment_build.cpp \
			Environment_ctor.cpp \
			Environment_method.cpp \
			Client.cpp \
			Client_io.cpp \
			ClientImpl.cpp \
			config/Config.cpp \
			main.cpp \
			Poller.cpp \
			Server_ctor.cpp \
			Server_io.cpp \
			Server_method.cpp \
			Server_process.cpp \
			URI.cpp \
			utils.cpp\
			VirtualServer.cpp \
			Worker_ctor.cpp \
			Worker_method.cpp \
			http/Body.cpp \
			http/chunk.cpp \
			http/Header.cpp \
			http/html.cpp \
			http/Message.cpp \
			http/Method.cpp \
			http/Request.cpp \
			http/Response.cpp \
			http/Response_io.cpp \
			http/Status.cpp \
			http/utils.cpp \
			http/Version.cpp \
			http/parse/except.cpp \
			http/parse/HeaderParser.cpp \
			http/parse/RequestParser.cpp \
			http/parse/MultipartParser.cpp \
			job/CGI_ctor.cpp \
			job/CGI_io.cpp \
			job/CGI_spawn.cpp \
			job/except.cpp \
			job/Job.cpp \
			job/Resource_builtin.cpp \
			job/Resource_delete.cpp \
			job/Resource_get.cpp \
			job/Resource_headers.cpp \
			job/Resource_io.cpp \
			job/Resource_post.cpp \
			logging/Logger.cpp \
			logging/AccessLogger.cpp \
			logging/ErrorLogger.cpp \
			logging/Variable.cpp \
			network/Address_IPv4.cpp \
			network/except.cpp\
			network/Handle.cpp\
			network/Poller_ctor.cpp \
			network/Poller_Event.cpp \
			network/Poller_method.cpp \
			network/Poller_Event.cpp \
			network/Address_IPv4.cpp \
			route/BaseRoute.cpp \
			route/Location.cpp \
			route/Route_ctor.cpp \
			route/Route_method.cpp

OBJ_FILES	:= $(patsubst %.cpp,%.o,$(SRC_FILES))

CXX			:= clang++ # On Codam machines, c++ is actually an alias to clang++.
CXXFLAGS	:= -Wall -Wextra -Werror -I$(INCLUDE_DIR) --std=c++20
DEBUG_FLAGS	:= -g -fsanitize=address
DEPFLAGS	:= -MMD $(@.o=.d) -MP
DEP_FILES	:= $(patsubst %.o,%.d,$(addprefix $(OBJ_DIR), $(OBJ_FILES)))

.PHONY: all clean fclean re test debug

all: $(NAME)

CXXFLAGS += $(DEBUG_FLAGS)
debug: $(NAME)

$(NAME): $(addprefix $(OBJ_DIR),$(OBJ_FILES))
	@echo Compiling executable: $@.
	@$(CXX) $(CXXFLAGS) $(EXEC_MAIN) $^ -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR) $(addprefix $(OBJ_DIR),$(SRC_SUBDIRS))
	@echo Compiling object file: $@.
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

test:
	@docker build -t webserv-tests -f ./tests/Dockerfile .
	docker run -it webserv-tests

-include $(DEP_FILES)
