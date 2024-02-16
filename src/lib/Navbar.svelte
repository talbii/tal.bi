<nav> 
    <a href="/"> <h1 class='head-a'> Ido Talbi </h1> </a>
    <div style="flex: 1;"> </div>
    <div> 
        <a href="/">home</a>
        <a href="/about">about</a>
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

h1 {
    line-height: 1;
}

.head-a {
    font-family: "Source Sans 3", serif;
    font-weight: 700;
    font-size: 2em;
    line-height: 1;
}

.head-a:hover {
    color: var(--text);
    text-decoration: underline;
}

a:hover {
    color: var(--color-link);
}

nav > div {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    gap: 5px;
}
</style>

