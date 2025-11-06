#!/bin/bash

# -------------------------
# Simplified Test Script for picoshell
# -------------------------

# Colors & formatting
GREEN="\033[32m"
RED="\033[31m"
CYAN="\033[36m"
BOLD="\033[1m"
RESET="\033[0m"
GRAY="\033[90m"

NAME=picoshell

# -------------------------
# Compile
# -------------------------
echo -e "${CYAN}${BOLD}🔧 Compiling picoshell...${RESET}"
cc -Wall -Wextra -Werror main.c picoshell.c -o "$NAME"
if [ $? -ne 0 ]; then
    echo -e "${RED}[ERROR] Compilation failed${RESET}"
    exit 1
else
    echo -e "${GREEN}[OK] Compilation successful${RESET}"
fi

EXEC="./$NAME"

# -------------------------
# Helper Functions
# -------------------------
print_header() {
    local TITLE="$1"
    echo -e "\n${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
    echo -e "${CYAN}${BOLD}▶ $TITLE${RESET}"
    echo -e "${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}\n"
}

print_section() {
    local TITLE="$1"
    echo -e "\n${BOLD}${CYAN}--- $TITLE ---${RESET}"
}

print_result() {
    local MSG="$1"
    local OK="$2"
    echo -e "   $([ $OK -eq 0 ] && echo -e ${GREEN}[OK]${RESET} || echo -e ${RED}[FAIL]${RESET}) $MSG"
}

run_test() {
    local DESC="$1"
    shift
    local EXPECT_CMD="$1"
    shift
    local ARGS=("$@")

    print_header "$DESC"
    echo -e "${BOLD}Command:${RESET} ${ARGS[*]}"

    TMP_OUT=$(mktemp)
    $EXEC "${ARGS[@]}" > "$TMP_OUT" 2>&1
    local RET=$?
    OUTPUT=$(cat "$TMP_OUT")
    rm "$TMP_OUT"

    # -------------------------
    # Output Section
    # -------------------------
    print_section "Output"
    echo "$OUTPUT"

    # -------------------------
    # Expected Output
    # -------------------------
    print_section "Expected Output"
    EXPECTED=$(eval $EXPECT_CMD)
    echo "$EXPECTED"

    # -------------------------
    # Compare output
    # -------------------------
    print_section "Comparison"
    if [ "$OUTPUT" == "$EXPECTED" ]; then
        print_result "Output matches expected" 0
    else
        print_result "Output differs from expected" 1
    fi

    # -------------------------
    # Return code Section
    # -------------------------
    print_section "Return Code"
    print_result "picoshell returned $RET" $RET
}

# -------------------------
# TEST CASES
# -------------------------
run_test "TEST 1: Simple ls command" "ls" /bin/ls
run_test "TEST 2: Pipe ls | grep picoshell" "ls | grep picoshell" /bin/ls "|" /usr/bin/grep picoshell
run_test "TEST 3: Multiple pipes echo | cat | sed" "echo 'squalala.' | cat | sed 's/a/b/g'" echo 'squalala.' "|" cat "|" sed 's/a/b/g'
run_test "TEST 4: Nonexistent command" "fakecmd" fakecmd
run_test "TEST 5: Complex pipe sequence" "echo abc | sed 's/a/A/' | tr c C" echo 'abc' "|" sed 's/a/A/' "|" tr 'c' 'C'

# -------------------------
# Finish
# -------------------------
echo -e "\n${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
echo -e "${BOLD}${GREEN}✅ All tests finished.${RESET}\n"

# Cleanup
rm "$NAME"
