import npyscreen
from target_selector import select_target


def settings(*args):
    form = npyscreen.Form(name='Number of Images')
    num_images = form.add(npyscreen.TitleText, name='Image Count:')

    form.edit()

    form = npyscreen.Form(name='Select Files')
    files = []
    files.append((form.add(npyscreen.TitleFilename, name="Target A:")))
    files.append((form.add(npyscreen.TitleFilename, name="Target B:")))
    files.append((form.add(npyscreen.TitleFilename, name="Flat Field A:")))
    files.append((form.add(npyscreen.TitleFilename, name="Flat Field B:")))
    files.append((form.add(npyscreen.TitleFilename, name="Dark Field A:")))
    files.append((form.add(npyscreen.TitleFilename, name="Dark Field B:")))
    files.append((form.add(npyscreen.TitleFilename, name="Subject 1A:")))
    files.append((form.add(npyscreen.TitleFilename, name="Subject 1B:")))

    form.edit()
    
    return select_target(files[0].value)



if __name__ == '__main__':
    print(npyscreen.wrapper_basic(settings))
