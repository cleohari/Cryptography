//
//  main.cpp
//  RC4
//
//  Created by tinoryj on 18/05/2017.
//  Copyright © 2017 tinoryj. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstring>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

class rc4Encryption {
public:
    rc4Encryption(int kl):keylen(kl) {
        srand((unsigned)time(NULL));
        for(int i = 0; i < kl; i++){  //rand keylen byte key
            int tmp = rand() % 256;
            K.push_back(char(tmp));
        }
    }
    void encryption(const string &,const string &,const string &);
private:
    unsigned char S[256]; //Style，256 byte
    unsigned char T[256]; //Temp，256 byte
    int keylen;        //range 1-256
    vector<char> K;      //variable-len key
    vector<char> k;      //key stream
    /*init S T*/
    void initial() {
        for (int i = 0; i < 256; i++){
            S[i] = i;
            T[i] = K[i % keylen];
        }
    }
    /*range S*/
    void rangeS() {
        int j = 0;
        for (int i = 0; i < 256; i++){
            j = (j + S[i] + T[i]) % 256;
            S[i] = S[i] + S[j];
            S[j] = S[i] - S[j];
            S[i] = S[i] - S[j];
        }
    }
    /*generate key stream , m's len = len*/
    void keyStream(long long int len);
};
void rc4Encryption::keyStream(long long int len) {
    initial();
    rangeS();
    
    int i=0,j=0,t;
    while(len--){
        i=(i+1)%256;
        j=(j+S[i])%256;
        
        S[i]=S[i]+S[j];
        S[j]=S[i]-S[j];
        S[i]=S[i]-S[j];
        
        t=(S[i]+S[j])%256;
        k.push_back(S[t]);
    }
}
void rc4Encryption::encryption(const string &plaintext,const string &ks,const string &ciphertext) {
    
    ifstream in;
    ofstream out,outks;
    
    in.open(plaintext);
    
    //get in stream len
    in.seekg(0,ios::end);
    long long int lenFile=in.tellg();
    in.seekg(0, ios::beg);
    
    //generate key stream
    keyStream(lenFile);
    outks.open(ks);
    for(int i=0;i<lenFile;++i){
        outks<<(k[i]);
    }
    outks.close();
    
    //read m into bits
    unsigned char *bits=new unsigned char[lenFile];
    in.read((char *)bits,lenFile);
    in.close();
    
    
    out.open(ciphertext);
    //out put
    for(int i=0;i<lenFile;++i){
        out<<(unsigned char)(bits[i]^k[i]);
    }
    out.close();
    delete []bits;
}

class rc4Decryption{
public:
    rc4Decryption(const string ks,const string ct):keystream(ks),ciphertext(ct) {}
    void decryption(const string &);
    
private:
    string ciphertext,keystream;
};

void rc4Decryption::decryption(const string &res){
    
    ifstream inks,incp;
    ofstream out;
    inks.open(keystream);
    incp.open(ciphertext);
    inks.seekg(0,ios::end);
    long long int lenFile = inks.tellg();
    inks.seekg(0, ios::beg);
    unsigned char *bitKey = new unsigned char[lenFile];
    inks.read((char *)bitKey, lenFile);
    inks.close();
    unsigned char *bitCip = new unsigned char[lenFile];
    incp.read((char *)bitCip, lenFile);
    incp.close();
    out.open(res);
    for(int i = 0; i < lenFile; i++)
        out<<(unsigned char)(bitKey[i]^bitCip[i]);
    out.close();
}

int main(int argc, char* argv[]){
    
    rc4Encryption rc4(16);
    rc4.encryption("mdata", "keystream", "cdata");
    cout<<"1"<<endl;
    rc4Decryption decrypt("keystream", "cdata");
    decrypt.decryption("DcData");
}
