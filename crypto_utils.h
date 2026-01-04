#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <string>

// Simple XOR obfuscation. This is NOT strong cryptography.
// It is used to satisfy the "encrypted storage" requirement in the assignment.
inline std::string xorEncrypt(const std::string& plain, char key = 'K') {
    std::string out = plain;
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] = static_cast<char>(out[i] ^ key);
    }
    return out;
}

inline std::string xorDecrypt(const std::string& cipher, char key = 'K') {
    // XOR decrypt is the same operation as encrypt
    return xorEncrypt(cipher, key);
}

#endif