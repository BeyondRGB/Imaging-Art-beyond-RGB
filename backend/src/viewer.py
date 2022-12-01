import npyscreen


def settings(*args):
    form = npyscreen.Form(name='Number of Images')
    num_images = form.add(npyscreen.TitleText, name='Number of Images:')

    form.edit()


if __name__ == '__main__':
    print(npyscreen.wrapper_basic(settings))
