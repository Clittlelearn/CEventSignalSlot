#include "openssl/types.h"
#include <cstddef>
#include <memory>
#ifndef _MY_SSL_
#include <openssl/evp.h>
#include <string>
class sm2PrivateKey;

 struct EVP_CUNSTOM{
        EVP_PKEY * pkey=NULL;
        ~EVP_CUNSTOM(){
            if(pkey!=NULL){
                EVP_PKEY_free(pkey);
            }
        }
    };

class sm2PublicKey{
    public:
    sm2PublicKey()=default;

    sm2PublicKey(const sm2PublicKey & other){
        m_pkey=other.m_pkey;
    }

    sm2PublicKey(const std::string & pub_str);

    sm2PublicKey(const unsigned char * pub_str,size_t len);

    std::string Encrypt(const std::string  & message,std::string & error);

    bool SignatureVerification32(const std::string &signature,const std::string & message,std::string & error);

    bool SignatureVerification(const std::string & signature,const std::string & message,std::string & error);


    std::string GetPublicString();

    std::string GetPublicStringBase64();
private:
   
    std::shared_ptr<EVP_CUNSTOM> m_pkey=nullptr;
};


class sm2PrivateKey{
    public:

    sm2PrivateKey();

    sm2PrivateKey(const std::string & priv_str);

    sm2PublicKey CreatePublic();

    std::string Decrypt(const std::string & encoded,std::string & error);

    std::string Signature32(const std::string & message,std::string & error);

    std::string Signature(const std::string & message ,std::string & error);

    std::string GetPrivateString();
    private:

    std::shared_ptr<EVP_CUNSTOM>  M_PKEY=nullptr;


};


#endif