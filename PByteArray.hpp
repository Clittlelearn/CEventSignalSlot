#ifndef _PBYTEARRAY_H_
#define _PBYTEARRAY_H_

#include <array>
#include <list>
#define P_ARRAY_SIZE 5

class PByteArray{
public:

 class Index{
    public:
        Index(){}
        Index(int bl,int ar,std::list<std::array<char,P_ARRAY_SIZE>> * ptr_t, 
        std::list<std::array<char,P_ARRAY_SIZE>>::iterator iter_){
            bolckIndex=bl;
            arrayIndex=ar;
            cachePtr=ptr_t;
            iter=iter_;
        }
        
        int bolckIndex;
        int arrayIndex;
        std::list<std::array<char,P_ARRAY_SIZE>> * cachePtr;
        std::list<std::array<char,P_ARRAY_SIZE>>::iterator iter;
        char & operator*(){
            std::array<char,P_ARRAY_SIZE> & ar=*iter;
            return ar.at(arrayIndex);
        }
        Index(Index & other){
            bolckIndex=other.bolckIndex;
            arrayIndex=other.arrayIndex;
            cachePtr=other.cachePtr;
            iter=other.iter;
        }

        int operator-(Index & other){
            int value_a=bolckIndex*P_ARRAY_SIZE + arrayIndex;
            int value_b=other.bolckIndex* P_ARRAY_SIZE + other.arrayIndex;
            return value_a-value_b;
        }

        bool operator==( Index & other){
            if(other.arrayIndex==this->arrayIndex&& other.bolckIndex == this->bolckIndex){
                return true;
            }
            return false;
        }
        Index operator+(int v){
            std::list<std::array<char,P_ARRAY_SIZE>>::iterator iter_=iter;
            int a=(v+(bolckIndex * P_ARRAY_SIZE)+arrayIndex)% P_ARRAY_SIZE;
            int b=((v+(bolckIndex * P_ARRAY_SIZE)+arrayIndex) /P_ARRAY_SIZE)%cachePtr->size();
    
            int distance=b-bolckIndex;
            if(b==0){
                iter_=cachePtr->begin();

            }else if(distance > 0){
                for(int i=0;i< distance;i++){
                    iter_++;
                }
            }else if(distance < 0){
                for(int i=0;i<-distance;i++){
                    iter_--;
                }
            }

            return Index{b,a,cachePtr,iter_};
        }
       
    };

    PByteArray(){
        cache.push_back(std::array<char,P_ARRAY_SIZE>{});
        begin=Index(0,0,&cache,cache.begin());
        end=Index(0,0,&cache,cache.begin());
    }

   int capacity(){
        return (cache.size()*P_ARRAY_SIZE)-1;
   }

    int size(){
        return end-begin;
    }

    int push(const char * data, int size_t){
        int ret=0;
        int capacity_t=capacity();
        int size_=size();

        if(capacity_t > size_+size_t){

        }else{
            int time=((size_+size_t)-capacity_t)/P_ARRAY_SIZE+1;
            for(int j=0;j< time;j++){
                cache.push_back(std::array<char,P_ARRAY_SIZE>{});
            }   
        }
        for(int i=0;i< size_t;i++){
            (*end)=data[i];
            end=end+1;
            ret++;
        }
        return ret;
    }

    int pop(char * dest,int size_t){
        int size_=size();
        int ret=0;
        size_=(size_ < size_t) ? size_ : size_t;
        for(int i=0;i< size_;i++){
            dest[i]=(*begin);
            (*begin)=0;
            begin=begin+1;
            ret++;
        }
        return ret;
    }

private:
   
    std::list<std::array<char,P_ARRAY_SIZE>>  cache;
    Index begin;
    Index end;
    
};

#endif