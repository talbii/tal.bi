export const codify = s => {
    return s.replace(/`(.*)`/g, "<code>$1</code>");
}

export const dateify = s => {
    return s.slice(0, 10);
}
