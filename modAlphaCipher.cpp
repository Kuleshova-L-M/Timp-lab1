#include "modAlphaCipher.h"

// Вспомогательные функции для проверки регистра и преобразования
namespace {
const std::wstring UPPER = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
const std::wstring LOWER = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя";

bool isUpperRussian(wchar_t c) { return UPPER.find(c) != std::wstring::npos; }
bool isLowerRussian(wchar_t c) { return LOWER.find(c) != std::wstring::npos; }
wchar_t toUpperRussian(wchar_t c) {
    size_t pos = LOWER.find(c);
    return (pos == std::wstring::npos) ? c : UPPER[pos];
}
}

//  Реализация валидации ключа
std::wstring modAlphaCipher::getValidKey(const std::wstring& s) const {
    if (s.empty())
        throw cipher_error("Empty key"); //  Проверка на пустой ключ

    std::wstring tmp;
    for (wchar_t c : s) {
        if (isLowerRussian(c))
            tmp.push_back(toUpperRussian(c)); // Автоперевод в верхний регистр
        else if (isUpperRussian(c))
            tmp.push_back(c);
        else
            throw cipher_error("Invalid key: only Russian letters are allowed"); // 
    }

    if (tmp.empty())
        throw cipher_error("Empty key");

    bool weak = true;
    for (size_t i = 1; i < tmp.size(); ++i) {
        if (tmp[i] != tmp[0]) { weak = false; break; }
    }
    if (weak) throw cipher_error("Weak key"); // Проверка на вырожденный ключ 

    return tmp;
}

// Реализация валидации открытого текста
std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s) const {
    std::wstring tmp;
    for (wchar_t c : s) {
        if (isLowerRussian(c))
            tmp.push_back(toUpperRussian(c));
        else if (isUpperRussian(c))
            tmp.push_back(c);
        // Все остальные символы (пробелы, цифры, знаки) просто игнорируются
    }
    if (tmp.empty())
        throw cipher_error("Empty open text"); // Если после очистки текст пуст
    return tmp;
}

//Реализация валидации шифротекста
std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s) const {
    if (s.empty())
        throw cipher_error("Empty cipher text");
    for (wchar_t c : s) {
        if (!isUpperRussian(c))
            throw cipher_error("Invalid cipher text: only uppercase Russian letters are allowed");
    }
    return s;
}

//  В конструктор добавлен вызов getValidKey()
modAlphaCipher::modAlphaCipher(const std::wstring& skey) {
    for (size_t i = 0; i < numAlpha.size(); ++i) {
        alphaNum[numAlpha[i]] = static_cast<int>(i);
    }
    key = convert(getValidKey(skey)); //  Было convert(skey)
}

//  В encrypt добавлена валидация текста и исправлена формула индекса ключа
std::wstring modAlphaCipher::encrypt(const std::wstring& open_text) {
    std::vector<int> work = convert(getValidOpenText(open_text)); // Добавлена валидация
    int n = static_cast<int>(alphaNum.size());

    for (size_t i = 0; i < work.size(); ++i) {
        // берется элемент ключа по кругу
        work[i] = (work[i] + key[i % key.size()]) % n; 
    }
    return convert(work);
}

//  В decrypt добавлена валидация текста и исправлена формула
std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text) {
    std::vector<int> work = convert(getValidCipherText(cipher_text)); //  Добавлена валидация
    int n = static_cast<int>(alphaNum.size());

    for (size_t i = 0; i < work.size(); ++i) {
        //Исправлена логика индексации ключа, как и в encrypt
        work[i] = (work[i] + n - key[i % key.size()]) % n; 
    }
    return convert(work);
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s) const {
    std::vector<int> result;
    result.reserve(s.size());
    for (wchar_t c : s) {
        auto it = alphaNum.find(c);
        //Защита от символов, которых нет в алфавите
        if (it == alphaNum.end()) throw cipher_error("Invalid character in text");
        result.push_back(it->second);
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v) const {
    std::wstring result;
    result.reserve(v.size());
    for (int i : v) {
        //Защита от выхода за пределы алфавита
        if (i < 0 || i >= static_cast<int>(numAlpha.size())) 
            throw cipher_error("Invalid character code");
        result.push_back(numAlpha[i]);
    }
    return result;
}
