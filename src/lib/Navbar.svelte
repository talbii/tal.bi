<nav> 
    <a class='head-a' href="/"> <h1> Ido Talbi </h1> </a>
    <div style="flex: 1;"> </div>
    <div> 
        <a href="/">home</a>
        <a href="/blog">blog</a>
        <a href="/contact.html">contact</a>
    </div>
</nav>

<style>
nav {
    border-bottom: 2px solid var(--border-color);
    padding: 5px 0;
/*    padding-bottom: 0; */
    margin: 5px 0;
    display: flex;
    flex-direction: row;
    align-items: flex-end;
}

a {
    display: inline-block;
    text-decoration: none;
    color: unset;
}

.head-a {
    font-family: 'Alegreya', serif;
    font-size: 1.8rem;
    line-height: 1;
}

.head-a:hover {
    color: var(--text);
    text-decoration: underline;
}

a:hover {
    color: var(--color-hover);
}

nav > div {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    gap: 5px;
}
</style>

