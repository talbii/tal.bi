<script>
    import { codify, dateify } from '$lib/util.js';
    import TagList from '$lib/TagList.svelte';

    export let path;
    export let meta;

    const make_title = () => {
        return `<h1 class="half"> ${codify(meta.title)} </h1>`;
    };
</script>

<article>
<div>
    {#if path}
        <a href={path}>
            {@html make_title()}
        </a>
    {:else}
        {@html make_title()}
    {/if}

    <p>
        {dateify(meta.date)}
        &centerdot;
        {meta.readingTime.text}
    </p>

    <TagList tags={meta.tags} />
</div>
</article>

<style>
div {
    border-bottom: 1px solid var(--border-color);
    /*    border-image: linear-gradient(
        to right,
        transparent 1%,
        black 1% 99%,
        transparent 99%
        ) 100% 1;*/
    margin: 0 5px;
    margin-bottom: 5px;
    padding: 7px;
}

h1, p, a {
    display: inline-block;
    text-align: unset;
}

a {
    color: inherit;
    text-decoration: none;
}

a:hover * {
    color: inherit;
    text-decoration: underline;
}
</style>
