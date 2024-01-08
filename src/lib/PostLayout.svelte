<script>
    import Metadata from './Metadata.svelte';
    import PostSummary from './PostSummary.svelte';
    import { dateify } from './util';

    export let date;
    export let title;
    export let tags;
    export let readingTime;
    export let favorite = undefined;
    export let description = undefined;

    const meta = { title, 'date': dateify(date), readingTime, tags, favorite };
</script>

<Metadata {title} {description} />

<article>
    <PostSummary path={undefined} {meta} />
    <slot/>
    <script src="https://giscus.app/client.js"
        data-repo="talbii/tal.bi"
        data-repo-id="R_kgDOGJveng"
        data-category="General"
        data-category-id="DIC_kwDOGJvens4Cb9Vy"
        data-mapping="specific"
        data-term={title}
        data-strict="0"
        data-reactions-enabled="1"
        data-emit-metadata="1"
        data-input-position="top"
        data-theme="dark"
        data-lang="en"
        data-loading="lazy"
        crossorigin="anonymous"
        async>
</script>
</article>
