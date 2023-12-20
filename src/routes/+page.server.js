const fetchPosts = async () => {
	const allPostFiles = import.meta.glob('/src/routes/posts/**/*.md');
	const iterablePostFiles = Object.entries(allPostFiles);

	const allPosts = await Promise.all(
		iterablePostFiles.map(async ([path, resolver]) => {
			const { metadata } = await resolver();
			const postPath = path.slice(11, -8);

			return {
				meta: metadata,
				path: postPath
			};
		})
	);

	return { posts: allPosts };
};

export const load = fetchPosts;
