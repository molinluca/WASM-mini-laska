import * as util from "./types.mjs"
const INT = 4;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Riempio la scacchiera con le pedine nelle celle corrispondenti
 * @param get_piece la funzione per recuperare il puntatore alla pedina
 * @param mem il contesto di memoria, per poter accedere al valore puntato della pedina (quella in formato buffer)
 */
export function fill(get_piece, mem) {
   clear();
   for (let i=0; i<22; i++) {

      /* Chiamo get_piece() per recuperare il puntatore alla pedina
       * Poi si usa la funzione mem.getInt() per prendere il dato corretto dalla pedina "bufferizzata" */
      let ptr = get_piece(i);

      /* Prendo le coordinate e il contenuto della torre dalla pedina "bufferizzata" */
      let y = mem.getInt(ptr);
      let x = mem.getInt(ptr + INT);
      let tower = [mem.getInt(ptr + INT*2), mem.getInt(ptr + INT*3), mem.getInt(ptr + INT*4)];

      /* Recupero la cella della scacchiera (in base alle coordinate) */
      let cell = document.querySelectorAll(".board_cell")[y*7 + x];
      if (!cell) continue;

      cell.setAttribute("piece", i);

      /* Inserisco i dischetti colorati nella giusta cella della scacchiera */
      for (let j=0; j<3; j++) {
         if (tower[j] === util.CPU || tower[j] === util.PROMOTED_CPU)      { cell.innerHTML += '<div class="piece CPU"></div>'; }
         else if (tower[j] === util.USR || tower[j] === util.PROMOTED_USR) { cell.innerHTML += '<div class="piece USR"></div>'; }

         /* Controllo che la testa della torre sia promossa.. */
         if (!(tower[j] % 2) && j === 0) cell.classList.add("promoted");
         else if (j === 0) cell.classList.remove("promoted");
      }
   }
}

/**
 * Concentra l'attenzione del giocatore sulla pedina selezionata, cambiando l'aspetto grafico della scacchiera
 * @param piece L'indice della pedina [0-21]
 * @param moves Le mosse (posizioni [0-48]) possibili alla pedina
 * @throws Error(...) se l'indice della pedina non e` numerico o se e` fuori dal range [0-21]
 */
export function focus(moves, piece) {
   if (piece === undefined || piece === null) throw new Error("Indice della pedina non definito");
   if (isNaN(piece))                          throw new Error("L'indice della pedina deve essere un numero");
   if (piece < 0 || piece > 21)               throw new Error("L'indice della pedina deve essere compreso fra [0-21]. { piece:"+piece+" }");

   let found = false;
   for (let i=0; i<moves.length; i++) {
      if (moves[i] < 0 || moves[i] > 48) continue;

      let target = document.querySelectorAll(".board_cell")[moves[i]];
      if (!!target) {
         found++;
         target.setAttribute("move", i);
      }

   }

   document.querySelector(`.board_cell[piece="${piece}"]`).setAttribute("main", this.piece);
   document.querySelector("table").classList.add("choice");
   if (!moves.length) unfocus();
}

/**
 * Ripristina lo stato corretto di visualizzazione della scacchiera (dopo una mossa o una deselezione)
 */
export function unfocus() {
   document.querySelector("table").classList.remove("choice");
   for (let i=0; i<49; i++) {
      let cell = document.querySelectorAll(".board_cell")[i];
      cell.removeAttribute("move");
      cell.removeAttribute("main");
   }
}

/**
 * Mostra un messaggio a video che spiega lo stato finale del gioco (chi ha vinto e come)
 * @param loser Il team che ha perso il match
 * @param has_pieces_left (boolean) Un indicatore per capire se ha ancora pedine in gioco
 */
export function show_end_state(loser, has_pieces_left) {
   let text = "The match concluded in a <span>tie</span>... No player has <span>legal moves</span> left";
   document.querySelector(".container").classList.add("game-over");

   if (loser === util.CPU_TEAM) {
      if (has_pieces_left) text = "The <span>BLUE PLAYER</span> won the match... The other player has <span>no legal moves</span> possible"
      else                 text = "The <span>BLUE PLAYER</span> won the match... The other player has <span>no pieces left</span>"
      document.querySelector("#end-state").setAttribute("blue", "true");
   }

   else if (loser === util.USR_TEAM) {
      if (has_pieces_left) text = "The <span>RED PLAYER</span> won the match... The other player has <span>no legal moves</span> possible"
      else                 text = "The <span>RED PLAYER</span> won the match... The other player has <span>no pieces left</span>"
      document.querySelector("#end-state").setAttribute("red", "true");
   }

   document.querySelector("#end-state .message .text").innerHTML = text;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Ad ogni ciclo prendo la cella corrispondente
 * Poi la svuoto da ogni proprieta` che e` stata settata precedentemente */
function clear() {
   for (let i=0; i<49; i++) {
      let cell = document.querySelectorAll(".board_cell")[i];
      cell.title = "";
      cell.classList.remove("promoted");
      cell.innerHTML = "";
      cell.removeAttribute("piece");
      cell.removeAttribute("move");
   }
}

