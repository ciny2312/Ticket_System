#include "vector.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include <ostream>
#include <string>

struct String {
  char s[70];
  String()=default;
  String(std::string ch){
    strcpy(s, ch.c_str());
  }
  String(char ch[]){
    strcpy(s,ch);
  }
  bool operator==(const String &y) const { return (!strcmp(s, y.s)); }
  bool operator!=(const String &y) const { return !(*this == y); }
  bool operator<(const String &y) const { return (strcmp(s, y.s) < 0); }

  bool operator>(const String &y) const { return (y < *this); }
  bool operator<=(const String &y) const { return !(*this > y); }

  bool operator>=(const String &y) const { return !(*this < y); }

  String & operator = (const String &y){
    strcpy(s,y.s);
    return *this;
  }
  friend std::ostream & operator << (std::ostream &o,const String& y);
};

inline std::ostream & operator << (std::ostream &o,const String& y){
  o<<y.s;
  return o;
}
template <class T1, class T2> struct value {
  T1 key;
  T2 val;

  value() = default;
  value(const T1 &_key, const T2 &_val) : key(_key), val(_val) {}
  value(const value &a) : key(a.key), val(a.val) {}

  bool operator==(const value &b) const { return key == b.key && val == b.val; }
  bool operator!=(const value &b) const { return !(*this == b); }
  bool operator<=(const value &b) const {
    if (key < b.key)
      return true;
    if (key > b.key)
      return false;
    return val <= b.val;
  }
  bool operator>=(const value &b) const {
    if (key > b.key)
      return true;
    if (key < b.key)
      return false;
    return val >= b.val;
  }
  bool operator<(const value &b) const { return !(*this >= b); }
  bool operator>(const value &b) const { return !(*this <= b); }
};
template <class T1, class T2> class BPtree {
  static const int M = 200;
  struct node {
    int son[M + 5];
    value<T1, T2> val[M + 5];
    bool is_leaf = false;
    int siz = 0, nxt = -1;
  };
  std::string filename;
  std::fstream f;
  int root, tot;
  const int ex, siz_node;
  void read_in_txt(int pos, node &x) {
    f.seekg(ex + siz_node * pos);
    f.read(reinterpret_cast<char *>(&x), siz_node);
  }
  void write_in_txt(int pos, node &x) {
    f.seekp(ex + siz_node * pos);
    f.write(reinterpret_cast<char *>(&x), siz_node);
  }
  value<T1, T2> las;
  bool internal_insert(node &p, int pos, value<T1, T2> v) {
    if (p.is_leaf) {
      int l = 0, r = p.siz;
      while (l < r) {
        int mid = (l + r) >> 1;
        if (v <= p.val[mid])
          r = mid;
        else
          l = mid + 1;
      }
      if (l != p.siz && p.val[l] == v)
        return false;
      for (int i = p.siz - 1; i >= l; i--) {
        p.val[i + 1] = p.val[i];
      }
      p.val[l] = v;
      p.siz++;
      if (p.siz > M) {
        node nw;
        nw.is_leaf = true;
        p.siz = M / 2;
        nw.siz = M / 2 + 1;
        for (int i = 0; i <= M / 2; i++) {
          nw.val[i] = p.val[i + M / 2];
        }
        nw.nxt = p.nxt;
        p.nxt = ++tot;
        if (pos == root) {
          node rt;
          rt.siz = 1;
          rt.son[0] = root;
          rt.son[1] = tot;
          rt.val[0] = p.val[M / 2];
          write_in_txt(pos, p);
          write_in_txt(tot, nw);
          root = ++tot;
          write_in_txt(root, rt);
          return false;
        }
        write_in_txt(pos, p);
        write_in_txt(tot, nw);
        las = nw.val[0];
        return true;
      }
      write_in_txt(pos, p);
      return false;
    }
    int l = 0, r = p.siz;
    while (l < r) {
      int mid = (l + r) >> 1;
      if (v <= p.val[mid])
        r = mid;
      else
        l = mid + 1;
    }
    if (l < p.siz && p.val[l] == v)
      l++;
    node nw;
    read_in_txt(p.son[l], nw);
    bool flag = internal_insert(nw, p.son[l], v);
    if (!flag)
      return false;
    for (int i = p.siz - 1; i >= l; i--) {
      p.val[i + 1] = p.val[i];
      p.son[i + 2] = p.son[i + 1];
    }
    p.val[l] = las;
    p.son[l + 1] = tot;
    p.siz++;
    if (p.siz > M) {
      node nw;
      ++tot;
      p.siz = M / 2;
      nw.siz = M / 2;
      for (int i = 0; i < M / 2; i++) {
        nw.val[i] = p.val[i + M / 2 + 1];
        nw.son[i] = p.son[i + M / 2 + 1];
      }
      nw.son[M / 2] = p.son[M + 1];
      if (root == pos) {
        node rt;
        rt.siz = 1;
        rt.son[0] = pos;
        rt.son[1] = tot;
        rt.val[0] = p.val[M / 2];
        write_in_txt(pos, p);
        write_in_txt(tot, nw);
        root = ++tot;
        write_in_txt(root, rt);
        return false;
      }
      write_in_txt(pos, p);
      write_in_txt(tot, nw);
      las = p.val[M / 2];
      return true;
    }
    write_in_txt(pos, p);
    return false;
  }
  bool internal_delete(node &p, int pos, value<T1, T2> v) {
    if (p.is_leaf) {
      int l = 0, r = p.siz;
      while (l < r) {
        int mid = (l + r) >> 1;
        if (v <= p.val[mid])
          r = mid;
        else
          l = mid + 1;
      }
      if (l < 0 || l == p.siz || p.val[l] != v)
        return false;
      for (int i = l; i < p.siz - 1; i++) {
        p.val[i] = p.val[i + 1];
      }
      p.siz--;
      write_in_txt(pos, p);
      if (p.siz < M / 2) {
        return true;
      }
      return false;
    }
    int l = 0, r = p.siz;
    while (l < r) {
      int mid = (l + r) >> 1;
      if (v <= p.val[mid])
        r = mid;
      else
        l = mid + 1;
    }
    if (l < p.siz && v == p.val[l])
      l++;
    node nw;
    read_in_txt(p.son[l], nw);
    bool flag = internal_delete(nw, p.son[l], v);
    if (!flag)
      return false;
    if (pos == root && p.siz == 1) {
      node ls, rs;
      read_in_txt(p.son[0], ls);
      read_in_txt(p.son[1], rs);
      if (ls.siz + rs.siz < M) {
        if (ls.is_leaf) {
          for (int i = 0; i < rs.siz; i++) {
            ls.val[i + ls.siz] = rs.val[i];
          }
          ls.siz += rs.siz;
          ls.nxt = rs.nxt;
          root = p.son[0];
          write_in_txt(root, ls);
          return false;
        }
        for (int i = 0; i < rs.siz; i++) {
          ls.val[i + ls.siz + 1] = rs.val[i];
          ls.son[i + ls.siz + 1] = rs.son[i];
        }
        ls.val[ls.siz] = p.val[0];
        ls.son[ls.siz + rs.siz + 1] = rs.son[rs.siz];
        ls.siz += rs.siz + 1;
        root = p.son[0];
        write_in_txt(root, ls);
        return false;
      }
    }
    if (l > 0) {
      node pre;
      read_in_txt(p.son[l - 1], pre);
      if (pre.siz > M / 2) {
        if (pre.is_leaf) {
          for (int i = nw.siz; i >= 1; i--) {
            nw.val[i] = nw.val[i - 1];
          }
          nw.val[0] = pre.val[pre.siz - 1];
          p.val[l - 1] = nw.val[0];
          pre.siz--;
          nw.siz++;
          write_in_txt(pos, p);
          write_in_txt(p.son[l], nw);
          write_in_txt(p.son[l - 1], pre);
          return false;
        }
        nw.son[nw.siz + 1] = nw.son[nw.siz];
        for (int i = nw.siz; i >= 1; i--) {
          nw.val[i] = nw.val[i - 1];
          nw.son[i] = nw.son[i - 1];
        }
        nw.val[0] = p.val[l - 1];
        nw.son[0] = pre.son[pre.siz];
        p.val[l - 1] = pre.val[pre.siz - 1];
        pre.siz--;
        nw.siz++;
        write_in_txt(pos, p);
        write_in_txt(p.son[l], nw);
        write_in_txt(p.son[l - 1], pre);
        return false;
      }
      if (pre.is_leaf) {
        for (int i = 0; i < nw.siz; i++) {
          pre.val[i + pre.siz] = nw.val[i];
        }
        pre.siz += nw.siz;
        pre.nxt = nw.nxt;
        for (int i = l - 1; i < p.siz - 1; i++) {
          p.val[i] = p.val[i + 1];
          p.son[i + 1] = p.son[i + 2];
        }
        p.siz--;
        write_in_txt(pos, p);
        write_in_txt(p.son[l - 1], pre);
        if (p.siz < M / 2)
          return true;
        return false;
      }
      for (int i = 0; i < nw.siz; i++) {
        pre.val[i + pre.siz + 1] = nw.val[i];
        pre.son[i + pre.siz + 1] = nw.son[i];
      }
      pre.val[pre.siz] = p.val[l - 1];
      pre.son[pre.siz + nw.siz + 1] = nw.son[nw.siz];
      pre.siz += nw.siz + 1;
      for (int i = l - 1; i < p.siz - 1; i++) {
        p.val[i] = p.val[i + 1];
        p.son[i + 1] = p.son[i + 2];
      }
      p.siz--;
      write_in_txt(pos, p);
      write_in_txt(p.son[l - 1], pre);
      if (p.siz < M / 2)
        return true;
      return false;
    }
    else if (l < p.siz) {
      node pre;
      read_in_txt(p.son[l + 1], pre);
      if (pre.siz > M / 2) {
        if (pre.is_leaf) {
          nw.val[nw.siz] = pre.val[0];
          for (int i = 0; i < pre.siz - 1; i++) {
            pre.val[i] = pre.val[i + 1];
          }
          p.val[l] = pre.val[0];
          pre.siz--;
          nw.siz++;
          write_in_txt(pos, p);
          write_in_txt(p.son[l], nw);
          write_in_txt(p.son[l + 1], pre);
          return false;
        }
        nw.val[nw.siz] = p.val[l];
        p.val[l] = pre.val[0];
        nw.son[nw.siz + 1] = pre.son[0];
        for (int i = 0; i < pre.siz - 1; i++) {
          pre.val[i] = pre.val[i + 1];
          pre.son[i] = pre.son[i + 1];
        }
        pre.son[pre.siz - 1] = pre.son[pre.siz];
        pre.siz--;
        nw.siz++;
        write_in_txt(pos, p);
        write_in_txt(p.son[l], nw);
        write_in_txt(p.son[l + 1], pre);
        return false;
      }
      if (nw.is_leaf) {
        for (int i = 0; i < pre.siz; i++) {
          nw.val[i + nw.siz] = pre.val[i];
        }
        nw.siz += pre.siz;
        nw.nxt = pre.nxt;
        for (int i = l; i < p.siz - 1; i++) {
          p.val[i] = p.val[i + 1];
          p.son[i + 1] = p.son[i + 2];
        }
        p.siz--;
        write_in_txt(pos, p);
        write_in_txt(p.son[l], nw);
        if (p.siz < M / 2)
          return true;
        return false;
      }
      for (int i = 0; i < pre.siz; i++) {
        nw.val[i + nw.siz + 1] = pre.val[i];
        nw.son[i + nw.siz + 1] = pre.son[i];
      }
      nw.val[nw.siz] = p.val[l];
      nw.son[nw.siz + pre.siz + 1] = pre.son[pre.siz];
      nw.siz += pre.siz + 1;
      for (int i = l; i < p.siz - 1; i++) {
        p.val[i] = p.val[i + 1];
        p.son[i + 1] = p.son[i + 2];
      }
      p.siz--;
      write_in_txt(pos, p);
      write_in_txt(p.son[l], nw);
      if (p.siz < M / 2)
        return true;
      return false;
    }
    return false;
  }

public:
  BPtree() : ex(sizeof(int) * 2), siz_node(sizeof(node)) { filename = ""; }
  BPtree(std::string str) : ex(sizeof(int) * 2), siz_node(sizeof(node)) {
    filename = str;
    f.open(filename);
    if (!f.is_open()) {
      f.open(filename, std::fstream::out);
      f.close();
      f.open(filename);
      f.seekp(0);
      tot = -1;
      root = -1;
      f.write(reinterpret_cast<char *>(&tot), sizeof(tot));
      f.write(reinterpret_cast<char *>(&root), sizeof(root));
    } else {
      f.seekg(0);
      f.read(reinterpret_cast<char *>(&tot), sizeof(tot));
      f.read(reinterpret_cast<char *>(&root), sizeof(root));
    }
  }
  ~BPtree() {
    f.seekp(0);
    f.write(reinterpret_cast<char *>(&tot), sizeof(tot));
    f.write(reinterpret_cast<char *>(&root), sizeof(root));
    f.close();
  }
  void clear(){
    tot=root=-1;
  }
  void insert(T1 key, T2 val) {
    value<T1, T2> x(key, val);
    if (root == -1) {
      node cur;
      cur.siz = 1;
      cur.is_leaf = true;
      cur.val[0] = x;
      root = ++tot;
      write_in_txt(root, cur);
      return;
    }
    node rt;
    read_in_txt(root, rt);
    internal_insert(rt, root, x);
  }
  void del(T1 key, T2 val) {
    if (root == -1) {
      return;
    }
    value<T1, T2> x(key, val);
    node rt;
    read_in_txt(root, rt);
    internal_delete(rt, root, x);
  }
  void find(T1 key,sjtu::vector<T2> &ans) {
    if (root == -1) {
      return ;
    }
    node p;
    read_in_txt(root, p);
    while (!p.is_leaf) {
      int l = 0, r = p.siz;
      while (l < r) {
        int mid = (l + r) >> 1;
        if (key <= p.val[mid].key)
          r = mid;
        else
          l = mid + 1;
      }
      read_in_txt(p.son[l], p);
    }
    int l = 0, r = p.siz;
    while (l < r) {
      int mid = (l + r) >> 1;
      if (key <= p.val[mid].key)
        r = mid;
      else
        l = mid + 1;
    }
    if (l > 0)
      l--;
    if (l < p.siz && key < p.val[l].key) {
      return ;
    }
    bool flag = false;
    while (true) {
      for (int i = l; i < p.siz; i++) {
        if (p.val[i].key == key) {
          ans.push_back(p.val[i].val);
        }
        if (p.val[i].key > key) {
          flag = true;
          break;
        }
      }
      if (flag) {
        break;
      }
      if (p.nxt == -1)
        break;
      read_in_txt(p.nxt, p);
      l = 0;
    }
    return ;
  }
};