---
author: talbi
date: 2021-10-05
tags: ["ipad"]
---

Everything I use to make programming (semi) doable on my iPad.
<!--more-->

## Preface

Reading this, you might be tempted to buy an iPad strictly for development. If you are in this position, then I warn you and highly encourage you to **not** buy an iPad for this purpose, as programming on this device is barely passable, and all of the issues I faced are due to not having a laptop.

So, if you are thinking about buying an iPad for programming, then... don't. If you already bought an iPad for different purposes (such as note taking), and would like to *also* program on it, then this article is for you.

## Requirements (& base apps I use)

First, the most useful app I found on the App Store has to be [iSH](https://apps.apple.com/us/app/ish-shell/id1436902243) - which is a (Alpine) Linux emulator. This is the app I use the most for local editing.

For SSH <sup>[1]</sup>, you can use [Termius](https://apps.apple.com/il/app/termius-ssh-client/id549039908). Finally, for Markdown editing (or note taking via Markdown syntax), you can use [Bear](https://apps.apple.com/il/app/bear/id1016366447). (though I haven't used it enough to give a fair review)

For file browsing I use Apple's Files app.

<sup>[1]</sup>I will only briefly talk about using SSH for development (i.e. connecting to an external machine), since personally I don't really like this approach, as I don't always have a good enough internet connection, and SSH-ing into a machine defeats the purpose of getting an iPad versus a laptop.

### Notes on iSH

As I mentioned before, iSH is an *emulator*, which means that the programs you run on it will not be directly ran on the CPU, but rather emulated, which means... iSH is **very slow**. I mentioned I use iSH for editing only (using `vim`, `nano`, `emacs`, etc.) as running any compiler/interpreter on even the most basic apps gets extermely slow. Still - it is possible if you don't mind the performance.

## Setting up iSH

After first installing the app, iSH doesn't come with any programming tools. I'd recommend to get fluent with a command-line editor, and to just use that. Additionally, you should also install manpages.  I use `vim`, so to install it write:

```shell
apk add vim man
```

*Note: I found `emacs` to be slower then I'd like.* 

### A Word on `apk`

`apk`, which is Alpine's package manager, is pretty much the way to add/remove programs to your iSH installation. To install (add) packages, use `add`, to remove use `del` and to update use `update`.

For more information, use `man apk`, or just `apk --help`.

### C/C++ Development

To get C/C++ development going, we will need to install a compiler, and the standard library. You may also want to install an assembler, however that is optional. Use the following code to install `gcc`, and the required C/C++ libraries:

```shell
apk add gcc g++ build-base 
```

After install these, you should be able to compile some C/C++ code, after which you will notice just how slow it really gets, which is why I don't recommend buying an iPad for development.

Another few programs you make want to get include `make`, `clang`, and `nasm`, to name a few.

### Python Development (barely)

Install CPython with the package `python3`, and perhaps also `ipython` (which I like for its many features):

```shell
apk add python3 ipython
```

Unfortunately, running any of these interpreters is abnormally slow, and therefore not very useable.

### Rust

Unforunately, Rust development (at the moment) is not supported in iSH (running `rustc` causes an error: "Illegal Instruction").. :(

If, for some reason, you still want to install the Rust compiler, use:

```shell
apk add cargo rustc
```

### Java

To install OpenJDK, first search for the latest version available from your sources. I used:

```shell
$ apk list | grep jdk
openjdk7-jre-7.281.2.6.24-r0 x86 {openjdk7} (GPL-2.0 with Classpath)
openjdk7-jre-lib-7.281.2.6.24-r0 x86 {openjdk7} (GPL-2.0 with Classpath)
openjdk8-8.275.01-r0 x86 {openjdk8} (custom)
openjdk8-doc-8.275.01-r0 x86 {openjdk8} (custom)
openjdk8-jre-base-8.275.01-r0 x86 {openjdk8} (custom)
openjdk7-7.281.2.6.24-r0 x86 {openjdk7} (GPL-2.0 with Classpath)
openjdk8-jre-lib-8.275.01-r0 x86 {openjdk8} (custom)
openjdk7-doc-7.281.2.6.24-r0 x86 {openjdk7} (GPL-2.0 with Classpath)
openjdk7-jre-base-7.281.2.6.24-r0 x86 {openjdk7} (GPL-2.0 with Classpath)
openjdk8-jre-8.275.01-r0 x86 {openjdk8} (custom)
openjdk8-dbg-8.275.01-r0 x86 {openjdk8} (custom)
openjdk8-demos-8.275.01-r0 x86 {openjdk8} (custom)
```

It appears that the latest version currenctly available to me is `openjdk8`, so I'd install that. Perhaps, when you are reading this a newer version is available -- in which case you should install that instead.

*Note: Attempting to install `openjdk8` appears to be crashing iSH every time I try to.*

### HTML + JavaScript

Obviously, writing HTML and JavaScript is simple, but to test them you'd need a way to view your results in a web browser. As such, you may install and host a web server on your iPad. First, install Python:

```shell
apk add python3
```

Then, create a directory `[..]/my_server/`, inside of create a file called `index.html`. After doing so, simply run `python3 http.server` while inside the directory:

```shell
mkdir my_server
cd my_server
"<h1> Hello World! </h1>" >> "index.html"
python3 http.server
```

Your server will be live on `localhost:8000`.

## Developing via SSH/on a network

In this final chapter, I will explore development using an external device, either SSH-ing into a machine or using a third party solution (such as `repl.it`). Personally, I prefer to avoid this solution as I don't always have strong internet connection, and also it seems kind of absurd having to rely on a different device (after all, why use the iPad at all then?).

I will use Termius, to connect to my MacBook Pro (although it should be identical for all computers). After enabling SSH login on your local machine (or server), open the Termius app and add a new host, in which you need to enter the following parameters:

- Hostname (IP of your machine)
- Username (Username you use to login)
- Password (The password you use for `sudo`)

after that, you should be able to SSH into your machine and utilise the power of a normal computer! 

Another service I mentioned is `repl.it`, which is just one of many services which do the same - host VS Code on the cloud. If you'd like to host yourself (and save monthly costs), look at [code-server](https://github.com/cdr/code-server). Recently GitHub has also started to provide this service, which they call GitHub Codespaces.

All these services are great, and will defiantly allow you to run anything, with better performance then doing it locally on iSH, but they often require monthly subscription-based payments (and also an internet connection). I feel like buying a laptop is a better choice then having to pay monthly just to do some basic programming - but thats my opinion.

## Summary

I don't think that (at least right now) programming on an iPad is really doable (sure, you can try), but perhaps in the future Apple will loosen a bit the guidelines, and allow users to take full control of the M1 chips currently supplied in iPads (which can **defiantly** run some hefty stuff).

Currently, the best solution is to just use an external machine, which defeats the purpose of an iPad (where you can just buy a laptop). I will again say that if you are thinking of getting an iPad to use for programming, then I advice you to not get one, as you will regret it.

---

*Written with ❤️ on an iPad*

