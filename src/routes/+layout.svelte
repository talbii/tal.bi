<script>
import '$src/app.css'
</script>

<nav> 
    <h1> Ido Talbi </h1>
    <div> 
        <a href="/">home</a>
        <a href="/blog">blog</a>
        <a href="/contact.html">contact</a>
    </div>
</nav>

<slot/>

<style>
nav {
    border-bottom: 2px solid black; 
    padding: 5px 0;
/*    padding-bottom: 0; */
    margin: 5px 0;
    display: flex;
    flex-direction: row;
    align-items: flex-end;
}

nav > h1 {
    font-family: 'Alegreya', serif;
    font-size: 2rem;
    display: inline-block;
    flex: 1;
}

nav > * a {
    text-decoration: none;
}
</style>

