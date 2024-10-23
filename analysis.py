import sys
from sklearn.metrics import silhouette_score
import pandas as pd
import numpy as np
import symnmf as sm


def euclidean_distance(point1, point2):
    return sum((point1[d] - point2[d]) ** 2 for d in range(len(point1))) ** 0.5


def parse_int(num):
    try:
        return int(float(num)) if int(float(num)) == float(num) else 0
    except:
        return 0


np.random.seed(1234)

error_msg = "An Error Has Occurred"
iter_num = 300
epsilon = 0.0001


def kmeans_clustering(k, path_to_file):
    input_file = open(path_to_file, "r")
    centroids = [
        tuple(float(x) for x in input_file.readline().strip().split(","))
        for _ in range(k)
    ]
    clustering = []

    for iter_i in range(int(iter_num)):
        # saves both clusters of points and clustering (indices)
        clusters = [[] for _ in range(k)]
        clustering = []
        input_file.seek(0)
        i = 0
        for line in input_file.readlines():
            point = tuple([float(x) for x in line.split(",")])
            distances = [euclidean_distance(point, centroids[i]) for i in range(k)]
            closest = min(range(k), key=lambda x: distances[x])
            clusters[closest].append(point)
            clustering.append(closest)
            i += 1

        new_centroids = [
            tuple(sum(coord) / len(cluster) for coord in zip(*cluster))
            for cluster in clusters
        ]

        if (
            max(euclidean_distance(centroids[i], new_centroids[i]) for i in range(k))
            < epsilon
        ):
            break

        centroids = new_centroids

    input_file.close()

    return np.array(clustering)


def symnmf_clustering(k, path_to_file):
    points = pd.read_csv(path_to_file, header=None)
    n = len(points)

    X = points.values.tolist()

    W = sm.norm(X)
    m = np.average(W)
    H = np.random.uniform(0, 2 * np.sqrt(m / k), (n, k)).tolist()
    res = np.array(sm.symnmf(H, W))

    return np.argmax(np.array(res), axis=1)


try:
    k = parse_int(sys.argv[1])
    file_name = sys.argv[2]

    x = pd.read_csv(file_name, header=None)

    symnmf_res = symnmf_clustering(k, file_name)
    kmeans_res = kmeans_clustering(k, file_name)

    print("nmf: %.4f" % silhouette_score(x, symnmf_res))
    print("nmf: %.4f" % silhouette_score(x, kmeans_res))


except:
    print(error_msg)
    sys.exit(1)
