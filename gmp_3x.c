#include <gmp.h>
#include <stdio.h>
#include <assert.h>

#define BASE_NUM 10

int main()
{
    /*
        mpz_t is the type defined for GMP integers.
        It is a pointer to the internals of the GMP integer data structure
    */
    char inputStr[512512];
    mpz_t num_x, num_1, num_2, result;
    int flag;

    printf ("Enter your number: ");
    scanf("%512512s" , inputStr); /* NOTE: never every write a call scanf ("%s", inputStr);
                                    You are leaving a security hole in your code. */


    /* 1. Initialize the number */
    mpz_init(num_x);
    /* 2. Parse the input string as a base 10 number */
    flag = mpz_set_str(num_x, inputStr, 10);
    assert (flag == 0); /* If flag is not 0 then the operation failed */

    mpz_init(num_1);
    mpz_set_ui(num_1, 1);

    mpz_init(num_2);
    mpz_set_ui(num_2, 2);

    mpz_init(result);
    mpz_set_ui(result, 0);

    while (mpz_cmp_ui(num_x, 1) != 0)
    {
        mpz_mod(result, num_x, num_2);

        if (mpz_cmp_ui(result, 0) == 0)
            mpz_div(num_x, num_x, num_2);
        else
        {
            mpz_mul_ui(num_x, num_x, 3);
            mpz_add_ui(num_x, num_x, 1);
        }

        mpz_out_str(stdout, BASE_NUM, num_x);
        printf("\n");
    }

    mpz_clear(num_x);
    mpz_clear(num_1);
    mpz_clear(num_2);
    mpz_clear(result);

    return 0;
}