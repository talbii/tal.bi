<script>
    import '$src/app.css'
    import Navbar from '$lib/Navbar.svelte'
    import CodeblockCopyButton from '$lib/CodeblockCopyButton.svelte';
    import { afterNavigate } from '$app/navigation';

    afterNavigate(() => {
        for (const node of document.querySelectorAll("pre > code")) {
            new CodeblockCopyButton({
                target: node,
                props: {node},
            })
        }
    });
</script>

<header>
    <Navbar />
</header>

<main>
    <slot/>
</main>
