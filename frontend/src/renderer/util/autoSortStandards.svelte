<script context="module">
    import { maxBy, each, includes, size, filter, countBy, minBy } from "lodash";

    const matchingStandards = [
        [
            // art
            "image",
            "print",
            "object",
            "art"
        ], [
            // color target
            "target",
            "color",
            "colors",
            "grid"
        ], [
            // flatfield
            "flat",
            "field",
            "white",
            "blank",
            "flatfield"
        ], [
            // darkfield
            "dark",
            "field",
            "black",
            "darkfield"
        ]
    ];

    const probabilityScoreProperties = [
        'artObjectProbability',
        'targetProbability',
        'flatFieldProbability',
        'darkFieldProbability'
    ];

    /**
     * Sorts images to the externalStack
     *  - externalStack requires the following fields:
     *      - imageA, imageB
     *      - targetA, targetB
     *      - flatfieldA, flatfieldB
     *      - darkfieldA, darkfieldB
     *  (targets are ignored if there are fewer than 8 images)
     * @param images
     * @param externalStack
     * @returns any remaining unsorted images
     */
    export function autoSortImages(images, externalStack) {

        let includeTarget = false;
        if(size(images) < 6) {
            return images;
        } else if(size(images) >= 8) {
            includeTarget = true;
        }

        // scan each image name (and path) for possible matches
        each(images, function (image){
            each(probabilityScoreProperties, function (property) {
                image[property] = 0;
            });
            image.imageA = false;

            for(let i = 0; i < matchingStandards.length; i++) {
                each(matchingStandards[i], function (string) {
                    if(includes(image.name.toLowerCase(), string.toLowerCase())) {
                        image[probabilityScoreProperties[i]]++;
                    }
                });
            }
        });

        let imageStack = {
            bestArtImages: [],
            bestTargetImages: [],
            bestFlatFieldImages: [],
            bestDarkFieldImages: []
        }

        // select best images for each category
        let bestArtImage = maxBy(images, probabilityScoreProperties[0]);
        images = filter(images, (e) => e !== bestArtImage)
        imageStack.bestArtImages = [bestArtImage, maxBy(images, probabilityScoreProperties[0])];
        images = filter(images, (e) => e !== maxBy(images, probabilityScoreProperties[0]))

        if(includeTarget) {
            let bestTargetImage = maxBy(images, probabilityScoreProperties[1]);
            images = filter(images, (e) => e !== bestTargetImage)
            imageStack.bestTargetImages = [bestTargetImage, maxBy(images, probabilityScoreProperties[1])];
            images = filter(images, (e) => e !== maxBy(images, probabilityScoreProperties[1]))
        }

        let bestFlatFieldImage = maxBy(images, probabilityScoreProperties[2]);
        images = filter(images, (e) => e !== bestFlatFieldImage)
        imageStack.bestFlatFieldImages = [bestFlatFieldImage, maxBy(images, probabilityScoreProperties[2])];
        images = filter(images, (e) => e !== maxBy(images, probabilityScoreProperties[2]))

        let bestDarkFieldImage = maxBy(images, probabilityScoreProperties[3]);
        images = filter(images, (e) => e !== bestDarkFieldImage)
        imageStack.bestDarkFieldImages = [bestDarkFieldImage, maxBy(images, probabilityScoreProperties[3])];
        images = filter(images, (e) => e !== maxBy(images, probabilityScoreProperties[3]))

        // fill in the image stack
        externalStack.imageA = [maxBy(imageStack?.bestArtImages, (image) => countBy(image?.name)?.A)];
        externalStack.imageB = [minBy(imageStack?.bestArtImages, (image) => countBy(image?.name)?.A)];
        if(includeTarget) {
            externalStack.targetA = [maxBy(imageStack?.bestTargetImages, (image) => countBy(image?.name)?.A)];
            externalStack.targetB = [minBy(imageStack?.bestTargetImages, (image) => countBy(image?.name)?.A)];
        }
        externalStack.flatfieldA = [maxBy(imageStack?.bestFlatFieldImages, (image) => countBy(image?.name)?.A)];
        externalStack.flatfieldB = [minBy(imageStack?.bestFlatFieldImages, (image) => countBy(image?.name)?.A)];
        externalStack.darkfieldA = [maxBy(imageStack?.bestDarkFieldImages, (image) => countBy(image?.name)?.A)];
        externalStack.darkfieldB = [minBy(imageStack?.bestDarkFieldImages, (image) => countBy(image?.name)?.A)];

        // return any images that weren't assigned
        return images;
    }

</script>
