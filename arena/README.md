# Arena MVP

A command-line tool that plays games between two UCI chess engines.

## Requirements

Launch two UCI engines.
Play N games.
Alternate colors.
Support a configurable time control.
Display wins, losses, and draws.
Optionally save games as PGN.

## Example

```
Arena --engine ./ChessIT --opponent stockfish --games 100 --tc 5+0.1
```