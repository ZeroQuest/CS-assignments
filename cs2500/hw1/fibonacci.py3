#!/bin/python3

def fibo(n: int) -> int:
    if n <= 1 :
        return n
    else:
        return fibo(n-1) + fibo(n-2)

def naive_matrix_mult(S, P):
    a = len(S)
    b = len(S[0])
    g = len(P)
    h = len(P[0])

    if b != g:
        return -1
    
    Q = [[0 for _ in range(h)] for _ in range(a)]
    num_mult = 0

    for m in range(a):
        for r in range (h):
            Q[m][r] = 0
            for k in range(g):
                Q[m][r] += S[m][k] * P[k][r]
                num_mult += 1

    return num_mult #returns the number of multiplications

def naive_matrix(S,P):
    a = len(S)
    b = len(S[0])
    g = len(P)
    h = len(P[0])

    if b != g:
        return -1
    
    Q = [[0 for _ in range(h)] for _ in range(a)]

    for m in range(a):
        for r in range (h):
            Q[m][r] = 0
            for k in range(g):
                Q[m][r] += S[m][k] * P[k][r]

    return Q #returns the matrix result

def fibo_number(n, fib_list):
    
    if fib_list[n] == None:
        fib_list[n] = 1
    else:
        fib_list[n] += 1

    if n <= 1:
        return n
    else:
        return fibo_number(n-1, fib_list) + fibo_number(n-2, fib_list)

def my_output(n, fib_list):
    for x in range(n+1):
        print(f"fibo({x}) : {fib_list[x]}")

def main():
    n: int = int(input())

    fib_list: int = [None] * (n+1) #Creates a placeholder list of size n+1

    S = [[0,1],[1,1]]
    P = [[0,1],[1,1]]
   
    matrix = 0

    fib = fibo(n)

    for x in range(n-1):
        matrix += naive_matrix_mult(S,P) #Calculates the multiplications of the first iteration of the sequence
        S = naive_matrix(S,P)

    print(fib)                  #prints the fibonacci sequence number
    fibo_number(n, fib_list)    #calculates the number of fibo() calls for each value 0 - n
    my_output(n, fib_list)      #prints the number of fibo() calls for each value 0 - n
    print(matrix)                #prints the number of matrix multiplications

if __name__ == "__main__":
    main()
