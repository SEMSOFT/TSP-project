import matplotlib.pyplot as plt
import math
import numpy as np


def func(s):
    if "EOF" in s:
        return ""
    return float(s.replace("\n", ""))


def func2(s):
    if "EOF" in s:
        return ""
    return s.replace("\n", "")


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
        line = lines[0].split(" ")
        if "DIMENSION" in line[0]:
            dim = int(line[-1])
        if "NODE_COORD_SECTION" in line[0]:
            lines = lines[1:dim + 1]
            break
        lines = lines[1:]
    coords = []
    for line in lines:
        z = func2(line).split(" ")
        x = []
        for zz in z:
            if zz != "":
                x.append(zz)
        coords.append((float(x[-2]), float(x[-1])))
    return dim, coords


def calc_dist(u, v):
    return math.sqrt((u[0] - v[0]) ** 2 + (u[1] - v[1]) ** 2)


test_case_name = input("test_case_file:")
best_file_name = test_case_name[:-4] + ".opt.tour"
file_name = "sol_" + test_case_name
have_opt = int(input("opt solution exists? (0, 1)"))

dim, coords = read_test_case(test_case_name)
if have_opt == 1:
    best_lines = read_from_file(best_file_name)
our_lines = read_from_file(file_name)

best_weight = 0
cur_weight = 0
if have_opt == 1:
    for i in range(dim):
        best_weight += calc_dist(
            coords[int(best_lines[i]) - 1],
            coords[int(best_lines[(i + 1) % dim]) - 1]
        )
        cur_weight += calc_dist(
            coords[int(our_lines[i + 1])],
            coords[int(our_lines[(i + 1) % dim + 1])]
        )


print("Our tour weight: ", our_lines[0])
if have_opt == 1:
    print("Best tour weight: ", best_weight)
    print("ratio: ", our_lines[0] / best_weight)

points = np.zeros((dim + 1, 2), float)

order = list(map(int, our_lines[1:]))
for i in range(dim):
    points[i, :] = [int(coords[order[i]][0]), int(coords[order[i]][1])]
# for i in list(map(int, our_lines[1:])):
#     points[i, :] = [int(coords[i][0]), int(coords[i][1])]
points[-1, :] = points[0, :]

plt.plot(points[:, 0], points[:, 1])
plt.scatter(points[:, 0], points[:, 1])
plt.show()
