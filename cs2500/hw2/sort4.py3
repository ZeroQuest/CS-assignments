#!/usr/bin/python3

def bubbleSort(lista) -> tuple[int, int]:
    n = len(lista)
    numComp = 0
    numSwap = 0

    for i in range(n):
        for k in range(0, n-1-i):
            numComp = numComp + 1
            if lista[k+1] < lista[k]:
                lista[k], lista[k+1] = lista[k+1], lista[k]
                numSwap = numSwap + 1
                #print(lista)
    
    return numComp, numSwap

def selectionSort(lista) -> tuple[int, int]:
    n = len(lista)
    numComp = 0
    numSwap = 0

    for i in range(n-1):
        min = i 
        for k in range(i+1, n):
            numComp = numComp + 1
            if lista[k] < lista[min]:
                min = k
        lista[i], lista[min] = lista[min], lista[i]
        numSwap = numSwap + 1
    return numComp, numSwap

def insertionSort(lista) -> tuple[int, int]:
    n = len(lista)
    numComp = 0
    numAssign = 0

    for i in range(1, n):
        v = lista[i]
        j = i-1

        if j>=0:
            numComp = numComp + 1

        while j>=0 and lista[j] > v:
            lista[j+1] = lista[j]
            j = j-1
            numAssign = numAssign + 1

            if j>=0:
                numComp = numComp + 1
        lista[j+1] = v
        numAssign = numAssign + 1
    return numComp, numAssign

def mergeSort(lista) ->int:
    n = len(lista)
    numComp = 0

    if n > 1:
        mid = n//2
        left = lista[:mid]
        right = lista[mid:]
        leftComp = mergeSort(left)
        rightComp = mergeSort(right)
        mergeComp = merge(lista, left, right)
        numComp = numComp + mergeComp + leftComp + rightComp
    return numComp

def merge(lista, left, right) -> int:
    p = len(left)
    q = len(right)
    i = 0
    j = 0
    k = 0
    numComp = 0

    while i<p and j<q:
        numComp = numComp + 1
        if left[i]<right[j]:
            lista[k] = left[i]
            i = i+1
            #print("In Left", lista)
        else:
            lista[k] = right[j]
            j = j+1
            #print("In right", lista)
        k = k+1

    if i==p:
        while j<q:
            lista[k] = right[j]
            j = j+1
            k = k+1
            #print("Rest right", lista)
    else:
        while i<p:
            lista[k] = left[i]
            i = i+1
            k = k+1
            #print("Rest left", lista)
    return numComp

def main():
    numElements = int(input())
    sortList = []

    for i in range(numElements):
        sortList.append(int(input()))

    a=sortList[:]
    b=sortList[:]
    c=sortList[:]
    d=sortList[:]

    #print(f"Initial list pre-sort: {sortList}")

    bubble = bubbleSort(a)
    #print(f"List after Bubble Sort: {a}")
    #print(f"\t Number of comparisons: {bubble[0]}")
    #print(f"\t Number of swaps: {bubble[1]}")
    print(f"{bubble[0]}\t{bubble[1]}")

    select = selectionSort(b)
    #print(f"List after Selection Sort: {b}")
    #print(f"\t Number of comparisons: {select[0]}")
    #print(f"\t Number of swaps: {select[1]}")
    print(f"{select[0]}\t{select[1]}")

    insert = insertionSort(c)
    #print(f"List after Insertion Sort: {c}")
    #print(f"\t Number of comparisons: {insert[0]}")
    #print(f"\t Number of array assignments: {insert[1]}")
    print(f"{insert[0]}\t{insert[1]}")

    mer = mergeSort(d)
    #print(f"List after Merge Sort: {d}")
    #print(f"\t Number of comparisons: {mer}")
    print(f"{mer}")

    return 0

if __name__ == "__main__":
    main()

