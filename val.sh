#!/bin/bash
#valgrind --tool=memcheck --leak-check=yes --track-fds=yes --show-reachable=yes --error-limit=no --log-file=out.txt ./dev_test "240a64fe3240"
valgrind --tool=memcheck --leak-check=yes --track-fds=yes --show-reachable=yes --error-limit=no --log-file=out.txt ./zyc1
