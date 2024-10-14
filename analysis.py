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


error_msg = "An Error Has Occurred"
iter_num = 300
epsilon = 0.0001


def kmeans(k, path_to_file):
    input_file = open(path_to_file, "r")
    input_file.seek(0)

    centroids = [
        tuple(float(x) for x in input_file.readline().strip().split(","))
        for _ in range(k)
    ]

    for iter_i in range(int(iter_num)):
        # clear previous clusters
        clusters = [[] for _ in range(k)]
        # reset pointer for file reading
        input_file.seek(0)
        i = 0
        for line in input_file.readlines():
            # iterate over each point and assign to the closest cluster
            point = tuple([float(x) for x in line.split(",")])
            distances = [euclidean_distance(point, centroids[i]) for i in range(k)]
            closest = min(range(k), key=lambda x: distances[x])
            clusters[closest].append(point)
            i += 1

        # calculating new centroids for checking convergence
        new_centroids = [
            tuple(sum(coord) / len(cluster) for coord in zip(*cluster))
            for cluster in clusters
        ]

        # if max convergence for all new centroids is less than epsilon then the condition applies for all
        if (
            max(euclidean_distance(centroids[i], new_centroids[i]) for i in range(k))
            < epsilon
        ):
            centroids = new_centroids
            break

        centroids = new_centroids

    input_file.close()

    for centroid in centroids:
        print(",".join(["%.4f" % coord for coord in centroid]))


def symnmf_clustering(k, path_to_file):
    input_file = open(path_to_file, "r")

    points = pd.read_csv(file_name, header=None)
    n = len(points)

    X = points.values.tolist()

    W = sm.norm(X)
    m = np.average(W)
    H = np.random.uniform(0, 2 * np.sqrt(m / k), (n, k)).tolist()
    res = sm.symnmf(H, W)


try:
    k = parse_int(sys.argv[1])
    file_name = sys.argv[2]

    input_file = open(file_name, "r")

    centroids = [
        tuple(float(x) for x in input_file.readline().strip().split(","))
        for _ in range(k)
    ]

    for iter_i in range(int(iter_num)):
        # clear previous clusters
        clusters = [[] for _ in range(k)]
        # reset pointer for file reading
        input_file.seek(0)
        i = 0
        for line in input_file.readlines():
            # iterate over each point and assign to the closest cluster
            point = tuple([float(x) for x in line.split(",")])
            distances = [euclidean_distance(point, centroids[i]) for i in range(k)]
            closest = min(range(k), key=lambda x: distances[x])
            clusters[closest].append(point)
            i += 1

        # calculating new centroids for checking convergence
        new_centroids = [
            tuple(sum(coord) / len(cluster) for coord in zip(*cluster))
            for cluster in clusters
        ]

        # if max convergence for all new centroids is less than epsilon then the condition applies for all
        if (
            max(euclidean_distance(centroids[i], new_centroids[i]) for i in range(k))
            < epsilon
        ):
            centroids = new_centroids
            break

        centroids = new_centroids

    input_file.close()

    for centroid in centroids:
        print(",".join(["%.4f" % coord for coord in centroid]))

except:
    print(error_msg)
    sys.exit(1)
