const colors = require('tailwindcss/colors');

module.exports = {
  content: ['./public/index.html', './src/**/*.svelte'],
  darkMode: 'class', // or 'media' or 'class'
  theme: {
    colors: {
      transparent: 'transparent',
      current: 'currentColor',
      black: colors.black,
      white: colors.white,
      indigo: colors.indigo,
      red: colors.rose,
      blue: colors.blue,
      yellow: colors.amber,
      green: colors.emerald,
      gray: {
        50: '#f2f2f7',
        100: '#e5e5ea',
        200: '#c7c7cc',
        300: '#939394',
        400: '#8e8e93',
        500: '#636366',
        600: '#3a3a3c',
        700: '#2c2c2e',
        800: '#1c1c1e',
        900: '#0e0e0f',
      }
    },
    extend: {},
  },
  plugins: [],
};
