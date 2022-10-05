for (int i = 1; i < np; i++)
    {

        // Odd iteration
        if (i % 2 == 1)
        {

            // Odd process number
            if (id % 2 == 1)
            {

                // Compare-exchange with the right neighbor process
                if (id < np - 1)
                {
                    printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador direita %d\n", i, odd_even(i), id, odd_even(id), id, id + 1);
                }
            }

            else
            {

                // Compare-exchange with the left neighbor process
                if (id > 0)
                {
                    printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador esquerda %d\n", i, odd_even(i), id, odd_even(id), id, id - 1);
                }
            }
        }

        // Even iteration
        if (i % 2 == 0)
        {

            // Even process number
            if (id % 2 == 0)
            {

                // Compare-exchange with the right neighbor process
                if (id < np - 1)
                {
                    printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador direita %d\n", i, odd_even(i), id, odd_even(id), id, id + 1);
                }
            }

            // Compare-exchange with the left neighbor process
            else
            {
                printf("I=%d(%s), Id=%d(%s) : Processador %d compara com processador esquerda %d\n", i, odd_even(i), id, odd_even(id), id, id - 1);
            }
        }
    }