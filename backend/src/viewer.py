import npyscreen


def settings(*args):
    form = npyscreen.Form(name='Number of Images')
    num_images = form.add(npyscreen.TitleText, name='Number of Images:')

    form.edit()

    form = npyscreen.Form(name='Select Files')
    for i in range(0, int(num_images.value)):
        file = form.add(npyscreen.TitleFilename, name="File:")

    form.edit()

    return num_images.value


if __name__ == '__main__':
    print(npyscreen.wrapper_basic(settings))
