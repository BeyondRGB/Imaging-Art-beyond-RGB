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
     * Sorts images to the externalStack using a sort that checks the suffix for a keyword and an image identifier that matches the 
     *  other images
     *  - externalStack requires the following fields:
     *      - imageA, imageB
     *      - targetA, targetB
     *      - flatfieldA, flatfieldB
     *      - darkfieldA, darkfieldB
     * 
     * @param images
     * @param externalStack
     * @returns any remaining unsorted images
     */
    export function suffixSortImages(images, externalStack){
        if(size(images) < 6) {
            return images;
        }

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

        //Creating keys for the sorted array as well as the external stack, in order
        var img_stck_keys = ["image", "target", "flatfield", "darkfield"]
        var ext_stck_keys = ["imageA", "imageB", "targetA", "targetB", "flatfieldA", "flatfieldB", "darkfieldA", "darkfieldB"]
        var ext_idx = 0
        // For a loop the length of the sorted array:
        for(let i = 0; i<size(img_stck_keys); i++){
            for(let c = 0; c<2; c++){
                var t = image_stack.get(img_stck_keys[i])[c]
                // If there exists an image at the set key and key'ed array index:
                if(t != null){
                    // Add it to the external stack
                    externalStack[ext_stck_keys[ext_idx+c]] = [t]
                    // Note on how this works: external stack ID goes up in increments of 2
                    // c is always 0 or 1
                    // [name]A is always at even indexes and [name]B is always at odd indexes, so images put into A or B will always match
                    // So the increments in c allow the odd external stack keys to be hit during this loop
                    images.splice(images.indexOf(t), 1)
                }
                else{
                    // If there is no match in the sorted array, set the external stack to have a []
                    externalStack[ext_stck_keys[ext_idx+c]] = []
                    
                }
                if(ext_idx<2){
                        // imageA and imageB require an extra [] around them no matter if it's 
                        externalStack[ext_stck_keys[ext_idx+c]] = [externalStack[ext_stck_keys[ext_idx+c]]]
                    }
            }
            ext_idx+=2
        }
        console.log("SUFFIX AUTOSORT " + JSON.stringify(externalStack, null, 4))

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
            var remaining_images = null
            remaining_images = suffixSortImages(images, externalStack);
            console.log("remaining images: ")
            console.table(size(remaining_images))

            return remaining_images;
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

        console.log("AUTOSORTED:")
        console.table(JSON.stringify(externalStack, null, 4))

        // return any images that weren't assigned
        return images;
    }
}

  /**
     * Sorts images to the externalStack using a sort that checks the suffix for a keyword and an image identifier that matches the 
     *  other images
     *  - externalStack requires the following fields:
     *      - imageA, imageB
     *      - targetA, targetB
     *      - flatfieldA, flatfieldB
     *      - darkfieldA, darkfieldB
     * 
     * @param images
     * @param externalStack
     * @returns any remaining unsorted images
     */
     export function suffixBatchSortImages(images, externalStack){
        console.log("SUFFIX BATCH AUTOSORT")
         // Create a mapping of known image types to the [[]]
         let image_stack = new Map([
            ["target", [null, null]],
            ["image", new Array((size(images) - 6))],
            ["darkfield", [null, null]],
            [ "flatfield", [null, null]]
        ])

        if(size(images) < 6) {
            return images;
        }
        
        // List of endings- 'id's for the image- to the images
        let image_id = []

        // List of filenames for art objects, which functions as an 'id' for the art images to compare to one another
        // Art filenames that don't match one another will not sort
        // 
        let art_filenames = []

        // for IMAGE in IMAGES, 
        each(images, function (image){
            var img_str = split(image.name.toLowerCase(), '-');
            let art_filename = image.name.slice(0, image.name.lastIndexOf('-'))
            // Tear off just the suffix, ignoring all other hyphens in the filename barring the one connected to the suffix (i.e. the last hyphen)
            img_str = img_str[img_str.length - 1];
            // Split into the image type and image id
            let img_parts = split(img_str, '_')

            // img_str becomes the image type, and img_id becomes the image identifier
            img_str = img_parts[0];
            let img_id = img_parts[1];

            // if the ids have not yet been captured and the image id isn't already in image ids, add it in there
            // this could get buggy if there are more than 2 suffixes, either by design or by accident. 
            // Only the first 2 suffixes are recorded in this statement
            if(image_id.length < 2){
                if(indexOf(img_id) == -1){
                    image_id.push(img_id);
                }
            }

            // If the image has the art object type (i.e. is in the first suffixStandards set of names), that there are fewer registered filenames than 
            // there are art objects divided by two (checking to make sure we only capture), and that the filename hasn't already been captured in art_filenames
            if (indexOf(suffixStandards[0], img_str) >= 0 ){
                if(size(art_filenames)<Math.floor((size(images)-6)/2)){
                    if(indexOf(art_filenames, art_filename) == -1){
                    // Add it to art_filenames
                    art_filenames.push(art_filename)
                    }
                }   
            }

            for(let i = 0; i < suffixStandards.length; i++) {
                var ind = indexOf(suffixStandards[i], img_str)
                // If the given suffix is in the array of suffixes, add it to the image stack,
                //  where the key for the image stack belongs to the list of suffixes that the matched suffix is from
                if(ind!= -1){
                    var temp = image_stack.get(suffixStandards[i][0]) 
                    console.log(image)
                    var ar_fl = 0
                    if(suffixStandards[i][0] == "image"){
                        // If this is an "image" type file, then with the batch set the indexes cannot just be 0 and 1, rather 0.... i 
                        // where i is the number of "image" type images in the image set. ar_fl increases the index only if this is an 
                        // "image" type image and the filename has been recorded, IE a set of images exists with that file name. 
                        ar_fl = art_filenames.indexOf(art_filename)*2
                        // ar_fl increases the index by the index of the filename * 2 --> a filename index of 0 would add nothing, so 
                        // the 0 and 1 indexes would be reserved for those, a filename index of 1 would reserve 2 and 3, and so forth.
                        // With this specific indexes are still reserved for specific images, so that the variable number of images 
                        // will not override each other.
                    }
                    temp[image_id.indexOf(img_id) + ar_fl] = image  
                    image_stack.set(suffixStandards[i][0], temp);
                }
            }
        });


        //Creating keys for the sorted array as well as the external stack, in order
        var img_stck_keys = ["target", "flatfield", "darkfield"]
        var ext_stck_keys = ["targetA", "targetB", "flatfieldA", "flatfieldB", "darkfieldA", "darkfieldB"]
        var ext_idx = 0
        // For a loop the length of the sorted array:
        for(let i = 0; i<size(img_stck_keys); i++){
            for(let c = 0; c<2; c++){
                var t = image_stack.get(img_stck_keys[i])[c]
                // If there exists an image at the set key and key'ed array index:
                if(t != null){
                    // Add it to the external stack
                    externalStack[ext_stck_keys[ext_idx+c]] = [t]
                    // Note on how this works: external stack ID goes up in increments of 2
                    // c is always 0 or 1
                    // [name]A is always at even indexes and [name]B is always at odd indexes, so images put into A or B will always match
                    // So the increments in c allow the odd external stack keys to be hit during this loop
                    images.splice(images.indexOf(t), 1)
                }
                else{
                    // If there is no match in the sorted array, set the external stack to have a []
                    externalStack[ext_stck_keys[ext_idx+c]] = []
                    
                }
                //if(ext_idx<2){
                        // imageA and imageB require an extra [] around them no matter if it's 
                      //  externalStack[ext_stck_keys[ext_idx+c]] = [externalStack[ext_stck_keys[ext_idx+c]]]
                  //  }
            }
            ext_idx+=2
        }

        console.log("prog point: ")


        var img_key = "image"
        var img_stk = image_stack.get(img_key)
        var img_ext_stck_keys = ["imageA", "imageB"]
        
        console.table(img_stk)
        
        //Looking back through the rest of the remaining files for the target images -- only has a number of matches up to double the length of
        // art_filenames list 
        for(let i = 0; i<size(image_stack); i++){
            var t = img_stk[i]
            if(t != null){
                // var art_filename = t.name.slice(0, t.name.lastIndexOf('-'))
                // var ar_fl = art_filenames.indexOf(art_filename)*2

                // Add it to the external stack
                externalStack[img_ext_stck_keys[i%2]][Math.floor(i/2)] = [t]
                images.splice(images.indexOf(t), 1)

            }
            else{
                externalStack[img_ext_stck_keys[i%2]][Math.floor(i/2)] = []
            }
        }



        console.log("SUFFIX AUTOSORT " + JSON.stringify(externalStack, null, 4))

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
    export function autoSortBatchImages(images, externalStack) {

        let includeTarget = true;

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
            var remaining_images = null
            remaining_images = suffixBatchSortImages(images, externalStack);
            console.log("remaining images: ")
            console.table(size(remaining_images))

            return remaining_images;
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

        console.log("ext_stack setup: ")
        console.table(externalStack['imageA'])

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

        console.log("BATCH STACK:")
        console.table(JSON.stringify(externalStack, null, 4))

        console.log("SIZE OF IMAGES: " + size(images))
        console.log("leftovers: " + JSON.stringify(leftovers, null, 4))
        console.log("images: " +JSON.stringify(images, null, 4))


        

        // return any images that weren't assigned
        return leftovers;
    }
}

</script>
