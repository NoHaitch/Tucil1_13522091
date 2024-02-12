#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <string>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;
using namespace chrono;

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
        vector<int> rewards;

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
                cout <<"   reward: " << rewards[i] << ", seq: ";
                for(int j = 0; j < seq[i].size()-1; j += 2){
                    cout << seq[i][j] << seq[i][j+1] << " ";
                } cout << endl;
            }
        

            cout << endl;
        }    

        bool solveGame(int *resPoint, string *resPath, vector<vector<int>> *resMatrixPath){ 
            generatePaths();


            if(paths.empty()){
                return 1;
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

            if(maxPoints == 0){
                maxPath = "No Solution";
                maxMatrixPath = {};
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
            if(solveGame(&resPoint, &resPath, &resMatrixPath)){
                cout << "Solving failed" << endl;
                return;
            } 

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            // duration is in microsecond

            cout << "\n\033[1;33m========== RESULT ==========\033[0m" << endl;
            cout << "Max Points: " << resPoint << endl;
            cout << "Buffer : ";
            if(resPath == "No Solution"){
                cout << resPath << endl;
            } else{
                for(int i = 0; i < resPath.size()-1; i += 2){
                    cout << resPath[i] << resPath[i+1] << " ";
                } cout << endl;
            }

            cout << "paths : ";
            if(resMatrixPath.empty()){
                cout << "No Solution" << endl;
            } else{
                cout << endl;
                for(auto& coordinats: resMatrixPath){
                    cout << "" << (coordinats[0] + 1) << ", "<< (coordinats[1] + 1) << endl;
                } cout << endl;
            }
            
            cout << "Time taken: " << int(duration.count()/1000) << " ms\n" << endl;

            while(1){
                string input;
                cout << "Save solution ? (y/n): \033[1;32m";
                cin >> input;
                cout << "\033[0m";
                if(input == "y" || input == "Y" || input == "yes" || input == "Yes"){
                    stringstream outputStream;
                    streambuf* originalCoutBuffer = cout.rdbuf(); 
                    cout.rdbuf(outputStream.rdbuf());

                    cout << resPoint << endl;
                    if(resPath == "No Solution"){
                        cout << resPath << endl;
                    } else{
                        for(int i = 0; i < resPath.size()-1; i += 2){
                            cout << resPath[i] << resPath[i+1] << " ";
                        } cout << endl;
                    }

                    if(resMatrixPath.empty()){
                        cout << "No Solution" << endl;
                    } else{
                        for(auto& coordinats: resMatrixPath){
                            cout << "" << (coordinats[0] + 1) << ", "<< (coordinats[1] + 1) << endl;
                        }
                    }
                    
                    cout << "Time taken: " << int(duration.count()/1000) << " ms";

                    cout.rdbuf(originalCoutBuffer); 

                    saveOutput(outputStream.str());

                    // DATA FOR GUI
                    stringstream outputStreamGUI;
                    streambuf* originalCoutBufferGUI = cout.rdbuf(); 
                    cout.rdbuf(outputStreamGUI.rdbuf());
                    
                    cout << width << " " << height << endl;
                    for (int i = 0; i < height; ++i) {
                        for (int j = 0; j < width; ++j) {
                            cout << matrix[i][j] << " ";
                        }
                        cout << endl;
                    }

                    for (int i = 0; i < seqAmount; ++i) {
                        cout << rewards[i] << " ";
                        for(int j = 0; j < seq[i].size()-1; j += 2){
                            cout << seq[i][j] << seq[i][j+1] << " ";
                        } cout << endl;
                    }

                    cout.rdbuf(originalCoutBufferGUI); 

                    saveOutputGUI(outputStreamGUI.str());

                    break;
                } else if(input == "n" || input == "N" || input == "no" || input == "No"){
                    break;
                }
            }
        }

        void inputIO(){
            string input;
            int tokenAmount;
            int maxSeqSize;
            vector<string> uniqueToken;

            /* Unique Token Amount */
            while(1){
                cout << "\nAmount of Unique Token: \033[1;32m";
                cin >> input;
                cout << "\033[0m";

                if (stringstream(input) >> tokenAmount) {
                    if (tokenAmount > 1) {
                        break;
                    } else {
                        cout << "Invalid input. Token must be a number > 1.\n";
                    }
                } else {
                    cout << "Invalid input. Please enter a number.\n";
                }
            }

            /* Unique Token */
            while(1){
                cout << "Unique Token: \033[1;32m";
                for(int i = 0; i < tokenAmount; i++){
                    string token;
                    cin >> token;
                    if(token.size() != 2){
                        cout << "\033[0mInvalid Input. A token is made of 2 character.\033[1;32m" << endl;
                    } else{
                        uniqueToken.push_back(token);
                    }
                }
                cout << "\033[0m";

                unordered_set<string> seen;
                for(int i = 0; i < tokenAmount; i++){
                    seen.insert(uniqueToken[i]);
                }

                if (seen.size() < uniqueToken.size()) {
                    cout << "Invalid Input. Tokens must all be unique\n";
                } else {
                    break;
                }
            }

            /* Buffer Size*/
            while(1){
                cout << "Buffer Size: \033[1;32m";
                cin >> input;
                cout << "\033[0m";

                if (stringstream(input) >> bufferSize) {
                    if (bufferSize >= 0) {
                        break;
                    } else {
                        cout << "Invalid input. Buffer size must be positif.\n";
                    }
                } else {
                    cout << "Invalid input. Please enter a number.\n";
                }
            }
            
            /* Matrix Size*/
            while(1){
                string input2;
                cout << "Matrix size: \033[1;32m";
                cin >> input >> input2;
                cout << "\033[0m";

                if (stringstream(input) >> width && stringstream(input2) >> height) {
                    if (width >= 1 && height >= 1) {
                        break;
                    } else {
                        cout << "Invalid input. Matrix Width must be positif.\n";
                    }
                } else {
                    cout << "Invalid input. Please enter a number.\n";
                }
            }
            
            /* Sequence Amount*/
            while(1){
                cout << "Sequence Amount: \033[1;32m";
                cin >> input;
                cout << "\033[0m";

                if (stringstream(input) >> seqAmount) {
                    if (seqAmount >= 1) {
                        break;
                    } else {
                        cout << "Invalid input. Max Sequence Size must be positif.\n";
                    }
                } else {
                    cout << "Invalid input. Please enter a number.\n";
                }
            }
            
            /* Max Sequence Size*/
            while(1){
                cout << "Max Sequence Size: \033[1;32m";
                cin >> input;
                cout << "\033[0m";

                if (stringstream(input) >> maxSeqSize) {
                    if (maxSeqSize >= 1) {
                        break;
                    } else {
                        cout << "Invalid input. Max Sequence Size must be positif.\n";
                    }
                } else {
                    cout << "Invalid input. Please enter a number.\n";
                }
            }

            genMatrix(uniqueToken);
            genSeq(uniqueToken, maxSeqSize);
        }

    private:
        void saveOutput(string outputStream) {
            string baseFilename = "output";
            string filename = "../test/" + baseFilename + ".txt";

            ofstream file(filename);
            file << outputStream;
            file.close();

            cout << "Solution saved to " << filename << endl;
        }

        void saveOutputGUI(string outputStream) {
            string filename = "../bin/temp";

            ofstream file(filename);
            file << outputStream;
            file.close();
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
                for(int x = 0; x < width; x++){
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
                    string temp = currPath + matrix[lastSignificantIndex][x];
                    genPaths(currBuffer + 1, x, temp, seenPath);
                    seenPath.pop_back();
                }
            }
        }

        void genSeq(vector<string> uniqueToken, int maxSeqSize){
            for(int i = 0; i < seqAmount; i++){
                seqLen.push_back((rand() % (maxSeqSize)) + 1);
                rewards.push_back((rand() % (40)) + 10);
                
                string temp;
                for(int j = 0; j < seqLen[i]; j++){
                    temp += uniqueToken[rand() % uniqueToken.size()];
                }
                seq.push_back(temp);
            }

            for(int i = 0; i < seqAmount; i++){
                seqLen[i] = seqLen[i]*2;
            }
        }

        void genMatrix(vector<string> uniqueToken){
            for(int i = 0; i < height; i++){
                vector<string> row;
                for(int j = 0; j < width; j++){
                    row.push_back(uniqueToken[rand() % uniqueToken.size()]);
                }
                matrix.push_back(row);
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
                            points += rewards[j];
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

bool readFile(Game& game) {
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
                int bSize;

                if (!(ss >> bSize)) {
                    cerr << "Error parsing buffer size on line " << lineNumber << endl;
                    return 0;
                }

                char remainingChar;
                if (ss >> remainingChar) {
                    cerr << "Error parsing buffer size on line " << lineNumber << endl;
                    return 0;
                }

                if(bSize < 0){
                    cerr << "Error buffer size must be non-negatif" << endl;
                    return 0;
                }

                game.bufferSize = bSize;
            }

            // Parse matrix_width and matrix_height
            else if (lineNumber == 2) {
                if (!(ss >> game.width >> game.height)) {
                    cerr << "Error parsing matrix dimensions on line " << lineNumber << endl;
                    return 0;
                }
                
                char remainingChar;
                if (ss >> remainingChar) {
                    cerr << "Error parsing matrix dimensions on line " << lineNumber << endl;
                    return 0;
                }

                if(game.width <= 0 || game.height <= 0){
                    cerr << "Error matrix size must be a positif number" << endl;
                    return 0;
                }

                game.matrix.resize(game.height, vector<string>(game.width));
            }

            // Parse matrix elements
            else if (lineNumber <= game.height + 2) {
                int row = lineNumber - 3;
                int col = 0;
                while (ss >> token) {
                    if (col >= game.width) {
                        cerr << "Error: Incorrect number of elements on line" << lineNumber << endl;
                        return 0;
                    }

                    if(token.size() != 2){
                        cerr << "Error parsing matrix on line " << lineNumber << endl;
                        return 0;
                    }
                    game.matrix[row][col++] = token;
                }

            }

            // Parse number_of_sequences
            else if (lineNumber == game.height + 3) {
                if (!(ss >> token) || !(stringstream(token) >> game.seqAmount)) {
                    cerr << "Error parsing amount of sequance on line " << lineNumber << endl;
                    return 0;
                }

                char remainingChar;
                if (ss >> remainingChar) {
                    cerr << "Error parsing amount of sequance on line " << lineNumber << endl;
                    return 0;
                }

                if(game.seqAmount <= 0){
                    cerr << "Error amount of sequance must be at least 1" << endl;
                    return 0;
                }
            }

            // Parse sequences and rewards
            else if (lineNumber >= game.height + 4 && lineNumber <= game.height + 4 + 2 * game.seqAmount) {
                int seqIndex = (lineNumber - game.height - 4) / 2;
                bool isSequence = (lineNumber - game.height - 4) % 2 == 0;

                if (isSequence) {
                    string sequenceStr;
                    while (ss >> token) {
                        if(token.size() != 2){
                            cerr << "Error parsing sequance on line " << lineNumber << endl;
                            return 0;
                        }
                        sequenceStr += token;
                    }
                    game.seq.push_back(sequenceStr);
                } else {
                    int reward;
                    if (!(ss >> reward)){
                        cerr << "Error parsing reward for sequence " << seqIndex << endl;
                        return 0;
                    }
                    game.rewards.push_back(reward);

                    char remainingChar;
                    if (ss >> remainingChar) {
                        cerr << "Error parsing sequance reward on line " << lineNumber << endl;
                        return 0;
                    }
                }

            } else {
                cerr << "Error: Unexpected line content on line " << lineNumber << endl;
                return 0;
            }

            lineNumber++;
        }

        MyReadFile.close();
    } else {
        cerr << "Error opening file!" << endl;
        return 0; 
    }

    // Get sequnce string len
    for(int i = 0; i < game.seqAmount; i++){
        game.seqLen.push_back(game.seq[i].size());
    }

    // Get max points possible
    game.maxPointsPossible = 0;
    for(int i = 0; i < game.seqAmount; i++){
        game.maxPointsPossible += game.rewards[i];
    }

    return 1;

}

bool readGameFromFile(const string& filename, Game& game) {
    // Check file extension
    if (filename.substr(filename.find_last_of('.') + 1) != "txt") {
        cerr << "Error: Only .txt files are supported." << endl;
        return false;
    }

    ifstream MyReadFile(filename);
    if (!MyReadFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return false;
    }

    string line;
    int lineNumber = 1;

    while (getline(MyReadFile, line)) {
        stringstream ss(line);

        // Parse buffer_size
        if (lineNumber == 1) {
            int bSize;
            if (!(ss >> bSize)) {
                cerr << "Error parsing buffer_size on line " << lineNumber << endl;
                return false;
            }
            game.bufferSize = bSize;
        }

        // Parse matrix_width and matrix_height
        else if (lineNumber == 2) {
            int w, h;
            if (!(ss >> w >> h)) {
                cerr << "Error parsing matrix dimensions on line " << lineNumber << endl;
                return false;
            }
            game.width = w;
            game.height = h;
            game.matrix.resize(h, vector<string>(w));
        }

        // Parse matrix elements
        else if (lineNumber <= game.height + 2) {
            int row = lineNumber - 3;
            int col = 0;
            string token;
            while (ss >> token) {
                if (col >= game.width) {
                    cerr << "Error: Too many elements in row " << row << endl;
                    return false;
                }
                game.matrix[row][col++] = token;
            }
            if (col != game.width) {
                cerr << "Error: Missing elements in row " << row << endl;
                return false;
            }
        }

        // Parse number_of_sequences
        else if (lineNumber == game.height + 3) {
            int seqNum;
            if (!(ss >> seqNum)) {
                cerr << "Error parsing number_of_sequences on line " << lineNumber << endl;
                return false;
            }
            game.seqAmount = seqNum;
        }

        // Parse sequences and rewards
        else if (lineNumber >= game.height + 4 && lineNumber <= game.height + 4 + 2 * game.seqAmount) {
            int seqIndex = (lineNumber - game.height - 4) / 2;
            bool isSequence = (lineNumber - game.height - 4) % 2 == 0;

            if (isSequence) {
                game.seq.push_back(line); // Store entire sequence string
            } else {
                int reward;
                if (!(ss >> reward)) {
                    cerr << "Error parsing reward for sequence " << seqIndex << endl;
                    return false;
                }
                game.rewards.push_back(reward);
            }
        }

        else {
            cerr << "Error: Unexpected line content on line " << lineNumber << endl;
            return false;
        }

        lineNumber++;
    }

    MyReadFile.close();
    return true;
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
            if(!readFile(game)){
                cout << "File read failed.\n" << endl;
            } else{
                game.printGameVar();
                game.solveGameIO();
            }
        } else if (inputMethod == "2"){
            game.inputIO();
            game.printGameVar();
            game.solveGameIO();
        } else if (inputMethod == "3"){
            break;
        } else{
            cout << "Invalid input. Please input the number.\n" << endl;
            game.printGameVar();
        }
    }

    cout << "Program exited" << endl;
    return 0;
}