//
// Created by 123 on 2020/1/12.
//

#ifndef BASE64_BASE64_H
#define BASE64_BASE64_H

#include <string>
#include <random>
#include <ctime>

namespace base64_bzx {

	class base64 {
	public:
		char* decode(char* src);
		char* encode(char* src);
        void check(size_t content_length=50,int check_round = 1000);
	private:
	    void encoder_Three_Byte(char *src,char *dst);
        void encoder_Two_Byte(char *src, char *dst);
        void encoder_One_Byte(char *src, char *dst);
        char decoder(char ele);
        char code[64]={
           'A','B','C','D','E','F','G','H',
           'I','J','K','L','M','N','O','P',
           'Q','R','S','T','U','V','W','X',
           'Y','Z','a','b','c','d','e','f',
           'g','h','i','j','k','l','m','n',
           'o','p','q','r','s','t','u','v',
           'w','x','y','z','0','1','2','3',
           '4','5','6','7','8','9','+','/'
        };

	};


    void base64::encoder_One_Byte(char *src,char *dst){
        dst[0] = code[(src[0]&0xFC)>>2];
        dst[1] = code[(src[0]&0x3)<<4];
        dst[2] = '=';
        dst[3] = '=';
	}

    void base64::encoder_Two_Byte(char *src,char *dst)
    {
        dst[0] = code[ (src[0]&0xFC)>>2];
        dst[1] = code[((src[0]&0x03)<<4) + ((src[1]&0xF0)>>4)];
        dst[2] = code[ (src[1]&0x0F)<<2];
        dst[3] = '=';
    }


    void base64::encoder_Three_Byte(char *src,char *dst)
    {
        dst[0] = code[ (src[0]&0xFC)>>2];
        dst[1] = code[((src[0]&0x03)<<4) + ((src[1]&0xF0)>>4)];
        dst[2] = code[((src[1]&0x0F)<<2) + ((src[2]&0xC0)>>6)];
        dst[3] = code[(src[2])&0x3F];
    }


    char* base64::encode(char* src)
    {
        size_t Length = strlen(src);
        size_t new_Length = 4 * ceil(1.0 * Length/3) + 1;  // 预留'\0'位
        char *dst = (char*)malloc(new_Length* sizeof(char));
        char *first_free = dst;
        int i = 0;
        if(Length >= 3){
            for(; i < Length;i+=3){
                encoder_Three_Byte(src,first_free);
                first_free += 4;
                src +=3;
            }
            first_free -=4;
            src-=3;
        }

        switch (Length%3){
            case 1:
                encoder_One_Byte(src,first_free);
                break;
            case 2:
                encoder_Two_Byte(src,first_free);
                break;
        }

        dst[new_Length-1] = '\0';
        return dst;
    }

    char* base64::decode(char* src)
    {
        size_t Length = strlen(src);
        size_t new_Length =  3 * floor(1.0 *Length/4) + 1;  // 预留'\0'位
        char *dst = (char*)malloc(new_Length* sizeof(char));
        char *first_free = dst;

        int i = 0;
        for(;i <Length;i+=4){
            // step 1
            first_free[0] = (decoder(src[0])<<2) +  ((decoder(src[1])&0x30)>>4);
            first_free[1] = ((decoder(src[1])&0x0F)<<4) + ((decoder(src[2])&0x3C)>>2);
            first_free[2] = ((decoder(src[2])&0x03)<<6) + ((decoder(src[3])&0x3F));
            // step 2
            src +=4;
            first_free +=3;
        }
        // end
        dst[new_Length-1] = '\0';
        return dst;
    }

    char base64::decoder(char ele)
    {
        if(ele == '='){
            return 0;
        }
        else if(ele <=47){  // '+' and '/' ,62~63
            return ele=='+'?62:63;
        }
        else if(ele <= 57){  // 52~61
            return ele + 4;
        }
        else if(ele <=90){   // 0~25
            return ele-65;
        }
        return ele - 71; // 26~51
    }

    /*
     * 检查编码是否正确，以及耗费时间,
     * 随机字符串，字符串长度为1-50；
     */
    void base64::check(size_t content_length,int check_round)
    {
        time_t bt = clock();
        srand(time(0));
        int right_case = 0;
        std::string str_A,str_B;
        size_t Length = 0;
        char *str = (char*)malloc((content_length+1)* sizeof(char));
        int i = 0;
        while (i < check_round){
            // 生成字符串
            memset(str,0,content_length+1);
            Length = 0;
            while (Length < content_length){
                str_A += char(65 + rand()%(90-65));
                ++Length;
            }

            memcpy(str,str_A.c_str(),str_A.length());
            str[str_A.length()] = '\0';
            str = encode(str);
            str_B = decode(str);

            if(str_A == str_B){
                right_case +=1;
            }
            ++i;
            //std::cout <<str_A<<" "<<str_B<<" \n";
            str_A = "";
        }
        std::cout<<"right rate: " <<100.0 * right_case/check_round<<"%\n";
        std::cout<<"total time: " << (clock()-bt)/CLOCKS_PER_SEC<<" s\n";
    }
}

#endif //BASE64_BASE64_H
