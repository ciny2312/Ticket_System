#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <memory>

namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector 
{
	private:
	size_t cnt,siz;
	T* v;
	std::allocator<T> allo;
	public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		size_t id;
		T* p;
		iterator (const size_t _id,T* _p):id(_id),p(_p){}
		iterator operator+(const int &n) const 
		{
			return iterator(id+n,p+n);
			//TODO
		}
		iterator operator-(const int &n) const 
		{
			return iterator(id-n,p-n);
			//TODO
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			int k=id-rhs.id;
			if(rhs.p+k!=p) throw invalid_iterator();
			return abs(k);
			//TODO
		}
		iterator& operator+=(const int &n) 
		{
			id+=n;p+=n;
			return *this;
			//TODO
		}
		iterator& operator-=(const int &n) 
		{
			id-=n;p-=n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			return iterator(id++,p++);
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			id++;p++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			return iterator(id--,p--);
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			id--;p--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *p;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return p==rhs.p;
		}
		bool operator==(const const_iterator &rhs) const {
			return p==rhs.p;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(p==rhs.p);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(p==rhs.p);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	private:
		/*TODO*/
	public:
		size_t id;
		T* p;
		const_iterator (const size_t _id,T* _p):id(_id),p(_p){}
		const_iterator operator+(const int &n) const 
		{
			return const_iterator(id+n,p+n);
			//TODO
		}
		const_iterator operator-(const int &n) const 
		{
			return const_iterator(id-n,p-n);
			//TODO
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const 
		{
			int k=id-rhs.id;
			if(rhs.p+k!=p) throw invalid_iterator();
			return abs(k);
			//TODO
		}
		const_iterator& operator+=(const int &n) 
		{
			id+=n;p+=n;
			return *this;
			//TODO
		}
		const_iterator& operator-=(const int &n) 
		{
			id-=n;p-=n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			return const_iterator(id++,p++);
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			id++;p++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			return const_iterator(id--,p--);
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
			id--;p--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const{
			return *p;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return p==rhs.p;
		}
		bool operator==(const const_iterator &rhs) const {
			return p==rhs.p;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(p==rhs.p);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(p==rhs.p);
		}

	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() {
		siz=10;cnt=0;
		v=allo.allocate(siz);
	}
	vector(const vector &other) {
		cnt=other.cnt;
		siz=other.siz;
		v=allo.allocate(siz);
		for(int i=0;i<cnt;i++){
			std::allocator_traits<decltype(allo)>::construct(allo,v+i,other.v[i]);
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for(int i=0;i<cnt;i++){
			std::allocator_traits<decltype(allo)>::destroy(allo,v+i);
		}
		allo.deallocate(v,siz);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if(v==other.v) return *this;
		for(int i=0;i<cnt;i++){
			std::allocator_traits<decltype(allo)>::destroy(allo,v+i);
		}
		allo.deallocate(v,siz);
		cnt=other.cnt;
		siz=other.siz;
		v=allo.allocate(siz);
		for(int i=0;i<cnt;i++){
			std::allocator_traits<decltype(allo)>::construct(allo,v+i,other.v[i]);
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if(pos<0||pos>=cnt) throw index_out_of_bound();
		return v[pos];
	}
	const T & at(const size_t &pos) const {
		if(pos<0||pos>=cnt) throw index_out_of_bound();
		return v[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if(pos<0||pos>=cnt) throw index_out_of_bound();
		return v[pos];
	}
	const T & operator[](const size_t &pos) const {
		if(pos<0||pos>=cnt) throw index_out_of_bound();
		return v[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if(cnt==0) throw container_is_empty();
		return v[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if(cnt==0) throw container_is_empty();
		return v[cnt-1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(0,&v[0]);
	}
	const_iterator cbegin() const {
		return const_iterator(0,&v[0]);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(cnt,&v[cnt]);
	}
	const_iterator cend() const {
		return const_iterator(cnt,&v[cnt]);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return cnt==0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return cnt;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for(int i=0;i<cnt;i++){
			std::allocator_traits<decltype(allo)>::destroy(allo,v+i);
		}
		allo.deallocate(v,siz);
		siz=10;cnt=0;
		v=allo.allocate(siz);
	}
	void double_space(){
		T *nw=allo.allocate(siz*2);
		for(int i=0;i<cnt;i++){
			std::allocator_traits<decltype(allo)>::construct(allo,nw+i,v[i]);
			std::allocator_traits<decltype(allo)>::destroy(allo,v+i);
		}
		allo.deallocate(v,siz);
		siz*=2;v=nw;
	}
	void d_double_space(){
		T *nw=allo.allocate(siz/2);
		for(int i=0;i<cnt;i++){
			std::allocator_traits<decltype(allo)>::construct(allo,nw+i,v[i]);
			std::allocator_traits<decltype(allo)>::destroy(allo,v+i);
		}
		allo.deallocate(v,siz);
		siz/=2;v=nw;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		if(cnt>=siz-5){
			double_space();
		}
		cnt++;
		for(int i=cnt-1;i>pos.id;i--){
			if(i==cnt-1){
				std::allocator_traits<decltype(allo)>::construct(allo,v+i,v[i-1]);
			}
			else v[i]=v[i-1];
		}
		if(pos.id==cnt-1){
			std::allocator_traits<decltype(allo)>::construct(allo,v+pos.id,value);
		}
		else v[pos.id]=value;
		return iterator(pos.id,&v[pos.id]);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t ind, const T &value) {
		if(ind>cnt) throw index_out_of_bound();
		if(cnt>=siz-5){
			double_space();
		}
		cnt++;
		for(int i=cnt-1;i>ind;i++){
			if(i==cnt-1){
				std::allocator_traits<decltype(allo)>::construct(allo,v+i,v[i-1]);
			}
			else v[i]=v[i-1];
		}
		if(ind==cnt-1){
			std::allocator_traits<decltype(allo)>::construct(allo,v+ind,value);
		}
		else v[ind]=value;
		return iterator(ind,&v[ind]);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		cnt--;
		for(int i=pos.id;i<cnt;i++){
			v[i]=v[i+1];
		}
		std::allocator_traits<std::allocator<T> >::destroy(allo,v+cnt);
		if(cnt<siz/2-5){
			d_double_space();
		}
		return iterator(pos.id,&v[pos.id]);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t ind) {
		if(ind>=cnt) throw index_out_of_bound();
		cnt--;
		for(int i=ind;i<cnt;i++){
			v[i]=v[i+1];
		}
		std::allocator_traits<decltype(allo)>::destroy(allo,v+cnt);
		if(cnt<siz/2-5){
			d_double_space();
		}
		return iterator(ind,&v[ind]);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		insert(cnt,value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		erase(cnt-1);
	}
};


}

#endif
