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

    #return Q #returns the fibonacci sequence result
    return num_mult

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

def fibo_matrix(S, P):
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

    return Q #returns the fibonacci sequence result
    #return num_mult

def main():
    n: int = int(input())

    fib_list: int = [None] * (n+1) #Creates a placeholder list of size n+1

    n_minus: int = n-1
    n_plus: int = n+1

    Y = [[0,1],[1,1]]
    
    S = [[fibo(n_minus),fibo(n)],[fibo(n),fibo(n_plus)]]
    P = [[0,1],[1,1]]
    
    fib = fibo(n)

    for x in range(n-2):
        Y = fibo_matrix(Y, P)
        print(Y)

    matrix = naive_matrix_mult(S,P) #Calculates the multiplications of the first iteration of the sequence
    total = matrix * (n-1) #Calculates the total number of multiplications

    print(fib)                  #prints the fibonacci sequence number
    fibo_number(n, fib_list)    #calculates the number of fibo() calls for each value 0 - n
    my_output(n, fib_list)      #prints the number of fibo() calls for each value 0 - n
    print(total)                #prints the number of matrix multiplications

if __name__ == "__main__":
    main()
