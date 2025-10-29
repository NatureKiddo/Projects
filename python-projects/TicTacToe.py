import random

class Player:
    #Stores player name, symbol, and type (human or computer).
    def __init__(self, name, symbol, is_human=True):
        self.name = name
        self.symbol = symbol
        self.is_human = is_human

class Board:
    #Manages the game board, display, and move validation.
    def __init__(self):
        self.grid = [[' ' for _ in range(3)] for _ in range(3)]
        self.coords_to_indices = {
            'a1': (0, 0), 'a2': (0, 1), 'a3': (0, 2),
            'b1': (1, 0), 'b2': (1, 1), 'b3': (1, 2),
            'c1': (2, 0), 'c2': (2, 1), 'c3': (2, 2)
        }
        self.indices_to_coords = {v: k for k, v in self.coords_to_indices.items()}

    def display(self):
        #Displays the board grid with labeled rows and columns.
        print("\n    1   2   3")
        print("  +---+---+---+")
        for i, row in enumerate(self.grid):
            row_label = chr(ord('A') + i)
            print(f"{row_label} │ {row[0]} │ {row[1]} │ {row[2]} │")
            if i < 2:
                print("  +---+---+---+")
        print("  +---+---+---+")

    def validate_move(self, move):
        #Checks if a move is valid (e.g., cell is empty).
        move = move.lower()
        if move in self.coords_to_indices:
            row, col = self.coords_to_indices[move]
            if self.grid[row][col] == ' ':
                return True, (row, col)
            return False, "This spot is already taken."
        return False, "Invalid input. Please use format like 'A1', 'B2', etc."

    def make_move(self, row, col, symbol):
        #Updates the board with a player's move.
        self.grid[row][col] = symbol

    def get_empty_cells(self):
        #Returns a list of all available move coordinates.
        empty_cells = []
        for r in range(3):
            for c in range(3):
                if self.grid[r][c] == ' ':
                    empty_cells.append((r, c))
        return empty_cells

    def check_win(self, symbol):
        # Checks for a win condition for a given player symbol.
        # Check rows, columns, and diagonals
        win_conditions = [
            # Rows
            [(0, 0), (0, 1), (0, 2)], [(1, 0), (1, 1), (1, 2)], [(2, 0), (2, 1), (2, 2)],
            # Columns
            [(0, 0), (1, 0), (2, 0)], [(0, 1), (1, 1), (2, 1)], [(0, 2), (1, 2), (2, 2)],
            # Diagonals
            [(0, 0), (1, 1), (2, 2)], [(0, 2), (1, 1), (2, 0)]
        ]
        for combo in win_conditions:
            if all(self.grid[r][c] == symbol for r, c in combo):
                return True
        return False

    def is_draw(self):
        #Checks if the game is a draw.
        return all(self.grid[r][c] != ' ' for r in range(3) for c in range(3))

class Game:
    #Controls game flow, input handling, and AI logic.
    def __init__(self):
        self.players = []
        self.board = Board()
        self.turn = 0  # Index for current player
        self.wins = {'X': 0, 'O': 0, 'Draws': 0}

    def _get_player_info(self):
        #Asks the user for game mode and player names.
        print("---------- Welcome to Tic Tac Toe ----------")
        mode_choice = input("Select game mode: 1. Player vs Player | 2. Player vs Computer\nEnter 1 or 2: ")
        while mode_choice not in ['1', '2']:
            mode_choice = input("Invalid choice. Please enter 1 or 2: ")

        p1_name = input("Enter Player 1 name (X): ")
        self.players.append(Player(p1_name, 'X', is_human=True))

        if mode_choice == '1':
            p2_name = input("Enter Player 2 name (O): ")
            self.players.append(Player(p2_name, 'O', is_human=True))
        else:
            self.players.append(Player("Computer", 'O', is_human=False))

    def _get_human_move(self):
        #Prompts a human player for their move and validates it.
        while True:
            move = input(f"{self.players[self.turn].name}'s turn ({self.players[self.turn].symbol}), enter position (e.g., A1): ")
            is_valid, message = self.board.validate_move(move)
            if is_valid:
                return message  # Returns (row, col)
            else:
                print(message)

    def _get_computer_move(self):
        #Implements the computer's AI strategy.
        board_copy = Board()
        board_copy.grid = [row[:] for row in self.board.grid]
        
        # 1. Try to win
        for move in board_copy.get_empty_cells():
            board_copy.make_move(move[0], move[1], self.players[self.turn].symbol)
            if board_copy.check_win(self.players[self.turn].symbol):
                return move
            board_copy.make_move(move[0], move[1], ' ')  # Undo move

        # 2. Block opponent
        opponent_symbol = self.players[(self.turn + 1) % 2].symbol
        for move in board_copy.get_empty_cells():
            board_copy.make_move(move[0], move[1], opponent_symbol)
            if board_copy.check_win(opponent_symbol):
                return move
            board_copy.make_move(move[0], move[1], ' ') # Undo move

        # 3. Take center
        if (1, 1) in board_copy.get_empty_cells():
            return (1, 1)
        
        # 4. Take corners
        corners = [(0, 0), (0, 2), (2, 0), (2, 2)]
        available_corners = [c for c in corners if c in board_copy.get_empty_cells()]
        if available_corners:
            return random.choice(available_corners)
        
        # 5. Take sides
        available_sides = [c for c in [(0, 1), (1, 0), (1, 2), (2, 1)] if c in board_copy.get_empty_cells()]
        if available_sides:
            return random.choice(available_sides)
        
        return random.choice(board_copy.get_empty_cells())

    def _play_turn(self):
        #Manages a single turn of the game.
        player = self.players[self.turn]
        if player.is_human:
            row, col = self._get_human_move()
        else:
            print("Computer is thinking...")
            row, col = self._get_computer_move()
            print(f"Computer chooses: {self.board.indices_to_coords[(row, col)].upper()}")
        
        self.board.make_move(row, col, player.symbol)
        self.board.display()

    def _check_game_over(self):
        #Checks for win/draw conditions and returns winner or draw status.
        player = self.players[self.turn]
        if self.board.check_win(player.symbol):
            self.wins[player.symbol] += 1
            print(f"\nGame Over! {player.name} wins!")
            return 'win'
        if self.board.is_draw():
            self.wins['Draws'] += 1
            print("\nGame Over! It's a draw.")
            return 'draw'
        return None
    
    def _display_scoreboard(self):
        #Prints the current game scoreboard.
        print("\n----------Scoreboard-----------")
        print(f"{self.players[0].name} (X) wins: {self.wins['X']}")
        print(f"{self.players[1].name} (O) wins: {self.wins['O']}")
        print(f"Draws: {self.wins['Draws']}")
        print("-----------------------------------\n")

    def play_game(self):
        #The main game loop, including setup, turns, and replay.
        self._get_player_info()
        
        while True:
            self.board = Board()
            self.turn = 0
            game_over = False

            while not game_over:
                self.board.display()
                self._play_turn()
                result = self._check_game_over()
                
                if result:
                    game_over = True
                else:
                    self.turn = (self.turn + 1) % 2

            self._display_scoreboard()
            
            replay = input("Do you want to play again? (yes/no): ").lower()
            if replay not in ['yes', 'y']:
                print("Thanks for playing! Enjoy your day further on.")
                break

if __name__ == "__main__":
    game_instance = Game()
    game_instance.play_game()
