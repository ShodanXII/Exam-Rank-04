#!/bin/bash
set -e

NAME=ft_popen_test

cc -Wall -Wextra -Werror main.c ft_popen.c -o "$NAME"

./"$NAME"

rm $NAME