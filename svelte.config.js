const sveltePreprocess = require("svelte-preprocess");
const autoprefixer = require("autoprefixer");
const tailwindcss = require("tailwindcss");

module.exports = {
  preprocess: sveltePreprocess({
    postcss: {
      plugins: [
        autoprefixer,
        tailwindcss
      ],
    },
  }),
};