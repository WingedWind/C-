import numpy as np
from numpy import linalg as LA
import random

m = 100
num_max = 100

# a = np.random.randint(10, size = (3, 3))
# print(a, "\n")
# print(LA.det(a), "\n")

amount_files = 30

for i in range(0, amount_files):

    m = random.randint(0, 120)
    num_max = random.randint(1, 6)
    matrix = np.random.randint(num_max, size=(m, m))

    while (LA.det(matrix) == 0):
        m = random.randint(0, 120)
        num_max = random.randint(1, 6)
        matrix = np.random.randint(num_max, size=(m, m))

    with open(f'test_{i}.dat', 'w') as f:

        f.write(str(m) + '\n')

        for j in range(0, m):
            for k in range(0, m):
                f.write(str(matrix[j][k]) + ' ')
            f.write('\n')

    with open(f'answer_{i}.dat', 'w') as f_a:
        det = LA.det(matrix)
        if det > 0:
            det += 0.5
        if det < 0:
            det -= 0.5
        f_a.write(str(int(det)))