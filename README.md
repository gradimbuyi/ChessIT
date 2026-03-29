# ChessIT

This project aims to implement a simple chess engine that can be integrated into lichess. In order for it to run efficiently, I've chosen C++ as my development language.

## Minimum Viable Product

Produces a fully functional chess engine that can communicate via UCI protocol and play complete games on the platform Lichess.

- [x] Legal move generation for all chess pieces.
  - Proper check detection
  - Bitboard based board representation
  - Full support for promotion, castling, en passant, and piece movements
- [x] Make and unmake moves.
- [ ] Basic search
  - Implement minimax tree with alpha-beta pruning
  - Fixed search depth (4 or 5)
- [ ] Evaluation function
  - Material based evaluation
  - Standard piece value (eg. pawns == 100)
- [ ] UCI Protocol Support
  - Correct handling of uci, isready, go, stop, quit
  - Outputs valid bestmove
  - Compatible with UCI GUIs and Lichess bot integration

## File Structure

```
├── /.venv          python dependencies
├── /.vscode        vscode config folder
├── /engine         chess program
├── /scripts        potential scripts
├── .env
├── .gitignore
├── bot.py
├── README.md
├── requirements.txt
```
