import math
import random
import sys
import time

sys.setrecursionlimit(7000)


def generuj_dane(n):
    return [random.randrange(1, n) for _ in range(n)]


def generuj_dane_4(n, z):
    return [random.randrange(1, math.floor(n * z)) for _ in range(n)]


def licz_czas(funkcja, dane):
    kopia = dane.copy()
    start = time.time()
    posortowane = funkcja(kopia)
    koniec = time.time()
    assert posortowane == sorted(dane)
    return koniec - start


def licz_czas_quick(funkcja, dane):
    kopia = dane.copy()
    start = time.time()
    posortowane = funkcja(kopia, 0, len(kopia) - 1)
    koniec = time.time()
    assert posortowane == sorted(dane)
    return koniec - start


def bubble_sort(lst):
    n = len(lst)
    for i in range(n):
        for j in range(n - i - 1):
            if lst[j] > lst[j + 1]:
                lst[j], lst[j + 1] = lst[j + 1], lst[j]

    return lst


def selection_sort(lst):
    n = len(lst)
    for i in range(n):
        min_idx = i
        for j in range(i + 1, n):
            if lst[j] < lst[min_idx]:
                min_idx = j
        lst[i], lst[min_idx] = lst[min_idx], lst[i]
    return lst


def counting_sort(lst):
    max_val = max(lst)
    counts = [0] * (max_val + 1)
    output = [0] * len(lst)

    for val in lst:
        counts[val] += 1

    for i in range(1, len(counts)):
        counts[i] += counts[i - 1]

    for val in lst:
        output[counts[val] - 1] = val
        counts[val] -= 1

    return output


def insert_sort(lst):
    for i, elem in enumerate(lst):
        j = i - 1
        while j >= 0 and elem < lst[j]:
            lst[j + 1] = lst[j]
            j = j - 1
        lst[j + 1] = elem
    return lst


def merge_sort(lst):
    if len(lst) <= 1:
        return lst

    srodek = len(lst) // 2
    czesc_lewa = lst[:srodek]
    czesc_prawa = lst[srodek:]

    merge_sort(czesc_lewa)
    merge_sort(czesc_prawa)

    idx_lewy = 0
    idx_prawy = 0
    idx_lst = 0

    while idx_lewy < len(czesc_lewa) and idx_prawy < len(czesc_prawa):
        if czesc_lewa[idx_lewy] < czesc_prawa[idx_prawy]:
            lst[idx_lst] = czesc_lewa[idx_lewy]
            idx_lewy += 1
        else:
            lst[idx_lst] = czesc_prawa[idx_prawy]
            idx_prawy += 1
        idx_lst += 1

    if idx_lewy < len(czesc_lewa):
        lst[idx_lst:] = czesc_lewa[idx_lewy:]

    elif idx_prawy < len(czesc_prawa):
        lst[idx_lst:] = czesc_prawa[idx_prawy:]

    return lst


def q_part_side(lst, left, right):
    pivot = lst[right]
    p = left
    r = right

    while True:
        while lst[p] < pivot:
            p += 1
        while lst[r] > pivot:
            r -= 1

        if p < r:
            lst[p], lst[r] = lst[r], lst[p]
            p += 1
            r -= 1
        else:
            if r == right:
                r -= 1
            return r


def q_part_middle(lst, left, right):
    pivot = lst[(left + right) // 2]
    p = left
    r = right

    while True:
        while lst[p] < pivot:
            p += 1
        while lst[r] > pivot:
            r -= 1

        if p < r:
            lst[p], lst[r] = lst[r], lst[p]
            p += 1
            r -= 1
        else:
            if r == right:
                r -= 1
            return r


def quick_sort_middle(lst, left, right):
    if left < right:
        q = q_part_middle(lst, left, right)
        quick_sort_middle(lst, left, q)
        quick_sort_middle(lst, q + 1, right)
    return lst


def quick_sort_side(lst, left, right):
    if left < right:
        q = q_part_side(lst, left, right)
        quick_sort_side(lst, left, q)
        quick_sort_side(lst, q + 1, right)
    return lst


def heap_sort(dane):
    n = len(dane)
    for i in range(n // 2 - 1, -1, -1):
        heap(dane, n, i)
    for i in range(n - 1, 0, -1):
        dane[0], dane[i] = dane[i], dane[0]
        heap(dane, i, 0)
    return dane


def heap(dane, n, i):
    najwiekszy = i
    l = 2 * i + 1
    p = 2 * i + 2
    if l < n and dane[l] > dane[najwiekszy]:
        najwiekszy = l
    if p < n and dane[p] > dane[najwiekszy]:
        najwiekszy = p
    if najwiekszy != i:
        dane[i], dane[najwiekszy] = dane[najwiekszy], dane[i]
        heap(dane, n, najwiekszy)


def zadanie2():
    funkcje_sortowania = [
        selection_sort,
        insert_sort,
        bubble_sort,
        heap_sort,
        merge_sort,
        counting_sort,
    ]

    for n in range(5000, 12000 + 1, 500):
        wygenerowane_dane = generuj_dane(n)
        czasy = [
            licz_czas(funkcja, wygenerowane_dane)
            for funkcja in funkcje_sortowania
        ]
        czas_quick = licz_czas_quick(quick_sort_middle, wygenerowane_dane)
        czasy.append(czas_quick)
        zaok_czasy = [str(round(elem * 1000, 3)) for elem in czasy]
        print(n, " ".join(zaok_czasy))

    funkcje_szybkie = [heap_sort, merge_sort, counting_sort]
    for n in range(15000, 30000 + 1, 3000):
        wygenerowane_dane = generuj_dane(n)
        czasy = [
            licz_czas(funkcja, wygenerowane_dane)
            for funkcja in funkcje_szybkie
        ]
        czas_quick = licz_czas_quick(quick_sort_middle, wygenerowane_dane)
        czasy.append(czas_quick)
        zaok_czasy = [str(round(elem * 1000, 3)) for elem in czasy]
        print(f"{n} - - -", end=" ")
        print(" ".join(zaok_czasy))


def zadanie3():
    def licz(dane):
        czas_is = licz_czas(insert_sort, dane.copy())
        czas_qs_middle = licz_czas_quick(quick_sort_middle, dane.copy())
        czas_qs_side = licz_czas_quick(quick_sort_side, dane.copy())
        czasy = [czas_is, czas_qs_middle, czas_qs_side]
        zaok_czasy = [str(round(elem * 1000, 3)) for elem in czasy]
        return zaok_czasy

    print("Liczby losowe")
    for n in range(5000, 13000 + 1, 500):
        wyg_dane_losowe = generuj_dane(n)
        czasy_losowe = licz(wyg_dane_losowe)
        print(n, " ".join(czasy_losowe))

    print("Przypadek pesymistyczny")
    for n in range(1000, 6500 + 1, 500):
        dane_rosnace = list(range(1, n + 1))
        czasy_pes = licz(dane_rosnace)
        print(n, " ".join(czasy_pes))


def zadanie4():
    def licz(dane):
        czas_cs = licz_czas(counting_sort, dane.copy())
        czas_qs_middle = licz_czas_quick(quick_sort_middle, dane.copy())
        czasy = [czas_cs, czas_qs_middle]
        zaok_czasy = [str(round(elem * 1000, 3)) for elem in czasy]
        return zaok_czasy

    print("Counting sort 100n")
    for n in range(30000, 50000 + 1, 1000):
        dane_100x = generuj_dane_4(n, 100)
        czasy_100 = licz(dane_100x)
        print(n, " ".join(czasy_100))

    print("Counting sort 0.01n")
    for n in range(100000, 200000 + 1, 5000):
        dane_001x = generuj_dane_4(n, 0.01)
        czasy_001 = licz(dane_001x)
        print(n, " ".join(czasy_001))


zadanie2()
zadanie3()
zadanie4()
