#pragma once

#include <memory>
#include <iostream>
#include <utility>
#include <tuple>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <functional>

#include <cassert>

// forward declaration to resolve cross-referencing
namespace my {
    template<typename K, typename T>
    class treemap;
}

// forward declaration to resolve cross-referencing
template<typename K, typename T>
void swap(my::treemap<K,T>& lhs, my::treemap<K,T>& rhs);

template<typename K, typename T>
void export_treemap(const std::string& filename, const my::treemap<K, T>& t);

namespace __private {
  template<typename K, typename T>
  void createNode(std::fstream& file, unsigned int& counter, std::shared_ptr<typename my::treemap<K,T>::node> current);
}

namespace my {

/*
 * class treemap<K,T>
 * represents an associative container (dictionary) with unique keys
 * implemented by a binary search tree
 *
 */
template<typename K, typename T>
class treemap
{

  public:

      // public type aliases
      using key_type = K;
      using mapped_type = T;
      using value_type = std::pair<K, T>;

  protected:

    /*
     * hierarchical node structure for class treemap
     *
     */
    class node {
      public:
        explicit node(const value_type& vt)
          : data(vt)
        {}

        [[nodiscard]] inline const K& key() const noexcept {
          return data.first;
        }

        [[nodiscard]] inline const T& value() const noexcept {
          return data.second;
        }

        // key and value represented as pair (so they can be easily passed along together)
        value_type data;
        std::weak_ptr<node> parent;
        std::shared_ptr<node> left;
        std::shared_ptr<node> right;

    }; // class node

public:
    // iterator: references a node within the tree
    class iterator {

    protected:

        // treemap is a friend, can call protected constructor
        friend class treemap;

        // construct iterator referencing a speciic node
        // - only treemap shall be allowed to do so
        iterator(const treemap& map, std::shared_ptr<node> node = std::shared_ptr<typename treemap::node>())
          : map(map),
            current(node)
        {}
    public:

        // access data of referenced map element (node)
        value_type& operator*()
        {
          std::shared_ptr<node> node = current.lock();

          // will crash with SIGSEV on misuse (for example calling for end())
          return node->data;
        }

        value_type* operator->()
        {
          std::shared_ptr<node> node = current.lock();
          if (!node) {
            return nullptr;
          }

          return &node->data;
        }

        // two iterators are equal if they point to the same node
        bool operator==(const iterator& it) const
        {
            return current.lock() == it.current.lock();
        }

        bool operator!=(const iterator& it) const
        {
            return !(*this == it);
        }

        operator bool() const {
          return !current.expired();
        }

        // next element in map, pre-increment
        // note: must modify self!
        iterator& operator++()
        {
          std::shared_ptr<node> p = current.lock();
          if (!p) {
            current = std::shared_ptr<node>();
            return *this;
          }

          if (p->left) {
            current = p->left;
          }
          else if (p->right) {
            current = p->right;
          }
          else {
            current = find_next(p);
          }

          return *this;
        }

        // prev element in map, pre-decrement
        // note: must modify self!
        iterator& operator--()
        {
          std::shared_ptr<node> p = current.lock();
          if (!p) {
            current = FindLast(map.root);
            return *this;
          }

          std::shared_ptr<node> parent = p->parent.lock();
          if (!parent) {
            current = std::shared_ptr<node>();
            return *this;
          }

          if (p == parent->left) {
            current = parent;
          }
          else if (p == parent->right && !parent->left) {
            current = parent;
          }
          else {
            current = find_prev(parent->left);
          }

          return *this;
        }

      protected:
        static std::shared_ptr<node> FindLast(std::shared_ptr<node> start) {
          if (!start) {
            return std::shared_ptr<node>();
          }

          std::shared_ptr<node> p = start;
          while (p->right || p->left) {
            if(p->right) {
              p = p->right;
            }
            else {
              p = p->left;
            }
          }

          return p;
        }

        std::shared_ptr<node> find_next(std::shared_ptr<node> p) {
          std::shared_ptr<node> parent = p->parent.lock();
          if (!parent) {
            return std::shared_ptr<node>();
          }

          if (p == parent->left && parent->right) {
            return parent->right;
          }

          return find_next(parent);
        }

        std::shared_ptr<node> find_prev(std::shared_ptr<node> p) {
          if (!p) {
            return std::shared_ptr<node>();
          }

          if (p->right) {
            return find_prev(p->right);
          }
          else if (p->left) {
            return find_prev(p->left);
          }

          return p;
        }

      private:
        // non-owning reference to the actual node
        std::weak_ptr<node> current;
        const treemap& map;
    }; // class iterator

    // ordered iterator: references a node within the tree
    class ordered_iterator {

    protected:

        // treemap is a friend, can call protected constructor
        friend class treemap;

        // construct iterator referencing a speciic node
        // - only treemap shall be allowed to do so
        ordered_iterator(const treemap& map, std::shared_ptr<node> node = std::shared_ptr<typename treemap::node>())
          : map(map),
            current(node)
        {}
    public:

        // access data of referenced map element (node)
        value_type& operator*()
        {
          std::shared_ptr<node> node = current.lock();

          // will crash with SIGSEV on misuse (for example calling for end())
          return node->data;
        }

        value_type* operator->()
        {
          std::shared_ptr<node> node = current.lock();
          if (!node) {
            return nullptr;
          }

          return &node->data;
        }

        // two iterators are equal if they point to the same node
        bool operator==(const ordered_iterator& it) const
        {
            return current.lock() == it.current.lock();
        }

        bool operator!=(const ordered_iterator& it) const
        {
            return !(*this == it);
        }

        operator bool() const {
          return !current.expired();
        }

        // next element in map, pre-increment
        // note: must modify self!
        ordered_iterator& operator++()
        {
          std::shared_ptr<node> p = current.lock();
          if (!p) {
            current = std::shared_ptr<node>();
            return *this;
          }

          std::shared_ptr<node> parent = p->parent.lock();
          if (p->right) {
            current = FindFirst(p->right);
          }
          else if (parent && parent->left == p) {
            current = parent;
          }
          else {
            while(parent && (!parent->right || parent->right == p)) {
              p = parent;
              parent = p->parent.lock();
            }

            if (!parent) {
              current = std::shared_ptr<node>();
            }
            else {
              current = parent->right;
            }
          }

          return *this;
        }

        // prev element in map, pre-decrement
        // note: must modify self!
        ordered_iterator& operator--()
        {
          std::shared_ptr<node> p = current.lock();
          if (!p) {
            current = FindLast(map.root);
            return *this;
          }

          if (p == FindFirst(map.root)) {
            current = std::shared_ptr<node>();
            return *this;
          }

          if (p->left) {
            current = FindLast(p->left);
            return *this;
          }

          std::shared_ptr<node> parent = p->parent.lock();
          if (!parent) {
            current = std::shared_ptr<node>();
            return *this;
          }

          if (parent && p == parent->right) {
            current = parent;
            return *this;
          }

          while(parent && (!parent->left || parent->left == p)) {
            p = parent;
            parent = p->parent.lock();
          }

          if (!p) {
            current = std::shared_ptr<node>();
          }
          else {
            current = p;
          }

          return *this;
        }

        static std::shared_ptr<node> FindFirst(std::shared_ptr<node> start) {
          if (!start) {
            return std::shared_ptr<node>();
          }

          std::shared_ptr<node> p = start;
          while(p->left) {
            p = p->left;
          }

          return p;
        }

        static std::shared_ptr<node> FindLast(std::shared_ptr<node> start) {
          if (!start) {
            return std::shared_ptr<node>();
          }

          std::shared_ptr<node> p = start;
          while (p->right) {
            p = p->right;
          }

          return p;
        }
      private:
        // non-owning reference to the actual node
        std::weak_ptr<node> current;
        const treemap& map;
    }; // class iterator

    struct _ordered {
      public:
        explicit _ordered(treemap& map)
          : map(map)
        {}

        _ordered(const _ordered&) = default;

        ordered_iterator begin() {
          return map.obegin();
        }

        ordered_iterator end() {
          return map.oend();
        }
      private:
        treemap& map;
    };


    // used for copy&move
    template<typename KK, typename TT>
    friend void ::swap(treemap<KK,TT>& , treemap<KK,TT>& );

    template<typename KK, typename TT>
    friend void ::export_treemap(const std::string&, const treemap<KK,TT>&);

    template<typename KK, typename TT>
    friend void __private::createNode(std::fstream&, unsigned int&, std::shared_ptr<typename my::treemap<KK,TT>::node>);

    // construct empty map
    treemap();

    // move ctor
    treemap(treemap<K,T>&&);

    // deep copy ctor
    treemap(const treemap<K,T>&);

    // how often is the element contained in the map?
    // (for this type of container, can only return 0 or 1)
    [[nodiscard]] inline size_t count(const K&) const noexcept;

    // assignment (move & copy)
    treemap<K,T>& operator=(treemap<K,T>);

    // remove/destroy all elements
    void clear();

    // random read-only access to value by key, does not modify map
    T operator[](const K& ) const;

    // random write access to value by key
    T& operator[](const K&);

    // number of elements in map (nodes in tree)
    [[nodiscard]] inline size_t size() const noexcept;

    // iterator referencing first element (node) in map
    iterator begin();

    // iterator referencing first element (node) in ordered map
    ordered_iterator obegin();

    // iterator referencing no element (node) in map
    iterator end() const noexcept;

    // iterator referencing no element (node) in ordered map
    ordered_iterator oend() const noexcept;

    _ordered ordered();

    // add a new element into the tree
    // returns pair, consisting of:
    // - iterator to element
    // - bool
    //   - true if element was inserted;
    //   - false if key was already in map (will not overwrite existing value)
    std::pair<iterator,bool> insert(const K&, const T&);

    // add a new element into the tree, or overwrite existing element if key already in map
    // returns:
    // - iterator to element
    // - true if element was newly created; false if existing element was overwritten
    std::pair<iterator,bool> insert_or_assign(const K&, const T&);

    // find element with specific key. returns end() if not found.
    iterator find(const K&) const;

    // Tries to eliminate ugly chains
    void optimize();

    void enableOptimization();
    void disableOptimization();

  private:
    iterator create(const K&, const T&);
    std::pair<iterator, bool> get(const K&);

  protected:
    std::shared_ptr<node> root = std::shared_ptr<node>();
    std::size_t nodeCount  = 0;
    std::less<K> compare = std::less<K>();
    bool useOptimization = false;
};

template<typename K, typename T>
treemap<K,T>::treemap() {}

template<typename K, typename T>
void treemap<K,T>::clear()
{
  root = nullptr;
  nodeCount = 0;
}

// random read-only access to value by key
template<typename K, typename T>
T treemap<K,T>::operator[](const K& key) const
{
  iterator element = find(key);
  if (element != end()) {
    return element->second;
  }

  return T();
}

// random write access to value by key
template<typename K, typename T>
T& treemap<K,T>::operator[](const K& key)
{
    auto [element, isNew] = get(key);
    return element->second;
}

// number of elements in map (nodes in tree)
template<typename K, typename T>
[[nodiscard]] inline size_t treemap<K,T>::size() const noexcept
{
    return nodeCount;
}

// move ctor
template<typename K, typename T>
treemap<K,T>::treemap(treemap<K,T>&& other)
{
    root = other.root;
    nodeCount = other.nodeCount;

    other.root = nullptr;
    other.nodeCount = 0;
}

// deep copy ctor
template<typename K, typename T>
treemap<K,T>::treemap(const treemap<K,T>& other)
{
  for (iterator it = iterator(other, other.root); it != other.end(); ++it) {
    std::pair<K,T> element = *it;
    insert(element.first, T(element.second));
  }
}

// assignment (move & copy)
template<typename K, typename T>
treemap<K,T>& treemap<K,T>::operator=(treemap<K,T> t)
{
    root = t.root;
    nodeCount = t.nodeCount;

    t.root = nullptr;
    t.nodeCount = 0;

    return *this;
}


// iterator referencing first element (node) in map
template<typename K, typename T>
typename treemap<K,T>::iterator treemap<K,T>::begin()
{
    return iterator(*this, root);
}

// iterator referencing first element (node) in map
template<typename K, typename T>
typename treemap<K,T>::ordered_iterator treemap<K,T>::obegin()
{
    return ordered_iterator(*this, ordered_iterator::FindFirst(root));
}

// iterator referencing no element (node) in map
template<typename K, typename T>
typename treemap<K,T>::iterator treemap<K,T>::end() const noexcept
{
    return iterator(*this);
}

// iterator referencing no element (node) in ordered map
template<typename K, typename T>
typename treemap<K,T>::ordered_iterator treemap<K,T>::oend() const noexcept
{
    return ordered_iterator(*this);
}

template<typename K, typename T>
typename treemap<K,T>::_ordered treemap<K,T>::ordered() {
  return _ordered(*this);
}

// add a new element into the tree
// returns:
// - iterator to element
// - true if element was inserted; false if key was already in map
template<typename K, typename T>
std::pair<typename treemap<K,T>::iterator,bool> treemap<K,T>::insert(const K& key, const T& value)
{
  auto [element, isNew] = get(key);
  if (isNew) {
    element->second = value;
  }

  return std::make_pair(element, isNew);
}

// add a new element into the tree, or overwrite existing element if key already in map
// returns:
// - iterator to element
// - true if element was newly created; false if existing element was overwritten
template<typename K, typename T>
std::pair<typename treemap<K,T>::iterator,bool>
treemap<K,T>::insert_or_assign(const K& key, const T& value)
{
  auto [element, isNew] = get(key);
  element->second = value;

  return std::make_pair(element, isNew);
}

// find element with specific key. returns end() if not found.
template<typename K, typename T>
typename treemap<K,T>::iterator
treemap<K,T>::find(const K& key) const
{
  std::shared_ptr<node> current = root;
  while (current && current->data.first != key) {
    if (compare(key, current->key())) {
      current = current->left;
    }
    else {
      current = current->right;
    }
  }

  if (!current) {
    return end();
  }

  return iterator(*this, current);
}

// how often is the element contained in the map?
template<typename K, typename T>
[[nodiscard]] inline size_t treemap<K,T>::count(const K& key) const noexcept
{
    return find(key) ? 1 : 0;
}

template<typename K, typename T>
typename treemap<K,T>::iterator treemap<K,T>::create(const K& key, const T& value) {
  if (!root) {
    root = std::make_shared<node>(std::make_pair(key, value));
    ++nodeCount;
    return iterator(*this, root);
  }

  std::shared_ptr<node> current = root;
  while(current) {
    if (key == current->key()) {
      // can not create already existing element
      return end();
    }

    if (compare(key, current->key())) {
      // left
      if (!current->left) {
        current->left = std::make_shared<node>(std::make_pair(key, value));
        current->left->parent = current;
        ++nodeCount;

        iterator result = iterator(*this, current->left);
        if (useOptimization) {
          optimize();
        }
        return result;
      }

      current = current->left;
    }
    else {
      // right
      if (!current->right) {
        current->right = std::make_shared<node>(std::make_pair(key, value));
        current->right->parent = current;
        ++nodeCount;

        iterator result = iterator(*this, current->right);
        if (useOptimization) {
          optimize();
        }
        return result;
      }

      current = current->right;
    }
  }

  return end();
}

template<typename K, typename T>
std::pair<typename treemap<K,T>::iterator, bool> treemap<K,T>::get(const K& key) {
  iterator element = find(key);
  if (element != end()) {
    return std::make_pair(element, false);
  }

  return std::make_pair(create(key, T()), true);
}

template<typename K, typename T>
void treemap<K,T>::optimize() {
  bool found = false;

  do {
    found = false;
    for(iterator it = begin(); it != end(); ++it) {
      std::shared_ptr<node> current = it.current.lock();
      std::shared_ptr<node> parent = (current ? current->parent.lock() : std::shared_ptr<node>());
      std::shared_ptr<node> grandparent = (parent ? parent->parent.lock() : std::shared_ptr<node>());

      if (!current || !parent || !grandparent)
        continue;

      // Left Chain
      if (!parent->right && !grandparent->right) {
        // Reorder!
        std::shared_ptr<node> grandgrandparent = grandparent->parent.lock();
        parent->parent = grandparent->parent;
        parent->right = grandparent;
        grandparent->parent = parent;
        grandparent->left = nullptr;

        if (grandgrandparent) {
          if (grandgrandparent->left == grandparent) {
            grandgrandparent->left = parent;
          }
          else if (grandgrandparent->right == grandparent) {
            grandgrandparent->right = parent;
          }
        }

        if (grandparent == root) {
          root = parent;
        }

        found = true;
        break;
      }

      // Right Chain
      if (!parent->left && !grandparent->left) {
        // Reorder!
        std::shared_ptr<node> grandgrandparent = grandparent->parent.lock();
        parent->parent = grandparent->parent;
        parent->left = grandparent;
        grandparent->parent = parent;
        grandparent->right = nullptr;

        if (grandgrandparent) {
          if (grandgrandparent->left == grandparent) {
            grandgrandparent->left = parent;
          }
          else if (grandgrandparent->right == grandparent) {
            grandgrandparent->right = parent;
          }
        }

        if (grandparent == root) {
          root = parent;
        }

        found = true;
        break;
      }
    }

    if (found) {
      static int count = 0;
      std::stringstream ss;
      ss << "optimization-" << count << ".dot";

      export_treemap(ss.str(), *this);
    }
  } while(found);
}

template<typename K, typename T>
void treemap<K,T>::enableOptimization() {
  useOptimization = true;
}

template<typename K, typename T>
void treemap<K,T>::disableOptimization() {
  useOptimization = false;
}

} // namespace my

// swap is overloaded in global namespace
// see https://stackoverflow.com/questions/11562/how-to-overload-stdswap
// (answer by Attention Mozza314)
template<typename K, typename T>
void swap(my::treemap<K,T>& a, my::treemap<K,T>& b)
{
  my::treemap<K,T> tmp = a;

  // a = b; would create a copy, which is a performance loss
  a.root = b.root;
  a.nodeCount = b.nodeCount;

  // b = tmp; would create a copy, which is a performance loss
  b.root = tmp.root;
  b.nodeCount = tmp.nodeCount;
}

namespace __private {
  template<typename K, typename T>
  void createNode(std::fstream& file, unsigned int& counter, std::shared_ptr<typename my::treemap<K,T>::node> current) {
    if (!current)
      return;

    if (!current->left && !current->right) {
      return;
    }


    if (!current->left) {
      std::stringstream name;
      name << "node" << (++counter);

      file << "\t" << name.str() << " [label=\"\"];" << std::endl;
      file << "\t\"" << current->key() << "\" -> \"" << name.str() << "\";" << std::endl;
    }
    else {
      file << "\t\"" << current->key() << "\" -> \"" << current->left->key() << "\";" << std::endl;
    }

    if (!current->right) {
      std::stringstream name;
      name << "node" << (++counter);

      file << "\t" << name.str() << " [label=\"\"];" << std::endl;
      file << "\t\"" << current->key() << "\" -> \"" << name.str() << "\";" << std::endl;
    }
    else {
      file << "\t\"" << current->key() << "\" -> \"" << current->right->key() << "\";" << std::endl;
    }
    file << std::endl;

    createNode<K,T>(file, counter, current->left);
    createNode<K,T>(file, counter, current->right);
  };
}

// helper debug print of treemap
template<typename K, typename T>
void export_treemap(const std::string& filename, const my::treemap<K, T>& t) {
  std::fstream file;
  unsigned int counter = 0;

  file.open(filename, std::ios_base::out);
  if (!file.is_open()) {
    return;
  }

  file << "digraph G {" << std::endl;
  __private::createNode<K, T>(file, counter, t.root);
  file << "}" << std::endl;
  file.close();
}

