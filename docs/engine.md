# ChessIT Engine

An engine that evaluates positions, calculate moves, and makes the move based on evaluation. Written in C++, this engine communicates with Lichess through the website's public facing API endpoints by utilizing the UCI communication protocol.

Users can also interact with the engine through its CLI. See below for instructions.

## How to Use

### Building the Program

The project makes use of Make to compile and run the program.

You can download make here: https://www.gnu.org/software/make/

#### Cleaning the build:

```
make clean
```

#### Running the tests:

This requires the developer to have the googletest dependencies downaloaded. Instructions can be found on the link above.

```
make test
```

#### Running the program:

```
make run
```

### Interacting with the Command Line Interface

Upon running the program, users are given serval of interactive options to choose from. They are first greeted with the following:

```
Chess Engine - type 'help' for commands
>
```

By using the help commmand, they're able to see and choose any of the following process:

```
Chess Engine - type 'help' for commands
> help

Commands:
  perft <depth>           - run perft to depth N
  perft divide <depth>    - run perft divide at depth N
  perft stats <depth>     - get perft stats at depth N
  load <fen>              - load a position from FEN
  display                 - print current board
  reset                   - reset to starting position
  help                    - show this menu
  uci                     - switches into UCI mode
  exit                    - quit

>
```

## Testing

The testing is done with googletest. More information can be found here: https://github.com/google/googletest.
