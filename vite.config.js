import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'
import path from "path";

// https://vitejs.dev/config/
export default defineConfig({
  base: "./",
  alias: {
    "@components": path.resolve(__dirname, 'src/renderer/components'),
    "@pages": path.resolve(__dirname, 'src/renderer/pages')
  },
  build: {
    outDir: "public"
  },
  plugins: [
    svelte({
      configFile: 'svelte.config.js'
    })
  ]
})
