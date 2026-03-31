import requests

from client.src.config import HEADERS, BASE_URL

"""
Controllers for lichess API communication. Sends the appropriate message
(ex. accept challenge, deny challenge, and make move) dependening on the engine's availability 
and move choices.
"""

class LichessClient: 
    def stream_events(self): 
        url = f"{BASE_URL}/stream/event"

        with requests.get(url, headers=HEADERS, stream=True) as request:
            for line in request.iter_lines():
                if line:
                    yield line

    def accept_challenge(self, challenge_id):
        url = f"{BASE_URL}/challenge/{challenge_id}/accept"
        requests.post(url, headers=HEADERS)

    def deny_challenge(self, challenge_id):
        url = f"{BASE_URL}/challenge/{challenge_id}/deny"
        requests.post(url, headers=HEADERS)

    def make_move(self, game_id, move):
        url = f"{BASE_URL}/bot/game/{game_id}/move/{move}"
        requests.post(url, headers=HEADERS)

    def stream_game(self, game_id):
        url = f"{BASE_URL}/bot/game/stream/{game_id}"
       
        with requests.get(url, headers=HEADERS, stream=True) as request:
            for line in request.iter_lines():
                if line:
                    yield line

        