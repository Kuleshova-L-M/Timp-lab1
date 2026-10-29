#include <iostream>
#include <locale>
#include <clocale>
#include <string>
#include <limits> // Добавлено для очистки буфера
#include "modAlphaCipher.h"

int main() {
    std::setlocale(LC_ALL, "");
    std::wcin.imbue(std::locale(""));
    std::wcout.imbue(std::locale(""));

    try {
        std::wstring key;
        std::wcout << L"Введите ключ: ";
        
        //  Используем getline, чтобы прочитать пустую строку
        std::getline(std::wcin, key); 

        modAlphaCipher cipher(key);

        unsigned op;
        do {
            std::wcout << L"Шифратор готов. Введите операцию (0-выход, 1-зашифровать, 2-расшифровать): ";
            
            if (!(std::wcin >> op)) {
                std::wcin.clear();
                std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
                std::wcout << L"Ошибка ввода. Введите число.\n";
                continue;
            }

            if (op > 2) {
                std::wcout << L"Недопустимая операция\n";
                continue;
            }

            if (op > 0) {
                std::wstring text;
                std::wcout << L"Введите текст: ";
                
                // Очищаем буфер от Enter, который остался после ввода цифры
                std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
                
                //  Используем getline, чтобы поймать пустую строку или пробелы
                std::getline(std::wcin, text);

                try {
                    if (op == 1) {
                        std::wcout << L"Зашифрованный текст: "
                                   << cipher.encrypt(text) << L"\n";
                    } else {
                        std::wcout << L"Расшифрованный текст: "
                                   << cipher.decrypt(text) << L"\n";
                    }
                } catch (const cipher_error& e) {
                    std::wcerr << L"Ошибка: " << e.what() << L"\n";
                }
            }
        } while (op != 0);
    } catch (const cipher_error& e) {
        std::wcerr << L"Ошибка: " << e.what() << L"\n";
    } catch (const std::exception& e) {
        std::wcerr << L"Ошибка: " << e.what() << L"\n";
    }

    return 0;
}
