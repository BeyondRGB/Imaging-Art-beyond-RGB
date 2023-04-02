import numpy as np


def cpplr(path):
    x = 7968    # double check x and y are in the correct order
    y = 5320
    z = 3
    arr = np.fromfile(path, int, -1, " ")
    arr = np.reshape(arr, (x, y, z))
    return arr