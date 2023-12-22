<script>
    export let title = undefined;
    /* TODO https://ogp.me */

    const title_ = title ? `${title} - ` : '';
</script>

<svelte:head>
    <title> {title_} Ido Talbi </title>
</svelte:head>


