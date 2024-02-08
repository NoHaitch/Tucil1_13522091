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
        int maxPointsPossible;
        vector<vector<string>> matrix;
        vector<string> seq;
        vector<string> paths;
        vector<int> seqLen;
        vector<int> prize;

    Game() : bufferSize(0), width(0), height(0), seqAmount(0) {}

    public:
        void printGameVar(){
            cout << "=== Game object variables ===" << endl;
            cout << "- bufferSize: " << bufferSize << endl;
            cout << "- width: " << width << endl;
            cout << "- height: " << height << endl;
            cout << "- seqAmount: " << seqAmount << endl;

            // Print matrixs
            cout << "- matrix:" << endl;
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    cout << matrix[i][j] << " ";
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

        void solveGame(int *resPoint, string *resPath){
            int maxPoints = 0;
            string maxPath;

            generatePaths();

            if(paths.empty()){
                return;
            }
            
            for(string path: paths){
                int point = pathToPoints(path);
                if(point >= maxPoints && point != 0){
                    cout << path << " : " << point << endl;
                    string cleandePath = cleanNonUsedBuffer(path);
                    if(cleandePath.size() < maxPath.size() || maxPath.empty()){
                        maxPoints = point;
                        maxPath = cleandePath;
                    }
                }
            }

            *resPoint = maxPoints;
            *resPath = maxPath;
        }

    private:
        void generatePaths(){
            if(paths.empty()){
                genPaths(0, 0, "", {});
            }
        }
    
        void genPaths(int currBuffer, int lastSignificantIndex, string currPath, vector<vector<int>> seenPath){
            if(currBuffer == bufferSize){
                paths.push_back(currPath);
                return;
            }

            if(currBuffer == 0){
                // first move
                for(int x = 0; x < width; x++){
                    genPaths(1, x, matrix[0][x], {{x,0}});
                }

            } else if(currBuffer % 2 == 1){
                // move vertical
                for(int y = 0; y < height; y++){
                    bool seen = false;
                    for(auto& coordinats : seenPath){
                        if(coordinats[0] == lastSignificantIndex && coordinats[1] == y){
                            seen = true;
                            break;
                        }
                    }
                    if(seen){
                        continue;
                    }
                    seenPath.push_back({lastSignificantIndex, y});
                    genPaths(currBuffer + 1, y, currPath + matrix[y][lastSignificantIndex], seenPath);
                    seenPath.pop_back();
                }

            } else{
                // move horizontal
                for(int x = 0; x < height; x++){
                    bool seen = false;
                    for(auto& coordinats : seenPath){
                        if(coordinats[0] == x && coordinats[1] == lastSignificantIndex){
                            seen = true;
                            break;
                        }
                    }
                    if(seen){
                        continue;
                    }
                    seenPath.push_back({x, lastSignificantIndex});
                    genPaths(currBuffer + 1, x, currPath + matrix[lastSignificantIndex][x], seenPath);
                    seenPath.pop_back();
                }
            }
        }
    
        int pathToPoints(string path){
            int points = 0;
            int pathLen = path.size();
            bool seqUsed[seqAmount] {false};

            int i = 0;
            while(i < pathLen){
                for(int j = 0; j < seqAmount; j++){
                    if(path[i] == seq[j][0] && !seqUsed[j] && pathLen-i+1 >= seqLen[j]){
                        int k = 1;
                        for(; k < seqLen[j]; k++){
                            if(path[i+k] != seq[j][k]){
                                break;
                            }
                        }

                        if(k == seqLen[j]){
                            points += prize[j];
                            seqUsed[j] = true;
                        }
                    }
                }
                
                i++;
            }

            return points;
        }

        string cleanNonUsedBuffer(string path){
            int lastIdx = 0;
            int points = 0;
            int pathLen = path.size();
            bool seqUsed[seqAmount] {false};
            int i = 0;
            while(i < pathLen){
                for(int j = 0; j < seqAmount; j++){
                    if(path[i] == seq[j][0] && !seqUsed[j] && pathLen-i+1 >= seqLen[j]){
                        int k = 1;
                        for(; k < seqLen[j]; k++){
                            if(path[i+k] != seq[j][k]){
                                break;
                            }
                        }

                        if(k == seqLen[j]){
                            lastIdx = i+k;
                            seqUsed[j] = true;
                        }
                    }
                }
                
                i++;
            }

            return path.erase(lastIdx,path.size()-1);
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
                // Resize matrix vector based on parsed dimensions
                game.matrix.resize(game.height, vector<string>(game.width));
            }

            // Parse matrix elements
            else if (lineNumber <= game.height + 2) {
                int row = lineNumber - 3;
                int col = 0;
                while (ss >> token) {
                    game.matrix[row][col++] = token;
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

    // Get sequnce string len
    for(int i = 0; i < game.seqAmount; i++){
        game.seqLen.push_back(game.seq[i].size());
    }

    // Get max points possible
    game.maxPointsPossible = 0;
    for(int i = 0; i < game.seqAmount; i++){
        game.maxPointsPossible += game.prize[i];
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
                game.printGameVar();
                
            }
        } else if (inputMethod == "2"){
            int maxPoints;
            string path;
            game.solveGame(&maxPoints, &path);

            cout << "Points : " << maxPoints << endl;
            cout << "Path : " << path << endl;

        } else{
            cout << "Invalid input. Please input the number.\n" << endl;
        }
    }



    // game.printGameVar();


    return 0;
}