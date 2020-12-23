/** @module util/types */

/** Il valore per una pedina NON PROMOSSA del team ROSSO
 *  @type {number} */
export const CPU          =  1;
/** Il valore per una pedina NON PROMOSSA del team BLU
 *  @type {number} */
export const USR          = -1;
/** Il valore per una pedina PROMOSSA del team ROSSO
 *  @type {number} */
export const PROMOTED_CPU =  2;
/** Il valore per una pedina PROMOSSA del team BLU
 *  @type {number} */
export const PROMOTED_USR = -2;

/** Il valore per rappresentare una cella che non contiene nessuna pedina
 *  @type {number} */
export const VOID_CELL = -1;

/** Il valore per rappresentare il team ROSSO
 *  @type {number} */
export const CPU_TEAM = 0;
/** Il valore per rappresentare il team BLU
 *  @type {number} */
export const USR_TEAM = 1;
/** Il valore per rappresentare nessun team (placeholder)
 *  @type {number} */
export const DISPOSED = 99;

/** Il valore per rappresentare uno stato del gioco in cui la mossa tentata NON si e' verificata
 *  @type {number} */
export const STATE_NOT_MOVED     = 0;
/** Il valore per rappresentare uno stato del gioco in cui il team BLU ha perso perche' si e' bloccato
 *  @type {number} */
export const STATE_USR_NO_MOVES  = 1;
/** Il valore per rappresentare uno stato del gioco in cui il team ROSSO ha perso perche' si e' bloccato
 *  @type {number} */
export const STATE_CPU_NO_MOVES  = 2;
/** Il valore per rappresentare uno stato del gioco in cui il team BLU ha perso perche' non ha piu' pedine
 *  @type {number} */
export const STATE_USR_NO_PIECES = 3;
/** Il valore per rappresentare uno stato del gioco in cui il team ROSSO ha perso perche' non ha piu' pedine
 *  @type {number} */
export const STATE_CPU_NO_PIECES = 4;

/** Il valore per rappresentare una partita GIOCATORE contro GIOCATORE
 *  @type {number} */
export const STATE_GAME_PVP      = 10;
/** Il valore per rappresentare una partita GIOCATORE contro COMPUTER (senza ricorsione)
 *  @type {number} */
export const STATE_GAME_PVE_EASY = 11;
/** Il valore per rappresentare una partita GIOCATORE contro COMPUTER (con ricorsione base)
 *  @type {number} */
export const STATE_GAME_PVE_MED  = 12;
/** Il valore per rappresentare una partita GIOCATORE contro COMPUTER (con ricorsione profonda)
 *  @type {number} */
export const STATE_GAME_PVE_HARD = 13;
/** Il valore per rappresentare una partita NON INIZIALIZZATA (placeholder)
 *  @type {number} */
export const STATE_GAME_NONE     = 99;

/**
 * Esce direttamente dal gioco chiudendo l'interfaccia grafica (dealloca automaticamente tutta la memoria usata)
 */
export function quit() {
   const { remote } = require('electron');
   remote.getCurrentWindow().close();
}

/**
 * Un metodo per aggirare il problema della non esistenza di una funzione sleep sincrona, crea un nuovo oggetto {Promise} e lo
 * lascia in attesa fino allo scadere dei millisecondi passati come argomento
 * @param {number} ms I millisecondi da aspettare
 * @returns {Promise<unknown>} Il ritorno DEVE essere gestito da una `async function` o da una `callback` per avere effetto
 */
export function sleep(ms) {
   return new Promise((resolve) => setTimeout(resolve, ms));
}

/**
 * Mostra la sezione delle informazioni e dei crediti, il controllo e' accessibile solo dal menu'
 * tuttavia se chiamata in un contesto di gioco funziona ugualmente
 */
export function show_credits() {
   document.querySelector("#info").classList.add("credits-on");
}

/**
 * Nasconde la sezione delle informazioni e dei crediti, il controllo e' accessibile solo dal menu'
 * tuttavia se chiamata in un contesto di gioco funziona ugualmente
 */
export function hide_credits() {
   document.querySelector("#info").classList.remove("credits-on");
}

/**
 * Mostra l'interfaccia di scelta di difficolta' del computer `(EASY - MEDIUM - HARD)`
 */
export function show_choice() {
   document.querySelector("#diff").classList.add("choice-on");
}

/**
 * Nasconde l'interfaccia di scelta di difficolta' del computer
 */
export function hide_choice() {
   document.querySelector("#diff").classList.remove("choice-on");
}

/**
 * Quando una partita si e' conclusa, con l'apparizione della schermata riassuntiva, questa funzione
 * modifica lo stile della schermata portando in risalto la scacchiera (in secondo piano) in modo da dare
 * la possibilita' all'utente di capire l'esito della partita adeguatamente
 */
export function toggle_inspection() {
   document.querySelector(".container").classList.toggle("game-over");
   document.querySelector(".container").classList.toggle("inspect");
}
