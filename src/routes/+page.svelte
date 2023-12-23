<script>
    import PostSummary from '$lib/PostSummary.svelte'
    import Metadata from '$src/lib/Metadata.svelte';
    import { time_ago } from '$src/lib/util.js';
    export let data;

    const display_date = (meta) => {
        let newmeta = structuredClone(meta);
        newmeta.date = time_ago(new Date(newmeta.date));

        return newmeta;
    };
</script>

<Metadata />

{#each data.posts as post}
    <article>
        <PostSummary path={post.path} meta={display_date(post.meta)} />
    </article>
{/each}
