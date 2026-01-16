/**
 * CSV Export Utilities
 * Centralized CSV generation and download functions
 */

/**
 * Creates CSV content from spectral line chart data
 * @param {Array} inputData - Array of spectral line data objects
 * @param {Array} wavelengthArray - Array of wavelength values
 * @returns {string} CSV content string
 */
export function createSpectralCSVContent(inputData, wavelengthArray) {
	let csvContent = "data:text/csv;charset=utf-8,";
	const delimiter = ",";

	// Add column headers as first row
	csvContent += "wavelength,";
	inputData.forEach(spectralLine => {
		csvContent += spectralLine.name + delimiter;
	});
	csvContent += "\r\n";

	// For each row of wavelength values
	for (let i = 0; i < wavelengthArray.length; i++) {
		// Add the wavelength value first
		csvContent += wavelengthArray[i].toString() + delimiter;

		// Then add each spectral line's percentage value at that wavelength
		inputData.forEach(spectralLine => {
			// Do not want to assume spectralLine.data is in increasing order of wavelength
			// so filter the array for the current wavelength value
			let filteredArray = spectralLine.data.filter(item => {
				return item.x == wavelengthArray[i];
			});
			// Assume filteredArray has one object in it
			// Divide y value by 100, so value will be between 0 and 1
			csvContent += filteredArray[0].y / 100 + delimiter;
		});

		csvContent += "\r\n";
	}

	return csvContent;
}

/**
 * Creates CSV content from heatmap data
 * @param {Array} mapData - 2D array of heatmap values
 * @param {number|null} p90Value - 90th percentile value
 * @returns {string} CSV content string
 */
export function createHeatmapCSVContent(mapData, p90Value = null) {
	// Reverse without mutating because it's upside down
	const flipped = [...mapData].reverse();

	// Construct CSV by hand
	let csv = '"Row","Col","Value"\r\n';
	for (let i = 0; i < flipped.length; i++) {
		for (let j = 0; j < flipped[i].length; j++) {
			csv += [String(i + 1), String.fromCharCode(j + 65), flipped[i][j]]
				.map(v => `"${v}"`)
				.join(",");
			csv += "\r\n";
		}
	}

	if (p90Value !== null) {
		csv += `\r\n"Computed 90th Percentile","${p90Value.toFixed(2)}"\r\n`;
	}

	return csv;
}

/**
 * Downloads CSV content as a file
 * @param {string} csvContent - The CSV content to download
 * @param {string} filename - The filename for the download
 */
export function downloadCSV(csvContent, filename = "export.csv") {
	const encodedUri = encodeURI(csvContent);
	const link = document.createElement("a");
	link.setAttribute("href", encodedUri);
	link.setAttribute("download", filename);
	link.click();
}

/**
 * Convenience function to create and download spectral CSV
 * @param {Array} inputData - Array of spectral line data objects
 * @param {Array} wavelengthArray - Array of wavelength values
 * @param {string} filename - The filename for the download
 */
export function exportSpectralCSV(inputData, wavelengthArray, filename = "SpectralLineChart.csv") {
	const csvContent = createSpectralCSVContent(inputData, wavelengthArray);
	downloadCSV(csvContent, filename);
}

/**
 * Convenience function to create and download heatmap CSV
 * @param {Array} mapData - 2D array of heatmap values
 * @param {number|null} p90Value - 90th percentile value
 * @param {string} filename - The filename for the download
 */
export function exportHeatmapCSV(mapData, p90Value = null, filename = "calibrationReport.csv") {
	const csvContent = createHeatmapCSVContent(mapData, p90Value);
	downloadCSV(csvContent, filename);
}
