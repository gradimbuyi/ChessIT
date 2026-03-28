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
├── /external       external dependencies
├── /scripts        potential python scripts
├── /src            headers and source files
├── /tests          test files
├── .gitignore
├── main.cpp
├── README.md
├── sources.txt
```

## Testing

The program makes use of the Catch2 library to perform integration test. Within the folder `external/catch2`, the files `catch_amalgamated.cpp` and `catch_amalgamated.hpp` allows us to import the library within our project.

These files can be found on the [Catch2 GitHub Repository](https://github.com/catchorg/Catch2).
