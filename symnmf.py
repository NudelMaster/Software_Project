import sys
import numpy as np
import pandas as pd
import symnmf as sm


def parse_int(num):
    try:
        return int(float(num)) if int(float(num)) == float(num) else 0
    except:
        return 0


def parse_float(num):
    try:
        return float(num)
    except:
        return -1


np.random.seed(1234)

error_msg = "An Error Has Occurred"
max_iter = 300
eps = 0.0001

if len(sys.argv) < 4:
    print(error_msg)
    sys.exit(1)

try:
    k = parse_int(sys.argv[1])
    goal = sys.argv[2]
    file_name = sys.argv[3]

    points = pd.read_csv(file_name, header=None)
    n = len(points)

    if not (1 < k < n):
        sys.exit(1)

    # A = np.fromfunction(
    #     lambda i, j: (
    #         np.exp(-(np.pow(np.linalg.norm(points.iloc[i] - points.iloc[j]), 2) / 2))
    #         if i != j
    #         else 0
    #     ),
    #     (n, n),
    # )
    #     D_inv_sqrt = np.diag([np.sum(A[i]) for i in range(n)])
    # D = sm.ddg(X)

    X = points.values.tolist()
    H = np.random.uniform(0, 2 * np.sqrt(m / k), (n, k))
    W = sm.norm(X)

    # TODO check Nova's python version
    match goal:
        case "symnmf":
            res = sm.symnmf(H, W)
        case "sym":
            res = sm.sym(X)
        case "ddg":
            res = sm.ddg(X)
        case "norm":
            res = sm.norm(X)
        case _:
            res = []

    for line in res:
        print(",".join(["%.4f" % coord for coord in line]))

except:
    print(error_msg)
    sys.exit(1)
