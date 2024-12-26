# Assignment: Tic-Tac-Toe

You should work in the `assignment` folder.

You should modify the files: 
- `assignment/index.html` : The main HTML file. 
- `assignment/js/index.js` : The javascript file that will compute the tax.

You should use Bulma CSS for styling, and Vue 3 for the javascript.
If you load `index.html` in a browser, you should see a Tic-Tac-Toe board. 
You can use that as starting point for your assignment. 

## The Game

The game should work as follows.

Initially, the board is empty.  
When a player clicks on the board, if the cell is empty and no one has won yet, then the cell should be filled with the player's mark.
The X player plays first, the O player plays second.
The game ends when a player places three of their marks in a row, either horizontally, vertically, or diagonally.
The cells with the winning marks should be highlighted using the "is-success" class from Bulma.

In detail, a cell should have the following structure. There must be 9 `<button>` elements. 
Each button element should have class: 

* `button` : The Bulma button class.
* `is-success` : If the cell is part of the winning row.

The button should also have associated a click-event listener so that when the button is clicked, the cell is filled with the player's mark.

Inside the button, there should be a `<span>` element to contain the icon.  The span should be: 

* `<span><i class="fa fw"></i></span>` : The cell is empty.
* `<span><i class="fa fw fa-remove"></i></span>` : The X player has played there. 
* `<span><i class="fa fw fa-circle-o"></i></span>` : The O player has played there.

At the bottom of the page, there should be a New Game button that resets the game to the initial state.  The button should have id "new-game":

    <button id="new-game" ...>New Game</button>


## Implementation considerations

The challenge here is to keep the code simple and clean. Remember you can use things like 

    <i "class={'fa fw fa-remove': cell[i][j] == 1}"></i>

That is, you should think carefully about which state variables to keep, and how to best connect the HTML to the state in a concise way.  In the instructor solution, the HMTL is 50 lines long, and the javascript is 60 lines long (and the code was not especially optimized for consision).

## Points

The assignment is worth 10 points, that will be assigned as follows: 

* 1 point: The HTML is valid and a board is displayed.
* 1 point: The New Game button resets the game to the initial state.
* 1 point: Initially all cells are empty. 
* 1 point: The X player plays first.
* 3 points: When a player clicks on the board, if the cell is empty and no one has won yet, then the cell should be filled with the player's mark; players alternate.
* 3 points: The game ends when a player places three of their marks in a row, either horizontally, vertically, or diagonally. The cells with the winning marks are highlighted using the "is-success" class from Bulma.

You can grade the assignment yourself, like this: 

    python grade.py

## Submission

To submit, first crete the zip file, via: 

    python zipit.py

This creates the file `submission.zip`.  Submit this file to the Google Form, and **be sure to press SUBMIT on the form**.  Just uploading the file to the form is not enough. 
