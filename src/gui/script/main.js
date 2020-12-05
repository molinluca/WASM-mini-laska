const INT = 4; /* The int size in Bytes */

class LaskaGame {
   constructor(instance, memory, type) {
      if (!instance || !memory) throw new Error("Uncaught error generating the game engine...");
      if (isNaN(type)) throw new Error("Uncaught error: the game type MUST BE a number");
      this.ANSI    = instance;
      this.mem     = memory;
      this.focus   = undefined;
      this.state   = type;

      document.querySelector("#end-state").removeAttribute("blue");
      document.querySelector("#end-state").removeAttribute("red");

      this.ANSI.new_game(type);
      this.fill();
   }

   /* Gets the updated state of the game
   *  NOTE: if the state describes a game ending, it will be shown */
   update_state(state) {

      this.state = state;
      switch(state) {
         case STATE_USR_NO_MOVES:
            this.end_match({loser: USR_TEAM, has_pieces_left: true});
            break;
         case STATE_CPU_NO_MOVES:
            this.end_match({loser: CPU_TEAM, has_pieces_left: true});
            break;
         case STATE_USR_NO_PIECES:
            this.end_match({loser: USR_TEAM, has_pieces_left: false});
            break;
         case STATE_CPU_NO_PIECES:
            this.end_match({loser: CPU_TEAM, has_pieces_left: false});
            break;

         default: return;
      }

   }

   /* Fills the game table with the Pieces in the right positions */
   fill() {
      this.clear();
      for (let i=0; i<22; i++) {
         let ptr = this.ANSI.get_piece(i);
         let content = this.mem.getInt(ptr); /* The first element of the buffered_piece (the y coordinate) */
         if (content >= 0) {
            let y = this.mem.getInt(ptr);
            let x = this.mem.getInt(ptr + INT);
            let tower = [this.mem.getInt(ptr + INT*2), this.mem.getInt(ptr + INT*3), this.mem.getInt(ptr + INT*4)];
            let cell = document.querySelectorAll(".board_cell")[y*7 + x];

            cell.title = `Piece ${i}`;
            cell.setAttribute("piece", i);

            /* Fills the cell with the tower content */
            for (let j=0; j<3; j++) {
               if (tower[j] === CPU || tower[j] === PROMOTED_CPU) {
                  cell.innerHTML += '<div class="piece CPU"></div>';
               }

               else if (tower[j] === USR || tower[j] === PROMOTED_USR) {
                  cell.innerHTML += '<div class="piece USR"></div>';
               }

               /* If the first piece of the tower is promoted... */
               if ((tower[j] % 2 === 0) && j === 0) cell.classList.add("promoted");
               else if (j === 0) cell.classList.remove("promoted");
            }
         }
      }
   }

   /* Clears the game table in order to fill it with the incoming changes */
   clear() {
      for (let i=0; i<49; i++) {
         let cell = document.querySelectorAll(".board_cell")[i];
         cell.title = "";
         cell.classList.remove("promoted");
         cell.innerHTML = "";
         cell.removeAttribute("piece");
         cell.removeAttribute("move");
      }
   }

   /* Calls the "ANSI C" move function in order to move a piece in the selected direction */
   play(piece, direction) {
      if (isNaN(piece) || isNaN(direction)) throw new Error("Trying to use NON NUMERIC values to move a Piece");
      let played = this.ANSI.do_move(piece, direction);
      if (played) {
         this.fill();
         this.unfocus();
         this.update_state(played);
      }
   }

   /* Either selects a piece to move or tries to move it */
   action({piece, move}) {
      // if (id === null) throw new Error("Null cell id");
      if (move !== null || piece !== null) {
         if (this.focus === undefined) {
            this.focus = piece;
            this.load_moves(piece);
         }

         else if (move !== null) {
            this.play(this.focus, move);
         }

         else if (this.focus === piece) {
            this.unfocus();
         }
      }
   }

   /* Loads all the possible moves of the selected piece */
   load_moves(piece) {
      if (piece === undefined || piece === null) throw Error("Undefined piece");
      if (isNaN(piece)) throw Error("Invalid piece");
      if (piece < 0 || piece > 21) throw Error("Invalid piece");

      let found = 0;
      let ptr   = this.ANSI.get_moves(piece);

      for (let i=0; i<4; i++) {
         let y = this.mem.getInt(ptr + (2*i)*INT);
         let x = this.mem.getInt(ptr + (2*i+1)*INT);

         if (x >= 0 && y >= 0) {
            let target = document.querySelectorAll(".board_cell")[y*7 + x];
            if (!!target) {
               found++;
               target.setAttribute("move", i);
            }
         }
      }

      if (found > 0) {
         document.querySelector(`.board_cell[piece="${piece}"]`).setAttribute("main", this.piece);
         document.querySelector("table").classList.add("choice");
      }

      else {
         this.unfocus();
      }
   }

   /* Removes the focus of the Piece that has been selected before */
   unfocus() {
      this.focus = undefined;
      document.querySelector("table").classList.remove("choice");
      for (let i=0; i<49; i++) {
         let cell = document.querySelectorAll(".board_cell")[i];
         cell.removeAttribute("move");
         cell.removeAttribute("main");
      }
   }

   /* Completely deletes the game session */
   destroy() {
      this.ANSI.end_game();
   }

   /* End game state handler */
   end_match({loser, has_pieces_left}) {
      this.destroy();

      let text = "The match concluded in a <span>tie</span>... No player has <span>legal moves</span> left";
      document.querySelector(".container").classList.add("game-over");

      if (loser === CPU_TEAM) {
         if (has_pieces_left) text = "The <span>BLUE PLAYER</span> won the match... The other player has <span>no legal moves</span> possible"
         else                 text = "The <span>BLUE PLAYER</span> won the match... The other player has <span>no pieces left</span>"
         document.querySelector("#end-state").setAttribute("blue", true);
      }

      else if (loser === USR_TEAM) {
         if (has_pieces_left) text = "The <span>RED PLAYER</span> won the match... The other player has <span>no legal moves</span> possible"
         else                 text = "The <span>RED PLAYER</span> won the match... The other player has <span>no pieces left</span>"
         document.querySelector("#end-state").setAttribute("red", true);
      }

      document.querySelector("#end-state .message .text").innerHTML = text;
   }
}







function test() {
   let tm, iteration = 0;
   tm = setInterval(() => {
      let piece = Math.floor(Math.random() * 22);
      let move  = Math.floor(Math.random() * 4);
      Game.GameInstance.play(piece, move);
      iteration++;
      if (iteration > 100000 || Game.GameInstance.state !== 10) {
         clearInterval(tm);
         console.warn("Finito", Game.GameInstance.state);
      }
   }, 0);
}
