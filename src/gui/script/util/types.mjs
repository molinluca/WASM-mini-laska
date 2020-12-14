export const CPU          =  1;
export const USR          = -1;
export const PROMOTED_CPU =  2;
export const PROMOTED_USR = -2;

export const VOID_CELL = -1;

export const CPU_TEAM = 0;
export const USR_TEAM = 1;
export const DISPOSED = 99;

export const STATE_NOT_MOVED     = 0;
export const STATE_USR_NO_MOVES  = 1;
export const STATE_CPU_NO_MOVES  = 2;
export const STATE_USR_NO_PIECES = 3;
export const STATE_CPU_NO_PIECES = 4;
export const STATE_GAME_PVP      = 10;
export const STATE_GAME_PVE_EASY = 11;
export const STATE_GAME_PVE_MED  = 12;
export const STATE_GAME_PVE_HARD = 13;
export const STATE_GAME_NONE     = 99;


export function quit() {
   const { remote } = require('electron');
   remote.getCurrentWindow().close();
}

export function sleep(ms) {
   return new Promise((resolve) => setTimeout(resolve, ms));
}

export function show_credits() {
   document.querySelector("#info").classList.add("credits-on");
}

export function hide_credits() {
   document.querySelector("#info").classList.remove("credits-on");
}

export function show_choice() {
   document.querySelector("#diff").classList.add("choice-on");
}

export function hide_choice() {
   document.querySelector("#diff").classList.remove("choice-on");
}
