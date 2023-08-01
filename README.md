# tal.bi: My Website!

Hi! This repository holds the code for both my homepage ([tal.bi](www.tal.bi)), and my blog ([blog.tal.bi](blog.tal.bi)). Yes, I am aware that this is a very odd design choice, and I'll probably change it some day :)

## Homepage

My homepage is written in raw HTML, with styling done only via CSS. There is a JavaScript file which holds basic code to inject a navigation bar. This is used only to avoid re-typing the bar on each page (not like theres a lot of pages, anyway...).

## Blog

The blog is compiled using [Hugo](https://gohugo.io). I am using the [Anubis](https://github.com/Mitrichius/hugo-theme-anubis) theme.

I enabled math by adding the file [`math.html`](https://github.com/talbii/tal.bi/blob/blog/layouts/partials/math.html) in Hugo's `partials` folder. I use $KaTeX$ - I advise you to do so as well! (MathJax is horribly slow).

## Okay, how does stuff work?

On each commit to a branch (`master` for the homepage and `blog` for the blog), an Amazon Amplify instance is triggered to recompile the website (in the case of the blog, the homepage doesn't require much processing). Afterwards the new website is uploaded to the cloud to be viewed. This process takes about 5 minutes.

## Disclaimer

I do not collect data!! My websites don't store any cookies, don't track your IP or use your information in any way.
