#!/bin/bash

# -------------------------
# TSP Tester
# -------------------------

GREEN="\033[32m"
RED="\033[31m"
CYAN="\033[36m"
BOLD="\033[1m"
RESET="\033[0m"

NAME=tsp
TEST_DIR="tests_from_exam"

# -------------------------
# Compile
# -------------------------
echo -e "${CYAN}${BOLD}▶ COMPILING...${RESET}"
cc -Wall -Wextra -Werror "$NAME.c" -o "$NAME" -lm
if [ $? -ne 0 ]; then
    echo -e "${RED}[FAIL] Compilation failed${RESET}"
    exit 1
else
    echo -e "${GREEN}✅ Compilation OK${RESET}"
fi

# -------------------------
# Test cases
# -------------------------
FILES=("$TEST_DIR"/*)
TOTAL=${#FILES[@]}
PASS=0

for file in "${FILES[@]}"; do
    EXPECTED=$(basename "$file")            # filename is expected float
    EXPECTED=$(printf "%.2f" "$EXPECTED")   # force 2 decimals

    echo -e "\n${CYAN}${BOLD}▶ TEST: $EXPECTED${RESET}"

    # Run program
    OUTPUT=$(./$NAME < "$file")
    OUTPUT=$(printf "%.2f" "$OUTPUT")       # force 2 decimals

    if [ "$OUTPUT" = "$EXPECTED" ]; then
        echo -e "   ${GREEN}✅ OK${RESET}"
        PASS=$((PASS+1))
    else
        echo -e "   ${RED}❌ KO${RESET}"
        echo -e "   --- Your Output ---"
        echo "   $OUTPUT"
        echo -e "   --- Expected ---"
        echo "   $EXPECTED"
    fi
done

# -------------------------
# Summary
# -------------------------
echo -e "\n${BOLD}${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"
echo -e "${BOLD}SUMMARY: $PASS / $TOTAL OK${RESET}"
echo -e "${BOLD}${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${RESET}"

# Cleanup
rm -f "$NAME"
