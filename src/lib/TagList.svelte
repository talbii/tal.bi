<script>
    export let tags;
</script>

<ul>
{#each tags || [] as tag}
    <li>{tag}</li>
{/each}
</ul>

<style>
ul { 
    display: inline;
    padding: 0;
    margin: 0;
}

ul li {
    font-size: 0.8rem;
    display: inline;
    margin-right: 5px;
    padding: 1px 3px;
    border: 1px solid var(--border-color);
    border-radius: 0.25rem;
}
</style>
