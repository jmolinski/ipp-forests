import os
import random
import sys

from typing import Any, Union, List

from py_impl import LASY_TYPE, p_add, p_check, p_del


def mk_identifier(max_len: int = 12, exact_length: int = -1) -> str:
    length = exact_length if exact_length != -1 else random.choice(range(4, max_len))
    return "".join([chr(x) for x in random.choices(range(33, 120), k=length)])


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


def make_add_tests(
    lasy: LASY_TYPE, test_size: float, identifier_generator: Any
) -> List[str]:
    tests = []

    for _ in range(resize(200, test_size)):
        identifier = identifier_generator()
        tests.append(f"ADD {identifier}")
        p_add(lasy, [identifier])

    forests = list(lasy.keys())

    tests.append(f"PRINT")

    for _ in range(resize(800, test_size)):
        identifier = identifier_generator()
        forest = random.choice(forests)
        tests.append(f"ADD {forest} {identifier}")
        p_add(lasy, [forest, identifier])

    tests.append(f"PRINT {forests[0]}")
    for f in forests:
        tests.append(f"PRINT {f}")

    forests_trees = [(f, t) for f in forests for t in lasy[f].keys()]

    for _ in range(resize(2000, test_size)):
        identifier = identifier_generator()
        (forest, tree) = random.choice(forests_trees)
        tests.append(f"ADD {forest} {tree} {identifier}")
        p_add(lasy, [forest, tree, identifier])

    # add an animal to a forest/tree that does not exist
    f, t, a = identifier_generator(), identifier_generator(), identifier_generator()
    tests.append(f"ADD {f} {t} {a}")
    p_add(lasy, [f, t, a])

    forests_trees = [(f, t) for f in forests for t in lasy[f].keys()]
    tests.append(f"PRINT {forests_trees[0][0]} {forests_trees[0][1]}")
    for f, t in forests_trees:
        tests.append(f"PRINT {f} {t}")

    return tests


def make_del_tests(
    lasy: LASY_TYPE, test_size: float, identifier_generator: Any
) -> List[str]:
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


def test_wildcards(lasy: LASY_TYPE):
    print("CHECK A B C")
    print("CHECK * JP2")
    print("CHECK * * GMD")

    forests = list(lasy.keys())
    forests_trees = [(f, t) for f in forests for t in lasy[f].keys()]
    trees_only = [t for (f, t) in forests_trees]
    animals = [a for (f, t) in forests_trees for a in lasy[f][t].keys()]

    for _ in range(5):
        print(f"CHECK * {random.choice(trees_only)}")

    for _ in range(5):
        print(f"CHECK * * {random.choice(animals)}")


class SilencePrint:
    def __enter__(self) -> None:
        self._original_stdout = sys.stdout
        sys.stdout = open(os.devnull, "w")

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> None:
        sys.stdout.close()
        sys.stdout = self._original_stdout


def generate_tests(
    test_size: float,
    spacing_threshold: float,
    long_spacing: Union[bool, int] = False,
    identifier_generator: Any = mk_identifier,
    spaces_only=False,
    append_spacing_beginning=False,
    no_prints=False,
) -> None:
    lasy: LASY_TYPE = {}

    with SilencePrint():
        add_tests = make_add_tests(lasy, test_size, identifier_generator)
        # del_tests = make_del_tests(
        #    lasy, test_size, identifier_generator
        # )
        tests = add_tests

    for t in tests:
        if no_prints:
            if "PRINT" in t:
                continue

        random_spacing = make_random_spacing(threshold=spacing_threshold)
        beginning_spacing = ""

        if long_spacing:
            if type(long_spacing) == bool:
                long_spacing = 1000
            random_spacing = "".join(
                make_random_spacing(spacing_threshold) for _ in range(long_spacing)
            )
        if spaces_only:
            random_spacing = " "
        if append_spacing_beginning:
            beginning_spacing = random_spacing

        print(beginning_spacing + t.replace(" ", random_spacing) + random_spacing)


if __name__ == "__main__":

    def gen_wrap(gen_to_wrap):
        gen = gen_to_wrap()

        def wrapped():
            return next(gen)

        return wrapped

    @gen_wrap
    def generator_long_names(*args, **kwargs):
        while True:
            yield "".join([mk_identifier(exact_length=12) for _ in range(25000)])

    @gen_wrap
    def generator_short_names(*args, **kwargs):
        while True:
            yield mk_identifier(exact_length=10)

    @gen_wrap
    def generator_colliding_names():
        while True:
            yield mk_identifier(exact_length=1)

    @gen_wrap
    def generator_sorted_names():
        names = sorted([mk_identifier(exact_length=10) for _ in range(50 * 1000)])

        for n in names:
            yield n

    generate_tests(
        test_size=0.01,
        spacing_threshold=1,
        long_spacing=250,
        identifier_generator=generator_short_names,
        spaces_only=False,
        no_prints=False,
        append_spacing_beginning=True,
    )

