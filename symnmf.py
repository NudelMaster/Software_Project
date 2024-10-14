import sys
import numpy as np
import pandas as pd
import symnmf as sm


def parse_int(num):
    try:
        return int(float(num)) if int(float(num)) == float(num) else 0
    except:
        return 0


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

    X = points.values.tolist()

    if goal == "symnmf":
        W = sm.norm(X)
        m = np.average(W)
        H = np.random.uniform(0, 2 * np.sqrt(m / k), (n, k)).tolist()
        res = sm.symnmf(H, W)
    elif goal == "sym":
        res = sm.sym(X)
    elif goal == "ddg":
        res = sm.ddg(X)
    elif goal == "norm":
        res = sm.norm(X)
    else:
        res = []

    for line in res:
        print(",".join(["%.4f" % coord for coord in line]))

except:
    print(error_msg)
    sys.exit(1)
