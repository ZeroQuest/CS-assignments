#!/usr/bin/python3


def bubbleSort(lista) -> int:
    n = len(lista)
    #numComp = 0
    numSwap = 0

    for i in range(n):
        for k in range(0, n-1-i):
            #numComp = numComp + 1
            if lista[k+1] < lista[k]:
                lista[k], lista[k+1] = lista[k+1], lista[k]
                numSwap = numSwap + 1
                #print(lista)
    
    return numSwap

def HoarePartition(lista, l, r) -> tuple[int, int]:
    i = l #= 0
    j = r #= len(lista) - 1
    pivot = lista[l]

    numSwap = 0

    while i < j:
        while i < j and lista[i] <= pivot:
            i = i+1 
        while lista[j] > pivot:
            j = j-1
        #print(f"Before inner swap: {lista}")
        lista[i], lista[j] = lista[j], lista[i]
        #print(f"After inner swap: {lista}")
        #print("\tSwap 1")
        numSwap = numSwap + 1

    #print(f"Before swap: {lista}")
    lista[i], lista[j] = lista[j], lista[i]
    #print(f"After swap: {lista}")
    #print("\t Swap 2")
    #print(f"Before swap2: {lista}")
    lista[l], lista[j] = lista[j], lista[l]
    #print(f"\tAfter swap2: {lista}")
    numSwap = numSwap + 2

    return j, numSwap

def QuickSort(lista, l, r) -> int:
    #l = 0
    #r = len(lista) - 1
    numSwap = 0

    if l < r:
        h = HoarePartition(lista, l, r)
        p = h[0]
        numSwap = numSwap + h[1]
        #print(f"Partition Swaps: {h[1]}")
        #left = lista[:p]
        #right = lista[p+1:]
        leftSwap = QuickSort(lista, l, p-1)
        #print(f"Left swaps {leftSwap}")
        rightSwap = QuickSort(lista, p+1, r)
        #print(f"Right swaps {rightSwap}")

        #print(f"Merged list: {lista}")

        numSwap = numSwap + leftSwap + rightSwap
        #print(f"Total swaps {numSwap}")

    return numSwap

def main():
    numElements = int(input())
    sortList = []

    for i in range(numElements):
        sortList.append(int(input()))

    a=sortList[:]
    b=sortList[:]

    bubble = bubbleSort(a)
    print(f"{bubble}")

    quick = QuickSort(b, 0, len(sortList)-1)
    print(f"{quick}")
    #print(f"QuickSorted List: {b}")

    return 0

if __name__ == "__main__":
    main()
