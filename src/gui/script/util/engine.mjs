/** @module util/engine */
const wasm = require("webassembly");
import * as util   from "./types.mjs"
import * as worker from "./worker.mjs";

/**
 * Il valore (in BYTES) per la dimensione di un intero.
 * Viene usato da alcune funzioni che accedono alla memoria del motore di gioco per prendere i valori dei buffer un intero per volta
 * @type {number}
 */
const INT = 4;

/**
 * Contiene la maggiorparte dei dati del motore di gioco, a partire dal link delle varie funzioni esposte (da `link.c`)
 * fino alla memoria occupata dalla stessa.
 * Inoltre presenta una struttura dati che contiene lo stato del gioco in esecuzione `(tipo, turno, pedina selezionata, finito?)`
 *
 * @type {{game: {is_ended: boolean, focus: any, turn: any, type: any}, mem: object, link: object}}
 */
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
 * Carica il file compilato `(.wasm)` del motore di gioco. Se tutto va a buon fine genera anche i vari gestori di input delle
 * varie celle della scacchiera
 * @param {string} bin il percorso del file .wasm
 * @returns {Promise<void>} Non ha ritorno, ma la sua esecuzione deve essere gestita in maniera asincrona
 * @throws {Error} Se il file compilato del motore di gioco e' incompatibile o corrotto (o non esiste)
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
 * @param {any} type Tipologia del gioco
 * @throws {Error} Se non e' stato caricato nessun motore di gioco
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
         self.game.turn     = self.link.get_turn();
         worker.change_team_tag(self.game.turn);
         worker.fill(self.link.get_piece, self.mem);

         if (type === util.STATE_GAME_PVP) document.querySelector(".game-turn").classList.remove("hidden");
         else                              document.querySelector(".game-turn").classList.add("hidden");

         showGame();
         break;

      default: throw new Error("Invalid game type... ("+type+")");
   }
}

/**
 * Termina qualsiasi sessione di gioco in corso, quindi anche se il gioco non si e' concluso, o se vi e' stata qualche incongruenza
 * nella sua generazione, quindi nullo, lo termina ugualmente, per poi tornare all'interfaccia del menu' iniziale di gioco
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
 * @param {any} play_res Risultato della mossa
 */
function update(play_res) {
   self.game.type = play_res;
   self.game.turn = self.link.get_turn();

   worker.change_team_tag(self.game.turn);

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
 * @returns {Promise<void>} Return di tipo `"dont-care"`, il suo risultato deve essere gestito da una `callback` o per lo meno atteso
 * in una funzione asincrona
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
 * @param {any} focus La pedina selezionata
 * @param {any} move La direzione
 * @returns {Promise<void>} Return di tipo `"dont-care"`, il suo risultato deve essere gestito da una `callback` o per lo meno atteso
 * in una funzione asincrona
 * @throws {Error} Se il gioco si e' gia concluso o se gli argomenti `(focus, move)` sono `NaN` lancia un errore
 * @note Attenzione!!! Se uno degli argomenti dovesse essere `undefined`, potrebbe essere riconosciuto come il valore `0`
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
 * @param {any} piece La pedina da selezionare (non deve per forza essere definita)
 * @param {any} move La direzione della mossa (non deve per forza essere definita)
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
 * Gestisce la pressione del puntatore del mouse (o del tap con un touchscreen) e recupera le informazioni necessarie.
 * In particolare se e' gia stata selezionata una pedina, se la cella interessata rappresenta una mossa possibile, tenta di eseguirla,
 * altrimenti tenta di selezionare la pedina della cella interessata
 * @param {any} e L'elemento cliccato
 * @throws {Error} Se nessuna sessione di gioco e' in atto lancia un errore dichiarando che non ha trovato nessuna istanza di gioco
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
 * Richiede al motore di gioco le possibili mosse di una specifica pedina. Siccome sono salvate in un buffer legato ad un puntatore intero,
 * la funzione si prende cura di leggere ciascun elemento convertendoli in informazioni utili da mostrare all'utente
 * @param {any} piece La pedina di cui calcolare le mosse
 * @note Se la pedina non puo' essere mossa, o non ci si trova nel turno adeguato, non fa nulla
 * @throws {Error} Se il valore della pedina e' `undefined`, `null`, `NaN` o semplicemente fuori dal range `[0-21]` lancia un errore
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
 * Cambia la schermata di gioco mostrando l'interfaccia di gioco (scacchiera + controlli)
 * @throws {Error} Se nessuna sessione di gioco e' in atto lancia un errore dichiarando che non ha trovato nessuna istanza di gioco
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
 * Cambia la schermata di gioco mostrando il menu' di avvio
 * @note Attenzione! Questa funzione da sola non elimina la sessione di gioco in corso!
 */
function showMenu() {
   document.querySelector("#menu").classList.remove("inactive");
   document.querySelector(".container").classList.add("inactive");
   document.querySelector(".container").classList.remove("game-over");
}
