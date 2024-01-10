<script>
    export let title = undefined;
    export let description = undefined;
    /* TODO https://ogp.me */

    const title_ = title ? `${title} - ` : '';
    const description_ = description || (title ? `A blog post about ${title}` : '');
</script>

<svelte:head>
    <title> {title_} Ido Talbi </title>
    <meta name="description" content={description_ || "Ido Talbi's website."}>
</svelte:head>


