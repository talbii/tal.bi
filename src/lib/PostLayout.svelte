<script>
    import { codify, dateify } from '$lib/util.js';

    export let date;
    export let title;
    export let tags;

    /* ignored yet intentionally defined */
    export let data;
    export let author;
    export let math;
    export let form;
</script>

<p> Post date: {dateify(date)} </p>
<h1 class="half"> {@html codify(title)} </h1>
<p> {tags} </p>

<article>
    <slot/>
</article>
