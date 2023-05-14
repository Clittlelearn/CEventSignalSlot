#ifndef _PBYTEARRAY_H_
#define _PBYTEARRAY_H_

#include <array>
#include <vector>
#include <list>
#include <string>
#define P_ARRAY_SIZE 5

class PByteArray
{
public:
    class Index
    {
    public:
        Index() {}
        Index(int bl, int ar, std::list<std::vector<char>> *ptr_t,
              std::list<std::vector<char>>::iterator iter_, int size_increase_)
        {
            bolckIndex = bl;
            arrayIndex = ar;
            cachePtr = ptr_t;
            iter = iter_;
            size_increase_index = size_increase_;
        }

        char &operator*()
        {
            std::vector<char> &ar = *iter;
            return ar.at(arrayIndex);
        }
        Index(const Index &other)
        {
            bolckIndex = other.bolckIndex;
            arrayIndex = other.arrayIndex;
            cachePtr = other.cachePtr;
            iter = other.iter;
            size_increase_index=other.size_increase_index;
        }

        int operator-(Index &other)
        {
            int value_a = bolckIndex * size_increase_index + arrayIndex;
            int value_b = other.bolckIndex * size_increase_index + other.arrayIndex;
            return value_a - value_b;
        }

        bool operator==(Index &other)
        {
            if (other.arrayIndex == this->arrayIndex && other.bolckIndex == this->bolckIndex)
            {
                return true;
            }
            return false;
        }
        bool operator!=(Index &other)
        {
            if (other.arrayIndex == this->arrayIndex && other.bolckIndex == this->bolckIndex)
            {
                return false;
            }
            return true;
        }
        int push_unsafe(const char *data, int size)
        {
            int ret=0;
            int a = 0;
            int b = 0;
            int distance = 0;
            for (int i = 0; i < size; i++)
            {
                std::vector<char> &ar = *iter;
                ar.at(arrayIndex)=*data;
                data++;

                a = (1 + ( bolckIndex * size_increase_index) + arrayIndex) % size_increase_index;
                b = ((1 + ( bolckIndex * size_increase_index) + arrayIndex) / size_increase_index) % cachePtr->size();
                distance = b - bolckIndex;
                if (b == 0)
                {
                    iter = cachePtr->begin();
                }
                else if (distance > 0)
                {
                    for (int i = 0; i < distance; i++)
                    {
                        iter++;
                    }
                }
                else if (distance < 0)
                {
                    for (int i = 0; i < -distance; i++)
                    {
                        iter--;
                    }
                }

                arrayIndex = a;
                bolckIndex = b;
                ret++;
            }
            return ret;
        }


        int pop_unsafe(char *data, int size)
        {
            int ret=0;
            int a = 0;
            int b = 0;
            int distance = 0;
            for (int i = 0; i < size; i++)
            {
                std::vector<char> &ar = *iter;
                *data=ar.at(arrayIndex);
                ar.at(arrayIndex)=0;
                data++;

                a = (1 + (bolckIndex * size_increase_index)+ arrayIndex) % size_increase_index;
                b = ((1 +(bolckIndex * size_increase_index)+ arrayIndex) / size_increase_index) % cachePtr->size();
                distance = b - bolckIndex;
                if (b == 0)
                {
                    iter = cachePtr->begin();
                }
                else if (distance > 0)
                {
                    for (int i = 0; i < distance; i++)
                    {
                        iter++;
                    }
                }
                else if (distance < 0)
                {
                    for (int i = 0; i < -distance; i++)
                    {
                        iter--;
                    }
                }

                arrayIndex = a;
                bolckIndex = b;
                ret++;
            }
            return ret;
        }

       

        Index operator+(int v)
        {
            std::list<std::vector<char>>::iterator iter_ = iter;
            int a = (v + (bolckIndex * size_increase_index) + arrayIndex) % size_increase_index;
            int b = ((v + (bolckIndex * size_increase_index) + arrayIndex) / size_increase_index) % cachePtr->size();

            int distance = b - bolckIndex;
            if (b == 0)
            {
                iter_ = cachePtr->begin();
            }
            else if (distance > 0)
            {
                for (int i = 0; i < distance; i++)
                {
                    iter_++;
                }
            }
            else if (distance < 0)
            {
                for (int i = 0; i < -distance; i++)
                {
                    iter_--;
                }
            }

            return Index{b, a, cachePtr, iter_, size_increase_index};
        }


        Index & operator++(int)
        {
           
            int a = (1 + (bolckIndex * size_increase_index) + arrayIndex) % size_increase_index;
            int b = ((1 + (bolckIndex * size_increase_index) + arrayIndex) / size_increase_index) % cachePtr->size();

            int distance = b - bolckIndex;
            if (b == 0)
            {
                iter = cachePtr->begin();
            }
            else if (distance > 0)
            {
                for (int i = 0; i < distance; i++)
                {
                    iter++;
                }
            }
            else if (distance < 0)
            {
                for (int i = 0; i < -distance; i++)
                {
                    iter--;
                }
            }

            arrayIndex = a;
            bolckIndex = b;
            return *this;
        }

    private:
        friend PByteArray;
        int bolckIndex;
        int arrayIndex;
        std::list<std::vector<char>> *cachePtr;
        std::list<std::vector<char>>::iterator iter;
        int size_increase_index;
    };

    PByteArray(int size_num = 1024)
    {
        
        cache.push_back(std::vector<char>(size_num, 0));
        begin = Index(0, 0, &cache, cache.begin(), size_num);
        end = Index(0, 0, &cache, cache.begin(), size_num);
        size_increase = size_num;
    }

    PByteArray(const PByteArray & other){
        this->cache=other.cache;
        this->size_increase=other.size_increase;
        this->begin=Index(other.begin.bolckIndex,other.begin.arrayIndex,&cache,cache.begin(),size_increase);
        this->end=Index(other.end.bolckIndex,other.end.arrayIndex,&cache,cache.begin(),size_increase);
        for(int i=0;i< other.begin.bolckIndex;i++){
            begin.iter++;
        }
        for(int i=0;i < other.end.bolckIndex;i++){
            end.iter++;
        }
    }

    PByteArray & operator=(const PByteArray & other){
        this->cache=other.cache;
        this->size_increase=other.size_increase;
        this->begin=Index(other.begin.bolckIndex,other.begin.arrayIndex,&cache,cache.begin(),size_increase);
        this->end=Index(other.end.bolckIndex,other.end.arrayIndex,&cache,cache.begin(),size_increase);
        for(int i=0;i< other.begin.bolckIndex;i++){
            begin.iter++;
        }
        for(int i=0;i < other.end.bolckIndex;i++){
            end.iter++;
        }
        return *this;
    }

    int capacity()
    {
        return (cache.size() * size_increase) - 1;
    }

    int size()
    {
        
        int now_size = end - begin;
        int capacity_ = cache.size() * size_increase;

        int ret = (now_size + capacity_) % capacity_;
        return ret;
    }

    int push(const char *data, int Msize_t)
    {
        int ret = 0;
        int capacity_t = capacity();
        int size_ = size();

        if (capacity_t > size_ + Msize_t)
        {
        }
        else
        {
            int time = ((size_ + Msize_t) - capacity_t) / size_increase + 1;
            for (int j = 0; j < time; j++)
            {
                 if(begin.bolckIndex > end.bolckIndex){
                    begin.bolckIndex++;
                    cache.insert(begin.iter,std::vector<char>(size_increase, 0));
                }else if(begin.bolckIndex==end.bolckIndex){

                      if(begin.arrayIndex > end.arrayIndex){
                        auto nBigenIter=begin.iter;
                        std::vector<char> tts=*nBigenIter;
                        cache.insert(++nBigenIter,tts);
                        begin.iter++;
                        begin.bolckIndex++;

                      }else{
                            cache.insert(begin.iter,std::vector<char>(size_increase, 0));
                            begin.bolckIndex++;
                            end.bolckIndex++;
                      }
                }else{
                    cache.insert(begin.iter,std::vector<char>(size_increase, 0));
                    begin.bolckIndex++;
                    end.bolckIndex++;
                }
            }
        }
        end.push_unsafe(data,Msize_t);
        return ret;
    }

    int pop(char *dest, int Msize_t)
    {
        int size_ = size();
        int ret = 0;
        size_ = (size_ < Msize_t) ? size_ : Msize_t;
        
        return begin.pop_unsafe(dest,size_);
    }

    std::string read_to_string(){
        std::string ts(size(),0);
        int i=0;
        for(Index s=begin;s!=end;s++){
            ts[i]=*s;
            i++;
        }
        return ts;
    }

    std::string read_to_string(int size_){
        size_=(size() > size_) ? size_ : size();
        std::string ts(size_,0);
        int i=0;
        for(Index s=begin;s!=end;s++){
            ts[i]=*s;
            i++;
        }
        return ts;
    }

    std::string pop_to_string(){
        std::string ts(size(),0);
        int i=0;
        for(;begin!=end;begin++){
            ts[i]=*begin;
            *begin=0;
           i++;
        }
        return ts;
    }

    std::string pop_to_string(int size_){
        size_=(size() > size_) ? size_ : size();
        std::string ts(size_,0);
        int i=0;
        for(;begin!=end && i< size_;begin++){
            ts[i]=*begin;
            *begin=0;
            i++;
        }
        return ts;
    }

    int push_from_string(const std::string & str){
        int ret = 0;
        int capacity_t = capacity();
        int Msize_t=str.size();
        int size_ = size();

        if (capacity_t > size_ + Msize_t)
        {
        }
        else
        {
            int time = (Msize_t) / size_increase + 2;
            for (int j = 0; j < time; j++)
            {
                //cache.insert(std::vector<char>(size_increase, 0));
               
                if(begin.bolckIndex > end.bolckIndex){
                    begin.bolckIndex++;
                    cache.insert(begin.iter,std::vector<char>(size_increase, 0));
                }else if(begin.bolckIndex==end.bolckIndex){

                      if(begin.arrayIndex > end.arrayIndex){
                        auto nBigenIter=begin.iter;
                        std::vector<char> tts=*nBigenIter;
                        cache.insert(++nBigenIter,tts);
                        begin.iter++;
                        begin.bolckIndex++;

                      }else{
                            cache.insert(begin.iter,std::vector<char>(size_increase, 0));
                            begin.bolckIndex++;
                            end.bolckIndex++;
                      }
                }else{
                    cache.insert(begin.iter,std::vector<char>(size_increase, 0));
                    begin.bolckIndex++;
                    end.bolckIndex++;
                }
            }
        }
       return end.push_unsafe(str.c_str(),str.size());
    }
private:
    int size_increase;
    std::list<std::vector<char>> cache;
    Index begin;
    Index end;
    
};

#endif