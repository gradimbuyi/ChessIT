import json

from client.src.client import LichessClient
from client.src.game import Game
from client.src.interface import EngineInterface

def main():
    lichess_client = LichessClient()
    engine = EngineInterface("engine/build/engine")

    busy = False

    for event in lichess_client.stream_events():
        if not event.strip():
            continue

        data = json.loads(event)
        
        event_type = data.get("type")

        if event_type is None or event_type not in ("challenge", "gameStart"):
            continue

        if event_type == "challenge":
            challenge_id = data["challenge"]["id"]

            if not busy:
                lichess_client.accept_challenge(challenge_id)
                print(f"Accepted challenge {challenge_id}")
            else:
                lichess_client.deny_challenge(challenge_id)
                print(f"Deny challenge {challenge_id}. A game is in session.")

        elif event_type == "gameStart":
            game_id = data["game"]["id"]
            
            print(f"Starting game {game_id}")

            busy = True
            game = Game(game_id, lichess_client, engine)
            game.run()

            print(f"Game is finished. Accepting new challenges")
            busy = False        

if __name__ == "__main__":
    main()