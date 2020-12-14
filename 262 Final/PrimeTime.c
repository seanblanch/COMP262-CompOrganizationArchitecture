/******************************************************************************
* FILE: PrimeTime.c
* DESCRIPTION:
*   OpenMP Program - Primes of Composite - C Version
*   In this simple program, we find two primes that multiply into a given
*   composite. The composite has to be a multiple of two primes. The master
*   thread checks if the composite is 4, 6, 9 or a multiple of 2 or 3.
*   The remaining threads utilize a base 6 matrix to find the prime factors.
*
*   * The program ONLY works when the composite is a multiple of
*     two prime numbers. We assume user input respects this condition.
*
* AUTHORS: Ferid Ruano and Sean Blanchard
* DATE: 5/9/2019
*
* OS: Manjaro Deepin Linux
* TERMINAL COMPILE CMD: gcc -fopenmp PrimeTime.c  -o  primetime
*
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main()
{
    int tid; // Store thread number
    unsigned long long int composite; // Variable to store user provided composite
    setlocale(LC_NUMERIC, ""); // Set locale for commas in printing prime factors

    // Loop until user inputs terminating command
    do
    {
        // Ask for a composite
        printf("\nEnter a composite of two prime numbers ('0' to Stop): ");
        scanf("%llu", &composite);

        if (composite != 0)
        {
            // Create a limit to terminate parallel for loop.
            // (2 * any prime) is possible, therefore,
            // (composite / 2) is the limit of possible prime factors
            unsigned long long limit = composite / 2;
            int primesFound = 0; // Used to keep track of factors

            // Create a parallel region
            #pragma omp parallel private(tid)
            {
                tid = omp_get_thread_num(); // Get thread number

                // Only master thread checks for prime factors of 2 & 3
                // or a composite itself being 4, 6, or 9.
                if (tid == 0)
                {
                    if (composite == 4)
                        printf("The prime factor are both: 2\n");
                    else if (composite != 6 && composite % 2 == 0)
                    {
                        primesFound++; // One prime factor found
                        printf("A prime factor is: 2\n");
                    }

                    if (composite == 6)
                        printf("The prime factor are: 2 and 3\n");
                    else if (composite == 9)
                        printf("The prime factors are both: 3\n");
                    else if ((composite - (composite / 3) * 3) == 0)
                    {
                        primesFound++; // One prime factor found
                        printf("A prime factor is: 3\n");
                    }
                }

                // All other threads share resources to parallelize a for loop
                #pragma omp for
                // For loop acts as a base 6 matrix iterator and
                // checks two rows per loop for prime factors
                for (unsigned long long int i = 5; i <= limit; i += 12)
                {
                    // Check column 5 on base 6 matrix
                    if (composite % i == 0)
                    {
                        primesFound++; // One prime factor found
                        // Check if prime factor squared is composite
                        if (i * i == composite)
                        {
                            printf("The prime factors are both: %'llu \n", i);
                            primesFound++; // Increment primesFound to terminate loop
                        }
                        else
                            printf("A prime factor is: %'llu \n", i);
                    }

                    // Check column 1 on base 6 matrix
                    if ((composite % (i + 2)) == 0)
                    {
                        primesFound++; // One prime factor found
                        // Check if prime factor squared is composite
                        if ((i + 2) * (i + 2) == composite)
                        {
                            printf("The prime factors are both: %'llu \n", i + 2);
                            primesFound++; // Increment primesFound to terminate loop
                        }
                        else
                            printf("A prime factor is: %'llu \n", i + 2);
                    }

                    // Check one row down at column 5 of base 6 matrix
                    if ((composite % (i + 6)) == 0)
                    {
                        primesFound++; // One prime factor found
                        // Check if prime factor squared is composite
                        if ((i + 6) * (i + 6) == composite)
                        {
                            printf("The prime factors are both: %'llu \n", i + 6);
                            primesFound++; // Increment primesFound to terminate loop
                        }
                        else
                            printf("A prime factor is: %'llu \n", i + 6);
                    }

                    // Check one row down at column 5 of base 6 matrix
                    if ((composite % (i + 8)) == 0)
                    {
                        primesFound++; // One prime factor found
                        // Check if prime factor squared is composite
                        if ((i + 8) * (i + 8) == composite)
                        {
                            printf("The prime factors are both: %'llu \n", i + 8);
                            primesFound++; // Increment primesFound to terminate loop
                        }
                        else
                            printf("A prime factor is: %'llu \n", i + 8);
                    }

                    // Check if both prime factors have been found
                    if (primesFound >= 2)
                        i = limit + 1;
                }
            }
        }
    }
    while (composite != 0);
    printf("Ending program. Goodbye !\n");
    return 0;
}