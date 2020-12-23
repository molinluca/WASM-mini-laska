/** @module util/worker */
import * as util from "./types.mjs"
const INT = 4;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Questa funzione accede agli elementi in html che compongono la scacchiera ed aggiunge i vari dischetti colorati che compongono
 * gli strati delle torri nelle posizioni corrispondenti, prendendosi particolarmente cura di evidenziare le torri che hanno la pedina in testa
 * promossa
 * @param {function} get_piece la funzione (proveniente dal motore di gioco) per recuperare il puntatore alla pedina (in formato buffer)
 * @param {object} mem il contesto di memoria, per poter accedere al valore puntato della pedina
 */
export function fill(get_piece, mem) {
   clear();
   for (let i=0; i<22; i++) {

      let ptr = get_piece(i);

      let y = mem.getInt(ptr);
      let x = mem.getInt(ptr + INT);
      let tower = [mem.getInt(ptr + INT*2), mem.getInt(ptr + INT*3), mem.getInt(ptr + INT*4)];

      let cell = document.querySelectorAll(".board_cell")[y*7 + x];
      if (!cell) continue;

      cell.setAttribute("piece", i);

      for (let j=0; j<3; j++) {
         if (tower[j] === util.CPU || tower[j] === util.PROMOTED_CPU)      { cell.innerHTML += '<div class="piece CPU"></div>'; }
         else if (tower[j] === util.USR || tower[j] === util.PROMOTED_USR) { cell.innerHTML += '<div class="piece USR"></div>'; }

         if (!(tower[j] % 2) && j === 0) cell.classList.add("promoted");
         else if (j === 0) cell.classList.remove("promoted");
      }
   }
}

/**
 * Concentra l'attenzione del giocatore sulla pedina selezionata, cambiando l'aspetto grafico della scacchiera.
 * In particolare mette fuori fuoco le altre celle, eccetto quelle delle mosse possibili che vengono evidenziate
 * @param {any} piece L'indice della pedina [0-21]
 * @param {any} moves Le mosse (posizioni [0-48]) possibili alla pedina
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

   document.querySelector(`.board_cell[piece="${piece}"]`).setAttribute("main", piece);
   document.querySelector("table").classList.add("choice");
   if (!moves.length) unfocus();
}

/**
 * Ripristina lo stato originale della scacchiera rimettendo a fuoco tutte le celle e rimuovendo l'evidenziazione
 * delle possibili mosse della pedina selezionata precedentemente. Sia che sia stata mossa che sia stata deselezionata
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
 * Mostra un piccolo popup, sfuocando il resto dell'interfaccia, che mostra all'utente lo stato finale del gioco.
 * In particolare mostra il vincitore e spiega il motivo per cui l'altro ha perso
 * @param {number} loser Il team che ha perso il match
 * @param {boolean} has_pieces_left (boolean) Un indicatore per capire se ha ancora pedine in gioco
 */
export function show_end_state(loser, has_pieces_left) {
   let text = "The match concluded in a <span>tie</span>... No player has <span>legal moves</span> left";
   document.querySelector(".container").classList.add("game-over");
   document.querySelector("#end-state").removeAttribute("blue");
   document.querySelector("#end-state").removeAttribute("red");

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

/**
 * Cambia il colore ed il testo dell'etichetta che evidenzia quale team deve giocare il turno
 * L'etichetta ha visibilita' solo nella modalita' Giocatore vs Giocatore (PvP)
 * @param {number} team Il team che deve giocare il turno
 */
export function change_team_tag(team) {
   if (isNaN(team)) throw new Error("The 'team' parameter MUST BE A NUMBER!!");
   let tag = document.querySelector(".game-turn");

   if (team === util.CPU_TEAM) {
      tag.classList.add("red");
      tag.classList.remove("blue");
      tag.innerHTML = "<b>RED TEAM</b> turn"
   }

   else {
      tag.classList.add("blue");
      tag.classList.remove("red");
      tag.innerHTML = "<b>BLUE TEAM</b> turn"
   }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Svuota ogni cella della scacchiera, in questo caso interpretata come una `flattened matrix` per ripristinare le impostazioni iniziali.
 * Rappresenta il modo piu' rapido per svuotare il contenuto della scacchiera prima di effettuare un nuovo riempimento o prima di creare una nuova sessione pulita di gioco
 */
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

