#ifndef __BOX_H_
#define __BOX_H_
#include <cstddef>
#include <cstring>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include <iostream>

class ReadBoxQueue;
class ReadBox {
public:
	class index {
	public:
		size_t read(unsigned char* bf, size_t len) {
			if (Len == 0) {
				return 0;
			}
			size_t le_ = (Len > len) ? len : Len;
			memcpy(bf, p, le_);
			p = p + le_;
			Len -= le_;
			return le_;
		}

		std::string read(size_t len) {
			if (Len == 0) {
				return "";
			}
			size_t le_ = (Len > len) ? len : Len;
			std::string ret((const char *)p, le_);
			p = p + le_;
			Len -= le_;
			return ret;
		}

		bool isValid(){
			if (p != nullptr) {
				return true;
			}
			return false;
		}

		bool isEmpty() {
			if (Len == 0) {
				return true;
			}
			return false;
		}

       

	private:
		friend ReadBox;
		friend class ReadBoxQueue;
		index() {}
		index(unsigned char* p_,size_t le) :p(p_),Len(le) {};
		size_t Len = 0;
		unsigned char* p=nullptr;
	};

	
	index CreateIndex() {
		return index(ptr.get(), _len);
	}

	ReadBox() {}
	ReadBox(const std::string& str) {
		unsigned char* bf = new unsigned char[str.size()];
		memcpy(bf, str.c_str(), str.size());
		ptr = std::shared_ptr<unsigned char>(bf, [](unsigned char* p) {delete[] p; });
		_len = str.size();
	}
	ReadBox(const unsigned char* p, size_t len) {
        unsigned char* bf = new unsigned char[len];
		memcpy(bf, p, len);
		ptr = std::shared_ptr<unsigned char>(bf, [](unsigned char* p) {delete[] p; });
		_len = len;
    }
    ReadBox(unsigned char* p, size_t len,bool fl) {
		ptr = std::shared_ptr<unsigned char>(p, [](unsigned char* p) {delete[] p; });
		_len = len;
    }
	ReadBox(const ReadBox& other) {
		ptr = other.ptr;
		_len = other._len;
	}
	ReadBox(const ReadBox&& other) noexcept {
		ptr = other.ptr;
		_len = other._len;
	}

	void operator=(const ReadBox& other) {
		ptr = other.ptr;
		_len = other._len;
	}

	void operator=(const ReadBox&& other) noexcept {
		ptr = other.ptr;
		_len = other._len;
	}

	unsigned char operator[](size_t index) {
		return (ptr.get())[index];
	}
	size_t lenght() const{
		return _len;
	}
     unsigned char * unsafe_ptr(){
            return ptr.get();
      }
private:
	std::shared_ptr<unsigned char> ptr;
	size_t _len;
};


class ReadBoxQueue {
public:
	void push(const ReadBox& box){
		boxqueue.push(box);
        AllBytes+=box.lenght();
	}
	size_t read(unsigned char* buf, size_t len) {
		bool is_end = false;
		size_t readBytes = 0;
		if (!_index.isValid()) {
			_index = _nextBoxIndex(is_end);
		}
		while (0 < len && _index.isValid()) {
			if (is_end) {
				return readBytes;
			}
			size_t ret = _index.read(buf, len);
			buf = buf + ret;
			readBytes += ret;
			len -= ret;
            AllBytes-=ret;
			if (ret == 0 || _index.isEmpty()) {
				boxqueue.pop();
				_index = _nextBoxIndex(is_end);
			}
		}
		return readBytes;
	}


	std::string read(size_t len) {
		bool is_end = false;
		std::string readBytes;
		if (!_index.isValid()) {
			_index = _nextBoxIndex(is_end);
		}
		while (0 < len && _index.isValid()) {
			if (is_end) {
				return readBytes;
			}
			std::string ret = _index.read(len);
			readBytes += ret;
            AllBytes-=ret.size();
			len -= ret.size();
			if (ret.size() == 0 || _index.isEmpty()) {
				boxqueue.pop();
                
				_index = _nextBoxIndex(is_end);
			}
		}
		return readBytes;

	}
    size_t lenght(){
        return AllBytes;
    }
private:
	ReadBox::index _nextBoxIndex(bool &is_end) {
		if (boxqueue.empty()) {
			is_end = true;
			return ReadBox::index();
		}
		ReadBox box = boxqueue.front();
		is_end = false;
		return box.CreateIndex();
	}
	std::queue<ReadBox> boxqueue;
	ReadBox::index  _index;
    uint64_t AllBytes;
};
#endif