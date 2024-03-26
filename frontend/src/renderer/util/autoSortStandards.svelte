<script context="module">
    import { maxBy, split, each, size, filter } from "lodash";
    import { findBestMatch } from "./stringCompare";

    const matchingStandards = [
        [
            // art
            "image",
            "print",
            "object",
            "art",
            "exhibit",
            "paint",
            "subject",
            "item"
        ], [
            // color target
            "target",
            "color",
            "grid",
            "map",
            "passport"
        ], [
            // flatfield
            "flat",
            "white",
            "ff",
            "flatfield",
            "vignetting",
            "vignette",
            "light",
            "correction"
        ], [
            // darkfield
            "dark",
            "black",
            "darkfield",
            "current",
            "signal",
            "internal",
            "camera",
            "correction"
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

        console.log(images)
        console.log(externalStack)
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
                console.log(image)
                each(matchingStandards[i], function (string) {
                    const count = split(image.name.toLowerCase(), string.toLowerCase()).length - 1;
                    image[probabilityScoreProperties[i]] += count;
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

        // Uses string similarity to determine which images go into which columns
        const sortImageByLighting = function (image1, image2, exampleImageA) {
            if(findBestMatch(exampleImageA.name, [image1.name, image2.name]).bestMatchIndex === 0) {
                return [image1, image2];
            }
            return [image2, image1];
        };

        // fill in the image stack
        externalStack.imageA = [[imageStack?.bestArtImages[0]]];
        externalStack.imageB = [[imageStack?.bestArtImages[1]]];

        // handle A - B sorting
        if(includeTarget) {
            imageStack.bestTargetImages = sortImageByLighting(imageStack?.bestTargetImages[0], imageStack?.bestTargetImages[1], externalStack.imageA[0][0]);
            externalStack.targetA = [imageStack?.bestTargetImages[0]];
            externalStack.targetB = [imageStack?.bestTargetImages[1]];
        }

        imageStack.bestFlatFieldImages = sortImageByLighting(imageStack?.bestFlatFieldImages[0], imageStack?.bestFlatFieldImages[1], externalStack.imageA[0][0]);
        externalStack.flatfieldA = [imageStack?.bestFlatFieldImages[0]];
        externalStack.flatfieldB = [imageStack?.bestFlatFieldImages[1]];

        imageStack.bestDarkFieldImages = sortImageByLighting(imageStack?.bestDarkFieldImages[0], imageStack?.bestDarkFieldImages[1], externalStack.imageA[0][0]);
        externalStack.darkfieldA = [imageStack?.bestDarkFieldImages[0]];
        externalStack.darkfieldB = [imageStack?.bestDarkFieldImages[1]];

        // return any images that weren't assigned
        return images;
    }

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
    export function autoSortBatchImages(images, externalStack) {

        let includeTarget = true;
        // scan each image name (and path) for possible matches
        each(images, function (image){
            each(probabilityScoreProperties, function (property) {
                image[property] = 0;
            });
            image.imageA = false;

            for(let i = 0; i < matchingStandards.length; i++) {
                each(matchingStandards[i], function (string) {
                    const count = split(image.name.toLowerCase(), string.toLowerCase()).length - 1;
                    image[probabilityScoreProperties[i]] += count;
                });
            }
        });
        let imageStack = {
            bestArtImages: [],
            bestTargetImages: [],
            bestFlatFieldImages: [],
            bestDarkFieldImages: []
        }
     

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

        //let bestArtImage = maxBy(images, probabilityScoreProperties[0]);
        // images = filter(images, (e) => e !== bestArtImage)
        imageStack.bestArtImages = images;

        console.log("best art images")
        console.log(imageStack.bestArtImages)

        
        let artImagesByNumber = {}
        // select best images for each category
         const extractNumber = function (artObj) {
            var numb = artObj.id.match(/\d/g);
            numb = numb.join("");
            console.log(numb)
            if(artImagesByNumber[numb] != null){
                artImagesByNumber[numb].push(artObj)
            }
            else{
                artImagesByNumber[numb] = [artObj]
            }
        }

        each(imageStack.bestArtImages, extractNumber)

        console.log("artImagesByNumber")
        console.log(artImagesByNumber)

        // images = filter(images, (e) => e !== maxBy(images, probabilityScoreProperties[0]))

        // Uses string similarity to determine which images go into which columns
        const sortImageByLighting = function (image1, image2, exampleImageA) {
            if(findBestMatch(exampleImageA.name, [image1.name, image2.name]).bestMatchIndex === 0) {
                return [image1, image2];
            }
            return [image2, image1];
        };

        externalStack.flatfieldA = [imageStack?.bestFlatFieldImages[0]];
        externalStack.flatfieldB = [imageStack?.bestFlatFieldImages[1]];

        // handle A - B sorting
        if(includeTarget) {
            imageStack.bestTargetImages = sortImageByLighting(imageStack?.bestTargetImages[0], imageStack?.bestTargetImages[1], externalStack.flatfieldA[0]);
            externalStack.targetA = [imageStack?.bestTargetImages[0]];
            externalStack.targetB = [imageStack?.bestTargetImages[1]];
        }

        imageStack.bestDarkFieldImages = sortImageByLighting(imageStack?.bestDarkFieldImages[0], imageStack?.bestDarkFieldImages[1], externalStack.flatfieldA[0]);
        externalStack.darkfieldA = [imageStack?.bestDarkFieldImages[0]];
        externalStack.darkfieldB = [imageStack?.bestDarkFieldImages[1]];

        let i = 0;

        let leftovers = []

        each(Object.keys(artImagesByNumber), function(artImageSetKey) {
            let artImageSet = artImagesByNumber[artImageSetKey]
            if(artImageSet.length == 2){
                artImageSet = sortImageByLighting(artImageSet[0], artImageSet[1], externalStack.flatfieldA[0])
                externalStack.imageA[i] = [artImageSet[0]]
                externalStack.imageB[i] = [artImageSet[1]]
                i++
                console.log(externalStack)
            }
            else{
                leftovers.push(...artImageSet)
            }
        });
        

        // return any images that weren't assigned
        return leftovers;
    }

</script>
