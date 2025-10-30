import { defineConfig } from 'vite';
import { svelte } from '@sveltejs/vite-plugin-svelte';
import tsconfigPaths from 'vite-tsconfig-paths';
import istanbul from 'vite-plugin-istanbul';

// https://vitejs.dev/config/
export default defineConfig({
  base: "./",
  build: {
    outDir: "public"
  },
  plugins: [
    svelte({
      configFile: 'svelte.config.js'
    }),
    tsconfigPaths(),
    // Enable coverage instrumentation when COVERAGE env var is set
    ...(process.env.COVERAGE === 'true' ? [
      istanbul({
        include: 'src/*',
        exclude: ['node_modules', 'test'],
        extension: ['.js', '.ts', '.svelte'],
      })
    ] : []),
  ],
  optimizeDeps: { include: ['@carbon/charts'] },
  server: {
    port: 3000
  }
});
