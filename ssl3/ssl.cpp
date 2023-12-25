#include "ssl.h"
#include "openssl/crypto.h"
#include "openssl/types.h"
#include "openssl/x509.h"
#include <cstddef>
#include <cstring>
#include <debug.h>
#include <memory>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/pem.h>
#include <string>

std::string GetErrorStr() {
    unsigned long er = 0;

    char erbuf[512] = {0};

    size_t erlen = 512;

    er = ERR_get_error();
    ERR_error_string_n(er, erbuf, erlen);
    return std::string(erbuf, erlen);
}

LOGINSTANCE("ssl.log")
sm2PublicKey::sm2PublicKey(const unsigned char *pub_str, size_t len) {
    EVP_PKEY *pkey_t = NULL;
    // pkey_t=d2i_PublicKey(EVP_PKEY_SM2,NULL, &pub_str, len);
    pkey_t = d2i_PUBKEY(NULL, &pub_str, len);
    std::string error;
    if (pkey_t == NULL) {
        error = GetErrorStr();
        errorL(error);
        return;
    }
    EVP_CUNSTOM *cst = new EVP_CUNSTOM{pkey_t};
    m_pkey = std::shared_ptr<EVP_CUNSTOM>(cst);
}

sm2PublicKey::sm2PublicKey(const std::string &pub_str) {
    EVP_PKEY *pkey_t = NULL;
    unsigned char *c_str = new unsigned char[pub_str.size()];
    memcpy(c_str, (const void *)pub_str.c_str(), pub_str.size());
    pkey_t = d2i_PUBKEY(NULL, (const unsigned char **)&c_str, pub_str.size());
    std::string error;
    if (pkey_t == NULL) {
        error = GetErrorStr();
        errorL(error);
        return;
    }
    EVP_CUNSTOM *cst = new EVP_CUNSTOM{pkey_t};
    m_pkey = std::shared_ptr<EVP_CUNSTOM>(cst);
    delete[] c_str;
}

std::string sm2PublicKey::Encrypt(const std::string &message,
                                  std::string &error) {
    std::string encodedstr;
    EVP_PKEY_CTX *pkctx = NULL;
    int retV = 1;
    if (!(pkctx = EVP_PKEY_CTX_new(m_pkey.get()->pkey, NULL))) {
        error = GetErrorStr();
        errorL("EVP_PKEY_CTX_new:%s",error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }
    retV = EVP_PKEY_encrypt_init(pkctx);
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_encrypt_init:%s",error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }

    size_t outbuflen = 0;
    unsigned char *outbuf = NULL;
    retV = EVP_PKEY_encrypt(pkctx, NULL, &outbuflen,
                            (const unsigned char *)message.c_str(),
                            message.size());
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_encrypt:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }
    if (outbuflen == 0) {
        errorL("EVP_PKEY_encrypt:%s", "no memery");
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }

    outbuf = new unsigned char[outbuflen];

    retV = EVP_PKEY_encrypt(pkctx, outbuf, &outbuflen,
                            (const unsigned char *)message.c_str(),
                            message.size());
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_encrypt:%s",error);
        EVP_PKEY_CTX_free(pkctx);
        delete[] outbuf;
        return "";
    }
    encodedstr = std::string((const char *)outbuf, outbuflen);
    delete[] outbuf;
    EVP_PKEY_CTX_free(pkctx);
    return encodedstr;
}

bool sm2PublicKey::SignatureVerification32(const std::string &signature,
                                           const std::string &message,
                                           std::string &error) {

    EVP_PKEY_CTX *pkctx = NULL;
    unsigned char *outbuf = NULL;
    size_t outlen = 0;
    int retV = 1;
    if (!(pkctx = EVP_PKEY_CTX_new(m_pkey.get()->pkey, NULL))) {
        error = GetErrorStr();
        errorL("EVP_PKEY_CTX_new:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        return false;
    }

    retV = EVP_PKEY_verify_init(pkctx);
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_verify_init:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        return false;
    }

    retV = EVP_PKEY_verify(
        pkctx, (const unsigned char *)signature.c_str(), signature.size(),
        (const unsigned char *)message.c_str(), message.size());
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_sign:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        return false;
    }
    EVP_PKEY_CTX_free(pkctx);
    return true;
}

bool sm2PublicKey::SignatureVerification(const std::string &signature,
                                         const std::string &message,
                                         std::string &error) {
    std::string signatured;
    EVP_MD_CTX *mdctx = NULL;
    int retV = 0;
    if (!(mdctx = EVP_MD_CTX_create())) {
        error = GetErrorStr();
        errorL("EVP_MD_CTX_create:%s", error);
        return false;
    }
    retV =
        EVP_DigestVerifyInit(mdctx, NULL, EVP_sm3(), NULL, m_pkey.get()->pkey);
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_DigestVerifyInit:%s",error);
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    retV = EVP_DigestVerifyUpdate(mdctx, message.c_str(), message.size());
    if (retV <= 0) {
        error = GetErrorStr();
        EVP_MD_CTX_free(mdctx);
        errorL("EVP_DigestVerifyUpdate:%s", error);
        return false;
    }
    retV = EVP_DigestVerifyFinal(
        mdctx, (const unsigned char *)signature.c_str(), signature.size());
    if (retV <= 0) {
        error = GetErrorStr();
        EVP_MD_CTX_free(mdctx);
        errorL("EVP_DigestVerifyFinal:%s", error);
        return false;
    }
    EVP_MD_CTX_free(mdctx);
    return true;
}

std::string sm2PublicKey::GetPublicString() {
    unsigned char *buffer = nullptr;
    int retV = i2d_PUBKEY(m_pkey.get()->pkey, &buffer);
    if (retV <= 0) {
        errorL("i2d_PublicKey:%s", GetErrorStr());
        return "";
    }
    std::string ret((const char *)buffer, retV);
    // delete[] buffer;
    return ret;
}

std::string sm2PublicKey::GetPublicStringBase64() { return ""; }

sm2PrivateKey::sm2PrivateKey() {
    EVP_PKEY *ret = NULL;

    EVP_PKEY_CTX *pkctx = NULL;

    pkctx = EVP_PKEY_CTX_new_id(EVP_PKEY_SM2, NULL);
    if (pkctx == NULL) {
        errorL("EVP_PKEY_CTX_new_id");
        return;
    }
    int retV = 1;
    retV = EVP_PKEY_keygen_init(pkctx);

    if (retV <= 0) {
        errorL("EVP_PKEY_keygen_init:%s",GetErrorStr());
        EVP_PKEY_CTX_free(pkctx);
        return;
    }

    retV = EVP_PKEY_keygen(pkctx, &ret);
    if (retV <= 0) {
        errorL("EVP_PKEY_keygen:%s", GetErrorStr());
        EVP_PKEY_CTX_free(pkctx);
        return;
    }
    EVP_CUNSTOM *cst = new EVP_CUNSTOM{ret};
    M_PKEY = std::shared_ptr<EVP_CUNSTOM>(cst);
    EVP_PKEY_CTX_free(pkctx);
}
sm2PrivateKey::sm2PrivateKey(const std::string &priv_str) {

}

sm2PublicKey sm2PrivateKey::CreatePublic() {
    unsigned char *buffer = nullptr;
    int retV = i2d_PUBKEY(M_PKEY.get()->pkey, &buffer);
    if (retV <= 0) {
        errorL("i2d_PUBKEY:%s", GetErrorStr());
        return sm2PublicKey{};
    }
    sm2PublicKey pub(buffer, retV);
    //OPENSSL_free(buffer);
    return pub;
}

std::string sm2PrivateKey::Decrypt(const std::string &encoded,
                                   std::string &error) {
    std::string decodedstr;
    EVP_PKEY_CTX *pkctx = NULL;
    unsigned char *outbuf = NULL;
    size_t outlen = 0;

    int retV = 1;
    if (!(pkctx = EVP_PKEY_CTX_new(M_PKEY.get()->pkey, NULL))) {
        error = GetErrorStr();
        errorL("EVP_PKEY_CTX_new:%s",error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }
    retV = EVP_PKEY_decrypt_init(pkctx);
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_decrypt_init:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }
    retV = EVP_PKEY_decrypt(pkctx, NULL, &outlen,
                            (const unsigned char *)encoded.c_str(),
                            encoded.size());
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_encrypt_init:%s",error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }

    if (outlen == 0) {
        errorL("EVP_PKEY_decrypt:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }

    outbuf = new unsigned char[outlen];

    retV = EVP_PKEY_decrypt(pkctx, outbuf, &outlen,
                            (const unsigned char *)encoded.c_str(),
                            encoded.size());
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_encrypt_init:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        delete[] outbuf;
        return "";
    }

    decodedstr = std::string((const char *)outbuf, outlen);
    delete[] outbuf;

    EVP_PKEY_CTX_free(pkctx);
    return decodedstr;
}

std::string sm2PrivateKey::Signature32(const std::string &message,
                                       std::string &error) {

    std::string signatured;

    EVP_PKEY_CTX *pkctx = NULL;
    unsigned char *outbuf = NULL;
    size_t outlen = 0;
    int retV = 1;
    if (message.size() != 32) {
        error = "size not eq 32";
        return "";
    }
    if (!(pkctx = EVP_PKEY_CTX_new(M_PKEY.get()->pkey, NULL))) {
        error = GetErrorStr();
        errorL("EVP_PKEY_CTX_new:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }

    retV = EVP_PKEY_sign_init(pkctx);
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_sign_init:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }
    debugL("msglen:%s", message.size());
    retV =
        EVP_PKEY_sign(pkctx, NULL, &outlen,
                      (const unsigned char *)message.c_str(), message.size());
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_sign:%s" , error);
        EVP_PKEY_CTX_free(pkctx);
        return "";
    }
    debugL(outlen);
    outbuf = new unsigned char[outlen];
    retV =
        EVP_PKEY_sign(pkctx, outbuf, &outlen,
                      (const unsigned char *)message.c_str(), message.size());
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_PKEY_sign:%s", error);
        EVP_PKEY_CTX_free(pkctx);
        delete[] outbuf;
        return "";
    }
    signatured = std::string((const char *)outbuf, outlen);
    EVP_PKEY_CTX_free(pkctx);
    return signatured;
}

std::string sm2PrivateKey::Signature(const std::string &message,
                                     std::string &error) {
    std::string signatured;
    EVP_MD_CTX *mdctx = NULL;
    unsigned char *outbuf = NULL;
    size_t outbuflen = 0;
    int retV = 0;
    if (!(mdctx = EVP_MD_CTX_create())) {
        error = GetErrorStr();
        errorL("EVP_MD_CTX_create:%s",error);
        return "";
    }
    retV = EVP_DigestSignInit(mdctx, NULL, EVP_sm3(), NULL, M_PKEY.get()->pkey);
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_DigestSignInit:%s" , error);
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    retV = EVP_DigestSignUpdate(mdctx, message.c_str(), message.size());
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_DigestSignUpdate:%s" , error);
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    retV = EVP_DigestSignFinal(mdctx, NULL, &outbuflen);
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_DigestSignFinal:%s", error);
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    outbuf = new unsigned char[outbuflen];

    retV = EVP_DigestSignFinal(mdctx, outbuf, &outbuflen);
    if (retV <= 0) {
        error = GetErrorStr();
        errorL("EVP_DigestSignFinal:%s", error);
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    signatured = std::string((const char *)outbuf, outbuflen);
    delete[] outbuf;
    return signatured;
}

std::string sm2PrivateKey::GetPrivateString() {
    unsigned char *buffer = nullptr;
    int p = i2d_PrivateKey(M_PKEY->pkey, &buffer);
    if (p <= 0) {
        debugL("i2d_PrivateKey:%s" , GetErrorStr());
        return "";
    }
    return std::string((const char *)buffer, p);
}