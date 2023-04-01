import rawpy as rp
from cv2 import cvtColor, COLOR_BayerRGGB2RGB


def rpwcv2(path):
    raw = rp.imread(path).raw_image
    rgb = cvtColor(raw, COLOR_BayerRGGB2RGB).astype('f4')
    return rgb
