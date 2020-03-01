import random

from typing import Any, Dict, List

from py_impl import LASY_TYPE, p_add, p_check, p_del, p_print


def mk_identifier():
    return "".join([chr(x) for x in random.choices(range(33, 120), k=7)])


def resize(base: int, mul: float) -> int:
    return int(base * mul)


def make_random_spacing(crazy=False):
    length = random.choice(range(1, 8))
    if crazy:
        characters = "\t \v\f\r"
    else:
        characters = " \t"
    return "".join(random.choices(characters, k=length))


def make_add_tests(lasy, test_size) -> List[str]:
    tests = []

    tests.append("ADD")
    p_add(lasy, [])

    for _ in range(resize(200, test_size)):
        identifier = mk_identifier()
        tests.append(f"ADD{make_random_spacing()}{identifier}")
        p_add(lasy, [identifier])

    forests = list(lasy.keys())

    tests.append(f"PRINT")

    for _ in range(resize(800, test_size)):
        identifier = mk_identifier()
        forest = random.choice(forests)
        tests.append(
            f"ADD{make_random_spacing()}{forest}{make_random_spacing()}{identifier}"
        )
        p_add(lasy, [forest, identifier])

    tests.append(f"PRINT{make_random_spacing()}{forests[0]}")
    for f in forests:
        tests.append(f"PRINT {f}")

    forests_trees = [(f, t) for f in forests for t in lasy[f].keys()]

    for _ in range(resize(2000, test_size)):
        identifier = mk_identifier()
        (forest, tree) = random.choice(forests_trees)
        tests.append(
            f"ADD{make_random_spacing()}{forest}{make_random_spacing()}{tree}{make_random_spacing()}{identifier}"
        )
        p_add(lasy, [forest, tree, identifier])

    # add an animal to a forest/tree that does not exist
    f, t, a = mk_identifier(), mk_identifier(), mk_identifier()
    tests.append(f'ADD {f} {t} {a}')
    p_add(lasy, [f, t, a])

    forests_trees = [(f, t) for f in forests for t in lasy[f].keys()]
    tests.append(
        f"PRINT{make_random_spacing()}{forests_trees[0][0]}{make_random_spacing()}{forests_trees[0][1]}"
    )
    for f, t in forests_trees:
        tests.append(f"PRINT {f} {t}")

    return tests


def generate_tests(test_size: float) -> None:
    lasy: LASY_TYPE = {}

    polecenia: Dict[str, Any] = {
        "ADD": p_add,
        "DEL": p_del,
        "PRINT": p_print,
        "CHECK": p_check,
    }

    tests = make_add_tests(lasy, test_size)

    for t in tests:
        print(t)


if __name__ == "__main__":
    generate_tests(0.1)
