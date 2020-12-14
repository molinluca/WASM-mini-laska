const wasm = require("webassembly");
import * as util   from "./types.mjs"
import * as worker from "./worker.mjs";

const INT  = 4;
const self = {
   mem: undefined,
   link: undefined,
   game: {
      type: util.STATE_GAME_NONE,
      turn: undefined,
      is_ended: false,
      focus: undefined
   }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Carica il file compilato da ANSI C in WASM e prepara il motore di gioco
 * @param bin il percorso del file .wasm
 * @returns {Promise<void>} Non ha ritorno, ma la sua esecuzione deve essere gestita in maniera asincrona
 * @throws {Error} Se il file compilato del motore di gioco e` incompatibile o corrotto (o non esiste)
 */
export async function load(bin) {
   let module = await wasm.load(bin);
   if (!module.memory || !module.exports) throw new Error("Binary file not loaded correctly, is missing some crucial parts [memory, exports]");

   self.mem  = module.memory;
   self.link = module.exports;

   for (let i=0; i<49; i++) {
      if (i%2) continue;
      document.querySelectorAll(".board_cell")[i].addEventListener("click", handle_click);
   }
}

/**
 * Crea un'istanza di gioco e la mostra a video per lasciar giocare l'utente
 * @param type Tipologia del gioco
 * @throws {Error} Se non e` stato caricato nessun motore di gioco
 */
export function create_game(type) {
   if (!self.mem || !self.link) throw new Error("Engine not loaded yet... You must LOAD the binaries first!!!");
   switch (type) {
      case util.STATE_GAME_PVP:
      case util.STATE_GAME_PVE_EASY:
      case util.STATE_GAME_PVE_MED:
      case util.STATE_GAME_PVE_HARD:
         self.link.new_game(type);
         self.game.type     = self.link.get_status();
         self.game.turn     = self.link.get_turn();
         self.game.is_ended = false;
         worker.fill(self.link.get_piece, self.mem);
         showGame();
         break;

      default: throw new Error("Invalid game type... ("+type+")");
   }
}

/**
 * Distrugge la sessione di gioco (anche se non e` terminata)
 */
export function dispose_game() {
   self.game.type     = util.STATE_GAME_NONE;
   self.game.turn     = undefined;
   self.game.is_ended = false;
   self.link.end_game();

   showMenu();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Aggiorna lo stato del gioco a quello corrente, dopo una mossa da parte di uno dei 2 giocatori
 * @param play_res Risultato della mossa
 */
function update(play_res) {
   self.game.type = play_res;

   switch (play_res) {
      case util.STATE_USR_NO_MOVES:
         worker.show_end_state(util.USR_TEAM, true);
         break;
      case util.STATE_CPU_NO_MOVES:
         worker.show_end_state(util.CPU_TEAM, true);
         break;
      case util.STATE_USR_NO_PIECES:
         worker.show_end_state(util.USR_TEAM, false);
         break;
      case util.STATE_CPU_NO_PIECES:
         worker.show_end_state(util.CPU_TEAM, false);
         break;

      default: return;
   }

   self.game.is_ended = true;
}

/**
 * Chiama l'esecuzione sincrona del movimento della CPU, dopo aver atteso un piccolo delay (per l'animazione grafica)
 * @returns {Promise<void>} Nessun ritorno, ma e` meglio gestire l'esecuzione in maniera asincrona
 */
async function await_CPU() {
   document.querySelector("table").classList.add("choice");
   await util.sleep(100);

   let played = self.link.play_CPU();
   if (played) {
      worker.fill(self.link.get_piece, self.mem);
      update(played);
   }

   worker.unfocus();
   self.game.focus = undefined;
}

/**
 * Fa giocare un team, muovendo una delle sue pedine
 * @param focus La pedina selezionata
 * @param move La direzione
 * @returns {Promise<void>} Nessun ritorno, ma e` meglio gestire l'esecuzione in maniera asincrona
 * @throws {Error} Se il gioco e` gia` concluso o se i valori di pedina e direzione sono incorretti
 */
async function play(focus, move) {
   if (self.game.is_ended) throw new Error("Cannot move if the game is ended!!!");
   if (isNaN(focus) || isNaN(move)) throw new Error("Invalid data to play [ "+focus+", "+move+" ]");
   let result = self.link.do_move(focus, move);

   if (result) {
      worker.fill(self.link.get_piece, self.mem);
      worker.unfocus();
      self.game.focus = undefined;

      update(result);

      if      (self.game.is_ended)                          return;
      if      (self.game.type === util.STATE_GAME_PVE_EASY) await await_CPU();
      else if (self.game.type === util.STATE_GAME_PVE_MED)  await await_CPU();
      else if (self.game.type === util.STATE_GAME_PVE_HARD) await await_CPU();
   }
}

/**
 * Gestisce la pressione di una cella (dal mouse) in modo da concentrare l'attenzione su una pedina da muovere o in modo da muovere la stessa
 * @param piece La pedina da selezionare (non deve per forza essere definita)
 * @param move La direzione della mossa (non deve per forza essere definita)
 */
async function action({piece, move}) {
   if (piece !== null || move !== null) {
      if (self.game.focus === undefined) {
         self.game.focus = piece;
         load_moves(piece);
      }

      else if (move !== null) {
         await play(self.game.focus, move);
      }

      else if (self.game.focus === piece) {
         self.game.focus = undefined;
         worker.unfocus();
      }
   }
}

/**
 * Gestisce la pressione del mouse recuperando dalla cella premuta i suoi attributi (pedina || direzione)
 * @param e L'elemento cliccato
 * @throws {Error} Se nessuna sessione di gioco e` in corso
 */
async function handle_click(e) {
   if (self.game.type === util.STATE_GAME_NONE) throw new Error("No game loaded yet!!!");

   let piece = e.path[0].getAttribute("piece");
   let move  = e.path[0].getAttribute("move");

   if (piece === null) piece = e.path[1].getAttribute("piece");
   if (move  === null) move  = e.path[1].getAttribute("move");

   await action({piece, move});
}

/**
 * Carica le mosse di una specifica pedina per poi mostrarle a video
 * @param piece La pedina di cui calcolare le mosse
 * @note Se la pedina non puo` essere mossa, o non ci si trova nel turno adeguato, non fa nulla
 * @throws {Error} Se il valore della pedina non e` corretto
 */
function load_moves(piece) {
   if (piece === undefined || piece === null) throw Error("Undefined piece");
   if (isNaN(piece)) throw Error("Invalid piece");
   if (piece < 0 || piece > 21) throw Error("Invalid piece");

   let list  = [];
   let legal = 0;
   let ptr   = self.link.get_moves(piece);

   for (let i=0; i<4; i++) {
      let y = self.mem.getInt(ptr + (2*i)*INT);
      let x = self.mem.getInt(ptr + (2*i+1)*INT);
      let pos = y*7 + x;

      if (x<0 || x>6 || y<0 || y>6) { list.push(-1); continue; }
      document.querySelectorAll(".board_cell")[pos].setAttribute("move", i);
      list.push(pos);
      legal++;
   }

   if (legal) { worker.focus(list, piece); }
   else       { worker.unfocus(); self.game.focus = undefined; }
}



/**
 * Mostra la schermata di gioco, sempre se ce ne sia uno attivo
 * @throws {Error} Se non c'e` nessun gioco in atto
 */
function showGame() {
   if (self.game.type === util.STATE_GAME_NONE) throw new Error("No game instances created yet");

   document.querySelector(".container").classList.remove("inactive");
   document.querySelector("#menu").classList.add("inactive");
   document.querySelector(".container").classList.remove("game-over");

   util.hide_credits();
   util.hide_choice();
}

/**
 * Mostra la schermata iniziale del menu di gioco
 */
function showMenu() {
   document.querySelector("#menu").classList.remove("inactive");
   document.querySelector(".container").classList.add("inactive");
   document.querySelector(".container").classList.remove("game-over");
}
