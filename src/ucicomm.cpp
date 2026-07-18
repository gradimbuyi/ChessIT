#include "../include/ucicomm.hpp"

UCIComm::UCIComm(Bitboard &bitboard) : bitboard(bitboard) {
    std::srand(std::time(nullptr));
}

Move UCIComm::parseMoveString(const std::string &move_string) {
    if(move_string.size() < 4) return Move(0, 0, QUIET);

    int  from_file = move_string[0] - 'a';
    int  from_rank = move_string[1] - '1';
    int  to_file   = move_string[2] - 'a';
    int  to_rank   = move_string[3] - '1';
    int  from      = from_rank * 8 + from_file;
    int  to        = to_rank   * 8 + to_file;
    char promo     = move_string.size() == 5 ? move_string[4] : ' ';
    
    std::vector<Move> moves;
    
    movegen.generateMoves(bitboard, moves);

    for(const Move &move : moves) {
        if((int) move.getFrom() != from || move.getTo() != to) continue ;
        if(move.getFrom() == -1) continue;

        int flag = move.getFlag();

        if(promo != ' ') {
            if(promo == 'q' && (flag == QUEEN_PROMOTION  || flag == QUEEN_PROMO_CAPTURE))  return move;
            if(promo == 'r' && (flag == ROOK_PROMOTION   || flag == ROOK_PROMO_CAPTURE))   return move;
            if(promo == 'b' && (flag == BISHOP_PROMOTION || flag == BISHOP_PROMO_CAPTURE)) return move;
            if(promo == 'n' && (flag == KNIGHT_PROMOTION || flag == KNIGHT_PROMO_CAPTURE)) return move;
        } else {
            return move;
        }
    }

    return Move(-1, -1, QUIET);
}

Move UCIComm::pickRandomMove() {
    std::vector<Move> moves;
    movegen.generateMoves(bitboard, moves);
    if(moves.empty()) return Move(0, 0, QUIET);
    return moves[std::rand() % moves.size()];
}

Move UCIComm::pickBestMove(int depth) {
    Eval eval(bitboard, movegen);
    return eval.search(depth);
}

void UCIComm::positionHandler(std::istringstream &iss) {
    std::string token;
    iss >> token;

    if(token == "startpos") {
        bitboard.loadFEN(STARTPOS);
        iss >> token;
    } 
    
    else if(token == "fen") {
        std::string fen;

        while(iss >> token && token != "moves") {
            fen += token + " ";
        }

        bitboard.loadFEN(fen);
    } 

    if(token == "moves") {
        while(iss >> token) {
            Move move = parseMoveString(token);
            bitboard.makeMove(move);
        }
    }
}

void UCIComm::goHandler(std::istringstream &iss) {
    std::string token;
    int depth = 3; 

    while(iss >> token) {
        if(token == "depth" && (iss >> depth)) {
            depth = std::min(depth, 5);  
        }
    }

    //Move best = pickBestMove(depth);
    Move best = pickBestMove(depth);
    
    int from = best.getFrom();
    int to   = best.getTo();
    int flag = best.getFlag();

    std::cout << "bestmove " << (char)('a' + (from & 7)) << (char)('1' + (from >> 3)) 
                             << (char)('a' + (to   & 7)) << (char)('1' + (to   >> 3));

    switch(flag) {
        case KNIGHT_PROMOTION : case KNIGHT_PROMO_CAPTURE : std::cout << 'n'; break;
        case BISHOP_PROMOTION : case BISHOP_PROMO_CAPTURE : std::cout << 'b'; break;
        case ROOK_PROMOTION   : case ROOK_PROMO_CAPTURE   : std::cout << 'r'; break;
        case QUEEN_PROMOTION  : case QUEEN_PROMO_CAPTURE  : std::cout << 'q'; break;
        default: break;
    }

    std::cout << "\n";
}

void UCIComm::run() {
    std::cout << "id name "   << ENGINE_NAME  << "\n";
    std::cout << "id author " << AUTHROR_NAME << "\n";
    std::cout << "option name Move Overhead type spin default 10 min 0 max 5000\n";
    std::cout << "option name Threads type spin default 1 min 1 max 128\n";
    std::cout << "option name Hash type spin default 16 min 1 max 33554432\n";
    std::cout << "uciok\n";

    std::string line;

    while(std::getline(std::cin, line)) {
        if(line.empty()) continue;

        std::istringstream iss(line);
        std::string        command;
        
        iss >> command;

        if(command == "quit") {
            std::cout.flush();
            std::exit(0); 
        }

        else if(command == "isready")    std::cout << "readyok\n";
        else if(command == "ucinewgame") bitboard.loadFEN(STARTPOS);
        else if(command == "position")   positionHandler(iss);
        else if(command == "go")         goHandler(iss);
        else if(command == "stop") ;
        else if(command == "setoption") ;

        std::cout.flush();
    }
}