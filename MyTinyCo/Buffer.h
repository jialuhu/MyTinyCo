//
// Created by Jialu  Hu on 2019-08-04.
//

#ifndef UNTITLED_BUFFER_H
#define UNTITLED_BUFFER_H

#include <vector>
#include <sys/uio.h>
#include <errno.h>
#include <assert.h>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include "Noncopyable.h"
namespace SiNet {
    class Buffer : Noncopyable{
    public:
        Buffer() : _buf(INIT_SIZE) {
        }

        ~Buffer() {
        }

        void append(const char *data, size_t len) {
            makeSpace(len);
            std::copy(data, data + len, _buf.begin() + _writeindex);
            _writeindex += len;
        }
        int findStr(char *s, const char *p, size_t plen)
        {
            const char *pattern = std::search(s, begin() + _writeindex, p, p + plen);
            return pattern == begin() + _writeindex ? -1 : pattern - s;
        }
        int findStr(const char *s, size_t len)
        {
            return findStr(peek(), s, len);
        }
        int findCrlf() { return findStr("\r\n", 2); }

        //扩展空间以及将内容往前挪动
        void makeSpace(size_t len) {
            if (len > writeable()) {
                if (len <= writeable() + prependable()) {
                    size_t readBytes = readable();
                    std::copy(peek(), peek() + readBytes, begin());
                    _readindex = 0;
                    _writeindex = _readindex + readBytes;
                } else
                    _buf.resize(_writeindex + len);
            }
        }

        void swap(Buffer& _buffer)
        {
            _buf.swap(_buffer._buf);
            std::swap(_readindex, _buffer._readindex);
            std::swap(_writeindex, _buffer._writeindex);
        }

        std::string &Buffer_str(std::string &s1) {
            int len = readable();
            s1.clear();
            for (int i = _readindex; i <= _writeindex; i++) {
                s1.push_back(_buf[i]);
            }
            //readindex_ = writeindex_ = 8;
            return s1;
        }

        // 返回字符串
        const char *c_str() {
            makeSpace(1);
            _buf[_writeindex] = '\0';
            return peek();
        }

        // 更新缓冲区
        void update(size_t len) {
            if (len < readable())
                _readindex += len;
            else
                _readindex = _writeindex = 0;
        }
        void retrieve(size_t len)
        {
            if (len < readable())
                _readindex += len;
            else
                _readindex = _writeindex = 0;
        }
        //void retrieveAll() { update(readable()); }
        size_t readFd(int fd, int &save);

        bool Buffer_find_str(const char *str, std::string &result, size_t len_) {
            std::string find_string;
            Buffer_str(find_string);
            int find = find_string.find(str) + len_;
            if (find - len_ == 0) {
                return false;
            }
            if (find != find_string.npos) {
                result = find_string.substr(0, find);
                size_t len = result.size();
                _readindex += len;
                if (readable() == 0) {
                    _readindex = 0;
                    _writeindex = 0;
                }
            } else {
                return false;
            }
            return true;
        }

        char &operator[](size_t idx) { return _buf[idx]; }

        static const size_t INIT_SIZE = 1024;

        char *begin() { return &*_buf.begin(); }

        char *peek() { return begin() + _readindex; }

        size_t prependable() const { return _readindex; }

        size_t readable() const { return _writeindex - _readindex; }

        size_t writeable() const { return _buf.size() - _writeindex; }

    private:
        std::vector<char> _buf;
        size_t _readindex = 0;
        size_t _writeindex = 0;
    };
}

#endif //UNTITLED_BUFFER_H
