//
// Created by Caikera on 2022/11/9.
//

#ifndef BIN_TREE_PRINTER_CHAR_BOX_H
#define BIN_TREE_PRINTER_CHAR_BOX_H

#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <string>

namespace koala {


    struct char_box_line{
    private:
        char* ptr;
        size_t size_x;
    public:
        char_box_line(char* ptr, size_t size_x);
        char& operator[](size_t x_idx);
    };

    inline char_box_line::char_box_line(char *ptr, size_t size_x):
    ptr(ptr), size_x(size_x)
    {}

    inline char& char_box_line::operator[](size_t x_idx) {
        if(x_idx > size_x - 1){
            std::printf("char_box x index is out of range. x_idx = %zu, "
                        "while size_x = %zu\n", x_idx, size_x);
            assert(false);
        }
        return *(ptr + x_idx);
    }

    struct char_box{
        char* ptr;
        size_t size_x;
        size_t size_y;
        char_box(size_t size_y, size_t size_x);
        explicit char_box(const std::string& str);
        char_box(const char_box& another);
        char_box(char_box&& another) noexcept;
        ~char_box();
        char_box& operator=(const char_box& another);
        char& operator()(size_t y_idx, size_t x_idx) const;
        char_box_line operator[](size_t y_idx);
        void write(size_t pos_y, size_t pos_x, const char* str);
        void write(size_t pos_y, size_t pos_x, const std::string& str);
        void write(size_t pos_y, size_t pos_x, const char_box& another);
        void print() const;
    };

    inline char_box::char_box(size_t size_y, size_t size_x):
    size_x(size_x), size_y(size_y)
    {
        ptr = new char[size_y * size_x];
        std::memset(ptr, ' ', size_y * size_x);
    }

    inline char_box::char_box(const std::string& str):
    char_box(1, str.length())
    {
        size_t x_cnt_max = 0;
        size_t x_cnt = 0;
        size_t y_cnt = 0;
        for(auto c: str){
            if(c == '\0') break;
            //std::printf("%c", c);
            if(c == '\n'){
                //std::printf("x_cnt_max = %zu\n", x_cnt_max);
                x_cnt = 0;
                y_cnt += 1;
            }
            else{
                x_cnt += 1;
                x_cnt_max = (x_cnt > x_cnt_max) ? x_cnt : x_cnt_max;
                //std::printf("%c: y_cnt_ x_cnt = %zu\n", c, x_cnt);
            }
        }
        size_y = y_cnt + 1;
        size_x = x_cnt_max + 1;
        //std::printf("\nbox size: (%zu, %zu)\n", size_y, size_x);
        ptr = new char[size_y*size_x];
        std::memset(ptr, ' ', size_y * size_x);
        write(0, 0, str);
        //print();
    }

    inline char_box::char_box(const char_box& another):
            char_box(another.size_y, another.size_x)
    {
        std::memcpy(ptr, another.ptr, size_y * size_x);
    }

    inline char_box::char_box(char_box &&another) noexcept :
    size_y(another.size_y), size_x(another.size_x), ptr(another.ptr)
    {
        another.ptr = nullptr;
    }

    inline char_box::~char_box(){
        //print();
        //std::printf("deconstructed.\n");
        delete[] ptr;
    }

    inline char_box& char_box::operator=(const char_box& another){
        if(&another == this)
            return *this;
        delete[] ptr;
        size_x = another.size_x;
        size_y = another.size_y;
        ptr = new char[size_y * size_x];
        std::memcpy(another.ptr, this->ptr, size_y * size_x);
        return *this;
    }

    inline char& char_box::operator()(size_t y_idx, size_t x_idx) const{
        return *(ptr + y_idx * size_x + x_idx);
    }

    inline char_box_line char_box::operator[](size_t y_idx){
        if(y_idx > size_y - 1){
            std::printf("char_box y index is out of range. y_idx = %zu, "
                        "while size_y = %zu\n", y_idx, size_y);
            assert(false);
        }
        return char_box_line{ptr + y_idx * size_x, size_x};
    }

    inline void char_box::print() const {
        for(auto i=0; i<=size_y-1; i++){
            for(auto j=0; j<=size_x-1; j++)
                std::printf("%c", *(ptr + i * size_x + j));
            std::printf("\n");
        }
    }

    inline void char_box::write(size_t pos_y, size_t pos_x, const char* str) {
        size_t idx = 0;
        size_t ydx = pos_y;
        size_t xdx = pos_x;

        if(ydx <= size_y - 1)
            while(str[idx]!='\0'){
                if(str[idx]=='\n'){
                    xdx = 0;
                    ydx += 1;
                    idx += 1;
                    continue;
                }
                if(ydx > size_y - 1) break;
                if(xdx > size_x - 1) break;
                (*this)(ydx, xdx) = str[idx];
                xdx += 1;
                idx += 1;
            }
    }

    inline void char_box::write(size_t pos_y, size_t pos_x, const std::string& str) {
        write(pos_y, pos_x, str.c_str());
    }

    inline void char_box::write(size_t pos_y, size_t pos_x, const char_box& another) {
        for(size_t y = pos_y; y <= pos_y + another.size_y - 1; y++){
            if(y > size_y - 1) break;
            for(size_t x = pos_x; x <= pos_x + another.size_x - 1; x++){
                if(x > size_x - 1) break;
                (*this)(y, x) = another(y - pos_y, x - pos_x);
            }
        }
    }


}


#endif //BIN_TREE_PRINTER_CHAR_BOX_H
