import json

from client.src.config import BOT_USERNAME

"""
This class defines and handle the game state. During each chess session,
it is responsible with checking the status of the game, and it dertimines whether
to wait on the opponent's move or make a move of its own.
"""

class Game:
    def __init__(self, game_id, client, engine):
        self.game_id = game_id
        self.client = client
        self.engine = engine
        self.moves = []
        self.color = None

    def run(self):
        stream = self.client.stream_game(self.game_id)

        for line in stream:
            data = json.loads(line)

            if data["type"] == "gameFull":
                self._handle_full_game(data)
            
            elif data["type"] == "gameState":
                if data.get("status") != "started":
                    print("Game over:", data.get("status"))
                    break
                
                self._handle_game_state(data)

    def _handle_full_game(self, data):
        moves = data["state"]["moves"]
        white = data["white"]["name"]

        self.moves = moves.split() if moves else []
        self.color = "white" if white == BOT_USERNAME else "black"
        self._maybe_make_move()

    def _handle_game_state(self, data):
        moves = data["moves"]
        new_moves = moves.split() if moves else []

        if len(new_moves) == len(self.moves):
            return

        self.moves = new_moves
        self._maybe_make_move()

    def _maybe_make_move(self):
        if not self._is_my_turn():
            return
        
        best_move = self.engine.get_best_move(self.moves)
        self.client.make_move(self.game_id, best_move)

    def _is_my_turn(self):
        if self.color is None:
            return False
        
        move_count = len(self.moves)

        if self.color == "white":
            return move_count % 2 == 0
        else:
            return move_count % 2 == 1

