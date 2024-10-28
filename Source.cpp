#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <locale>
#include <codecvt>

void hideInformation(const std::wstring& inputFilename, const std::wstring& outputFilename) {
    std::wifstream inFile(inputFilename);
    inFile.imbue(std::locale(inFile.getloc(), new std::codecvt_utf8<wchar_t>));
    std::wofstream outFile(outputFilename);
    outFile.imbue(std::locale(outFile.getloc(), new std::codecvt_utf8<wchar_t>));

    if (!inFile.is_open()) {
        std::wcerr << L"Error: Unable to open the input file at " << inputFilename << std::endl;
        return;
    }
    if (!outFile.is_open()) {
        std::wcerr << L"Error: Unable to create the output file at " << outputFilename << std::endl;
        return;
    }

    std::wstring content((std::istreambuf_iterator<wchar_t>(inFile)), std::istreambuf_iterator<wchar_t>());
    inFile.close();

 
    for (size_t i = 0; i < content.size(); ++i) {
        outFile << content[i];

        std::bitset<8> bits(content[i]);
        for (int j = 7; j >= 0; --j) {
            outFile << (bits[j] ? L"  " : L" "); 
        }
    }

    outFile.close();
    std::wcout << L"Information hidden successfully in " << outputFilename << std::endl;
}


void extractInformation(const std::wstring& filename, const std::wstring& outputFilename) {
    std::wifstream inFile(filename);
    inFile.imbue(std::locale(inFile.getloc(), new std::codecvt_utf8<wchar_t>));
    std::wofstream outFile(outputFilename);
    outFile.imbue(std::locale(outFile.getloc(), new std::codecvt_utf8<wchar_t>));

    if (!inFile.is_open()) {
        std::wcerr << L"Error: Unable to open the file at " << filename << std::endl;
        return;
    }
    if (!outFile.is_open()) {
        std::wcerr << L"Error: Unable to create the output file at " << outputFilename << std::endl;
        return;
    }

    std::wstring hiddenMessage;
    std::wstring line;
    std::bitset<8> bits;
    int bitIndex = 0;

    while (std::getline(inFile, line)) {
        for (size_t i = 0; i < line.size(); ++i) {

            if (line[i] == L' ') {
                if (i + 1 < line.size() && line[i + 1] == L' ') {
                    bits[7 - bitIndex] = 1;
                    i++;  
                }
                else {
                    bits[7 - bitIndex] = 0;  
                }

                bitIndex++;
             
                if (bitIndex == 8) {
                    hiddenMessage += static_cast<wchar_t>(bits.to_ulong());
                    outFile << hiddenMessage.back(); 
                    bitIndex = 0;
                    bits.reset();
                }
            }
        }
    }

    inFile.close();
    outFile.close();
    std::wcout << L"Extracted hidden message saved to " << outputFilename << std::endl;
}




int main() {
    std::wstring inputFilename = L"E:\\TZI\\Rozraha\\Secret!.txt";
    std::wstring hiddenFilename = L"E:\\TZI\\Rozraha\\HiddenSecret.txt";
    std::wstring outputFilename = L"E:\\TZI\\Rozraha\\ExtractedMessage.txt";

    // Виконуємо шифрування
    hideInformation(inputFilename, hiddenFilename);

    // Після шифрування автоматично викликаємо функцію дешифрування
    extractInformation(hiddenFilename, outputFilename);

    return 0;
}
