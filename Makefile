.PHONY:	all re clean fclean distclean leak_test uldl std
.SUFFIX: .cpp .o

NAME :=	container_test
STD_NAME := std_test

SRCS :=	main.cpp \

INCS :=

ifeq ($(shell uname), Linux)
CXX :=	g++
else
CXX :=	clang++
endif

LD := $(CXX)

SAN_FLAGS := -fsanitize=address -fsanitize=undefined

CXX_FLAGS := -DDSP_CHECK_VAL=false -g3 -Wall -Wextra -Werror -std=c++98 -I. $(SAN_FLAGS)
LD_LIBS :=

all:			$(NAME) $(STD_NAME)

$(NAME):		$(SRCS)
	$(CXX) $(CXX_FLAGS) -o $@ $(SRCS) $(LD_LIBS)

$(STD_NAME):	$(SRCS)
	$(CXX) $(CXX_FLAGS) -DTEST_NAMESPACE=std -o $@ $(SRCS) $(LD_LIBS)

clean:

fclean:			clean
	rm -f $(NAME)
	rm -f $(STD_NAME)
	rm -f leak_test

distclean:		fclean

re: 			distclean all

uldl:			LD_LIBS += -L. -luldl
uldl:			CXX_FLAGS += -DULDL_DBG
uldl:			all
