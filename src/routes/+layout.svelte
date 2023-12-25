<script>
    import '@fontsource-variable/source-sans-3';
    import '@fontsource-variable/source-code-pro';

    import '$src/app.css'
    import Navbar from '$lib/Navbar.svelte'
</script>

<header>
    <Navbar />
</header>

<main>
    <slot/>
</main>
