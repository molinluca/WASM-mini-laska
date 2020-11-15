class Engine {
   constructor(e) {
      if (!e) throw new Error("ENGINE not defined!!!");
      if (!e.exports || !e.memory) throw new Error("Invalid ENGINE");
      console.warn("Engine successfully loaded");
      this.GameInstance = undefined;
      this.ENGINE = e;
   }

   create_game_instance(game_type) {
      if (!this.ENGINE) throw new Error("ENGINE module not loaded yet");
      if (!!this.GameInstance) throw new Error("Cannot handle more than 1 game instance at a time");

      switch(game_type) {
         case STATE_GAME_PVP:
         case STATE_GAME_PVE_LOW:
         case STATE_GAME_PVE_HIGH:
            this.GameInstance = new LaskaGame(this.ENGINE.exports, this.ENGINE.memory, game_type);
            this.show_game();
            break;
         default:
            throw new Error(`Unhandled game type (${game_type})`);
      }

      console.debug("Engine: New game instance created. Level: " + game_type);
   }

   dispose_game_instance() {
      if (!this.ENGINE) throw new Error("ENGINE module not loaded yet");
      if (!this.GameInstance) throw new Error("No game instances to dispose");
      this.GameInstance.destroy();
      this.GameInstance = undefined;
      this.show_menu();
      console.debug("Engine: Game instance disposed\n\n");
   }

   click(e) {
      if (!this.GameInstance) throw new Error("No GameInstance found")
      
      let piece = e.path[0].getAttribute("piece");
      let move  = e.path[0].getAttribute("move");

      if (piece === null) piece = e.path[1].getAttribute("piece");
      if (move  === null) move  = e.path[1].getAttribute("move");

      this.GameInstance.action({piece: piece, move: move});
   }

   show_menu() {
      document.querySelector("#menu").classList.remove("inactive");
      document.querySelector(".container").classList.add("inactive");
   }

   show_game() {
      document.querySelector(".container").classList.remove("inactive");
      document.querySelector("#menu").classList.add("inactive");
   }
}



/* Entry point */
document.body.onload = () => {
   const wasm = require("webassembly");
   const nw   = require("nw.gui");
   
   wasm.load("script/binary/laska.wasm").then((module) => {
      Game = new Engine(module);
      
      let cells = document.querySelectorAll(".board_cell");
      for (let i=0; i<49; i++) {
         if (i % 2 === 0) {
            let c = cells[i];
            c.setAttribute("nth", i);
            c.addEventListener("click", handle_click);
         }
      }
      
      document.querySelector("#type1").classList.remove("inactive");
      document.querySelector("#type1").addEventListener("click", () => {Game.create_game_instance(STATE_GAME_PVP);});
      document.querySelector(".abort").addEventListener("click", () => {Game.dispose_game_instance();});
   });
   
   setTimeout(() => { nw.Window.get().show(); }, 1000);
}
