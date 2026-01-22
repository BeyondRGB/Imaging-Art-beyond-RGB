import { defineConfig } from "vite";
import { svelte } from "@sveltejs/vite-plugin-svelte";
import tsconfigPaths from "vite-tsconfig-paths";

// https://vitejs.dev/config/
export default defineConfig({
	base: "./",
	build: {
		outDir: "public",
	},
	plugins: [
		svelte({
			configFile: "svelte.config.js",
		}),
		tsconfigPaths(),
	],
	optimizeDeps: { include: ["@carbon/charts"] },
});
