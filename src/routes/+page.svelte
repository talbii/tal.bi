<script>
    import PostSummary from '$lib/PostSummary.svelte'
    export let data;
</script>

{#each data.posts as post}
    <PostSummary path={post.path} meta={post.meta} />
{/each}
