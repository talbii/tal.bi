<script>
    import PostSummary from '$lib/PostSummary.svelte'
    import Metadata from '$src/lib/Metadata.svelte';
    export let data;
</script>

<Metadata />

{#each data.posts as post}
    <article>
        <PostSummary path={post.path} meta={post.meta} />
    </article>
{/each}
