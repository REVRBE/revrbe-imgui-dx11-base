//
// magiXOR by REVRBE
//
// Simple usage, compile-time encryption and decryption with dynamic key generation using __TIME__
//
#include <iostream>
#include <string>
#include <cstring>

// Function pointer type for obfuscated key generation
typedef wchar_t (*GenerateKeyFunc)();

// Obfuscation for generating secret key
constexpr wchar_t obfuscatedSecretKey()
{
    constexpr GenerateKeyFunc generateSecretKey = []() -> wchar_t {
        constexpr char time[] = __TIME__;
        constexpr int keyLength = 64;
        wchar_t secretKey[keyLength + 1] = { 0 };

        // Extract minutes component from __TIME__
        constexpr int minutes = (time[3] - '0') * 10 + (time[4] - '0');

        for (int i = 0; i < keyLength; ++i)
        {
            secretKey[i] = time[(i % minutes) * 3];
        }

        return *secretKey;
    };

    return generateSecretKey();
}

// Obfuscation for generating obfuscation key
constexpr wchar_t obfuscatedObfuscationKey()
{
    constexpr GenerateKeyFunc generateObfuscationKey = []() -> wchar_t {
        constexpr char time[] = __TIME__;
        constexpr int keyLength = 64;
        wchar_t obfuscationKey[keyLength + 1] = { 0 };

        // Extract minutes component from __TIME__
        constexpr int minutes = (time[3] - '0') * 10 + (time[4] - '0');

        for (int i = 0; i < keyLength; ++i)
        {
            obfuscationKey[i] = time[((i + 2) % minutes) * 3];
        }

        return *obfuscationKey;
    };

    return generateObfuscationKey();
}

// Compile-time XOR encryption for a string literal
template <typename T, std::size_t N>
class magiXOR
{
public:
    constexpr magiXOR(const T(&str)[N])
        : secretKey(obfuscatedSecretKey()), obfuscationKey(obfuscatedObfuscationKey())
    {
        encryptString(str);
    }

    T* decrypt() const
    {
        T* decryptedStr = new T[N];
        T previousBlock = secretKey;

        for (std::size_t i = 0; i < N - 1; ++i)
        {
            decryptedStr[i] = decryptChar(encryptedStr[i]) ^ previousBlock;
            previousBlock = encryptedStr[i] ^ obfuscationKey;
        }

        decryptedStr[N - 1] = '\0';
        return decryptedStr;
    }

    const T* encrypt() const
    {
        return encryptedStr;
    }

    // Overload the << operator to handle printing of encrypted or decrypted strings
    friend std::ostream& operator<<(std::ostream& os, const magiXOR<T, N>& obj)
    {
        os << obj.decrypt();
        return os;
    }

private:
    const wchar_t secretKey;
    const wchar_t obfuscationKey;
    T encryptedStr[N];

    void encryptString(const T(&str)[N])
    {
        T previousBlock = secretKey;

        for (std::size_t i = 0; i < N - 1; ++i)
        {
            T encryptedChar = encryptChar(str[i] ^ previousBlock);
            encryptedStr[i] = encryptedChar;
            previousBlock = encryptedChar ^ obfuscationKey;
        }

        encryptedStr[N - 1] = '\0';
    }

    constexpr T encryptChar(const T ch) const
    {
        return ch ^ obfuscationKey;
    }

    constexpr T decryptChar(const T ch) const
    {
        return ch ^ obfuscationKey;
    }
};

// Specialization for char
template <>
class magiXOR<char, 2>
{
public:
    constexpr magiXOR(const char ch)
        : secretKey(obfuscatedSecretKey()), obfuscationKey(obfuscatedObfuscationKey()), encryptedChar(encryptChar(ch))
    {}

    char decrypt() const
    {
        return decryptChar(encryptedChar);
    }

    const char encrypt() const
    {
        return encryptedChar;
    }

    // Overload the << operator to handle printing of encrypted or decrypted characters
    friend std::ostream& operator<<(std::ostream& os, const magiXOR<char, 2>& obj)
    {
        os << obj.decrypt();
        return os;
    }

private:
    const wchar_t secretKey;
    const wchar_t obfuscationKey;
    const char encryptedChar;

    constexpr char encryptChar(const char ch) const
    {
        return ch ^ obfuscationKey;
    }

    constexpr char decryptChar(const char ch) const
    {
        return ch ^ obfuscationKey;
    }
};

// Specialization for wchar_t
template <>
class magiXOR<wchar_t, 2>
{
public:
    constexpr magiXOR(const wchar_t ch)
        : secretKey(obfuscatedSecretKey()), obfuscationKey(obfuscatedObfuscationKey()), encryptedChar(encryptChar(ch))
    {}

    wchar_t decrypt() const
    {
        return decryptChar(encryptedChar);
    }

    const wchar_t encrypt() const
    {
        return encryptedChar;
    }

    // Overload the << operator to handle printing of encrypted or decrypted wide characters
    friend std::wostream& operator<<(std::wostream& os, const magiXOR<wchar_t, 2>& obj)
    {
        os << obj.decrypt();
        return os;
    }

private:
    const wchar_t secretKey;
    const wchar_t obfuscationKey;
    const wchar_t encryptedChar;

    constexpr wchar_t encryptChar(const wchar_t ch) const
    {
        return ch ^ obfuscationKey;
    }

    constexpr wchar_t decryptChar(const wchar_t ch) const
    {
        return ch ^ obfuscationKey;
    }
};

// Overload for const char* to support string literals
template <typename T, std::size_t N>
magiXOR(const T(&)[N]) -> magiXOR<T, N>;

// Overload for const wchar_t to support wide char literals
magiXOR(const wchar_t)->magiXOR<wchar_t, 2>;

// Overload for const char to support char literals
magiXOR(const char)->magiXOR<char, 2>;

// MagiXOR macro for simplified encryption
#define magiXOR(str) magiXOR(str)

// Decrypt an array of encrypted strings
template <std::size_t N>
const char** decryptArray(const char* (&arr)[N])
{
    const char** decryptedArray = new const char* [N];
    const wchar_t* previousBlock = obfuscatedSecretKey();

    for (std::size_t i = 0; i < N; ++i)
    {
        magiXOR<char, 2> magiObj(arr[i]);
        const char* encryptedStr = magiObj.encrypt();
        std::size_t strLength = std::strlen(encryptedStr);
        char* decryptedStr = new char[strLength + 1];
        decryptedStr[strLength] = '\0';

        for (std::size_t j = 0; j < strLength; ++j)
        {
            decryptedStr[j] = magiObj.decrypt() ^ previousBlock[j % 2];
            previousBlock[j % 2] = encryptedStr[j];
        }

        decryptedArray[i] = decryptedStr;
    }

    return decryptedArray;
}