<script>
    import { codify, dateify } from '$lib/util.js';

    export let path;
    export let meta;
</script>

<div>
    <a href={path}>
        <h1 class="half"> {@html codify(meta.title)} </h1>
    </a>
    <p> {dateify(meta.date)} &centerdot; {meta.readingTime.text} </p>

<!--
    <ul>
    {#each meta.tags || [] as tag}
        <li> #{tag} </li>    
    {/each}
</ul>
-->

</div>

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
    padding: 5px;
}

    h1 {
        display: inline-block;
}

a {
    text-decoration: none;
    }
    a:hover { text-decoration: none; }

ul { 
    padding: 0;
    margin: 0;
    margin-bottom: 5px;
}

ul li {
    font-size: 0.9rem;
    display: inline;
    padding-right: 3px;
}
</style>
