#include<iostream>
#include <cstdint>
#include <vector>
// A straighforward implementation of red-black trees
namespace alg{

struct rb_tree{
  using color_t = uint_least8_t; struct node;
  using val_t = int;
  using np_t = uint64_t;
  np_t root_ = np_t{};
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
    v.insert(v.end(),l.begin(), l.end());
    root_ = l.size() ? node_size() : np_t{};
  }
  np_t nil() const  {return np_t{};}
  node & deref(np_t n){return *(node*)( (char*)v.data()+ n);}
  node const & deref(np_t n) const {return *(node*)( (char*)v.data()+ n);}
  size_t size() const {return v.size() - 1;}
  size_t root() const {return root_;}
  
  void left_rotate(np_t x){
    auto& xref = deref(x);
    auto y = xref.r;
    if (y == nil()) return; 
    auto& yref = deref(y);
    xref.r = yref.l;deref(yref.l).p = x;

    if (x == root())
     root_ = y;
    else if(deref(xref.p).l == x) deref(xref.p).l = y;
    else deref(xref.p).r = y;

    yref.l = x;
    yref.p = xref.p;
    xref.p = y;
    xref.p = y;
  }

  void right_rotate(np_t x){
    auto& refx{deref(x)};
    if (refx.l == nil()) return;
    auto y{refx.l};
    auto& refy{deref(y)};
    refx.l = refy.r;
    refy.r = x;
    if (root() == x) root_ = y;
    else if (deref(refx.p).l == x) deref(refx.p).l = y;
    else deref(refx.p).r = y;
    refy.p = refx.p;
    refx.p = y;
    deref(refx.l).p = x;
  }
  // Actual size used to store a node could differ from size required by the structure node.
  static constexpr size_t node_size() { return sizeof(node);}
};

static void dump(std::ostream& os, rb_tree::np_t np, rb_tree const & rb ){
 if (np == rb.nil()) return;
 bool leaf = rb.deref(np).l == rb.nil() && rb.deref(np).r == rb.nil(); 
 if (!leaf) os << "(";
 os << rb.deref(np).val; os << "[parent=" << rb.deref(rb.deref(np).p).val << "]";
 if (rb.root() == np) os << "(ROOT)"; 
 if (!leaf) os << " ";
 dump(os,rb.deref(np).l,rb);
 if (rb.deref(np).l != rb.nil() && rb.deref(rb.deref(np).l).l == rb.nil() && rb.deref(rb.deref(np).l).r == rb.nil()  ) os << " ";
 dump(os,rb.deref(np).r,rb);
 if (!leaf) os << ")";
}

std::ostream& operator << (std::ostream& os, rb_tree const & rb){
 dump(os,rb.root(), rb );
 return os;
}

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
  /*
  A:          left rot.   B:
      2          ==>         4
    /  \      right rot.    / \
   1    4        <==       2   5
       / \                / \
      3   5              1   3 
  */
  alg::rb_tree rb_a({
        {.p = 0,.l=alg::rb_tree::node_size()*2, .r = alg::rb_tree::node_size()*3, .val=2, .c = 0},
        {.p = alg::rb_tree::node_size(),.l=0, .r = 0, .val=1, .c = 0},
        {.p = alg::rb_tree::node_size(),.l=alg::rb_tree::node_size() * 4, .r = alg::rb_tree::node_size()*5, .val=4, .c = 0},
        {.p = alg::rb_tree::node_size()*3,.l= 0, .r = 0, .val=3, .c = 0},
        {.p = alg::rb_tree::node_size()*3,.l= 0, .r = 0, .val=5, .c = 0}
    });
  std::cout << rb_a << '\n';
  rb_a.left_rotate(alg::rb_tree::node_size());
  std::cout << rb_a << '\n';
  rb_a.right_rotate(alg::rb_tree::node_size()*3);
  std::cout << rb_a << '\n';
}

int main(){
    //test_color_ops();
    test_left_rotate();

    return 0;
}