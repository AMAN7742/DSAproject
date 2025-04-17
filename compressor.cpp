#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <bits/stdc++.h>

using namespace std;

class Node {
public:
    char ch;            
    int freq;          
    Node *left, *right; 

    Node(char ch, int freq) {

        this->ch = ch;
        this->freq = freq;
        left = right = nullptr;
    }
};

class MinHeap {
    vector<Node*> heap;

    void heapifyUp(int index) {
        while (index > 0 && heap[(index - 1) / 2]->freq > heap[index]->freq) {
            swap(heap[index], heap[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    }

    void heapifyDown(int index) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heap.size() && heap[left]->freq < heap[smallest]->freq) {
            smallest = left;
        }
        if (right < heap.size() && heap[right]->freq < heap[smallest]->freq) {
            smallest = right;
        }
        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    
    MinHeap() {}

    bool empty() const {
        return heap.empty();
    }

    int size() const {
        return heap.size();
    }

    void insert(Node* node) {
        heap.push_back(node);
        heapifyUp(heap.size() - 1);
    }

    Node* extractMin() {
        if (heap.empty()) return nullptr;
        Node* minNode = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return minNode;
    }
};

class HuffmanCoding {
    Node* root;              
    
    unordered_map<char, string> huffmanCodes; 
    
    unordered_map<char, int> frequencies;      
    string encodedStr;                      

    void buildFrequencyTable(const string &text) {
        frequencies.clear();
        for (char ch : text) {
            frequencies[ch]++;
        }
    }

    void buildHuffmanTree() {
        deleteTree(root); 
        root = nullptr;
        huffmanCodes.clear();

        vector<pair<char, int>> freqVec(frequencies.begin(), frequencies.end());
        sort(freqVec.begin(), freqVec.end());

        MinHeap minHeap;
        for (const auto& pair : freqVec) {
            minHeap.insert(new Node(pair.first, pair.second));
        }

        
        while (minHeap.size() > 1) {
            Node *left = minHeap.extractMin();
            Node *right = minHeap.extractMin();

            Node *newNode = new Node('\0', left->freq + right->freq);
            newNode->left = left;
            newNode->right = right;
            minHeap.insert(newNode);
        }

        root = minHeap.empty() ? nullptr : minHeap.extractMin();
        buildHuffmanCodes(root, ""); 
    }

    void deleteTree(Node* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    void buildHuffmanCodes(Node* node, string str) {
        if (!node) return;

        if (!node->left && !node->right) {
            huffmanCodes[node->ch] = str;
        }

        buildHuffmanCodes(node->left, str + "0");
        buildHuffmanCodes(node->right, str + "1");
    }

    string getEncodedString(const string &text) {
        string encodedStr = "";
        for (char ch : text) {
            if (huffmanCodes.find(ch) != huffmanCodes.end()) {
                encodedStr += huffmanCodes[ch];
            }
        }
        return encodedStr;
    }

    string decodeString(Node* node, const string &encodedStr) {
        string decodedStr = "";
        Node* current = node;
        
        for (char bit : encodedStr) {
            if (!current) {
                throw runtime_error("Invalid tree state during decoding");
            }

            current = (bit == '0') ? current->left : current->right;

            if (current && !current->left && !current->right) {
                decodedStr += current->ch;
                current = node;
            }
        }

        if (current != node) {
            throw runtime_error("Invalid encoding - incomplete sequence");
        }
        
        return decodedStr;
    }

    void saveEncodedToFile(const string& encodedStr, const string& encodedFile) {
        ofstream outFile(encodedFile, ios::binary);
        if (!outFile) {
            throw runtime_error("Cannot open output file");
        }

        int freqSize = frequencies.size();
        outFile.write(reinterpret_cast<const char*>(&freqSize), sizeof(freqSize));
        
        vector<pair<char, int>> freqVec(frequencies.begin(), frequencies.end());
        sort(freqVec.begin(), freqVec.end());
        
        for (const auto& pair : freqVec) {
            outFile.put(pair.first);
            outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }

        int encodedLength = encodedStr.length();
        outFile.write(reinterpret_cast<const char*>(&encodedLength), sizeof(encodedLength));

        unsigned char buffer = 0;
        int bitCount = 0;
        for (char bit : encodedStr) {
            buffer = (buffer << 1) | (bit == '1');
            bitCount++;
            if (bitCount == 8) {
                outFile.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }

        if (bitCount > 0) {
            buffer <<= (8 - bitCount);
            outFile.put(buffer);
        }

        outFile.close();
    }

    
    void loadEncodedFromFile(const string& encodedFile) {
        ifstream inFile(encodedFile, ios::binary);
        if (!inFile) {
            throw runtime_error("Cannot open input file");
        }

        int freqSize;
        inFile.read(reinterpret_cast<char*>(&freqSize), sizeof(freqSize));
        
        frequencies.clear();
        for (int i = 0; i < freqSize; i++) {
            char ch;
            int freq;
            inFile.get(ch);
            inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
            frequencies[ch] = freq;
        }

        int encodedLength;
        inFile.read(reinterpret_cast<char*>(&encodedLength), sizeof(encodedLength));

        encodedStr = "";
        unsigned char buffer;
        int bitsRead = 0;
        while (inFile.get(reinterpret_cast<char&>(buffer)) && bitsRead < encodedLength) {
            for (int i = 7; i >= 0 && bitsRead < encodedLength; i--) {
                encodedStr += ((buffer >> i) & 1) ? '1' : '0';
                bitsRead++;
            }
        }

        inFile.close();
        buildHuffmanTree();
    }

public:
    
    HuffmanCoding() : root(nullptr) {}
    
    
    ~HuffmanCoding() {
        deleteTree(root);
    }

    void encodeToFile(const string& inputFile, const string& encodedFile) {
        ifstream inFile(inputFile);
        if (!inFile) {
            throw runtime_error("Cannot open input file");
        }

        string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();

        buildFrequencyTable(text);
        buildHuffmanTree();
        string encodedStr = getEncodedString(text);
        saveEncodedToFile(encodedStr, encodedFile);
        
        cout << "File successfully compressed" << endl;
    }

    void decodeFromFile(const string& encodedFile, const string& outputFile) {
        loadEncodedFromFile(encodedFile);
        string decodedStr = decodeString(root, encodedStr);

        ofstream outFile(outputFile);
        if (!outFile) {
            throw runtime_error("Cannot open output file");
        }
        outFile << decodedStr;
        outFile.close();
        
        cout << "File successfully decompressed" << endl;
    }
};

int main() {
    try {
        HuffmanCoding huffman;
        cout << "Do you want to compress or decompress?: ";
        string process;
        cin >> process;

        if (process == "compress") {
            string inputFile, encodedFile;
            cout << "Enter input file name: ";
            cin >> inputFile;
            cout << "Enter output file name: ";
            cin >> encodedFile;
            huffman.encodeToFile(inputFile, encodedFile);
        }
        else if (process == "decompress") {
            string inputFile, decodedFile;
            cout << "Enter input file name: ";
            cin >> inputFile;
            cout << "Enter output file name: ";
            cin >> decodedFile;
            huffman.decodeFromFile(inputFile, decodedFile);
        }
        else {
            cout << "Invalid option. Please choose 'compress' or 'decompress'" << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}