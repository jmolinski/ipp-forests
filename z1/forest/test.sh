#!/usr/bin/env bash

if (($# != 2)); then
    echo "Usage $0 ./<program> <directory>"
    exit 1
fi

RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'

VALGRIND_ERROR_CODE=15
VALGRIND="valgrind -q"
PROGRAM=$1
DIRECTORY=$2

temp_out=$(mktemp)
temp_err=$(mktemp)
trap 'rm -f "temp_out"' INT TERM HUP EXIT
trap 'rm -f "temp_err"' INT TERM HUP EXIT

for i in $DIRECTORY/*.in ; do
    echo "Testing ${i#*$DIRECTORY/}..."
    cat $i | $VALGRIND $PROGRAM  2>"$temp_err" 1>"$temp_out"
    EXIT_CODE=$?

    if ((EXIT_CODE == VALGRIND_ERROR_CODE)); then
        echo -ne "${RED}Wykryto wycieki pamieci, ${NOCOLOR}"
    elif ((EXIT_CODE == 0)); then
        echo -ne "${GREEN}Brak wyciekow pamieci, ${NOCOLOR}"
    else
        echo -ne "${RED}Valgrind error, exit code ${EXIT_CODE}, ${NOCOLOR}"
    fi

    #Output
    if cmp -s ${i%in}out "$temp_out" ; then
        if cmp -s ${i%in}err "$temp_err"; then
            echo -e "${GREEN}Prawidlowe wyjscie${NOCOLOR}"
        else
            echo -e "${RED}Nieprawidlowe wyjscie na stderr${NOCOLOR}"
        fi
    else
        echo -e "${RED}Nieprawidlowe wyjscie na stdout${NOCOLOR}"
    fi
done

#Cleaning
rm -f current.err current.out