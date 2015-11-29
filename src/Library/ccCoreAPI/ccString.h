/*
 * ccString.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCCOREAPI_CCSTRING_H_
#define CCLIBRARY_CCCOREAPI_CCSTRING_H_

#include <string>

using namespace std;

class ccString : public string
{
public:
    ccString();
    ccString(const char* strInitData);
    ccString(const char* strFirst, const char* strLast);
    ccString(const ccString& _Right);

    virtual ~ccString();

public:
    ccString& operator=(const ccString& _Right)
    {   // assign _Right
        string::assign(_Right.c_str());

        return *this;
    }

    ccString& operator=(const std::string& _Right)
    {   // assign _Right
        string::assign(_Right.c_str());

        return *this;
    }

    ccString& operator=(const char *_Ptr)
    {   // assign [_Ptr, <null>)
        string::assign(_Ptr);

        return *this;
    }

    ccString& operator=(char _Ch)
    {   // assign 1 * _Ch
        string::assign(1, _Ch);

        return *this;
    }


public:
    void            ReplaceStringInPlace(const ccString& search, const ccString& replace);

    void            MakeUpper();
    void            MakeLower();

    void            Trim();
    void            TrimLeft();
    void            TrimRight();

    unsigned long   DecodeHex(int *length = NULL);

public:
    // Supports up to 512 bytes
    static  void    format(string& destStr, const char* format, ...);
    static  long    decodeStringToHex(const char* strVal, int nLength = -1, int* pConvertedSize = NULL);
    static  bool    encodeBase64(string& destStr, string& src);
    static  bool    decodeBase64(string& destStr, string& src);

private:
    enum CONST_VALUE
    {
        CV_MAX_FORMAT_BUFFER_SIZE = 512
    };
};

#endif /* CCLIBRARY_CCCOREAPI_CCSTRING_H_ */