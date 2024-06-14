NAME		:= webserv

SRC_DIR		:= ./source/
SRC_SUBDIRS	:= network/ cgi/ http/ http/Request_Parser/ logging/ route/
OBJ_DIR		:= ./object/
HDR_DIR		:= ./include/

SRC_FILES	:= main.cpp \
			Environment_ctor.cpp \
			Environment_method.cpp \
			Client.cpp \
			Client_io.cpp \
			Client_method.cpp \
			ClientImpl.cpp \
			html.cpp \
			Server_ctor.cpp \
			Server_io.cpp \
			Server_method.cpp \
			Server_process.cpp \
			Server_respond.cpp \
			URI.cpp \
			cgi/CGI_ctor.cpp \
			cgi/CGI_method.cpp \
			cgi/except.cpp \
			http/Method.cpp \
			http/Message.cpp \
			http/Request.cpp \
			http/Request_Parser/except.cpp \
			http/Request_Parser/Parser.cpp \
			http/Request_Parser/parse_body.cpp \
			http/Request_Parser/parse_chunks.cpp \
			http/Request_Parser/parse_headers.cpp \
			http/Request_Parser/parse_start.cpp \
			http/Request_Parser/utils.cpp \
			http/Response.cpp \
			http/Response_io.cpp \
			http/Status.cpp \
			http/Version.cpp \
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

CXX			:= c++
CXXFLAGS	+= -Wall -Wextra -Werror -I$(HDR_DIR) --std=c++20 -g
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
