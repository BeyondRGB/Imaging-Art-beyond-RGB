from difflib import SequenceMatcher


def max_probability(image_probabilities:list, role:str) -> dict:
    """
    Finds the image that has the best similarity for a given role
    [in] image_probabilities : a list of dictionaries that contain ratios for how simular each image is to a role
    [in] role                : the role currently looking at
    :[out] current_max_image : the dictionary that has the highest similarity for the role
    """
    current_max = 0
    current_max_image = []
    for image in image_probabilities:
        value = image[role]
        if value > current_max:
            current_max = value
            current_max_image = image
    return current_max_image


def compare_two_strings(first:str, second:str, example:str="") -> float or bool:
    """
    Compares two strings to which more closely matches a certain term (like Target)
    or compares the two string against an example string for a lighting condition
    [in] first        : a string representing a file name
    [in] second       : a string representing a file name or example name for role
    [in] example      : a string representing an example file name under a lighting condition
    [out] match_ratio : a ratio of how simular the first string is to the second string ~OR~
                        a boolean TRUE if the first string is more simular the example lighting condition
    """
    if example == "":
        return SequenceMatcher(None, first, second).ratio()
    else:
        return SequenceMatcher(None, first, example).ratio() >= SequenceMatcher(None, example, second).ratio()


def sort_images(images: list, artwork:bool) -> list:
    """
    Sorts images based on Role (Target, Flatfield, Darkfield, Artwork) and lighting condition.
    Lighting Condition does not matter as long as each "A" condition is at the same position relative to "B" condition
    [in] images         : a list of strings that represent the path to the images to be used
    [in] artwork        : boolean that represents if the target and artwork images are separate or not.  True: artwork and target are separate
    [out] sorted_images : images sorted based on role and lighting condition
    """
    matchingStandards = [
            ["image", "print", "object", "art", "exhibit","paint"], # art
        ["target", "color", "grid", "map", "passport" ], # target
        ["flat", "white", "ff", "flatfield", "vignetting", "vignette", "light", "correction"], # flat field
        ["dark", "black", "darkfield", "current", "signal", "internal", "camera", "correction" ] # dark field
    ]
    probabilityScoreProperties = ['artObjectProbability', 'targetProbability', 'flatFieldProbability', 'darkFieldProbability']
    image_probability = []
    for image in images:
        image_dict = {"name": image}
        shortened_name = image.split("/")[-1]
        for property in probabilityScoreProperties:
            image_dict[property] = 0

        for i in range(len(matchingStandards)):
            for string in matchingStandards[i]:
                string.lower()
                shortened_name.lower()
                image_dict[probabilityScoreProperties[i]] += compare_two_strings(string, shortened_name)
        image_probability.append(image_dict)

    sorted_images = []
    # target
    best_target_image = max_probability(image_probability, probabilityScoreProperties[1])
    sorted_images.append(best_target_image["name"])
    image_probability.remove(best_target_image)
    best_target_image = max_probability(image_probability, probabilityScoreProperties[1])
    sorted_images.append(best_target_image["name"])
    image_probability.remove(best_target_image)

    # flat field
    best_flatfield_image = max_probability(image_probability, probabilityScoreProperties[2])
    sorted_images.append(best_flatfield_image["name"])
    image_probability.remove(best_flatfield_image)
    best_flatfield_image = max_probability(image_probability, probabilityScoreProperties[2])
    sorted_images.append(best_flatfield_image["name"])
    image_probability.remove(best_flatfield_image)

    # dark field
    best_darkfield_image = max_probability(image_probability, probabilityScoreProperties[3])
    sorted_images.append(best_darkfield_image["name"])
    image_probability.remove(best_darkfield_image)
    best_darkfield_image = max_probability(image_probability, probabilityScoreProperties[3])
    sorted_images.append(best_darkfield_image["name"])
    image_probability.remove(best_darkfield_image)

    # art work
    if artwork:
        for i in range(len(image_probability)):
            best_artwork_image = max_probability(image_probability, probabilityScoreProperties[0])
            sorted_images.append(best_artwork_image["name"])
            image_probability.remove(best_artwork_image)

    example_image_a = sorted_images[0]
    for i in range(0,len(sorted_images),2):
        if not compare_two_strings(sorted_images[i], sorted_images[i+1], example_image_a):
            hold_image = sorted_images[i]
            sorted_images[i] = sorted_images[i+1]
            sorted_images[i+1] = hold_image

    return sorted_images
