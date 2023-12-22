<script>
    import { codify, dateify } from '$lib/util.js';
    import TagList from '$lib/TagList.svelte';

    export let path;
    export let meta;
</script>

<article>
<div>
    <a href={path}>
        <h1 class="half"> {@html codify(meta.title)} </h1>
    </a>
    <br>

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
    border-bottom: 1px solid black;
    /*    border-image: linear-gradient(
        to right,
        transparent 1%,
        black 1% 99%,
        transparent 99%
        ) 100% 1;*/
    margin: 0 5px;
    padding: 7px;
}

    h1 {
        display: inline-block;
}

a {
    text-decoration: none;
    }
    a:hover { text-decoration: none; }
</style>
