import * as util from "./util/types.mjs"
import * as engine from "./util/engine.mjs"

/**
 * Genera tutti i gestori di eventi per i click dei vari controlli dell'interfaccia grafica, eccetto le singole celle della scacchiera
 */
function create_handlers() {
   document.querySelector("#type1").classList.remove("inactive");
   document.querySelector("#typeE").classList.remove("inactive");

   document.querySelector("#type1").addEventListener("click", () => { engine.create_game(util.STATE_GAME_PVP); });
   document.querySelector("#type2").addEventListener("click", () => { engine.create_game(util.STATE_GAME_PVE_EASY) });
   document.querySelector("#type3").addEventListener("click", () => { engine.create_game(util.STATE_GAME_PVE_MED); });
   document.querySelector("#type4").addEventListener("click", () => { engine.create_game(util.STATE_GAME_PVE_HARD) });
   document.querySelector(".abort").addEventListener("click", () => { engine.dispose_game(); });
   document.querySelector(".btn").addEventListener("click",   () => { engine.dispose_game(); });

   document.querySelector("#typeE").addEventListener("click",   () => { util.show_choice(); });
   document.querySelector(".dismiss").addEventListener("click", () => { util.hide_choice(); });
   document.querySelector("#credits").addEventListener("click", () => { util.show_credits(); });
   document.querySelector(".close").addEventListener("click",   () => { util.hide_credits(); });
   document.querySelector("#exit").addEventListener("click",    () => { util.quit(); });
}

/**
 * Entry point dell'esecuzione, carica il motore di gioco compilato da ANSI C e crea tutti i gestori di eventi
 * @returns {Promise<void>} Output gestito da "body.onload", che la esegue non appena ha caricato l'intero documento
 */
async function main() {
   await engine.load("script/binary/laska.wasm");
   create_handlers();
}

/* Chiama main() non appena il documento HTML non si e` completamente caricato */
document.body.onload = main;
