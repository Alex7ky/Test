#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

typedef struct bigNumber
{
    char *number;
    size_t sizeNum;
    size_t currSize;
} bigNum;

bigNum createBigNum(size_t sizeNum)
{
    bigNum newBigNum;

    newBigNum.number = malloc(sizeNum);
    newBigNum.sizeNum = sizeNum;
    newBigNum.currSize = 0;

    memset(newBigNum.number, 0, sizeNum);

    return newBigNum;
}

void freeBigNum(bigNum *bn)
{
    free(bn->number);

    bn->number = NULL;
    bn->sizeNum = 0;
    bn->currSize = 0;
}

void copyBigNum(bigNum *bn1, bigNum *bn2)
{
    memcpy(bn1->number, bn2->number, bn2->currSize);
    bn1->sizeNum = bn2->sizeNum;
    bn1->currSize = bn2->currSize;
}

void initBigNum(bigNum *bn, int64_t number)
{
    int charNum = 0;
    int64_t i;
    int64_t tmpNumber = number;

    for (i = 0; tmpNumber != 0; i++)
    {
        charNum = tmpNumber % 10;
        tmpNumber = tmpNumber / 10;
        
        bn->number[i] = charNum + '0';
    }

    bn->currSize = i;
}

bigNum sumBigNum(bigNum bn1, bigNum bn2)
{
    bigNum resBigNum;
    bigNum *maxBigNum;
    size_t j = 0;
    size_t maxSize, minSize;
    int charNum1, charNum2, charNumRes;
    int addNum = 0;

    if (bn1.currSize >= bn2.currSize)
    {
        maxSize = bn1.currSize;
        minSize = bn2.currSize;
        maxBigNum = &bn1;
    }
    else
    {
        maxSize = bn2.currSize;
        minSize = bn1.currSize;
        maxBigNum = &bn2;
    }

    resBigNum.number = malloc(maxSize + 1);
    resBigNum.sizeNum = maxSize + 1;
    resBigNum.currSize = 0;

    for (size_t i = 0; i < minSize; i++)
    {
        charNum1 = bn1.number[i] - '0';
        charNum2 = bn2.number[i] - '0';

        charNumRes = charNum1 + charNum2 + addNum;

        if (charNumRes < 10)
        {
            resBigNum.number[j++] = charNumRes + '0';
            addNum = 0;
        }
        else
        {
            resBigNum.number[j++] = (charNumRes % 10) + '0';
            addNum = 1;
        }
    }

    for (size_t i = minSize; i < maxSize; i++)
    {
        charNumRes = maxBigNum->number[i] - '0';
        charNumRes += addNum;

        if (charNumRes < 10)
        {
            resBigNum.number[j++] = charNumRes + '0';
            addNum = 0;
        }
        else
        {
            resBigNum.number[j++] = (charNumRes % 10) + '0';
            addNum = 1;
        }
    }

    if (addNum == 1)
    {
        resBigNum.number[j++] = addNum + '0';
    }

    resBigNum.currSize = j;

    return resBigNum;
}

void printBigNum(bigNum bn)
{
    for (int64_t i = bn.currSize - 1; i >= 0; i--)
    {
        if (bn.number[i] == '\0')
            return;

        printf("%c", bn.number[i]);
    }
}

bigNum fib(int32_t n)
{
    bigNum fibBigNum;
    bigNum prevBigNum = createBigNum(1000);
    bigNum currBigNum = createBigNum(1000);

    initBigNum(&prevBigNum, 0);
    initBigNum(&currBigNum, 1);

    if (n < 1)
    {
        return prevBigNum;
    }

    if (n == 1)
    {
        return prevBigNum;
    }

    if (n == 2)
    {
        return currBigNum;
    }

    /*printf("fib:\n");*/
    for (int32_t i = 2; i < n; i++)
    {
        fibBigNum = sumBigNum(prevBigNum, currBigNum);
        copyBigNum(&prevBigNum, &currBigNum);
        copyBigNum(&currBigNum, &fibBigNum);
        freeBigNum(&fibBigNum);
        /*printBigNum(currBigNum);
        printf("\n");*/
    }

    freeBigNum(&prevBigNum);
    /*printf("\n");*/

    return currBigNum;
}

int main(void)
{
    int32_t N = 1;
    bigNum fibBigNum;

    printf("Input N: ");
    scanf("%" SCNd32, &N);

    printf("fib(%d) number:\n", N);
    fibBigNum = fib(N);

    printBigNum(fibBigNum);
    printf("\n");

    freeBigNum(&fibBigNum);

    return 0;
}