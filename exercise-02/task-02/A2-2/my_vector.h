#pragma once
#include <iostream>
#include <cstring>
#include <cmath>


namespace my {

  template <class ValueT>
  class vector {
  private:
    ValueT* _data = nullptr;
    size_t _capacity = 0;
    size_t _size = 0;

  public:

    vector(vector<ValueT> const &) = delete;
    vector<ValueT>& operator=(vector<ValueT> const &) = delete;

    explicit inline vector(size_t n) {
      reserve(n);
    }

    vector(size_t n, ValueT init)
      : vector(n) {

        for(size_t i = 0; i < n; ++i){
           ::new((void*)(_data + i)) ValueT(init);
        }
        _size = n;

    }

    vector() = default;

    virtual ~vector() {
      reserve(0);
    }

    void push_back(const ValueT& arg) {

      if(_capacity == _size) {
        reserve(_capacity*2);
      }


      // https://github.com/electronicarts/EASTL/blob/master/include/EASTL/vector.h (line 1934)
      ::new((void*)(_data +_size++)) ValueT(arg);

    }

    template<typename... Args>
    void emplace_back(Args&&... args) {

      if(_capacity == _size) {
        reserve(_capacity*2);
      }

      // https://github.com/electronicarts/EASTL/blob/master/include/EASTL/vector.h (line 1934)
      ::new((void*)(_data +_size++)) ValueT(std::forward<Args>(args)...);
    }

    ValueT pop_back() {
      //Expliziete Designentscheidung, dass ein Leerer Vektor zum Programmabsturtzt führt
      // crash > runtime error hunt
      ValueT ret = _data[--_size];
      (_data + _size)->~ValueT();
      return ret;
    }

    [[nodiscard]] inline const ValueT& operator[](const size_t index) const {
      return _data[index];
    }

    [[nodiscard]] inline ValueT& operator[](const size_t i) {
      return _data[i];
    }

    [[nodiscard]] inline bool empty() const{
      return _size == 0;
    }

    [[nodiscard]] inline size_t capacity() const{
      return _capacity;
    }

    [[nodiscard]] inline size_t size() const{
      return _size;
    }

    void clear() {
      while(_size) {
        pop_back();
      }
    }

    [[nodiscard]] ValueT& at(const size_t i) const {

      if(i < _size)
        return _data[i];

      throw std::out_of_range ("Index out of range");
    }

    void shrink_to_fit() {
      reserve(_size);
    }


    void reserve(size_t new_capacity) {
      if(new_capacity == 0) {
        if(_data) {
          clear();
          free(_data);
          _data = nullptr;
          _capacity = 0;
          _size = 0;
        }
        return;
      }

      if(_data == nullptr) {
        _data = static_cast<ValueT*>(std::calloc( new_capacity, sizeof(ValueT)));
      } else {
        if(new_capacity < _size) {
          for(size_t i = _size-1; i >= new_capacity; --i ) {
            (_data + i)->~ValueT();
          }
        }
        _data = static_cast<ValueT*>(std::realloc(_data,new_capacity*sizeof(ValueT)));
      }

      _size = std::min(_size,new_capacity);
      _capacity = new_capacity;
    }



  };
}
