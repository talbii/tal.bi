const fetchPosts = async () => {
//	const allPostFiles = import.meta.glob('/src/routes/blog/*.md');
	const allPostFiles = import.meta.glob('/src/routes/blog/**/*.md');
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

	return allPosts;
};

export const load = async () => {
    console.log("load!");
//    console.log(await fetchPosts());
    return { posts: await fetchPosts() }; 
}
