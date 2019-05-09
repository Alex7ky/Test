#include <gmp.h>
#include <stdio.h>

#define BASE_NUM 10

int main()
{
    /*
        mpz_t is the type defined for GMP integers.
        It is a pointer to the internals of the GMP integer data structure
    */
    mpz_t fib1, fib2, fib3;
    mpz_t *ptr_fib1, *ptr_fib2, *ptr_fib3;
    int i = 0, n = 0;

    printf ("Enter your number: ");
    scanf("%d", &n); 

    /* 1. Initialize the number */
    mpz_init(fib1);
    mpz_set_ui(fib1, 0);
    mpz_init(fib2);
    mpz_set_ui(fib2, 1);
    mpz_init(fib3);
    mpz_set_ui(fib3, 0);

    ptr_fib1 = &fib1;
    ptr_fib2 = &fib2;
    ptr_fib3 = &fib3;

    while (i < n)
    {
        mpz_add (*ptr_fib3, *ptr_fib2, *ptr_fib1);
        ptr_fib1 = ptr_fib2;
        ptr_fib2 = ptr_fib3;
        i = i + 1;
    }

    mpz_out_str(stdout, BASE_NUM, *ptr_fib3);
    printf ("\n");

    mpz_clear(fib1);
    mpz_clear(fib2);
    mpz_clear(fib3);

    return 0;
}

