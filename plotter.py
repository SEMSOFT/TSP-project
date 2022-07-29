import matplotlib.pyplot as plt
import math
import numpy as np


def func(s):
    if "EOF" in s:
        return ""
    return float(s.replace("\n", ""))

def read_from_file(fname):
    f = open(fname, "r")
    lines = f.readlines()
    f.close()
    lines = list(map(func, lines))
    return lines

def read_test_case(fname):
    f = open(fname, "r")
    lines = f.readlines()
    dim = 0
    while True:
        line = lines[0].split(" ");
        if "DIMENSION" in line[0]:
            dim = int(line[-1])
        if "NODE_COORD_SECTION" in line[0]:
            lines = lines[1:dim+1]
            break
        lines = lines[1:]
    coords = []
    for line in lines:
        z = line.split(" ")
        coords.append((float(z[-2]), float(z[-1])))
    return dim, coords

def calc_dist(u, v):
    return math.sqrt((u[0] - v[0])**2 + (u[1] - v[1])**2)


test_case_name = input("test_case_file:")
best_file_name = input("best tour file:")
file_name = input("your tour:")

dim, coords = read_test_case(test_case_name)
best_lines = read_from_file(best_file_name)
our_lines = read_from_file(file_name)

best_weight = 0

for i in range(dim):
    best_weight += calc_dist(
        coords[int(best_lines[i]) - 1],
        coords[int(best_lines[(i + 1) % dim]) - 1]
        )

print("Our tour weight: ", our_lines[0])
print("Best tour weight: ", best_weight)
print("ratio: ", our_lines[0] / best_weight)

points = np.zeros((dim + 1, 2), float)

for i in list(map(int, our_lines[1:])):
    points[i, :] = [int(coords[i][0]), int(coords[i][1])]
points[-1, :] = points[0, :]
# coords = coords[list(map(int, our_lines[1:])), :]

plt.plot(points[:, 0], points[:, 1])
# plt.plot(coords[:, 0], coords[:, 1])
