import rawpy as rp


def rponly(path):
    raw = rp.imread(path)
    return raw.postprocess(use_camera_wb=False,
                           use_auto_wb=False,
                           no_auto_bright=True,
                           output_color=rp.ColorSpace(0),
                           user_black=0,
                           output_bps=16).astype('f4')
