#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>
#include <algorithm>

using namespace std;
using namespace std::chrono;

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
        vector<vector<vector<int>>> matrixPaths;
        vector<int> seqLen;
        vector<int> prize;

    Game() : bufferSize(0), width(0), height(0), seqAmount(0) {}

    public:
        void printGameVar(){
            cout << "\n\033[1;33m========== Game Variables ==========\033[0m" << endl;
            cout << "> bufferSize: " << bufferSize << endl;
            cout << "> width: " << width << endl;
            cout << "> height: " << height << endl;
            cout << "> seqAmount: " << seqAmount << endl;

            // Print matrixs
            cout << "> matrix:" << endl;
            for (int i = 0; i < height; ++i) {
                cout << "   ";
                for (int j = 0; j < width; ++j) {
                    cout << matrix[i][j] << " ";
                }
                cout << endl;
            }

            // Print sequences
            cout << "> sequences: " << endl;
            for (int i = 0; i < seqAmount; ++i) {
                cout <<"   prize: " << prize[i] << ", seq: ";
                for(int j = 0; j < seq[i].size()-1; j += 2){
                    cout << seq[i][j] << seq[i][j+1] << " ";
                } cout << endl;
            }
        

            cout << endl;
        }    

        int solveGame(int *resPoint, string *resPath, vector<vector<int>> *resMatrixPath){
            generatePaths();

            if(paths.empty()){
                return -1;
            }

            int maxPoints = 0;
            string maxPath;
            vector<vector<int>> maxMatrixPath;
            
            for(int i = 0; i < paths.size(); i++){
                string path = paths[i];
                int point = pathToPoints(path);

                if(point >= maxPoints && point != 0){
                    if(maxPoints == 0 || point > maxPoints){
                        maxPoints = point;
                        maxPath = cleanNonUsedBuffer(path);
                        maxMatrixPath = cleanedNonUsedMatrixPath(maxPath, matrixPaths[i]);
                    } else{
                        string cleandePath = cleanNonUsedBuffer(path);
                        
                        if(cleandePath.size() < maxPath.size()){
                            maxPoints = point;
                            maxPath = cleandePath;
                            maxMatrixPath = cleanedNonUsedMatrixPath(maxPath, matrixPaths[i]);
                        }
                    }
                }
            }

            *resPoint = maxPoints;
            *resPath = maxPath;
            *resMatrixPath = maxMatrixPath;
            return 0;
        }

        void solveGameIO(){
            auto start = high_resolution_clock::now();

            int resPoint;
            string resPath;
            vector<vector<int>> resMatrixPath;
            if(solveGame(&resPoint, &resPath, &resMatrixPath) == -1){
                cout << "Solving failed" << endl;
                return;
            } 

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            // duration is in microsecond

            cout << "\n\033[1;33m========== RESULT ==========\033[0m" << endl;
            cout << "Max Points: " << resPoint << endl;
            cout << "Buffer : ";
            for(int i = 0; i < resPath.size()-1; i += 2){
                cout << resPath[i] << resPath[i+1] << " ";
            } cout << endl;

            cout << "paths : " << endl;
            for(auto& coordinats: resMatrixPath){
                cout << "" << (coordinats[0] + 1) << ", "<< (coordinats[1] + 1) << endl;
            } cout << endl;

            cout << "Time taken: " << int(duration.count()/1000) << " ms\n" << endl;

            while(1){
                string input;
                cout << "Save solution ?[y/n]\033[1;32m" << endl;
                cin >> input;
                cout << "\033[0m";
                if(input == "y" || input == "Y" || input == "yes" || input == "Yes"){
                    std::stringstream outputStream;
                    std::streambuf* originalCoutBuffer = std::cout.rdbuf(); 
                    std::cout.rdbuf(outputStream.rdbuf());

                    cout << resPoint << endl;
                    for(int i = 0; i < resPath.size()-1; i += 2){
                        cout << resPath[i] << resPath[i+1] << " ";
                    } cout << endl;

                    for(auto& coordinats: resMatrixPath){
                        cout << "" << (coordinats[0] + 1) << ", "<< (coordinats[1] + 1) << endl;
                    } cout << endl;

                    cout << int(duration.count()/1000) << " ms\n" << endl;

                    std::cout.rdbuf(originalCoutBuffer); 

                    saveOutput(outputStream.str());
                    break;
                }
            }
        }

    private:
        void saveOutput(string outputStream) {
            string baseFilename = "output";
            string filename = "../test/" + baseFilename + ".txt";

            std::ofstream file(filename);
            file << outputStream;
            file.close();

            std::cout << "Solution saved to " << filename << std::endl;
        }

        void generatePaths(){
            if(paths.empty()){
                genPaths(0, 0, "", {});
            }
        }
    
        void genPaths(int currBuffer, int lastSignificantIndex, string currPath, vector<vector<int>> seenPath){
            if(currBuffer == bufferSize){
                paths.push_back(currPath);
                matrixPaths.push_back(seenPath);
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

        vector<vector<int>> cleanedNonUsedMatrixPath(string cleanedBuffer, vector<vector<int>> matrixPath){
            while(matrixPath.size()*2 > cleanedBuffer.size()){
                matrixPath.pop_back();
            }
            return matrixPath;
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
    while(1){
        string inputMethod;
        Game game;

        cout << "\n====== Welcome to Breach Protocol Solver ======" << endl;
        cout << "Pick your method of input:\n1. Text File\n2. CLI\n3. Exit" << endl;
        cout << "\033[1;32m>>> \033[0m";
        cin >> inputMethod;

        if(inputMethod == "1"){
            if(readFile(game) == -1){
                cout << "File read failed. Wrong file format\n" << endl;
            } else{
                game.printGameVar();
                game.solveGameIO();
            }
        } else if (inputMethod == "2"){
            
        } else if (inputMethod == "3"){
            break;
        } else{
            cout << "Invalid input. Please input the number.\n" << endl;
        }
    }

    cout << "Program exited" << endl;
    return 0;
}