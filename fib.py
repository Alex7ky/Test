#n = int(input("input n: "))
n = 2000000
fib1 = 0
fib2 = 1
i = 1

while (i < n):
    fib3 = fib1 + fib2
    fib1 = fib2
    fib2 = fib3
    i = i + 1
print(int(fib3))