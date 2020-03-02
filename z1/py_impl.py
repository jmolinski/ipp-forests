import sys

from typing import Any, Dict, Generator, List

LASY_TYPE = Dict[str, Dict[str, Dict[str, Dict[str, Any]]]]


def error() -> None:
    print("ERROR", file=sys.stderr)


def p_add(lasy: LASY_TYPE, args: List[str]) -> None:
    if len(args) == 1:
        las = args[0]
        if las not in lasy:
            lasy[las] = {}
    elif len(args) == 2:
        las, drzewo = args
        if las not in lasy:
            lasy[las] = {}
        if drzewo not in lasy[las]:
            lasy[las][drzewo] = {}
    elif len(args) == 3:
        las, drzewo, zwierze = args
        if las not in lasy:
            lasy[las] = {}
        if drzewo not in lasy[las]:
            lasy[las][drzewo] = {}
        if zwierze not in lasy[las][drzewo]:
            lasy[las][drzewo][zwierze] = {}
    else:
        error()
        return

    print("OK")


def p_del(lasy: LASY_TYPE, args: List[str]) -> None:
    if len(args) > 3:
        error()
        return

    if len(args) == 0:
        for k in list(lasy.keys()):
            del lasy[k]
    try:
        if len(args) == 1:
            del lasy[args[0]]
        if len(args) == 2:
            del lasy[args[0]][args[1]]
        if len(args) == 3:
            del lasy[args[0]][args[1]][args[2]]
    except:
        # powinno cos wypisywac?
        # error()
        pass

    print("OK")


def p_check(lasy: LASY_TYPE, args: List[str]) -> None:
    to_en = {
        True: "YES",
        False: "NO",
    }
    try:
        if args[-1] == "*":
            error()
            return

        if len(args) == 1:
            print(to_en[args[0] in lasy])
        elif len(args) == 2:
            forests = list(lasy.values()) if args[0] == "*" else [lasy[args[0]]]
            print(to_en[any(args[1] in las for las in forests)])
        elif len(args) == 3:
            forests = list(lasy.values()) if args[0] == "*" else [lasy[args[0]]]
            if args[1] == "*":
                drzewa = [tree for las in forests for tree in las.values()]
            else:
                drzewa = [las[args[1]] for las in forests if args[1] in las]

            present = any(args[2] in tree for tree in drzewa)
            print(to_en[present])
        else:
            error()
            return
    except Exception as e:
        error()
        return


def p_print(lasy: LASY_TYPE, args: List[str]) -> None:
    try:
        if len(args) == 0:
            names = lasy.keys()
        elif len(args) == 1:
            names = lasy[args[0]].keys()
        elif len(args) == 2:
            names = lasy[args[0]][args[1]].keys()
        else:
            error()
            return
    except:
        pass
        # powinno wypisywac error jak obiekt nie istnieje?
        # error()
    else:
        for name in sorted(names):
            print(name)


def read_polecenia() -> Generator[List[str], None, None]:
    for line in sys.stdin.read().split("\n"):
        if not line.strip() or line.strip()[0] == "#":
            continue

        yield [stripped for a in line.split() if (stripped := a.strip())]


def main() -> None:
    lasy: LASY_TYPE = {}

    polecenia: Dict[str, Any] = {
        "ADD": p_add,
        "DEL": p_del,
        "PRINT": p_print,
        "CHECK": p_check,
    }

    for (polecenie, *args) in read_polecenia():
        if polecenie not in polecenia:
            error()
        else:
            polecenia[polecenie](lasy, args)


if __name__ == "__main__":
    main()
