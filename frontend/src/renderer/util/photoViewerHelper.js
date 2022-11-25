
// Author: Elijah Cooper
// z = (x - mix(x)) / (max(x) - min(x)) * 100     (how to scale between 0 and 100)
// the getZoom method returns the ratio of the image's width to the width of the viewport.
// that number has a logarithmic behavior when zooming in.
// so we can simply take the log of that number and scale it between 0 and 100.
export function getZoomPercentage(currentZoom) {
    let logZoom = (currentZoom / 59) * 100
    logZoom = Math.log(logZoom)
    return ((logZoom - 0.52763274) / (4.07168653 - 0.52763274)) * 100
}
