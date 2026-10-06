Module.onRuntimeInitialized = () => {
    start();
  };
  
  // COLORS
  const EMPTY = 0;
  const WHITE = 1;
  const BLACK = 2;
  // CONSTRAINTS
  const UNCONSTRAINED = -1;
  // NEIGHBOURHOODS
  const FULL = 0;
  const ORTHO = 1;
  const FULL_EXCLUDE = 2;
  const ORTHO_EXCLUDE = 3;
  // STATUS
  const ERROR = 0;
  const UNSATISFIED = 1;
  const SATISFIED = 2;
  
  const COLOR_BG = "#89b7b3"; 
  const COLOR_EMPTY = "#eee3ad";  
  const COLOR_WHITE = "#f49fa4";//#d4ff33
  const COLOR_BLACK = "#542a0e";//#334cff
  
  const COLOR_CONSTRAINT = "#000";
  const COLOR_CONSTRAINT_INV = "#fff";
  const COLOR_CONSTRAINT_ERROR = "#F00";
  const COLOR_CONSTRAINT_OK = "#888";
  
  const PADDING = 2;
  const MARGIN = 5;
  const CELL_SIZE = 32;
  const CONSTRAINT_FONT_SIZE = 18;
  
  // Global variables
  let g;
  let canvas;
  let ctx;
  let nb_rows;
  let nb_cols;
  
  // Render game in the canvas element
  function gameDisplay() {
    ctx.save();
  
    // Calculate canvas size
    canvas.width = (nb_cols * (CELL_SIZE + PADDING)) - PADDING + (2 * MARGIN);
    canvas.height = (nb_rows * (CELL_SIZE + PADDING)) - PADDING + (2 * MARGIN);
  
    ctx.fillStyle = COLOR_BG;
    ctx.fillRect(0, 0, canvas.width, canvas.height);
  
    for (let row = 0; row < nb_rows; row++) {
      for (let col = 0; col < nb_cols; col++) {
        // debugger;
        const color = Module._get_color(g, row, col);
  
        switch (color) {
          case WHITE:
            ctx.fillStyle = COLOR_WHITE;
            break;
          case BLACK:
            ctx.fillStyle = COLOR_BLACK;
            break;
          default:
          case EMPTY:
            ctx.fillStyle = COLOR_EMPTY;
            break;
        }
  
        ctx.fillRect(
          col * (CELL_SIZE + PADDING) + MARGIN,
          row * (CELL_SIZE + PADDING) + MARGIN,
          CELL_SIZE,
          CELL_SIZE,
        );
  
        const constraint = Module._get_constraint(g, row, col);
  
        if (constraint == UNCONSTRAINED) {
          continue;
        }
  
        const status = Module._get_status(g, row, col);
  
        ctx.font = `${CONSTRAINT_FONT_SIZE}px monospace`;
  
        switch (status) {
          case ERROR:
            ctx.fillStyle = COLOR_CONSTRAINT_ERROR;
            break;
          case SATISFIED:
            ctx.fillStyle = COLOR_CONSTRAINT_OK;
            break;
          default:
          case UNSATISFIED:
            ctx.fillStyle = color == BLACK
              ? COLOR_CONSTRAINT_INV
              : COLOR_CONSTRAINT;
            break;
        }
  
        const txt_width = ctx.measureText(constraint);
        ctx.fillText(
          constraint,
          col * (CELL_SIZE + PADDING) + (CELL_SIZE / 2) - (txt_width.width / 2) +
            MARGIN,
          row * (CELL_SIZE + PADDING) + (CELL_SIZE / 2) +
            (CONSTRAINT_FONT_SIZE / 2) + MARGIN,
        );
      }
    }
  
    ctx.restore();
  }
  
  // Load games and games infos into the web page
  function loadGame() {
    nb_rows = Module._nb_rows(g);
    nb_cols = Module._nb_cols(g);
    gameDisplay(g, canvas, ctx);
  
    document.getElementById("info__nrows").innerText = nb_rows;
    document.getElementById("info__ncols").innerText = nb_cols;
    // game_nb_solutions have some issues with non_square games (for some reasons...)
    document.getElementById("info__nsol").innerText = Module._nb_solutions(g);
    console.log("coucou");
    document.getElementById("info__wrapping").innerText = Module._is_wrapping(g)
      ? "yes"
      : "no";
  
    let neigh;
    switch (Module._get_neighbourhood(g)) {
      case ORTHO:
        neigh = "ORTHO";
        break;
      case FULL_EXCLUDE:
        neigh = "FULL_EXCLUDE";
        break;
      case ORTHO_EXCLUDE:
        neigh = "ORTHO_EXCLUDE";
        break;
      case FULL:
      default:
        neigh = "FULL";
        break;
    }
    document.getElementById("info__neigh").innerText = neigh;
  }
  
  function start() {
    g = Module._new_default();
    canvas = document.getElementById("game");
    ctx = canvas.getContext("2d");
  
    // Handle clicks on the canvas
    canvas.addEventListener("click", (event) => {
      const canvasRect = event.target.getBoundingClientRect();
      const x = event.x - canvasRect.x;
      const y = event.y - canvasRect.y;
  
      const i = Math.trunc((y - MARGIN) / (CELL_SIZE + PADDING));
      const j = Math.trunc((x - MARGIN) / (CELL_SIZE + PADDING));
  
      let color = Module._get_color(g, i, j);
      color = (color + 1) % 3;
      Module._play_move(g, i, j, color);
      gameDisplay();
  
      if (Module._won(g)) {
        alert("You WON !!!");
      }
    });
  
    // =====
    // CONTROL BUTTONS EVENTS
    // =====
  
    const onclick = (id, callback) => {
      document.getElementById(id).addEventListener("click", callback);
    };
  
    onclick("restart", () => {
      Module._restart(g);
      gameDisplay();
    });
  
    onclick("solve", () => {
      Module._solve(g);
      gameDisplay();
    });
  
    onclick("undo", () => {
      Module._undo(g);
      gameDisplay();
    });
  
    onclick("redo", () => {
      Module._redo(g);
      gameDisplay();
    });
  
    onclick("save", () => {
      const FILENAME = "game.txt";
  
      // Get a char* from a js string
      const encoded_name = new TextEncoder().encode(`/${FILENAME}\0`);
      const ptr = Module._malloc(FILENAME.length);
      const chunk = Module.HEAPU8.subarray(ptr, ptr + encoded_name.length);
      chunk.set(encoded_name);
  
      // Save the game into emscripten's virtual filesystem
      Module._save(g, ptr);
      Module._free(ptr);
  
      // Export file's data to the user
      const data = FS.readFile(`/${FILENAME}`, { encoding: "utf8" });
      const dl = document.createElement("a");
      dl.download = FILENAME;
      dl.href = URL.createObjectURL(new Blob([data]));
      dl.style.display = "none";
      document.body.appendChild(dl);
      dl.click();
      setTimeout(() => {
        document.body.removeChild(dl);
        URL.revokeObjectURL(dl.href);
      }, 2000);
    });
  
    const load = document.getElementById("load");
    load.addEventListener("change", () => {
      // Only execute if there is a selected file
      if (!load.files[0]) {
        return;
      }
  
      ctx.save();
      ctx.fillStyle = "#333";
      ctx.fillRect(0, 0, canvas.width, canvas.height);
  
      ctx.font = `12px monospace`;
      ctx.fillStyle = "#fff";
      ctx.fillText("Loading...", 0, canvas.height / 2);
      ctx.restore();
  
      const reader = new FileReader();
      reader.readAsArrayBuffer(load.files[0]);
      reader.addEventListener("loadend", async () => {
        const data = new Uint8Array(reader.result);
        const FILENAME = "/game.txt";
  
        // Put the loaded file into emscripten virtual file system
        await FS.writeFile(FILENAME, data);
  
        // Get a char* from a js string
        const encoded_name = new TextEncoder().encode(FILENAME + "\0");
        const ptr = Module._malloc(FILENAME.length);
        const chunk = Module.HEAPU8.subarray(ptr, ptr + encoded_name.length);
        chunk.set(encoded_name);
  
        // Load the game
        const new_g = Module._load(ptr);
        Module._free(ptr);
  
        Module._delete(g);
        g = new_g;
        loadGame();
      });
    });
  
    loadGame();
  }