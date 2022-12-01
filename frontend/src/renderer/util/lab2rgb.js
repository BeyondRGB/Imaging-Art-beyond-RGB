// @Drink whiskey (will make this more understandable)
// Author: Patrick Dalesio
// https://github.com/antimatter15/rgb-lab/blob/master/color.js
export function lab2rgb(lab) {
    let y = (lab[0] + 16) / 116;
    let x = lab[1] / 500 + y;
    let z = y - lab[2] / 200;
    let r, g, b;

    x = 0.95047 * (x**3 > 0.008856 ? x**3 : (x - 16 / 116) / 7.787);
    y = y**3 > 0.008856 ? y**3 : (y - 16 / 116) / 7.787;
    z = 1.08883 * (z**3 > 0.008856 ? z**3 : (z - 16 / 116) / 7.787);

    r = x * 3.2406 + y * -1.5372 + z * -0.4986;
    g = x * -0.9689 + y * 1.8758 + z * 0.0415;
    b = x * 0.0557 + y * -0.204 + z * 1.057;

    r = r > 0.0031308 ? 1.055 * Math.pow(r, 1 / 2.4) - 0.055 : 12.92 * r;
    g = g > 0.0031308 ? 1.055 * Math.pow(g, 1 / 2.4) - 0.055 : 12.92 * g;
    b = b > 0.0031308 ? 1.055 * Math.pow(b, 1 / 2.4) - 0.055 : 12.92 * b;

    return [
        Math.max(0, Math.min(1, r)) * 255,
        Math.max(0, Math.min(1, g)) * 255,
        Math.max(0, Math.min(1, b)) * 255,
    ];
}
