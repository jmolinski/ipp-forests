import sys
import random


def include_print():
    return random.random() < (1/10)


tests = list(sys.stdin.read().split('\n'))

tests = [
    t for t in tests if ('PRINT' not in t) or include_print()
]

for t in tests:
    print(t)

