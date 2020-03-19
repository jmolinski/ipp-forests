#!/usr/bin/env bash

if (($# != 2)); then
    echo "Usage $0 ./<program> <directory>"
    exit 1
fi

RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'

VALGRIND_ERROR_CODE=33
VALGRIND="valgrind --error-exitcode=${VALGRIND_ERROR_CODE} --leak-check=full --errors-for-leak-kinds=all -q"
PROGRAM=$1
DIRECTORY=$2

temp_out=$(mktemp)
temp_err=$(mktemp)
trap 'rm -f "temp_out"' INT TERM HUP EXIT
trap 'rm -f "temp_err"' INT TERM HUP EXIT

shopt -s nullglob  # to prevent error if directory is empty

for i in $DIRECTORY/*.in ; do
    echo "Test ${i#*$DIRECTORY/}..."
    cat "$i" | $VALGRIND "$PROGRAM"  2>"$temp_err" 1>"$temp_out"
    VALGRIND_EXIT_CODE=$?

    if ((VALGRIND_EXIT_CODE == VALGRIND_ERROR_CODE)); then
        echo -ne "${RED}Wykryto wycieki pamieci, ${NOCOLOR}"
    elif ((VALGRIND_EXIT_CODE == 0)); then
        echo -ne "${GREEN}Brak wyciekow pamieci, ${NOCOLOR}"
    else
        echo -ne "${RED}Valgrind error, kod bledu ${VALGRIND_EXIT_CODE}, ${NOCOLOR}"
    fi

    if cmp -s "${i%in}"out "$temp_out" ; then
        echo -ne "${GREEN}stdout ok, ${NOCOLOR}"
    else
        echo -ne "${RED}stdout nieprawidlowe, ${NOCOLOR}"
    fi

    if cmp -s "${i%in}"err "$temp_err"; then
        echo -e "${GREEN}stderr ok${NOCOLOR}"
    else
        echo -e "${RED}stderr nieprawidlowe${NOCOLOR}"
    fi
done
