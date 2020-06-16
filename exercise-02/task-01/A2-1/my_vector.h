#pragma once
#include <iostream>
#include <cstring>
#include <cmath>


namespace my {

  template <class ValueT>
  class vector {
  private:
    ValueT* data = nullptr;
    size_t capacity = 0;

  public:

    vector(vector<ValueT> const &) = delete;
    vector<ValueT>& operator=(vector<ValueT> const &) = delete;

    vector(size_t n, ValueT init = ValueT()) {
      resize(n);

      if(init == ValueT() ) {
        for(size_t i = 0; i < n; ++i){
          data[i] = init;
        }
      }
    }

    vector() = default;

    virtual ~vector() {
      clear();
    }

    void push_back(const ValueT& arg) {
      resize(capacity+1);
      data[capacity-1] = arg;
    }

    ValueT pop_back() {
      ValueT ret = data[capacity-1];
      resize(capacity-1);
      return ret;
    }

    [[nodiscard]] inline const ValueT& operator[](const size_t index) const {
      return data[index];
    }

    [[nodiscard]] inline ValueT& operator[](const size_t i) {
      return data[i];
    }

    [[nodiscard]] inline bool empty() const{
      return capacity == 0;
    }

    [[nodiscard]] inline size_t size() const{
      return capacity;
    }

    void clear() {
      resize(0);
    }

    [[nodiscard]] ValueT& at(const size_t i) const {

      if(i < capacity)
        return data[i];

      throw std::out_of_range ("Index out of range");
    }

  private:
    void resize(size_t new_size) {
      if(new_size == 0) {
        if(data) {
          delete [] data;
          data = nullptr;
          capacity = 0;
        }
        return;
      }

      ValueT *new_arr = new ValueT[new_size];

      if(data == nullptr) {
        data = new_arr;
        capacity = new_size;
        return;
      }

      for(size_t i = 0; i < std::min(capacity,new_size); ++i ) {
        new_arr[i] = data[i];
      }

      delete[] data;

      data = new_arr;
      capacity = new_size;
    }

  };
}
