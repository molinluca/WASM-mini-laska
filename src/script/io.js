document.body.onload = () => {
    let cells = document.querySelectorAll(".board_cell");
    for (let i=0; i<49; i++) {
        if (i % 2 === 0) {
            let c = cells[i];
            c.addEventListener("click", select);
            c.setAttribute("space", i);
        }
    }
}

function select(e) {
    let attr = e.path[0].getAttribute("content");
    if (attr === null) attr = e.path[1].getAttribute("content");

    alert(attr);
}
