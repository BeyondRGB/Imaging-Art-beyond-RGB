import rawpy as rp


def rponly(path):
    raw = rp.imread(path)
    rgb = raw.postprocess()
    return rgb
