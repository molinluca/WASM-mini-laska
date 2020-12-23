import * as util from "./util/types.mjs"
import * as engine from "./util/engine.mjs"

/**
 * Questa funzione genera ogni gestore di input, che sia un click del puntatore del mouse o un tap sul touchscreen, per l'interazione dell'utente
 * con i vari controlli del gioco
 * @note Attenzione! Questa funzione NON genera i gestori di input della scacchiera
 */
function create_handlers() {
   document.querySelector("#type1").classList.remove("inactive");
   document.querySelector("#typeE").classList.remove("inactive");

   document.querySelector("#type1").addEventListener("click", () => { engine.create_game(util.STATE_GAME_PVP); });
   document.querySelector("#type2").addEventListener("click", () => { engine.create_game(util.STATE_GAME_PVE_EASY) });
   document.querySelector("#type3").addEventListener("click", () => { engine.create_game(util.STATE_GAME_PVE_MED); });
   document.querySelector("#type4").addEventListener("click", () => { engine.create_game(util.STATE_GAME_PVE_HARD) });
   document.querySelector(".abort").addEventListener("click", () => { engine.dispose_game(); });
   document.querySelector(".back").addEventListener("click",  () => { engine.dispose_game(); });

   document.querySelector("#typeE").addEventListener("click",           () => { util.show_choice(); });
   document.querySelector(".dismiss").addEventListener("click",         () => { util.hide_choice(); });
   document.querySelector("#credits").addEventListener("click",         () => { util.show_credits(); });
   document.querySelector(".close").addEventListener("click",           () => { util.hide_credits(); });
   document.querySelector(".inspection-tool").addEventListener("click", () => { util.toggle_inspection(); });
   document.querySelector("#exit").addEventListener("click",            () => { util.quit(); });
}

/**
 * Si tratta dell'`entrypoint` (in alcuni linguaggi riconosciutco come `"main"`) per far si che il gioco inizializzi il motore di gioco correttamente
 * e imposti tutti i gestori di input dei vari controlli
 * @returns {Promise<void>} L'output viene gestito dal chiamante `document.body.onload`, che esegue il corpo della funzione non appena il documento
 * html si e' completamente generato
 */
async function main() {
   await engine.load("script/binary/laska.wasm");
   await util.sleep(100);
   document.body.classList.remove("preload");
   create_handlers();
}

document.body.onload = main;
