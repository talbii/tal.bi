<script>
    import { FontAwesomeIcon } from '@fortawesome/svelte-fontawesome';
    import { faCopy } from "@fortawesome/free-regular-svg-icons";

    export let node;
    const text = node.textContent;
</script>

<button on:click={() => console.log(`copy: ${text}`)}>
    <FontAwesomeIcon icon={faCopy}/>
</button>
