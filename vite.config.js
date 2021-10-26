import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'

// https://vitejs.dev/config/
export default defineConfig({
  base: "./",
  build:{
    outDir: "public"
  },
  plugins: [
    svelte({
      configFile: 'svelte.config.js'
    })
  ]
})
