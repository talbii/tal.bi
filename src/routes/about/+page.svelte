<script>
    import Metadata from "$src/lib/Metadata.svelte";
    import { FontAwesomeIcon } from '@fortawesome/svelte-fontawesome'
    import { faEnvelope } from '@fortawesome/free-regular-svg-icons'
    import { faGithub } from '@fortawesome/free-brands-svg-icons';
</script>

<Metadata title={'About'}/>

<div>
    <p> Hello! I'm <em>Ido</em>. I have completed a Bachelor's in computer science in the year 2023, and I am an Etgar alumnus at University of Haifa. </p>
    <p> Main intrests are in the mathematics and theoretical computer science realms. <i>Currently</i>, these include number theory, group theory, compiler optimization, complexity, and graph theory. </p>
    <p> My <a href="/"> blog </a> (originally hosted at <u>blog.tal.bi</u>) discusses said topics, giving new or interesting insights. </p>

    <br>

    <p> You can reach me in the following ways: </p>
    <ul>
        <li> <span> <FontAwesomeIcon icon={faEnvelope} /> </span> <a href="mailto:contact@tal.bi"> contact@tal.bi </a></li>
        <li> <span> <FontAwesomeIcon icon={faGithub} /> </span> <a href="https://github.com/talbii"> talbii </a> </li>
    </ul>

</div>

<style>
    div {
        display: flex;
        flex-direction: column;
        gap: 0.75rem;
    }

    div * {
        margin: 0;
    }

    ul {
        list-style: none;
        margin: 0 5px;
        padding: 0;
    }

    span {
        vertical-align: middle;
    }
</style>

