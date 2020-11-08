const wasm = require("webassembly");
const game = require("script/games/test001.json");
let laska  = undefined;
let mem    = undefined;

const R = 0;
const L = 1;
const BR = 2;
const BL = 3;

wasm.load("script/laska.wasm").then((module) => {
   laska = module.exports;
   mem = module.memory;

   console.warn("Game assets are LOADED!");
   laska.new_game(10);
   fill();
});

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
function fill() {
   if (!laska || !mem) return debug("Game assets are not loaded!!!");
   clear_board();
   for (let i=0; i<22; i++) {
      let ptr = laska.get_piece(i);
      let content = mem.getInt(ptr);
      if (content >= 0) {
         let y = mem.getInt(ptr);
         let x = mem.getInt(ptr+4);
         let tower = [mem.getInt(ptr+8), mem.getInt(ptr+12), mem.getInt(ptr+16)];

         document.querySelectorAll(".board_cell")[y*7 + x].title = `Piece ${i}`;
         for (let j=0; j<3; j++) {
            if (tower[j] === 1 || tower[j] === 2) {
               document.querySelectorAll(".board_cell")[y*7 + x].innerHTML += '<div class="piece CPU"><div>';
            } else if (tower[j] === -1 || tower[j] === -2) {
               document.querySelectorAll(".board_cell")[y*7 + x].innerHTML += '<div class="piece USR"><div>';
            }

            if (tower[j]%2 === 0 && j === 0)
               document.querySelectorAll(".board_cell")[y*7 + x].classList.add("promoted");
            else if (j === 0)
               document.querySelectorAll(".board_cell")[y*7 + x].classList.remove("promoted");
         }
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
function clear_board() {
   for (let i=0; i<49; i++) {
      document.querySelectorAll(".board_cell")[i].title = "";
      document.querySelectorAll(".board_cell")[i].classList.remove("promoted");
      document.querySelectorAll(".board_cell")[i].innerHTML = "";
   }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
function move(p, i) {
   if (!laska || !mem)       return debug("Game assets are not loaded!!!");
   if (isNaN(p) || isNaN(i)) return debug("Cannot parse NON NUMERIC values to move() function");
   laska.do_move(p, i);
   fill();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ONLY FOR DEBUG PURPOSES!!!
function debug(txt) {
   alert("ERROR: " + txt);
   throw new Error(txt);
}

// ONLY FOR DEBUG PURPOSES!!!
function print_piece(i) {
   if (!laska || !mem) return debug("Game assets are not loaded!!!");
   let ptr = laska.get_piece(i);
   console.log(`Piece {y: ${mem.getInt(ptr)}, x: ${mem.getInt(ptr+4)}, tower: [${mem.getInt(ptr+8)}, ${mem.getInt(ptr+12)}, ${mem.getInt(ptr+16)}]}`);
}

// ONLY FOR DEBUG PURPOSES!!!
function run_test() {
   let i = 0;
   let time = setInterval(() => {
      if (i == game.moves.length-1) clearInterval(time);
      console.warn("Moving piece ... ");
      move(game.moves[i][0], game.moves[i][1]);
      i++;
   }, 300);
}
