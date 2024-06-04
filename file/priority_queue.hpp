#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::greater<T> >
class priority_queue {
	class node{
		public:
		T val;
		node *ls,*rs;
		node(T _val,node* _ls=nullptr,node* _rs=nullptr):val(_val),ls(_ls),rs(_rs){}
	};
	node *root;
	size_t siz;
	Compare comp;
	void clear(node* &p){
		if(p==nullptr) return ;
		clear(p->ls);
		clear(p->rs);
		delete p;
		p=nullptr;
	}
	void dfs(node* &np,node *p){
		if(p==nullptr) return ;
		np=new node(p->val);
		dfs(np->ls,p->ls);
		dfs(np->rs,p->rs);
	}
	node* do_merge(node *p,node *q){
		if(p==nullptr) return q;
		if(q==nullptr) return p;
		if(comp(q->val,p->val)){
			node *st=p->rs;
			p->rs=p->ls;
			p->ls=do_merge(st,q);
			return p;
		}
		else{
			node *st=q->rs;
			q->rs=q->ls;
			q->ls=do_merge(p,st);
			return q;
		}
	}
public:
	/**
	 * TODO constructors
	 */
	priority_queue() {
		siz=0;root=nullptr;
	}
	priority_queue(const priority_queue &other) {
		siz=other.siz;
		dfs(root,other.root);
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue() {
		siz=0;
		clear(root);
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) {
		if(root==other.root) return *this;
		clear(root);
		siz=other.siz;
		dfs(root,other.root);
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
		if(empty()) throw container_is_empty();
		return root->val;
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
		priority_queue<T,Compare> other;
		other.siz=1;
		other.root=new node(e);
		merge(other);
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		if(empty()) throw container_is_empty();
		node *x=root->ls,*y=root->rs;
		delete root;
		siz--;
		root=do_merge(x,y);
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
		return siz;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
		return siz==0;
	}
	/**
	 * merge two priority_queues with at least O(logn) complexity.
	 * clear the other priority_queue.
	 */
	void merge(priority_queue &other) {
		siz+=other.siz;
		root=do_merge(root,other.root);
		other.root=nullptr;
		other.siz=0;
	}
};

}

#endif
