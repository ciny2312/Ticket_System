/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <pthread.h>
#include "utility.hpp"
#include "exceptions.hpp"
#include<iostream>

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	public:
	typedef pair<const Key, T> value_type;
	private:
	struct node{
		value_type v;
		node *fa,*ls,*rs;
		bool red;
		node(const value_type &_v,bool _red=true,node* _fa=nullptr,node* _ls=nullptr,node* _rs=nullptr):v(_v){
			red=_red;fa=_fa;ls=_ls;rs=_rs;
		}
		node(node &rhs):v(rhs.v){
			red=rhs.red;
			fa=rhs.fa;
			ls=rhs.ls;
			rs=rhs.rs;
		}
	};
	Compare comp;
	node *root;
	size_t siz;
	public:
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		node *it;node** id;
		bool ed;
		iterator() {
			// TODO
			it=nullptr;id=nullptr;
		}
		iterator(const iterator &other) {
			// TODO
			it=other.it;
			id=other.id;
			ed=other.ed;
		}
		iterator(node* _it,node** _id,bool _ed=false){
			it=_it;id=_id;ed=_ed;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int){
			if(it==nullptr){
				throw invalid_iterator();
			}
			if(ed){
				throw invalid_iterator();
			}
			iterator st=*this;
			if(it->rs){
				it=it->rs;
				while(it->ls){
					it=it->ls;
				}
				return st;
			}
			while(it->fa&&it->fa->rs==it){
				it=it->fa;
			}
			if(it->fa==nullptr){
				it=st.it;ed=true;
				return st;
			}
			it=it->fa;
			return st;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if(it==nullptr){
				throw invalid_iterator();
			}
			if(ed){
				throw invalid_iterator();
			}
			if(it->rs){
				it=it->rs;
				while(it->ls){
					it=it->ls;
				}
				return *this;
			}
			node *st=it;
			while(it->fa&&it->fa->rs==it){
				it=it->fa;
			}
			if(it->fa==nullptr){
				it=st;ed=true;
				return *this;
			}
			it=it->fa;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator st=*this;
			if(ed){
				it=*id;
				if(it==nullptr) throw invalid_iterator();
				while(it->rs){
					it=it->rs;
				}
				ed=false;
				return st;
			}
			if(it==nullptr){
				throw invalid_iterator();
			}
			if(it->ls){
				it=it->ls;
				while(it->rs){
					it=it->rs;
				}
				return st;
			}
			while(it->fa&&it->fa->ls==it){
				it=it->fa;
			}
			if(it->fa==nullptr){
				it=st.it;
				throw invalid_iterator();
			}
			it=it->fa;
			return st;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if(ed){
				it=*id;
				if(it==nullptr) throw invalid_iterator();
				while(it->rs){
					it=it->rs;
				}
				ed=false;
				return *this;
			}
			if(it==nullptr){
				throw invalid_iterator();
			}
			if(it->ls){
				it=it->ls;
				while(it->rs){
					it=it->rs;
				}
				return *this;
			}
			node* st=it;
			while(it->fa&&it->fa->ls==it){
				it=it->fa;
			}
			if(it->fa==nullptr){
				it=st;
				throw invalid_iterator();
			}
			it=it->fa;
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			return it->v;
		}
		bool operator==(const iterator &rhs) const {
			if(ed&&rhs.ed){
				return id==rhs.id;
			}
			return it==rhs.it&&ed==rhs.ed;
		}
		bool operator==(const const_iterator &rhs) const {
			if(ed&&rhs.ed){
				return id==rhs.id;
			}
			return it==rhs.it&&ed==rhs.ed;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this==rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this==rhs);
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type * operator->() const noexcept {
			return &(it->v);
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
		public:
			node *it;
			node *const*id;
			bool ed;
			const_iterator() {
				// TODO
				it=nullptr;
				id=nullptr;
			}
			const_iterator(const const_iterator &other) {
				// TODO
				it=other.it;
				id=other.id;
				ed=other.ed;
			}
			const_iterator(const iterator &other) {
				// TODO
				it=other.it;
				id=other.id;
				ed=other.ed;
			}
			const_iterator(node* _it,node*const* _id,bool _ed=false){
				it=_it;ed=_ed;id=_id;
			}
			// And other methods in iterator.
			// And other methods in iterator.
				// And other methods in iterator.
			/**
			* TODO iter++
			*/
			const_iterator operator++(int){
				if(it==nullptr){
					throw invalid_iterator();
				}
				if(ed){
					throw invalid_iterator();
				}
				const_iterator st=*this;
				if(it->rs){
					it=it->rs;
					while(it->ls){
						it=it->ls;
					}
					return st;
				}
				while(it->fa&&it->fa->rs==it){
					it=it->fa;
				}
				if(it->fa==nullptr){
					it=st.it;ed=true;
					return st;
				}
				it=it->fa;
				return st;
			}
			/**
			* TODO ++iter
			*/
			const_iterator & operator++() {
				if(it==nullptr){
					throw invalid_iterator();
				}
				if(ed){
					throw invalid_iterator();
				}
				if(it->rs){
					it=it->rs;
					while(it->ls){
						it=it->ls;
					}
					return *this;
				}
				node *st=it;
				while(it->fa&&it->fa->rs==it){
					it=it->fa;
				}
				if(it->fa==nullptr){
					it=st;ed=true;
					return *this;
				}
				it=it->fa;
				return *this;
			}
			/**
			* TODO iter--
			*/
			const_iterator operator--(int) {
				const_iterator st=*this;
				if(ed){
					it=*id;
					if(it==nullptr) throw invalid_iterator();
					while(it->rs){
						it=it->rs;
					}
					ed=false;
					return st;
				}
				if(it==nullptr){
					throw invalid_iterator();
				}
				if(it->ls){
					it=it->ls;
					while(it->rs){
						it=it->rs;
					}
					return st;
				}
				while(it->fa&&it->fa->ls==it){
					it=it->fa;
				}
				if(it->fa==nullptr){
					it=st.it;
					throw invalid_iterator();
				}
				it=it->fa;
				return st;
			}
			/**
			* TODO --iter
			*/
			const_iterator & operator--() {
				if(ed){
					it=*id;
					if(it==nullptr) throw invalid_iterator();
					while(it->rs){
						it=it->rs;
					}
					ed=false;
					return *this;
				}
				if(it==nullptr){
					throw invalid_iterator();
				}
				if(it->ls){
					it=it->ls;
					while(it->rs){
						it=it->rs;
					}
					return *this;
				}
				node *st=it;
				while(it->fa&&it->fa->ls==it){
					it=it->fa;
				}
				if(it->fa==nullptr){
					it=st;
					throw invalid_iterator();
				}
				it=it->fa;
				return *this;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			const value_type & operator*() const {
				return it->v;
			}
			bool operator==(const iterator &rhs) const {
				if(ed&&rhs.ed){	
					return id==rhs.id;
				}
				return it==rhs.it&&ed==rhs.ed;
			}
			bool operator==(const const_iterator &rhs) const {
				if(ed&&rhs.ed){
					return id==rhs.id;
				}
				return it==rhs.it&&ed==rhs.ed;
			}
			/**
			* some other operator for iterator.
			*/
			bool operator!=(const iterator &rhs) const {
				return !(*this==rhs);
			}
			bool operator!=(const const_iterator &rhs) const {
				return !(*this==rhs);
			}
			const value_type * operator->() const noexcept {
				return &(it->v);
			}
	};
	/**
	 * TODO two constructors
	 */
	private:
	void dfs(node *&p,node *q,node *f){
		if(q==nullptr){
			p=nullptr;
			return ;
		}
		p=new node(*q);
		p->fa=f;
		dfs(p->ls,q->ls,p);
		dfs(p->rs,q->rs,p);
	}
	void dfs_clear(node *&p){
		if(p==nullptr) return ;
		dfs_clear(p->ls);
		dfs_clear(p->rs);
		delete p;p=nullptr;
	}
	public:
	map() {
		root=nullptr;siz=0;
	}
	map(const map &other) {
		siz=other.siz;
		dfs(root,other.root,nullptr);
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
		if(root==other.root) return *this;
		siz=other.siz;
		dfs_clear(root);
		dfs(root,other.root,nullptr);
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() {
		dfs_clear(root);
		siz=0;
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		node *nw=root;
		while(nw!=nullptr){
			if(comp(nw->v.first,key)){
				nw=nw->rs;
			}
			else if(comp(key,nw->v.first)){
				nw=nw->ls;
			}
			else{
				break;
			}
		}
		if(nw==nullptr){
			throw index_out_of_bound();
		}
		return nw->v.second;
	}
	const T & at(const Key &key) const {
		node *nw=root;
		while(nw!=nullptr){
			if(comp(nw->v.first,key)){
				nw=nw->rs;
			}
			else if(comp(key,nw->v.first)){
				nw=nw->ls;
			}
			else{
				break;
			}
		}
		if(nw==nullptr){
			throw index_out_of_bound();
		}
		return nw->v.second;
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		auto pi=insert(value_type(key,T()));
		return pi.first->second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		node* nw=root;
		while(nw!=nullptr){
			if(comp(nw->v.first,key)){
				nw=nw->rs;
			}
			else if(comp(key,nw->v.first)){
				nw=nw->ls;
			}
			else{
				break;
			}
		}
		if(nw==nullptr){
			throw index_out_of_bound();
		}
		return nw->v.second;
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		node *nw=root;
		if(nw==nullptr){
			return iterator(nw,&root,true);
		}
		while(nw->ls!=nullptr){
			nw=nw->ls;
		}
		return iterator(nw,&root);
	}
	const_iterator cbegin() const {
		node *nw=root;
		if(nw==nullptr){
			return const_iterator(nw,&root,true);
		}
		while(nw->ls!=nullptr){
			nw=nw->ls;
		}
		return const_iterator(nw,&root);
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
		return iterator(root,&root,true);
	}
	const_iterator cend() const {
		return const_iterator(root,&root,true);
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
		return siz==0;
	}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
		return siz;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		dfs_clear(root);
		siz=0;
	}
	private:
	void LL(node *gp){
		node *p=gp->ls;
		bool r=gp->red;

		gp->red=p->red;
		p->red=r;
		if(gp->fa){
			p->fa=gp->fa;
			if(gp->fa->ls==gp){
				gp->fa->ls=p;
			}
			else{
				gp->fa->rs=p;
			}
		}
		else{
			root=p;
			p->fa=nullptr;
		}

		gp->ls=p->rs;
		if(gp->ls) gp->ls->fa=gp;

		p->rs=gp;
		gp->fa=p;
	}
	void RR(node *gp){
		node *p=gp->rs;
		bool r=gp->red;

		gp->red=p->red;
		p->red=r;
		if(gp->fa){
			p->fa=gp->fa;
			if(gp->fa->ls==gp){
				gp->fa->ls=p;
			}
			else{
				gp->fa->rs=p;
			}
		}
		else{
			root=p;
			p->fa=nullptr;
		}
		gp->rs=p->ls;
		if(gp->rs) gp->rs->fa=gp;

		p->ls=gp;
		gp->fa=p;
	}
	void LR(node *gp){
		node *p=gp->ls,*t=p->rs;
		bool r=gp->red;
		
		gp->red=t->red;
		t->red=r;
		if(gp->fa){
			t->fa=gp->fa;
			if(gp->fa->ls==gp){
				gp->fa->ls=t;
			}
			else{
				gp->fa->rs=t;
			}
		}
		else{
			root=t;
			t->fa=nullptr;
		}

		gp->ls=t->rs;
		if(gp->ls) gp->ls->fa=gp;

		p->rs=t->ls;
		if(p->rs) p->rs->fa=p;

		t->rs=gp;
		gp->fa=t;

		t->ls=p;
		p->fa=t;
	}
	void RL(node *gp){
		node *p=gp->rs,*t=p->ls;
		bool r=gp->red;
		
		gp->red=t->red;
		t->red=r;
		if(gp->fa){
			t->fa=gp->fa;
			if(gp->fa->ls==gp){
				gp->fa->ls=t;
			}
			else{
				gp->fa->rs=t;
			}
		}
		else{
			root=t;
			t->fa=nullptr;
		}

		gp->rs=t->ls;
		if(gp->rs) gp->rs->fa=gp;

		p->ls=t->rs;
		if(p->ls) p->ls->fa=p;

		t->ls=gp;
		gp->fa=t;

		t->rs=p;
		p->fa=t;
	}
	void updata_insert(node *p){
		if(p->fa==nullptr){
			root->red=false;
			return ;
		}
		node *f=p->fa;
		if(f->red==false){
			return ;
		}
		if(f==root){
			root->red=false;
			return ;
		}
		if(f->fa->ls==f){
			if(f->ls==p) LL(f->fa);
			else LR(f->fa);
		}
		else{
			if(f->rs==p) RR(f->fa);
			else RL(f->fa);

		}
	}
	void updata_erase(node *&p,Key val){
		if(p->red) return ;
		if(p==root){
			if(p->ls&&p->rs&&p->ls->red==p->rs->red){
				p->red=true;
				p->ls->red=p->rs->red=false;
				return ;
			}
			if(p->ls==nullptr){
				p->red=true;
				if(p->rs) p->rs->red=false;
				return ;
			}
			if(p->rs==nullptr){
				p->red=true;
				if(p->ls) p->ls->red=false;
				return ;
			}

		}
		node *t=nullptr;
		if(p->fa){
			if(p->fa->ls==p) t=p->fa->rs;
			else t=p->fa->ls;
		}/////////////
		if((p->ls==nullptr||p->ls->red==false)&&(p->rs==nullptr||p->rs->red==false)){
			if(t==nullptr||(t->ls==nullptr||t->ls->red==false)&&(t->rs==nullptr||t->rs->red==false)){
				p->fa->red=false;
				p->red=true;
				if(t) t->red=true;
				return ;
			}
			if(p->fa->ls==t){
				if(t->ls&&t->ls->red){
					t->ls->red=false;
					LL(p->fa);
				}
				else{
					LR(p->fa);
					p->fa->red=false;
				}
			}
			else if(t->rs&&t->rs->red==true){
				t->rs->red=false;
				RR(p->fa);
			}
			else{
				RL(p->fa);
				p->fa->red=false;
			}
			p->red=true;
		}
		else{
			if(comp(p->v.first,val)){
				p=p->rs;
				if(p->red==false){
					LL(p->fa);
					updata_erase(p,val);
				}
			}
			else if(comp(val,p->v.first)){
				p=p->ls;
				if(p->red==false){
					RR(p->fa);
					updata_erase(p,val);
				}
			}
			else{
				if(p->rs&&p->ls){
					if(p->rs->red==false){
						LL(p);
					}
					return ;
				}
				if(p->ls){
					LL(p);
				}
				else if(p->rs){
					RR(p);
				}
			}
		}
	}
	public:
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		if(root==nullptr){
			root=new node(value,false);
			siz=1;
			return pair<iterator,bool>(iterator(root,&root),true);;
		}
		node *p=root,*f=nullptr;
		while(p!=nullptr){
			if(p->ls&&p->rs&&p->ls->red&&p->rs->red){
				p->ls->red=p->rs->red=false;
				p->red=true;
				updata_insert(p);
			}
			if(comp(p->v.first,value.first)){
				f=p;
				p=p->rs;
			}
			else if(comp(value.first,p->v.first)){
				f=p;
				p=p->ls;
			}
			else{
				break;
			}
		}
		if(p==nullptr){
			p=new node(value);
			siz++;
			if(f!=nullptr) p->fa=f;
			if(comp(f->v.first,value.first)){
				f->rs=p;
			}
			else{
				f->ls=p;
			}
			updata_insert(p);
			root->red=false;
			return pair<iterator,bool>(iterator(p,&root),true);
		}
		return pair<iterator,bool>(iterator(p,&root),false);
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		if(pos.it==nullptr){
			throw invalid_iterator();
		}
		if(pos.id!=(&root)){
			throw invalid_iterator();
		}
		node* p=root;Key key=pos->first;
		if(pos.ed){
			throw invalid_iterator();
		}
		while(p!=nullptr){
			updata_erase(p,key);
			if(comp(p->v.first,key)){
				p=p->rs;
			}
			else if(comp(key,p->v.first)){
				p=p->ls;
			}
			else{
			/*	if(p->ls&&p->rs){
					node *nw=p->rs;
					while(nw->ls){
						nw=nw->ls;
					}
					node np=*p;
					delete p;
					p=new node(nw->v,np.red,np.fa,np.ls,np.rs);
					key=nw->v.first;
					p=p->rs;
					continue;
				}
			*/	
				if(p->ls){
					node *nw=p->ls;
					node np=*p;
					while(nw->rs){
						nw=nw->rs;
					}
					p->red=nw->red;
					nw->red=np.red;
					if(p->fa){
						if(p->fa->ls==p){
							p->fa->ls=nw;
						}
						else{
							p->fa->rs=nw;
						}
					}
					else{
						root=nw;
					}
					if(p->ls==nw){
						p->ls=nw->ls;
						if(p->ls) p->ls->fa=p;
						p->rs=nw->rs;
						if(p->rs) p->rs->fa=p;
						nw->ls=p;
						p->fa=nw;
						nw->rs=np.rs;
						if(nw->rs) nw->rs->fa=nw;
						nw->fa=np.fa;
						continue;
					}
					if(nw->fa){
						if(nw->fa->ls==nw){
							nw->fa->ls=p;
						}
						else{
							nw->fa->rs=p;
						}
					}
					p->ls=nw->ls;
					if(p->ls) p->ls->fa=p;
					p->rs=nw->rs;
					if(p->rs) p->rs->fa=p;
					p->fa=nw->fa;
					nw->ls=np.ls;
					if(nw->ls) nw->ls->fa=nw;
					nw->rs=np.rs;
					if(nw->rs) nw->rs->fa=nw;
					nw->fa=np.fa;

					p=nw->ls;
					continue;
				}
				if(p->rs){
					node *nw=p->rs;
					node np=*p;
					while(nw->ls){
						nw=nw->ls;
					}
					p->red=nw->red;
					nw->red=np.red;
					if(p->fa){
						if(p->fa->ls==p){
							p->fa->ls=nw;
						}
						else{
							p->fa->rs=nw;
						}
					}
					else{
						root=nw;
					}
					if(p->rs==nw){
						p->ls=nw->ls;
						if(p->ls) p->ls->fa=p;
						p->rs=nw->rs;
						if(p->rs) p->rs->fa=p;
						nw->rs=p;
						p->fa=nw;
						nw->ls=np.ls;
						if(nw->ls) nw->ls->fa=nw;
						nw->fa=np.fa;
						continue;
					}
					if(nw->fa){
						if(nw->fa->ls==nw){
							nw->fa->ls=p;
						}
						else{
							nw->fa->rs=p;
						}
					}
					p->ls=nw->ls;
					if(p->ls) p->ls->fa=p;
					p->rs=nw->rs;
					if(p->rs) p->rs->fa=p;
					p->fa=nw->fa;
					nw->ls=np.ls;
					if(nw->ls) nw->ls->fa=nw;
					nw->rs=np.rs;
					if(nw->rs) nw->rs->fa=nw;
					nw->fa=np.fa;

					p=nw->rs;
					continue;
				}
				if(p==root){
					delete root;
					root=nullptr;
					siz=0;
					return ;
				}
				if(p->fa->ls==p){
					p->fa->ls=nullptr;
				}
				else{
					p->fa->rs=nullptr;
				}
				delete p;
				siz--;
				root->red=false;
				return ;
			}
		}
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
		node *p=root;
		while(p!=nullptr){
			if(comp(p->v.first,key)){
				p=p->rs;
			}
			else if(comp(key,p->v.first)){
				p=p->ls;
			}
			else {
				return 1;
			}
		}
		return 0;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
		node* nw=root;
		while(nw!=nullptr){
			if(comp(nw->v.first,key)){
				nw=nw->rs;
			}
			else if(comp(key,nw->v.first)){
				nw=nw->ls;
			}
			else{
				break;
			}
		}
		if(nw==nullptr){
			return iterator(root,&root,true);
		}
		return iterator(nw,&root);
	}
	const_iterator find(const Key &key) const {
		node* nw=root;
		while(nw!=nullptr){
			if(comp(nw->v.first,key)){
				nw=nw->rs;
			}
			else if(comp(key,nw->v.first)){
				nw=nw->ls;
			}
			else{
				break;
			}
		}
		if(nw==nullptr){
			return const_iterator(root,&root,true);
		}
		return const_iterator(nw,&root);
	}
	void dfs_p(node *p){
		if(p==nullptr) return ;
		std::cout<<p->v.second<<" ";
		if(p->ls) std::cout<<"L: "<<p->ls->v.second<<' ';
		if(p->rs) std::cout<<"R: "<<p->rs->v.second;
		std::cout<<std::endl;
		dfs_p(p->ls);
		dfs_p(p->rs);
	}
	void print(){
		printf("Here it is.\n");
		dfs_p(root);
	}
};

}

#endif

/*
map{
	src{
		map.hpp
		utility.hpp
		exceptions.hpp
	}
	data{
		one{
			code.cpp
			answer.txt
		}
		two{
			code.cpp
			answer.txt
		}
		class-bint.hpp
		class-integer.hpp
		class-matrix.hpp
	}
}
*/
