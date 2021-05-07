"""
Tic Tac Toe Player
"""

import math
import copy

X = "X"
O = "O"
EMPTY = None


def initial_state():
    """
    Returns starting state of the board.
    """
    return [[EMPTY, EMPTY, EMPTY],
            [EMPTY, EMPTY, EMPTY],
            [EMPTY, EMPTY, EMPTY]]


def player(board):
    """
    Returns player who has the next turn on a board.
    """
    turnCounter = 0

    for row in board:
        for cell in row:
            if cell == X:
                turnCounter += 1
            elif cell == O:
                turnCounter -= 1
    
    if turnCounter == 1:
        return O
    else:
        return X


def actions(board):
    """
    Returns set of all possible actions (i, j) available on the board.
    """
    actions = set()
    for rowCount, row in enumerate(board):
        for colCount, cell in enumerate(row):
            if cell == EMPTY:
                actions.add((rowCount, colCount))
    return actions
    


def result(board, action):
    """
    Returns the board that results from making move (i, j) on the board.
    """
    tempBoard = copy.deepcopy(board)
    if tempBoard[action[0]][action[1]] != EMPTY:
       raise NameError('Invalid move')
    
    tempBoard[action[0]][action[1]] = player(board)
    return tempBoard



def winner(board):
    """
    Returns the winner of the game, if there is one.
    """
    xPositions = set()
    oPositions = set()
    for rowCount, row in enumerate(board):
        for colCount, cell in enumerate(row):
            if cell == X:
                xPositions.add((rowCount, colCount))
            elif cell == O:
                oPositions.add((rowCount, colCount))

    if check_winner(xPositions):
        return X
    elif check_winner(oPositions):
        return O
    else:
        return None


def check_winner(positions):
    """
    Checks a set of tuple coordinates of a single player for win conditions
    """
    #row & col counters incremented when a move is found within the relevant indicies row/col 
    #3 in any indicie for win
    rowCounter = [0,0,0]
    colCounter = [0,0,0]

    centerSquare = False
    #diag counter incremented when either opposite corner is present
    #2 for win + centerSquare
    diagonalCounter = [0,0]

    for move in positions: 
        #increment row/col counters 
        rowCounter[move[0]] += 1
        colCounter[move[1]] += 1

        if move == (1, 1):
            centerSquare = True
        #Check first diagonal
        elif move == (0, 0) or move == (2, 2):
            diagonalCounter[0] += 1
        #Check second diagonal 
        elif move == (0, 2) or move == (2, 0):
            diagonalCounter[1] +=1

    if 3 in rowCounter or 3 in colCounter:
        return True
    if centerSquare and 2 in diagonalCounter:
        return True
    
    return False
    

def terminal(board):
    """
    Returns True if game is over, False otherwise.
    """
    if winner(board) is not None or (not any(EMPTY in row for row in board) and winner(board) is None):
        return True
    else:
        return False

def utility(board):
    """
    Returns 1 if X has won the game, -1 if O has won, 0 otherwise.
    """
    if winner(board) ==  X:
        return 1
    elif winner(board) == O:
        return -1
    else:
        return 0


def minimax(board):
    """
    Returns the optimal action for the current player on the board.
    """
    if terminal(board):
        return None

    if player(board) == X:
        bestMove, value = max_value(board)
        return bestMove
    else:
        bestMove, value = min_value(board)
        return bestMove

def max_value(board):
    if terminal(board):
        return None, utility(board)

    ourValue = float('-inf')
    bestMove = None
    for action in actions(board):
        move, theirValue = min_value(result(board, action))
        if theirValue > ourValue:
            ourValue = theirValue
            bestMove = action

    return bestMove, ourValue

def min_value(board):
    if terminal(board):
        return None, utility(board)

    ourValue = float('inf')
    bestMove = None
    for action in actions(board):
        move, theirValue = max_value(result(board, action))
        if theirValue < ourValue:
            ourValue = theirValue
            bestMove = action
    
    return bestMove, ourValue
