#!/bin/bash

# -------------------------------------------------------------------
# Automated tester for permutations.c
# -------------------------------------------------------------------

BOLD="\033[1m"
RED="\033[31m"
GREEN="\033[32m"
CYAN="\033[36m"
RESET="\033[0m"
GRAY="\033[90m"

NAME="permutations"
SRC="${NAME}.c"
EXEC="./${NAME}"
TESTS=("a" "ab" "abc" "dbca" "fholan")

# Arrays to track results
declare -A RESULTS

# -------------------------------------------------------------------
# Compile the program
# -------------------------------------------------------------------
echo -e "${CYAN}${BOLD}🔧 Compiling ${SRC}...${RESET}"
gcc -Wall -Wextra -Werror "$SRC" -o "$NAME"
if [ $? -ne 0 ]; then
    echo -e "${RED}[ERROR] Compilation failed.${RESET}"
    exit 1
else
    echo -e "${GREEN}[OK] Compilation successful.${RESET}"
fi

# -------------------------------------------------------------------
# Utility functions
# -------------------------------------------------------------------
print_header() {
    echo -e "\n${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
    echo -e "${CYAN}${BOLD}▶ TEST: '$1'${RESET}"
    echo -e "${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}\n"
}

compare_output() {
    local got="$1"
    local expected="$2"
    local testname="$3"

    if diff -u "$got" "$expected" > /dev/null 2>&1; then
        echo -e "${GREEN}[OK] Output matches expected${RESET}"
        RESULTS["$testname"]="OK"
    else
        echo -e "${RED}[FAIL] Output differs${RESET}"
        RESULTS["$testname"]="KO"
        echo -e "\n${CYAN}--- Expected ---${RESET}"
        cat "$expected"
        echo -e "\n${CYAN}--- Got ---${RESET}"
        cat "$got"
    fi
}

# -------------------------------------------------------------------
# Run tests
# -------------------------------------------------------------------
for test_input in "${TESTS[@]}"; do
    print_header "$test_input"

    OUT_FILE=$(mktemp)
    EXP_FILE=$(mktemp)

    # Generate expected output using Python
    python3 - <<EOF > "$EXP_FILE"
from itertools import permutations
for p in sorted([''.join(x) for x in permutations("$test_input")]):
    print(p)
EOF

    # Run user program
    $EXEC "$test_input" > "$OUT_FILE" 2>/dev/null
    RET=$?

    echo -e "${BOLD}Command:${RESET} $EXEC $test_input"
    echo -e "${BOLD}Return Code:${RESET} $RET\n"

    echo -e "${BOLD}--- Output ---${RESET}"
    cat "$OUT_FILE"
    echo -e "\n${BOLD}--- Expected ---${RESET}"
    cat "$EXP_FILE"
    echo -e "\n${BOLD}--- Comparison ---${RESET}"

    compare_output "$OUT_FILE" "$EXP_FILE" "$test_input"

    rm -f "$OUT_FILE" "$EXP_FILE"
done

# -------------------------------------------------------------------
# Summary
# -------------------------------------------------------------------
echo -e "\n${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
echo -e "${CYAN}${BOLD}📊 TEST SUMMARY${RESET}"
echo -e "${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"

PASS_COUNT=0
FAIL_COUNT=0

for test_input in "${TESTS[@]}"; do
    result=${RESULTS["$test_input"]}
    if [ "$result" == "OK" ]; then
        echo -e "   ${GREEN}[OK]${RESET}  '$test_input'"
        ((PASS_COUNT++))
    else
        echo -e "   ${RED}[KO]${RESET}  '$test_input'"
        ((FAIL_COUNT++))
    fi
done

TOTAL=${#TESTS[@]}
echo -e "\n${BOLD}Result:${RESET} ${GREEN}$PASS_COUNT OK${RESET}, ${RED}$FAIL_COUNT KO${RESET}, out of $TOTAL total tests."

# -------------------------------------------------------------------
# Cleanup
# -------------------------------------------------------------------
echo -e "\n${CYAN}${BOLD}🧹 Cleaning up...${RESET}"
rm -f "$NAME"
echo -e "${GREEN}✅ All tests finished and cleaned up.${RESET}\n"
