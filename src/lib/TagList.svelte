<script>
    export let tags;
</script>

<ul>
{#each tags || [] as tag}
    <li> #{tag} </li> 
{/each}
</ul>

<style>
ul { 
    padding: 0;
    margin: 0;
}

ul li {
    font-size: 0.8rem;
    display: inline;
    padding-right: 3px;
}
</style>
