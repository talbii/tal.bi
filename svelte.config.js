import adapter from '@sveltejs/adapter-auto';
import { vitePreprocess } from '@sveltejs/vite-plugin-svelte';
import { mdsvex } from 'mdsvex'

import remarkMath from 'remark-math'
import rehypeKatexSvelte from 'rehype-katex-svelte'

/** @type {import('@sveltejs/kit').Config} */
const config = {
	// Consult https://kit.svelte.dev/docs/integrations#preprocessors
	// for more information about preprocessors
    extensions: ['.svelte', '.md', '.svx'],
    preprocess: [
        vitePreprocess(), 
        mdsvex({ 
            extensions: ['.md'],
            highlight: {
                alias: {  "c++" : 'cpp', },
            },
            remarkPlugins: [remarkMath],
            rehypePlugins: [rehypeKatexSvelte],
        }),
    ],

	kit: {
		// adapter-auto only supports some environments, see https://kit.svelte.dev/docs/adapter-auto for a list.
		// If your environment is not supported or you settled on a specific environment, switch out the adapter.
		// See https://kit.svelte.dev/docs/adapters for more information about adapters.
		adapter: adapter(),
        alias: {
            $src: 'src/',
        }
	}
};

export default config;
