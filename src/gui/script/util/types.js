let Game = undefined;

const CPU          = -1;
const USR          =  1;
const PROMOTED_CPU = -2;
const PROMOTED_USR =  2;

const VOID_CELL = -1;

const CPU_TEAM = 0;
const USR_TEAM = 1;
const DISPOSED = 99;

const STATE_NOT_MOVED     = 0;
const STATE_USR_NO_MOVES  = 1;
const STATE_CPU_NO_MOVES  = 2;
const STATE_USR_NO_PIECES = 3;
const STATE_CPU_NO_PIECES = 4;
const STATE_GAME_PVP      = 10;
const STATE_GAME_PVE_LOW  = 11;
const STATE_GAME_PVE_HIGH = 12;
const STATE_GAME_NONE     = 99;



function handle_click(e) {
   if (!!Game) Game.click(e);
}

function quit() {
   const { remote } = require('electron');
   remote.getCurrentWindow().close();
}
