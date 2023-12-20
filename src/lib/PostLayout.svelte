<script>
    export let date;
    export let title;
</script>

<script context="module">
  export let date;
  export let title;

  export let metadata = {
    'title': title,
    'date': date,
  };
</script>

<p> Post date: {date} </p>
<h1> {title} </h1>

<slot/>
