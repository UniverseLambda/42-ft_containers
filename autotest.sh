#!/bin/sh
make fclean std
./std_test > std_result
make fclean all
./container_test > ft_result

diff ft_result std_result | less
