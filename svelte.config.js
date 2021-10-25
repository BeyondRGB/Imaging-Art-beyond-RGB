const sveltePreprocess = require("svelte-preprocess");
const autoprefixer = require("autoprefixer");

module.exports = {
  preprocess: sveltePreprocess({
    postcss: {
      plugins: [autoprefixer],
    },
  }),
};