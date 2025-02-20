<script context="module">
    import { maxBy, split, each, size, filter, indexOf, remove } from "lodash";
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

    const suffixStandards = [
        [
            // Do not change which element is at index 0 unless you want to refactor the keys in suffixSortImages' image_stack

            // art
            "image",
            "print",
            "object",
            "art",
            "exhibit",
            "paint",
            "subject",
            "item",
            "obj"
        ], [
            // color target
            "target",
            "targets",
            "color",
            "grid",
            "map",
            "passport"
        ],
        [
            // flatfield
            "flatfield",
            "flat",
            "white",
            "ff",
            "vignetting",
            "vignette",
            "light",
            "correction", 
            "f", 
            "w"
        ],
        [
            // darkfield
            "darkfield",
            "d", 
            "darkcurrent", 
            "dc", 
            "black",
            "dark",
            "current",
            "signal",
            "internal",
            "camera",
        ]
    ]

    const probabilityScoreProperties = [
        'artObjectProbability',
        'targetProbability',
        'flatFieldProbability',
        'darkFieldProbability'
    ];


    /**
     * 
     * 
     * @param images
     * @param externalStack
     * @returns any remaining unsorted images
     */
    export function suffixSortImages(images, externalStack){
        console.log(images)
        console.log(externalStack)
        /**
         * Are these required..? so long as the code bounces any images that don't belong to a specific category
         * let includeTarget = false;
            if(size(images) < 6) {
                return images;
            } else if(size(images) >= 8) {
                includeTarget = true;
            }
        */

        // Create a mapping of known image types to the [[]]
        let image_stack = new Map([
            ["target", [null, null]],
            ["image", [null, null]],
            ["darkfield", [null, null]],
            [ "flatfield", [null, null]]
        ])

        // List of endings- 'id's for the image- to the images
        let image_id = []

        // for IMAGE in IMAGES, 
        each(images, function (image){
            var img_str = split(image.name.toLowerCase(), '-');
            // Tear off just the suffix, ignoring all other hyphens in the filename barring the one connected to the suffix (i.e. the last hyphen)
            img_str = img_str[img_str.length - 1];
            // Split into the image type and image id
            let img_parts = split(img_str, '_')

            // img_str becomes the image type, and img_id becomes the image identifier
            img_str = img_parts[0];
            let img_id = img_parts[1];

            // if the ids have not yet been captured and the image id isn't already in image ids, add it in there
            // this could get buggy if there are more than 2 suffixes, either by design or by accident. Only the first 2 suffixes are recorded in this statement
            if(image_id.length < 2){
                if(indexOf(img_id) == -1){
                    image_id.push(img_id);
                }
            }

            
            for(let i = 0; i < suffixStandards.length; i++) {
                var ind = indexOf(suffixStandards[i], img_str)
                // If the given suffix is in the array of suffixes, add it to the image stack,
                //  where the key for the image stack belongs to the list of suffixes that the matched suffix is from
                if(ind!= -1){
                    var temp = image_stack.get(suffixStandards[i][0])    
                    temp[image_id.indexOf(img_id)] = image  
                    image_stack.set(suffixStandards[i][0], temp);
                }
            }
        });
        
        console.log("image stack pre splice:")
        console.table(image_stack)

        // console.log("null test")
        // images.splice(null, 1)
        // console.table(images)

        externalStack.imageA = [[image_stack.get("image")[0]? image_stack.get("image")[0]: [{"id": []}]]]
        externalStack.imageB = [[image_stack.get("image")[1]? image_stack.get("image")[1]: [{"id": []}]]]
        // images = filter(images, (e) => e !== image_stack.get("image"))
        // images.splice(images.indexOf(image_stack.get("image")[0]), 1)
        // images.splice(images.indexOf(image_stack.get("image")[1]), 1)
        // console.log("minus images:")
        // console.table(images)
        // console.log(size(images))

        // tomorrow: try moving [] around just the operator outputs TODO
        // also try uhhh using keys array and id array to just skip assigning something to externalstack? if its null
        // idk
        // see if you can get rejected old autosort


        externalStack.targetA = [image_stack.get("target")[0]? image_stack.get("target")[0]: {"id": []}]
        externalStack.targetB = [image_stack.get("target")[1]? image_stack.get("target")[1]: {"id": []}]
        // images.splice(images.indexOf(image_stack.get("target")[0]), 1)
        // images.splice(images.indexOf(image_stack.get("target")[1]), 1)

        // console.log("minus targets:")
        // console.table(images)

        externalStack.flatfieldA = [image_stack.get("flatfield")[0]? image_stack.get("flatfield")[0]: {"id": []}]
        externalStack.flatfieldB = [image_stack.get("flatfield")[1]? image_stack.get("flatfield")[1]: {"id": []}]
        // images.splice(images.indexOf(image_stack.get("flatfield")[0]), 1)
        // images.splice(images.indexOf(image_stack.get("flatfield")[1]), 1)

        // console.log("minus flatfields:")
        // console.table(images)

        externalStack.darkfieldA = [image_stack.get("darkfield")[0]? image_stack.get("darkfield")[0]: {"id": []}]
        externalStack.darkfieldB = [image_stack.get("darkfield")[1]? image_stack.get("darkfield")[1]: {"id": []}]
        // images.splice(images.indexOf(image_stack.get("darkfield")[0]), 1)
        // images.splice(images.indexOf(image_stack.get("darkfield")[1]), 1)

        var img_stck_keys = ["image", "target", "flatfield", "darkfield"]
        for(let i = 0; i<size(img_stck_keys); i++){
            for(let c = 0; c<2; c++){
                var t = image_stack.get(img_stck_keys[i])[c]
                if(t != null){
                    images.splice(images.indexOf(t), 1)
                }
            }
        }

        console.log("minus splice loops:")
        console.table(images)

        return  images;

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
    export function autoSortImages(images, externalStack) {

        console.log(images)
        console.log(externalStack)
        let includeTarget = false;
        if(size(images) < 6) {
            return images;
        } else if(size(images) >= 8) {
            includeTarget = true;
        }

        // Keeping track of how many file names have a suffix cordoned from the rest of the file name with a - and has a _ separating only 2 parts
        var suffix_score = 0
        each(images, function(image){
            // Discuss with team later how the sort type is determined, TODO
            var img_test_str = split(image.name.toLowerCase(), '-')
            if(img_test_str.length > 1 && split(img_test_str[img_test_str.length - 1], '_').length ==2 ){
                suffix_score = suffix_score += 1
            }
        })
        
        // If the number of valid suffixes is more than half the size of the images array, then it is assumed 
        // that the user is using suffixes on their images. The code then uses the suffix sort;
        if(suffix_score > size(images)/2){
             each(images, function (image){
            each(probabilityScoreProperties, function (property) {
                image[property] = 0;
            });
            image.imageA = false;});

            images = suffixSortImages(images, externalStack);
            console.log("remaining images: ")
            console.table(size(images))

            /**
             * console.log("NEW MOVED STACK:")
                console.table(JSON.stringify(externalStack, null, 4))
            */

            return images;
        }
        // Otherwise, use the legacy sorting (not recommended)
        else{

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

        console.log("trying something")
        console.table(JSON.stringify(externalStack, null, 4))
        console.log("size: " + size(externalStack))
        console.log(typeof(externalStack))

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

        console.log("OLD STACK:")
        console.table(JSON.stringify(externalStack, null, 4))

        console.log("SIZE OF IMAGES: " + size(images))
        console.log(JSON.stringify(images, null, 4))

        // return any images that weren't assigned
        return images;
    }
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
