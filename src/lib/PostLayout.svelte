<script>
    import Metadata from './Metadata.svelte';
    import PostSummary from './PostSummary.svelte';

    export let date;
    export let title;
    export let tags;
    export let readingTime;
    export let favorite;

    /* ignored yet intentionally defined */
    export let data;
    export let author;
    export let math;
    export let form;

    const meta = { title, date, readingTime, tags, favorite };
</script>

<Metadata {title} />

<PostSummary path={undefined} {meta} />

<article>
    <slot/>
</article>
