import os
import random
import sys

from typing import Any, Dict, List

from py_impl import LASY_TYPE, p_add, p_check, p_del, p_print


def mk_identifier(max_len: int = 12) -> str:
    return "".join(
        [
            chr(x)
            for x in random.choices(range(33, 120), k=random.choice(range(4, max_len)))
        ]
    )


def resize(base: int, mul: float) -> int:
    return int(base * mul)


def make_random_spacing(threshold: float = 0.1) -> str:
    length = random.choice(range(1, 8)) if threshold > 0 else 1
    crazy = random.random() < threshold
    if crazy:
        length = random.choice(range(5, 15))
        characters = "\t \v\f\r"
    else:
        if threshold == 0:
            characters = " "
        else:
            characters = " \t"
    return "".join(random.choices(characters, k=length))


def make_add_tests(lasy: LASY_TYPE, test_size: float) -> List[str]:
    tests = ["ADD"]
    p_add(lasy, [])

    for _ in range(resize(200, test_size)):
        identifier = mk_identifier()
        tests.append(f"ADD {identifier}")
        p_add(lasy, [identifier])

    forests = list(lasy.keys())

    tests.append(f"PRINT")

    for _ in range(resize(800, test_size)):
        identifier = mk_identifier()
        forest = random.choice(forests)
        tests.append(f"ADD {forest} {identifier}")
        p_add(lasy, [forest, identifier])

    tests.append(f"PRINT {forests[0]}")
    for f in forests:
        tests.append(f"PRINT {f}")

    forests_trees = [(f, t) for f in forests for t in lasy[f].keys()]

    for _ in range(resize(2000, test_size)):
        identifier = mk_identifier()
        (forest, tree) = random.choice(forests_trees)
        tests.append(f"ADD {forest} {tree} {identifier}")
        p_add(lasy, [forest, tree, identifier])

    # add an animal to a forest/tree that does not exist
    f, t, a = mk_identifier(), mk_identifier(), mk_identifier()
    tests.append(f"ADD {f} {t} {a}")
    p_add(lasy, [f, t, a])

    forests_trees = [(f, t) for f in forests for t in lasy[f].keys()]
    tests.append(f"PRINT {forests_trees[0][0]} {forests_trees[0][1]}")
    for f, t in forests_trees:
        tests.append(f"PRINT {f} {t}")

    return tests


def make_del_tests(lasy: LASY_TYPE, test_size: float) -> List[str]:
    tests = []

    forests = list(lasy.keys())
    forests_trees = [(f, t) for f in forests for t in lasy[f].keys()]
    forests_trees_animals = [
        (f, t, a) for (f, t) in forests_trees for a in lasy[f][t].keys()
    ]

    f, t, a = forests_trees_animals[0]
    tests.append(f"DEL {f} {t} {a} {a}")
    p_del(lasy, [f, t, a, a])
    tests.append(f"PRINT {f} {t}")

    # idempotency
    tests.append(f"DEL {f} {t} {a}")
    p_del(lasy, [f, t, a])
    tests.append(f"PRINT {f} {t}")

    tests.append(f"DEL {f} {t} {a}")
    p_del(lasy, [f, t, a])
    tests.append(f"PRINT {f} {t}")

    for _ in range(resize(500, test_size)):
        f, t, a = random.choice(forests_trees_animals)
        tests.append(f"CHECK {f} {t} {a}")
        p_check(lasy, [f, t, a])
        tests.append(f"DEL {f} {t} {a}")
        p_del(lasy, [f, t, a])
        tests.append(f"CHECK {f} {t} {a}")
        p_check(lasy, [f, t, a])
        tests.append(f"PRINT {f} {t}")

    for _ in range(resize(200, test_size)):
        f, t = random.choice(forests_trees)
        tests.append(f"CHECK {f} {t}")
        p_check(lasy, [f, t])
        tests.append(f"DEL {f} {t}")
        p_del(lasy, [f, t])
        tests.append(f"CHECK {f} {t}")
        p_check(lasy, [f, t])
        tests.append(f"PRINT {f}")

    for _ in range(resize(50, test_size)):
        f = random.choice(forests)
        tests.append(f"CHECK {f}")
        p_check(lasy, [f])
        tests.append(f"DEL {f}")
        p_del(lasy, [f])
        tests.append(f"CHECK {f}")
        p_check(lasy, [f])
        tests.append(f"PRINT")

    tests.append(f"PRINT")
    tests.append("DEL")
    p_del(lasy, [])
    tests.append(f"PRINT")

    return tests


class SilencePrint:
    def __enter__(self) -> None:
        self._original_stdout = sys.stdout
        sys.stdout = open(os.devnull, "w")

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> None:
        sys.stdout.close()
        sys.stdout = self._original_stdout


def generate_tests(test_size: float, spacing_threshold: float) -> None:
    lasy: LASY_TYPE = {}

    with SilencePrint():
        tests = make_add_tests(lasy, test_size) + make_del_tests(lasy, test_size)

    for t in tests:
        print(
            t.replace(" ", make_random_spacing(threshold=spacing_threshold)).replace(
                "\n", make_random_spacing(threshold=spacing_threshold) + "\n"
            )
        )


if __name__ == "__main__":
    generate_tests(0.006, 0.0)
