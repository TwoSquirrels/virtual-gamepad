import { viteSingleFile } from "vite-plugin-singlefile";

export default {
  base: process.env.GITHUB_PAGES ? "REPOSITORY_NAME" : "./",
  root: "./src",
  publicDir: "../public",
  build: {
    outDir: "../dist",
    emptyOutDir: true,
  },
  plugins: [viteSingleFile()],
};
