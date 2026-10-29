#include <iostream>
#include <locale>
#include <clocale>
#include <string>
#include "modAlphaCipher.h"

int main() {
    std::setlocale(LC_ALL, "");
    std::wcin.imbue(std::locale(""));
    std::wcout.imbue(std::locale(""));

    // лобальный блок try-catch на всю программу
    try {
        std::wstring key;
        std::wcout << L"Введите ключ: ";
        std::wcin >> key;

        modAlphaCipher cipher(key); // Конструктор может выбросить исключение

        unsigned op;
        do {
            std::wcout << L"Операция (0-выход, 1-зашифровать, 2-расшифровать): ";
            if (!(std::wcin >> op))
                throw cipher_error("Invalid operation input");

            if (op > 2) {
                std::wcout << L"Недопустимая операция\n";
                continue;
            }

            if (op > 0) {
                std::wstring text;
                std::wcout << L"Введите текст: ";
                std::wcin >> text;

                // Локальный блок try-catch для перехвата ошибок валидации текста
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
