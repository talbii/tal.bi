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

const strcmp = (s1, s2) => {
    if (s1 < s2) return -1;
    if (s1 > s2) return 1;
    return 0;
};

export const load = async () => {
    return fetchPosts().then(x => {
        let y = x.posts;
        y.sort((a, b) => strcmp(a.meta.date, b.meta.date))
         .reverse();

        return { posts: y };
    });
};
