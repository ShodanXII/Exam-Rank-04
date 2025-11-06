#!/bin/bash

# tester.sh for sandbox assignment
# Usage: ./tester.sh

# Colors
RED="\033[31m"
GREEN="\033[32m"
CYAN="\033[36m"
BOLD="\033[1m"
RESET="\033[0m"

# Cleanup old binary
rm -f sandbox_test

# Compile
echo -e "${CYAN}Compiling...${RESET}"
cc -Wall -Wextra -Werror main.c sandbox.c -o sandbox_test

if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed.${RESET}"
    exit 1
fi

# Run sandbox_test in background
echo -e "${CYAN}Running sandbox tests...${RESET}"
./sandbox_test &
sandbox_pid=$!

# Give time for child processes to potentially die and become zombies
sleep 3

# Check for zombie children of sandbox_test
echo -e "${CYAN}Checking for zombie processes spawned by sandbox_test...${RESET}"
zombies=$(ps --ppid $sandbox_pid -o pid,stat,cmd | awk '$2 ~ /Z/')

if [ -z "$zombies" ]; then
    echo -e "${GREEN}No zombie processes detected. sandbox.c is properly reaping children.${RESET}"
else
    echo -e "${RED}Zombie processes detected!${RESET}"
    echo "$zombies"
fi

# Wait for sandbox_test to finish
wait $sandbox_pid

echo -e "${BOLD}${CYAN}Testing complete.${RESET}"

# Cleanup
rm -f sandbox_test
