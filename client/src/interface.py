import subprocess

"""
EngineInterface: Launches the engine and uses the UCI protocol to play the game.

The UCI protocol is the standard communication protocol that allows engines to interact with
a Chess GUIs. This interface reads inputs sent by the engine (through stdin), and writes
responses sent by the user (these are the challengers on Lichess). 
"""

class EngineInterface:
    def __init__(self, path):
        self.process = subprocess.Popen(
            [path, "uci"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1
        )

        self._init_uci()

    def _init_uci(self):
        self._send("uci")

        while True:
            line = self._read_line()
            
            if line == "uciok":
                break

        self._send("isready")

        while True:
            line = self._read_line()
            if line == "readyok":
                break

    def _send(self, command):
        self.process.stdin.write(command + "\n")
        self.process.stdin.flush()

    def _read_line(self):
        return self.process.stdout.readline().strip()

    def close(self):
        self._send("quit")
        self.process.terminate()

    def get_best_move(self, moves):
        if moves:
            moves_str = " ".join(moves)
            self._send(f"position startpos moves {moves_str}")

        else:
            self._send("position startpos")

        self._send("isready")
            
        while True:
            line = self._read_line()
            if line == "readyok":
                 break

        self._send("go movetime 1000")

        while True: 
            line = self._read_line()

            if line.startswith("bestmove"):
                return line.split()[1]
    

