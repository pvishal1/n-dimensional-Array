#include <iostream>
#include <unistd.h>
#include <utility>

namespace cs540 {

using namespace std;

class OutOfRange {};

template<typename T, size_t ... Dims>
class Array;

template<typename T, size_t D1, size_t ... Dims>
class Array<T, D1, Dims...> {
public:
	typedef T ValueType;
	Array<T, Dims...> a[D1];
	const size_t n = D1;
	static const size_t len = sizeof...(Dims)+1;

	class FirstDimensionMajorIterator;
	class LastDimensionMajorIterator;

	Array() {}

	Array(const Array &a1) {
		if((this->n != a1.n) || (this->len != a1.len)) {
			throw OutOfRange();
		}
		for (int var = 0; var < n; ++var) {
			this->a[var] = a1.a[var];
		}
	}

	template <typename U>
	Array(const Array<U, D1, Dims...> &a1) {
		if((this->n != a1.n) || (this->len != a1.len)) {
			throw OutOfRange();
		}
		for (int var = 0; var < n; ++var) {
			this->a[var] = a1.a[var];
		}
	}

	Array &operator=(const Array &a1) {
		if((this->n != a1.n) || (this->len != a1.len)) {
			throw OutOfRange();
		}
		if(this->a != a1.a) {
			for (int var = 0; var < n; ++var) {
				this->a[var] = a1.a[var];
			}
		}
		return *this;
	}

	template <typename U>
	Array &operator=(const Array<U, D1, Dims...> &a1) {
		if((this->n != a1.n) || (this->len != a1.len)) {
			throw OutOfRange();
		}
		for (int var = 0; var < n; ++var) {
			this->a[var] = a1.a[var];
		}
		return *this;
	}

	Array<T, Dims...> &operator[](size_t index) {
		if(index >= n) {
			throw OutOfRange();
		}
		return a[index];
	}

	const Array<T, Dims...> &operator[](size_t index) const {
		if(index >= n) {
			throw OutOfRange();
		}
		return a[index];
	}

	FirstDimensionMajorIterator fmbegin() {
		FirstDimensionMajorIterator f1;
		f1.index = 0;
		f1.n_ele = n;
		f1.ptr = this;
		f1.ele = a[0].fmbegin();
		f1.end = 0;
		return f1;
	}

	FirstDimensionMajorIterator fmend() {
		FirstDimensionMajorIterator f1;
		f1.index = 0;
		f1.n_ele = n;
		f1.ptr = this;
		f1.ele = a[0].fmend();
		f1.end = 1;
		return f1;
	}

	LastDimensionMajorIterator lmbegin() {
		LastDimensionMajorIterator l1;
		l1.end = 0;
		l1.index = 0;
		l1.n_ele = n;
		l1.ptr = this;
		l1.ele = a[0].lmbegin();
		return l1;
	}

	LastDimensionMajorIterator lmend() {
		LastDimensionMajorIterator l1;
		l1.index = 0;
		l1.n_ele = n;
		l1.ptr = this;
		l1.ele = a[0].lmend();
		l1.end = 1;
		return l1;
	}

	class FirstDimensionMajorIterator {
	public:
		Array<T, D1, Dims...> *ptr;
		int end;
		size_t n_ele, index;
		typename Array<T, Dims...>::FirstDimensionMajorIterator ele;

		FirstDimensionMajorIterator(): ptr(nullptr), n_ele(D1), index(0), end(0) {}

		FirstDimensionMajorIterator(const FirstDimensionMajorIterator &f1) {
			this->ptr = f1.ptr;
			this->n_ele = f1.n_ele;
			this->index = f1.index;
			this->end = f1.end;
			this->ele = f1.ele;
		}

		FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &f1) {
			this->ptr = f1.ptr;
			this->n_ele = f1.n_ele;
			this->index = f1.index;
			this->end = f1.end;
			this->ele = f1.ele;
			return *this;
		}

		friend bool operator==(const FirstDimensionMajorIterator &f1,
						const FirstDimensionMajorIterator &f2) {
			if((f1.ptr != f2.ptr) || (f1.n_ele != f2.n_ele) || (f1.index != f2.index) || (f1.end != f2.end)) {
				return false;
			}
			if(f1.end) {
				return true;
			}
			return (f1.ele == f2.ele);
		}

		friend bool operator!=(const FirstDimensionMajorIterator &f1,
						const FirstDimensionMajorIterator &f2) {
			return !(f1 == f2);
		}

		void incr() {
			ele.end = 0;
			ele.ptr = &(ptr->a[index]);
			ele.incr();
		}

		FirstDimensionMajorIterator &operator++() {
			ele++;
			if(ele.end == 1 && ele.index == 0) {
				if(++index == n_ele) {
					index = 0;
					end = 1;
				} else {
					end = 0;
				}
				incr();
			}
			return *this;
		}

		FirstDimensionMajorIterator operator++(int inc) {
			FirstDimensionMajorIterator f1 = *this;
			++(*this);
			return f1;
		}

		T &operator*() const {
			return *ele;
		}
	};

	class LastDimensionMajorIterator {
	public:
		Array<T, D1, Dims...> *ptr;
		int end;
		size_t n_ele, index;
		typename Array<T, Dims...>::LastDimensionMajorIterator ele;

		LastDimensionMajorIterator(): ptr(nullptr), n_ele(D1), index(0), end(0) {}

		LastDimensionMajorIterator(const LastDimensionMajorIterator &l1) {
			this->ptr = l1.ptr;
			this->n_ele = l1.n_ele;
			this->index = l1.index;
			this->end = l1.end;
			this->ele = l1.ele;
		}

		LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &l1) {
			this->ptr = l1.ptr;
			this->n_ele = l1.n_ele;
			this->index = l1.index;
			this->end = l1.end;
			this->ele = l1.ele;
			return *this;
		}

		friend bool operator==(const LastDimensionMajorIterator &l1,
								const LastDimensionMajorIterator &l2) {
			if((l1.ptr != l2.ptr) || (l1.n_ele != l2.n_ele) || (l1.index != l2.index) || (l1.end != l2.end)) {
				return false;
			}
			if(l1.end) {
				return true;
			}
			return (l1.ele == l2.ele);
		}

		friend bool operator!=(const LastDimensionMajorIterator &l1,
						const LastDimensionMajorIterator &l2) {
			return !(l1 == l2);
		}

		void incr() {
			ele.ptr = &(ptr->a[index]);
			ele.incr();
			end = ele.end;
		}

		LastDimensionMajorIterator &operator++() {
			index = (index+1) % n_ele;
			if(index == 0) {
				end = 1;
				ele++;
			}
			incr();
			return *this;
		}

		LastDimensionMajorIterator operator++(int inc) {
			LastDimensionMajorIterator l1 = *this;
			++(*this);
			return l1;
		}

		T &operator*() const {
			return *ele;
		}
	};
};

template<typename T, size_t D1>
class Array<T, D1>{
public:
	T a[D1];
	const size_t n = D1;
	typedef T ValueType;

	class FirstDimensionMajorIterator;
	class LastDimensionMajorIterator;

	Array() {

	}

	Array(const Array &a1) {
		if((this->n != a1.n)) {
			throw OutOfRange();
		}
		for (int var = 0; var < n; ++var) {
			this->a[var] = a1.a[var];
		}
	}

	template <typename U>
	Array(const Array<U, D1> &a1) {
		if((this->n != a1.n)) {
			throw OutOfRange();
		}
		for (int var = 0; var < n; ++var) {
			this->a[var] = a1.a[var];
		}
	}

	Array &operator=(const Array &a1) {
		if((this->n != a1.n)) {
			throw OutOfRange();
		}
		if(this->a != a1.a) {
			for (int var = 0; var < n; ++var) {
				this->a[var] = a1.a[var];
			}
		}
		return *this;
	}

	template <typename U>
	Array &operator=(const Array<U, D1> &a1) {
		if((this->n != a1.n)) {
			throw OutOfRange();
		}
		for (int var = 0; var < n; ++var) {
			this->a[var] = a1.a[var];
		}
		return *this;
	}

	T &operator[](size_t index) {
		if(index >= n) {
			throw OutOfRange();
		}
		return a[index];
	}

	const T &operator[](size_t index) const {
		if(index >= n) {
			throw OutOfRange();
		}
		return a[index];
	}

	FirstDimensionMajorIterator fmbegin() {
		FirstDimensionMajorIterator f1;
		f1.end = 0;
		f1.index = 0;
		f1.n_ele = n;
		f1.ptr = this;
		f1.ele = a;
		return f1;
	}

	FirstDimensionMajorIterator fmend() {
		FirstDimensionMajorIterator f1;
		f1.end = 1;
		f1.index = 0;
		f1.n_ele = n;
		f1.ptr = this;
		f1.ele = a;
		return f1;
	}

	LastDimensionMajorIterator lmbegin() {
		LastDimensionMajorIterator l1;
		l1.end = 0;
		l1.index = 0;
		l1.n_ele = n;
		l1.ptr = this;
		l1.ele = a;
		return l1;
	}

	LastDimensionMajorIterator lmend() {
		LastDimensionMajorIterator l1;
		l1.end = 1;
		l1.index = 0;
		l1.n_ele = n;
		l1.ptr = this;
		l1.ele = a;
		return l1;
	}

	class FirstDimensionMajorIterator {
	public:
		Array<T, D1> *ptr;
		int end;
		size_t n_ele, index;
		T *ele;

		FirstDimensionMajorIterator(): ptr(nullptr), n_ele(D1), index(0), end(0), ele(nullptr) {}

		FirstDimensionMajorIterator(const FirstDimensionMajorIterator &f1) {
			this->ptr = f1.ptr;
			this->n_ele = f1.n_ele;
			this->index = f1.index;
			this->end = f1.end;
			this->ele = f1.ele;
		}

		FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &f1) {
			this->ptr = f1.ptr;
			this->n_ele = f1.n_ele;
			this->index = f1.index;
			this->end = f1.end;
			this->ele = f1.ele;
			return *this;
		}

		friend bool operator==(const FirstDimensionMajorIterator &f1,
		                const FirstDimensionMajorIterator &f2) {
			if((f1.ptr != f2.ptr) || (f1.n_ele != f2.n_ele) || (f1.index != f2.index) || (f1.end != f2.end)) {
				return false;
			}
			if(f1.end) {
				return true;
			}
			return (f1.ele == f2.ele);
		}

		friend bool operator!=(const FirstDimensionMajorIterator &f1,
		                const FirstDimensionMajorIterator &f2) {
			return !(f1 == f2);
		}

		void incr() {
			ele = &(ptr->a[index]);
		}

		FirstDimensionMajorIterator &operator++() {
			if(++index == n_ele) {
				index = 0;
				end = 1;
			} else {
				end = 0;
			}
			incr();
			return *this;
		}

		FirstDimensionMajorIterator operator++(int inc) {
			FirstDimensionMajorIterator f1 = *this;
			++(*this);
			return f1;
		}

		T &operator*() const {
			return *ele;
		}

	};

	class LastDimensionMajorIterator {
	public:
		Array<T, D1> *ptr;
		int end;
		size_t n_ele, index;
		T *ele;

		LastDimensionMajorIterator(): ptr(nullptr), n_ele(D1), index(0), end(0), ele(nullptr) {}

		LastDimensionMajorIterator(const LastDimensionMajorIterator &l1) {
			this->ptr = l1.ptr;
			this->n_ele = l1.n_ele;
			this->index = l1.index;
			this->end = l1.end;
			this->ele = l1.ele;
		}

		LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &l1) {
			this->ptr = l1.ptr;
			this->n_ele = l1.n_ele;
			this->index = l1.index;
			this->end = l1.end;
			this->ele = l1.ele;
			return *this;
		}

		friend bool operator==(const LastDimensionMajorIterator &l1,
						const LastDimensionMajorIterator &l2) {
			if((l1.ptr != l2.ptr) || (l1.n_ele != l2.n_ele) || (l1.index != l2.index) || (l1.end != l2.end)) {
				return false;
			}
			if(l1.end) {
				return true;
			}
			return (l1.ele == l2.ele);
		}

		friend bool operator!=(const LastDimensionMajorIterator &l1,
						const LastDimensionMajorIterator &l2) {
			return !(l1 == l2);
		}

		void incr() {
			ele = &(ptr->a[index]);
		}

		LastDimensionMajorIterator &operator++() {
			if(++index == n_ele) {
				index = index % n_ele;
				end = 1;
			} else {
				end = 0;
			}
			incr();
			return *this;
		}

		LastDimensionMajorIterator operator++(int inc) {
			LastDimensionMajorIterator l1 = *this;
			++(*this);
			return l1;
		}

		T &operator*() const {
			return *ele;
		}
	};
};

}
