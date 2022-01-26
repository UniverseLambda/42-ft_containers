#!/bin/sh
make fclean uldl
./std_test > std_result
./container_test > ft_result

diff ft_result std_result | less
