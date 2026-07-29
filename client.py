import os
import subprocess
import requests
import json

from dotenv import load_dotenv

load_dotenv()

API_TOKEN = os.getenv("API_TOKEN")
BOT_USERNAME = os.getenv("BOT_USERNAME")
BASE_URL = "https://lichess.org/api"
HEADERS = {
    "Authorization": f"Bearer {API_TOKEN}"
}

class Engine:
    def __init__(self, path):
        self.process = subprocess.Popen([path, "uci"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True, bufsize=1)
        self._init_uci()

    def _init_uci(self):
        self._send("uci")
        
        while True:
            line = self.process.stdout.readline().strip()
            if line == "uciok":
                break

    def _send(self, command):
        if self.process.poll() is not None:
            raise RuntimeError("Engine is no longer running.")

        self.process.stdin.write(command + "\n")
        self.process.stdin.flush()
    
    def close(self):
        self._send("quit")
        self.process.wait(timeout=2)

    def get_best_move(self, moves):
        self._send(f"position startpos moves {' '.join(moves)}") if moves else self._send("position startpos")
        self._send("isready")

        while True:
            line = self.process.stdout.readline().strip()
            if line == "readyok":
                break

        self._send("go movetime 1000")

        while True:
            line = self.process.stdout.readline().strip()
            if line.startswith("bestmove"):
                return line.split()[1]

class Game:
    def __init__(self, game_id, client, engine):
        self.game_id = game_id
        self.client = client
        self.engine = engine
        self.moves = []
        self.color = None

    def run(self):
        stream = self.client.stream(self.game_id)

        for line in stream:
            data = json.loads(line)

            if data["type"] == "gameFull":
                self._handle_full_game(data)

            elif data["type"] == "gameState":
                if data.get("status") != "started":
                    print("Game over: ", data.get("status"))
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
        return (move_count % 2 == 0) if self.color == "white" else (move_count % 2 == 1)

class Client:
    def stream(self, game_id = None):
        url = f"{BASE_URL}/stream/event" if game_id is None else f"{BASE_URL}/bot/game/stream/{game_id}"

        with requests.get(url, headers=HEADERS, stream=True) as request:
            for line in request.iter_lines():
                if line:
                    yield line

    def challenge(self, game_id, accept = True):
        url = f"{BASE_URL}/challenge/{game_id}/accept" if accept else f"{BASE_URL}/challenge/{game_id}/deny"
        response = requests.post(url, headers=HEADERS)
        response.raise_for_status()

    def make_move(self, game_id, move):
        url = f"{BASE_URL}/bot/game/{game_id}/move/{move}"
        response = requests.post(url, headers=HEADERS)
        response.raise_for_status()
            
def main():
    engine = None
    
    try: 
        client = Client()
        engine = Engine("./build/Release/engine")
        
        busy = False

        for event in client.stream():
            if not event.strip():
                continue

            data = json.loads(event)
            event_type = data.get("type")

            if event_type is None or event_type not in {"challenge", "gameStart"}:
                continue

            if event_type == "challenge":
                challenge_id = data["challenge"]["id"]

                if not busy:
                    client.challenge(challenge_id, True)
                    print(f"Accept challenge {challenge_id}")
                else:
                    client.challenge(challenge_id, False)
                    print(f"Deny challenge {challenge_id}")
            
            elif event_type == "gameStart":
                game_id = data["game"]["id"]

                print(f"Starting game {game_id}")

                busy = True
                game = Game(game_id, client, engine)
                game.run()

                print(f"Game is finished. Accepting new challenges")
                busy = False
    finally:
        if engine:
            engine.close()

if __name__ == "__main__":
    main()