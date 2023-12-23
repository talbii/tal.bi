<script>
    import { codify, time_ago } from '$lib/util.js';
    import TagList from '$lib/TagList.svelte';
    import { FontAwesomeIcon } from '@fortawesome/svelte-fontawesome'
    import { faStar } from '@fortawesome/free-regular-svg-icons';

    export let path;
    export let meta;

    const title = codify(meta.title);
</script>

<div>
    {#if path}
        <a href={path}>
            <h1 class="half"> {@html title} </h1>
        </a>
    {:else}
        <h1 class="half"> {@html title} </h1>
    {/if}

    <p>
        {meta.date}
        &centerdot;
        {meta.readingTime.text}
    </p>

    <br>

    {#if meta.favorite}
        <span style="vertical-align: middle;">
            <FontAwesomeIcon icon={faStar} />
        </span>
    {/if}
    <TagList tags={meta.tags} />
</div>

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

h1 {
    display: block;
}

p {
    display: inline;
}

a {
    color: inherit;
    text-decoration: none;
    display: block;
    margin: 0;
    width: fit-content;
}

a:hover h1 {
    color: inherit;
    text-decoration: underline;
}
</style>
