# Downturn
A variation of the classic command-line game of Connect4 with a twist

# How to Run
 - After running make, run the program using -w, -h, and -r flags followed by a number, denoting width of the board, height of the board, and the number of pieces in a row required to win, respectively. (Note: cannot be more than 62 columns or rows and the run cannot exceed the number of columns or rows)
 - Flags -m and -b must be included, but will not affect gameplay (These alter the way the boards are stored, -m denoting a 2D matrix and -b in bits)
 - When prompted by turn, type the single character of the column where you would like to drop your piece.
 
# Rules
 - Starting with an empty board, players take turns dropping individual pieces and try to be the first to line up their pieces to the amount specified by the -r flag.
 - Players can also rotate the board, causing the pieces to fall according to gravity.
 - The first player to line up their pieces in a row of the number specified by the run wins
 - If the board fills up and no player has their pieces in a row of the amount specified, there is a draw.
