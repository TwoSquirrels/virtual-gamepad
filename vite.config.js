import { viteSingleFile } from "vite-plugin-singlefile";
import { createHtmlPlugin } from "vite-plugin-html";
import { compression } from "vite-plugin-compression2";

export default {
  base: process.env.GITHUB_PAGES ? "REPOSITORY_NAME" : "./",
  root: "./src",
  publicDir: "../public",
  build: {
    outDir: "../dist",
    emptyOutDir: true,
    minify: "terser",
  },
  plugins: [
    viteSingleFile(),
    createHtmlPlugin({
      minify: true,
    }),
    compression({
      algorithm: "brotliCompress",
      exclude: [/\.(br)$/, /\.(gz)$/],
      threshold: 0,
      deleteOriginalAssets: false,
    }),
  ],
};
