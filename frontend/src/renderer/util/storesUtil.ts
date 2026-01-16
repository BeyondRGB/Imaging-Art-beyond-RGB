import { isEmpty } from "lodash";

//Expects $processState?.artStacks[0].fields as input parameter; imageStack = get($processState, 'artStacks[0].fields');
//go through $processState?.artStacks[0].fields and get an accurate count of the sorted image
export function countFields(imageStack) {
	let sortedImagecount = 0;

	//field is a string
	for (var field in imageStack) {
		//imageA and imageB are 2d arrays
		if (field == "imageA" || field == "imageB") {
			for (let i = 0; i < imageStack[field].length; i++) {
				// imageA and imageB can have an arbitrary length of empty arrays
				// so for each nested array check they are not empty
				if (!isEmpty(imageStack[field][i])) {
					sortedImagecount += imageStack[field][i].length;
				}
			}
		} else {
			//other fields are 1d arrays
			sortedImagecount += imageStack[field].length; //these lengths should always be 1
		}
	}
	return sortedImagecount;
}
