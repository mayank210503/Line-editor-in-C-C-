#include <iostream>
#include <string>
#include <vector>
#include <stack>

class Cursor {
public:
    int lineNumber;
    int position;
    
    Cursor(int line = -1, int pos = -1) : lineNumber(line), position(pos) {}
};

class BufferState {
public:
    std::vector<std::string> lines;
    char operation;  // 'I': Insert, 'D': Delete, 'U': Update

    BufferState(const std::vector<std::string>& l, char op) 
        : lines(l), operation(op) {}
};

class LineEditor {
private:
    static const int MAX_LINES = 25;
    static const int UNDO_LIMIT = 3;
    
    std::vector<std::string> buffer;
    std::stack<BufferState> undoStack;
    std::stack<BufferState> redoStack;

public:
    LineEditor() {
        // Initialize buffer with empty lines up to MAX_LINES
        buffer.reserve(MAX_LINES);
    }

    void insertLine(int position, const std::string& text) {
        if (buffer.size() >= MAX_LINES) {
            throw std::runtime_error("Cannot insert line: buffer is full (maximum 25 lines)");
        }

        // If trying to insert beyond current size, fill with empty lines first
        while (position > buffer.size()) {
            buffer.push_back("");
        }

        saveBufferState('I');
        if (position == buffer.size()) {
            buffer.push_back(text);
        } else {
            buffer.insert(buffer.begin() + position, text);
        }
        
        std::cout << "Line inserted successfully. Buffer size: " << buffer.size() << "/" << MAX_LINES << std::endl;
    }

    void insertWord(const Cursor& cursor, const std::string& word) {
        if (cursor.lineNumber >= 0 && cursor.lineNumber < buffer.size()) {
            saveBufferState('U');
            buffer[cursor.lineNumber].insert(cursor.position, word);
        }
    }

    Cursor searchWord(const std::string& word) {
        for (size_t i = 0; i < buffer.size(); ++i) {
            size_t pos = buffer[i].find(word);
            if (pos != std::string::npos) {
                return Cursor(i, pos);
            }
        }
        return Cursor();
    }

    void displayLine(int lineNumber) const {
        if (lineNumber >= 0 && lineNumber < buffer.size()) {
            std::cout << "[Line No" << lineNumber + 1 << ":] " 
                     << (buffer[lineNumber].empty() ? "(empty)" : buffer[lineNumber]) 
                     << std::endl;
        }
    }

    void displayBuffer() const {
        if (buffer.empty()) {
            std::cout << "Buffer is empty\n";
            return;
        }
        std::cout << "\nBuffer contents (" << buffer.size() << "/" << MAX_LINES << " lines):\n";
        for (size_t i = 0; i < buffer.size(); ++i) {
            displayLine(i);
        }
    }

    void updateWord(const Cursor& cursor, const std::string& oldWord, 
                   const std::string& newWord) {
        if (cursor.lineNumber >= 0 && cursor.lineNumber < buffer.size()) {
            saveBufferState('U');
            std::string& line = buffer[cursor.lineNumber];
            size_t pos = line.find(oldWord, cursor.position);
            if (pos != std::string::npos) {
                line.replace(pos, oldWord.length(), newWord);
            }
        }
    }

    void updateLine(int lineNumber, int startPos, const std::string& newText) {
        if (lineNumber < 0 || lineNumber >= buffer.size()) {
            throw std::out_of_range("Invalid line number");
        }

        saveBufferState('U');
        std::string& line = buffer[lineNumber];
        if (startPos <= line.length()) {
            line = line.substr(0, startPos) + newText;
        }
    }

    void deleteLine(int lineNumber) {
        if (lineNumber < 0 || lineNumber >= buffer.size()) {
            throw std::out_of_range("Invalid line number");
        }

        saveBufferState('D');
        buffer.erase(buffer.begin() + lineNumber);
        std::cout << "Line deleted successfully. Buffer size: " << buffer.size() << "/" << MAX_LINES << std::endl;
    }

    void deleteWord(const Cursor& cursor, const std::string& word) {
        if (cursor.lineNumber >= 0 && cursor.lineNumber < buffer.size()) {
            saveBufferState('D');
            std::string& line = buffer[cursor.lineNumber];
            size_t pos = line.find(word, cursor.position);
            if (pos != std::string::npos) {
                line.erase(pos, word.length());
            }
        }
    }

    void undo() {
        if (!undoStack.empty()) {
            redoStack.push(BufferState(buffer, 'U'));
            buffer = undoStack.top().lines;
            undoStack.pop();
            std::cout << "Undo performed. Buffer size: " << buffer.size() << "/" << MAX_LINES << std::endl;
        } else {
            std::cout << "Nothing to undo\n";
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push(BufferState(buffer, 'U'));
            buffer = redoStack.top().lines;
            redoStack.pop();
            std::cout << "Redo performed. Buffer size: " << buffer.size() << "/" << MAX_LINES << std::endl;
        } else {
            std::cout << "Nothing to redo\n";
        }
    }

private:
    void saveBufferState(char operation) {
        undoStack.push(BufferState(buffer, operation));
        while (undoStack.size() > UNDO_LIMIT) {
            undoStack.pop();
        }
        redoStack = std::stack<BufferState>();
    }
};

int main() {
    LineEditor editor;
    std::string command;

    std::cout << "Line Editor Started (In-Memory Mode)\n";
    std::cout << "Maximum buffer size: 25 lines\n";
    
    while (true) {
        std::cout << "\nCommands: insert, search, display, update, delete, undo, redo, exit\n";
        std::cout << "Enter command: ";
        std::cin >> command;

        try {
            if (command == "insert") {
                int lineNum;
                std::string text;
                std::cout << "Enter line number (1-25): ";
                std::cin >> lineNum;
                if (lineNum < 1 || lineNum > 25) {
                    throw std::out_of_range("Line number must be between 1 and 25");
                }
                std::cout << "Enter text: ";
                std::cin.ignore();
                std::getline(std::cin, text);
                editor.insertLine(lineNum - 1, text);
            }
            else if (command == "search") {
                std::string word;
                std::cout << "Enter word to search: ";
                std::cin >> word;
                Cursor cursor = editor.searchWord(word);
                if (cursor.lineNumber != -1) {
                    std::cout << "Found at line " << cursor.lineNumber + 1 
                             << ", position " << cursor.position << std::endl;
                } else {
                    std::cout << "Word not found\n";
                }
            }
            else if (command == "display") {
                editor.displayBuffer();
            }
            else if (command == "update") {
                int lineNum, startPos;
                std::string text;
                std::cout << "Enter line number to update: ";
                std::cin >> lineNum;
                std::cout << "Enter starting position: ";
                std::cin >> startPos;
                std::cout << "Enter new text: ";
                std::cin.ignore();
                std::getline(std::cin, text);
                editor.updateLine(lineNum - 1, startPos, text);
            }
            else if (command == "delete") {
                int lineNum;
                std::cout << "Enter line number to delete: ";
                std::cin >> lineNum;
                editor.deleteLine(lineNum - 1);
            }
            else if (command == "undo") {
                editor.undo();
            }
            else if (command == "redo") {
                editor.redo();
            }
            else if (command == "exit") {
                break;
            }
            else {
                std::cout << "Invalid command\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
 
