import { defineConfig } from "vite";
import { svelte } from "@sveltejs/vite-plugin-svelte";
import tsconfigPaths from "vite-tsconfig-paths";

// https://vitejs.dev/config/
export default defineConfig({
	base: "./",
	// build: {
	// 	outDir: "public",
	// },
	plugins: [
		svelte({
			configFile: "svelte.config.js",
		}),
		tsconfigPaths(),
	],
	// optimizeDeps: { include: ["@carbon/charts"] },
	  // Dev-time prebundle (this is where your error is happening)
	optimizeDeps: {
		esbuildOptions: {
		target: "es2020",        // or "chrome94"
		supported: { bigint: true }
		}
	},

	// Build-time output
	build: {
		outDir: "public",
		target: "es2020"           // or "chrome94"
	},

	// Optional: also align esbuild’s transform target
	esbuild: {
		target: "es2020"           // or "chrome94"
	}

});
