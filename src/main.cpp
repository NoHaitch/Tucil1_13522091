#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
using namespace std;

class Game{
    public:
        int bufferSize;
        int width;
        int height;
        int seqAmount;
        vector<vector<string>> board;
        vector<string> seq;
        vector<int> prize;

    Game() : bufferSize(0), width(0), height(0), seqAmount(0) {}

    public:
        void printGameVar(){
            cout << "=== Game object variables ===" << endl;
            cout << "- bufferSize: " << bufferSize << endl;
            cout << "- width: " << width << endl;
            cout << "- height: " << height << endl;
            cout << "- seqAmount: " << seqAmount << endl;

            // Print boards
            cout << "- board:" << endl;
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    cout << board[i][j] << " ";
                }
                cout << endl;
            }

            // Print sequences
            cout << "- seq:" << endl;
            for (int i = 0; i < seqAmount; ++i) {
                cout << seq[i] << endl;
            }

            // Print prizes
            cout << "- prize:" << endl;
            for (int i = 0; i < prize.size(); ++i) {
                cout << prize[i] << " ";
            }
            cout << endl;
        }    

}; 

int readFile(Game& game) {
    string fileName;

    while (true) {
        cout << "\nFile name: \033[1;32m";
        cin >> fileName;
        cout << "\033[0m";

        if (filesystem::exists("../test/" + fileName)) {
            break;
        } else {
            cerr << "Error: File not found. Please try again." << endl;
        }
    }

    ifstream MyReadFile("../test/" + fileName);

    if (MyReadFile.is_open()) {
        string line;
        int lineNumber = 1;

        while (getline(MyReadFile, line)) {
            stringstream ss(line);
            string token;

            // Parse buffer_size
            if (lineNumber == 1) {
                if (!(ss >> token) || !(stringstream(token) >> game.bufferSize)) {
                    cerr << "Error parsing buffer_size on line " << lineNumber << endl;
                    return -1;
                }
            }

            // Parse matrix_width and matrix_height
            else if (lineNumber == 2) {
                if (!(ss >> game.width >> game.height)) {
                    cerr << "Error parsing matrix dimensions on line " << lineNumber << endl;
                    return -1;
                }
                // Resize board vector based on parsed dimensions
                game.board.resize(game.height, vector<string>(game.width));
            }

            // Parse matrix elements
            else if (lineNumber <= game.height + 2) {
                int row = lineNumber - 3;
                int col = 0;
                while (ss >> token) {
                    game.board[row][col++] = token;
                }
                if (col != game.width) {
                    cerr << "Error: Incorrect number of elements in row " << row << endl;
                    return -1;
                }
            }

            // Parse number_of_sequences
            else if (lineNumber == game.height + 3) {
                if (!(ss >> token) || !(stringstream(token) >> game.seqAmount)) {
                    cerr << "Error parsing number_of_sequences on line " << lineNumber << endl;
                    return -1;
                }
            }

            // Parse sequences and rewards
            else if (lineNumber >= game.height + 4 && lineNumber <= game.height + 4 + 2 * game.seqAmount) {
                int seqIndex = (lineNumber - game.height - 4) / 2;
                bool isSequence = (lineNumber - game.height - 4) % 2 == 0;

                if (isSequence) {
                    string sequenceStr;
                    while (ss >> token) {
                        sequenceStr += token; // No space, concatenate tokens directly
                    }
                    game.seq.push_back(sequenceStr);
                } else {
                    int reward;
                    if (!(ss >> reward)){
                        cerr << "Error parsing reward for sequence " << seqIndex << endl;
                        return -1;
                    }
                    game.prize.push_back(reward);
                }
            } else {
                cerr << "Error: Unexpected line content on line " << lineNumber << endl;
                return -1;
            }

            lineNumber++;
        }

        MyReadFile.close();
    } else {
        cerr << "Error opening file!" << endl;
        return -1;
    }

    return 0;

}

int main(){
    Game game;
    cout << "\n====== Welcome to Breach Protocol ======" << endl;
    cout << "Pick your method of input:\n1. Text File\n2. CLI" << endl;
    
    string inputMethod;
    while(1){
        cout << "\033[1;32m>>> \033[0m";
        cin >> inputMethod;

        if(inputMethod == "1"){
            if(readFile(game) == -1){
                cout << "File read failed. Wrong file format\n" << endl;
            } else{
                
            }
        } else if (inputMethod == "2"){
            // TODO
            break;
        } else{
            cout << "Invalid input. Please input the number.\n" << endl;
        }
    }



    game.printGameVar();


    return 0;
}