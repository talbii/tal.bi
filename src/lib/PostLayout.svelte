<script>
    import Metadata from './Metadata.svelte';
    import PostSummary from './PostSummary.svelte';
    import { dateify } from './util';

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

    const meta = { title, 'date': dateify(date), readingTime, tags, favorite };
</script>

<Metadata {title} />

<article>
    <PostSummary path={undefined} {meta} />
    <slot/>
</article>
