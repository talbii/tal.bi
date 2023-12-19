<script>
    export let data;
</script>
<h1>Welcome to SvelteKit</h1>
<p>Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation</p>

{#each data.posts as post}
    <a href={post.path}> <h1> {post.meta.title} </h1> </a>
{/each}
