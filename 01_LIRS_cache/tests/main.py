import random
import numpy as np

amount_files = 250

for i in range(0, amount_files, 5):

    num_files = 0

    amount_elements: int = random.randint(10 ** 2, 10 ** 4)

    normal:    object = np.random.normal(1, 8, amount_elements)
    poisson:   object = np.random.poisson(8, amount_elements)
    gamma:     object = np.random.gamma(2, 2, amount_elements)
    binom:     object = np.random.binomial(10, 0.5, amount_elements)
    geometric: object = np.random.geometric(0.1, amount_elements)

    for j in normal, poisson, gamma, binom, geometric:

        cash_size: int = random.randint(40, 110)
        with open(f'test_{i + num_files}.dat', 'w') as f:
            f.write(str(amount_elements) + ' ' + str(cash_size) + ' ' + ' '.join(map(str, map(int, j))))

        num_files += 1
