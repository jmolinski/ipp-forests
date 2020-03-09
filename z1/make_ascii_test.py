def mk_id(lst):
    return "".join([chr(x) for x in lst])


id_1 = mk_id([55, 99, 170])
id_2 = mk_id([230, 254, 199])
id_3 = mk_id([49, 31, 177])
id_4 = mk_id([49, 25, 177])
id_5 = mk_id([49, 0, 177])
id_6 = mk_id([49, 5, 177])

tests_no_err = f"""
#Nastepna linijki bez errorow - smieszne znaczki
ADD {id_1} {id_2}
CHECK {id_1} {id_2}
PRINT
PRINT {id_1}
DEL {id_1} {id_2}
PRINT {id_1}"""

tests_err = f"""

# Nastepne linijki z del/add error bo znaczki niebiale z 0-32'''
ADD {id_3}
ADD {id_4}
ADD {id_5}
ADD {id_6}
PRINT
DEL {id_3}
DEL {id_4}
PRINT
DEL {id_5}
DEL {id_6}
PRINT
CHECK {id_3}
CHECK {id_5}

# sprawdz czy nie zepsulo to parsera
ADD A
CHECK A
"""


print(tests_err)
