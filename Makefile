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

# SAN_FLAGS := -fsanitize=address -fsanitize=undefined

CXX_FLAGS := -DDSP_CHECK_VAL=false -g3 -Wall -Wextra -Werror -std=c++98 $(SAN_FLAGS)
LD_FLAGS := $(LD_FLAGS) -g3 -Wall -Wextra -Werror -std=c++98 $(SAN_FLAGS)
LD_LIBS :=

OBJS :=			$(SRCS:.cpp=.o)

all:			$(NAME)

$(NAME):		$(OBJS)
	$(LD) $(LD_FLAGS) -o $@ $(OBJS) $(LD_LIBS)

%.o: %.cpp		$(INCS)
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean:			clean
	rm -f $(NAME)
	rm -f $(STD_NAME)
	rm -f leak_test

distclean:		fclean

re: 			distclean all

uldl:			LD_LIBS += -luldl
uldl:			CXX_FLAGS += -DULDL_DBG
uldl:			$(NAME)

std:			$(STD_NAME)

$(STD_NAME):	CXX_FLAGS += -DTEST_NAMESPACE=std
$(STD_NAME):	$(OBJS)
	$(LD) $(LD_FLAGS) -o $@ $(OBJS) $(LD_LIBS)

leak_test:		leak_test.cpp Vector.hpp
	$(CXX) -g3 -Wall -Wextra -Werror -std=c++98 leak_test.cpp -o leak_test -luldl
