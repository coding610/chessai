#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include "chess.hpp"

class Diagnostics {
private:
    std::string sf = "diagnostics/search_logs.txt";
    std::string dsf = "diagnostics/depth_search_logs.txt";
    std::string ldsf = "diagnostics/low_depth_search_logs.txt";

public:
    // Engine
    int positions_searched = 0;
    int engine_move_index = 0;
    int ab = 0;
    bool write_low_depth_search_log = false;

    std::vector<chess::Move> path;
    std::vector<std::vector<chess::Move>> long_path;

public:
    void new_move() {
        this->positions_searched = 0;
        this->ab = 0;
        this->engine_move_index++;
    }

    void clear_search_log() {
        std::ofstream ofs;
        ofs.open("diagnostics/search_logs.txt", std::ofstream::out | std::ofstream::trunc);
        ofs.close();

        std::ofstream ofs2;
        ofs2.open("diagnostics/depth_search_logs.txt", std::ofstream::out | std::ofstream::trunc);
        ofs2.close();

        std::ofstream ofs3;
        ofs3.open("diagnostics/low_depth_search_logs.txt", std::ofstream::out | std::ofstream::trunc);
        ofs3.close();

        std::ofstream ofs4;
        ofs4.open("diagnostics/depth_all_search_logs.txt", std::ofstream::out | std::ofstream::trunc);
        ofs4.close();
    }

    void write_search_log(
        chess::Color engine_color,
        chess::Color search_color,
        int depth,
        chess::Move best_move,
        float evaluation,
        bool path
    ) {
        std::ofstream dia_file;
        dia_file.open("diagnostics/depth_search_logs.txt", std::ios::app);

        // --- Engine Color: WHITE -- Search Color: BLACK -- Depth: 3 -- Best_move: a2a3 -- Evaluation: -1.0 ---
        dia_file << "--- Engine Color: " << engine_color
                 << " -- Search Color: " << search_color
                 << " -- Depth: "        << depth
                 << " -- Current αβ pruning index: " << this->ab
                 << " -- Best Move: "    << best_move
                 << " -- Evaluation: "   << evaluation
                 << " -- Move Index: " << this->engine_move_index
                 << "\n";
        if (path) {
            dia_file << "---";
            for (auto& m : this->path) {
                dia_file << " - " << m;
            }
            dia_file << " ---\n";
        }

        dia_file.close();
    }

    void write_all_search_log(
        chess::Color engine_color,
        chess::Color search_color,
        int depth,
        chess::Move best_move,
        float evaluation,
        bool path
    ) {
        std::ofstream dia_file;
        dia_file.open("diagnostics/depth_all_search_logs.txt", std::ios::app);

        // --- Engine Color: WHITE -- Search Color: BLACK -- Depth: 3 -- Best_move: a2a3 -- Evaluation: -1.0 ---
        dia_file << "--- Engine Color: " << engine_color
                 << " -- Search Color: " << search_color
                 << " -- Depth: "        << depth
                 << " -- Current αβ pruning index: " << this->ab
                 << " -- Move: "    << best_move
                 << " -- Evaluation: "   << evaluation
                 << " -- Move Index: " << this->engine_move_index
                 << "\n";

        if (path) {
            dia_file << "---";
            for (auto& m : this->path) {
                dia_file << " - " << m;
            }
            dia_file << " ---\n";
        }

        dia_file.close();
    }

    void write_custom_search_log(chess::Color color, chess::Move best_move, auto time_end, auto time_begin) {
        std::ofstream df;
        df.open("diagnostics/search_logs.txt", std::ios::app);
        df << "\n--- " << color << " Engine Evaluation Finished -- Current Move: " << this->engine_move_index << " ---\n"
                    << "--- Position searched: " << this->positions_searched << " ---\n"
                    << "--- αβ Pruning Index: " << this->ab << " ---\n"
                    << "--- Duration: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count() << "ms ---\n"
                    << "--- Final Move Chooosen: " << best_move << " ---\n\n";

        df.close();
    }
};
