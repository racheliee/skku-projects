"use strict";

let app = {};

app.data = {
  data: function () {
    return {
      board: [
        ["", "", ""],
        ["", "", ""],
        ["", "", ""],
      ],
      currentPlayer: "X",
      winner: null,
      winning: [],
    };
  },
  methods: {
    play: function (row, col) {
      if (!this.winner && this.board[row][col] === "") {
        this.board[row][col] = this.currentPlayer;

        if (this.checkWinner()) {
          this.winner = this.currentPlayer;
        } else {
          this.currentPlayer = this.currentPlayer === "X" ? "O" : "X";
        }
      }
    },

    checkWinner: function () {
      const winningCombo = [
        [
          [0, 0],
          [0, 1],
          [0, 2],
        ],
        [
          [1, 0],
          [1, 1],
          [1, 2],
        ],
        [
          [2, 0],
          [2, 1],
          [2, 2],
        ],
        [
          [0, 0],
          [1, 0],
          [2, 0],
        ],
        [
          [0, 1],
          [1, 1],
          [2, 1],
        ],
        [
          [0, 2],
          [1, 2],
          [2, 2],
        ],
        [
          [0, 0],
          [1, 1],
          [2, 2],
        ],
        [
          [0, 2],
          [1, 1],
          [2, 0],
        ],
      ];

      for (const wc of winningCombo) {
        const [a, b, c] = wc;
        if (
          this.board[a[0]][a[1]] &&
          this.board[a[0]][a[1]] === this.board[b[0]][b[1]] &&
          this.board[a[0]][a[1]] === this.board[c[0]][c[1]]
        ) {
          this.winning = wc;
          return true;
        }
      }

      return false;
    },

    isWinningCell: function (row, col) {
      return this.winning.some(([r, c]) => r === row && c === col);
    },

    resetGame: function () {
      this.board = [
        ["", "", ""],
        ["", "", ""],
        ["", "", ""],
      ];
      this.currentPlayer = "X";
      this.winner = null;
      this.winning = [];
    },
  },
};

app.vue = Vue.createApp(app.data).mount("#app");
app.vue.recompute();
