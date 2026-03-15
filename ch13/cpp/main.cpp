#include<iostream>
#include <cstdint>
#include <vector>
// A straighforward implementation of red-black trees
namespace alg{
struct rb_tree{
  using color_t = uint_least8_t; struct node;
 private:
  using val_t = int;
  using np_t = uint64_t;
 public: 
  static constexpr bool is_red(color_t c)  {return (c & 1) == 1;}
  static constexpr bool is_black(color_t c)  {return (c & 1) == 0;}
  static constexpr void set_black(color_t& c)  {c &= 0xFE;}
  static constexpr void set_red(color_t& c)  {c |=  1;}
  struct node{
   np_t p,l,r;val_t val;color_t c;
  };
  std::vector<node> v;
  rb_tree(){ v.push_back({0,0,0,val_t{},color_t{}});}
  rb_tree(std::initializer_list<node> l){ 
    v.reserve(l.size() + 1);
    v.push_back({0,0,0,val_t{},color_t{}});
    v.insert(v.begin()+1,l);
  }
  np_t nil()  {return np_t{};}
  node & ref(np_t n){return v[n];}
  size_t size() const {return v.size() - 1;}
};

}

static void test_color_ops(){
    alg::rb_tree::color_t c{0};
    std::cout << "default value: " << "is red :" << alg::rb_tree::is_red(c) << " is black :" << alg::rb_tree::is_black(c) << '\n'; 
    alg::rb_tree::set_red(c);
    std::cout << "set_red(c): "<< "is red :" << alg::rb_tree::is_red(c) << " is black :" << alg::rb_tree::is_black(c) << '\n'; 
    alg::rb_tree::set_black(c);
    std::cout << "set_black(set_red(c)): " << "is red :" << alg::rb_tree::is_red(c) << " is black :" << alg::rb_tree::is_black(c) << '\n';
    alg::rb_tree::set_black(c);
    std::cout << "set_black(set_black(set_red(c))): " << "is red :" << alg::rb_tree::is_red(c) << " is black :" << alg::rb_tree::is_black(c) << '\n';
    alg::rb_tree::set_red(c);
    std::cout << "set_red(...) " << "is red :" << alg::rb_tree::is_red(c) << " is black :" << alg::rb_tree::is_black(c) << '\n'; 
    alg::rb_tree::set_red(c);
    std::cout << "set_red(set_red(...)) " << "is red :" << alg::rb_tree::is_red(c) << " is black :" << alg::rb_tree::is_black(c) << '\n'; 
}

static void test_left_rotate(){
    alg::rb_tree rb({
        {0,0,0,1,0}        
    });
    std::cout<< rb.size() << '\n';
    alg::rb_tree rb2;
    std::cout<< rb2.size() << '\n';
}

int main(){
    //test_color_ops();
    test_left_rotate();

    return 0;
}