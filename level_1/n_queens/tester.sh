#!/bin/bash

# -------------------------------------------------------------------
# Automated tester for n_queens.c
# -------------------------------------------------------------------

BOLD="\033[1m"
RED="\033[31m"
GREEN="\033[32m"
CYAN="\033[36m"
RESET="\033[0m"
GRAY="\033[90m"

NAME="n_queens"
SRC="${NAME}.c"
EXEC="./${NAME}"
TESTS=(0 1 2 3 4 5 6 7)

declare -A RESULTS

# -------------------------------------------------------------------
# Compilation
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
    echo -e "${CYAN}${BOLD}▶ TEST: n = $1${RESET}"
    echo -e "${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}\n"
}

compare_output() {
    local got="$1"
    local expected="$2"
    local testname="$3"

    if diff -u "$expected" "$got" > /dev/null 2>&1; then
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
for n in "${TESTS[@]}"; do
    print_header "$n"

    OUT_FILE=$(mktemp)
    EXP_FILE=$(mktemp)

    # Generate expected output in Python
    python3 - <<EOF > "$EXP_FILE"
def solve(n):
    res = []
    board = [-1]*n
    def valid(r, c):
        for i in range(r):
            if board[i] == c or abs(board[i]-c) == abs(i-r):
                return False
        return True
    def backtrack(r=0):
        if r == n:
            res.append(' '.join(map(str, board)))
            return
        for c in range(n):
            if valid(r, c):
                board[r] = c
                backtrack(r+1)
    backtrack()
    for s in res:
        print(s)

solve($n)
EOF

    # Run user's program
    $EXEC $n > "$OUT_FILE" 2>/dev/null
    RET=$?

    echo -e "${BOLD}Command:${RESET} $EXEC $n"
    echo -e "${BOLD}Return Code:${RESET} $RET\n"

    echo -e "${BOLD}--- Output ---${RESET}"
    cat "$OUT_FILE"
    echo -e "\n${BOLD}--- Expected ---${RESET}"
    cat "$EXP_FILE"
    echo -e "\n${BOLD}--- Comparison ---${RESET}"

    compare_output "$OUT_FILE" "$EXP_FILE" "$n"

    rm -f "$OUT_FILE" "$EXP_FILE"
done

# -------------------------------------------------------------------
# Summary
# -------------------------------------------------------------------
echo -e "\n${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
echo -e "${CYAN}${BOLD}📊 TEST SUMMARY${RESET}"
echo -e "${GRAY}${BOLD}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"

PASS=0
FAIL=0

for n in "${TESTS[@]}"; do
    result=${RESULTS["$n"]}
    if [ "$result" == "OK" ]; then
        echo -e "   ${GREEN}[OK]${RESET}  n=$n"
        ((PASS++))
    else
        echo -e "   ${RED}[KO]${RESET}  n=$n"
        ((FAIL++))
    fi
done

TOTAL=${#TESTS[@]}
echo -e "\n${BOLD}Result:${RESET} ${GREEN}$PASS OK${RESET}, ${RED}$FAIL KO${RESET}, out of $TOTAL total tests."

# -------------------------------------------------------------------
# Cleanup
# -------------------------------------------------------------------
echo -e "\n${CYAN}${BOLD}🧹 Cleaning up...${RESET}"
rm -f "$NAME"
echo -e "${GREEN}✅ All tests finished and cleaned up.${RESET}\n"
