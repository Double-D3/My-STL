#include <iostream> 

namespace MySTL
{
template<class T>
	class my_forward_list
	{
	private:
		class list_element;
		size_t size;
		list_element* ptr_before_begin;
		list_element* ptr_to_end;//ptr_before_begin и ptr_to_end настоящие границы списка - элементы без данных
		list_element* ptr_to_front;
		inline void check_boundary();//метод следит за правильными значеними ptr_to_front и ptr_before_begin
	public:
		class iterator;
		my_forward_list();
		my_forward_list(unsigned int size, const T& data = T());
		my_forward_list(unsigned int size, T&& data = T());
		my_forward_list(const my_forward_list& other);
		my_forward_list(my_forward_list&& other)noexcept;
		iterator begin() const;
		iterator before_begin() const;
		iterator end() const;
		T& front();
		iterator erase_after(iterator where);
		iterator erase_after(iterator first, iterator last);
		iterator insert_after(iterator where, const T& data); 
		iterator emplace_after(iterator where, T&& data);
		iterator emplace_front(T&& data);
		my_forward_list& operator=(const my_forward_list& other);
		my_forward_list& operator=(my_forward_list&& other)noexcept;
		bool empty();
		inline void push_front(const T& data);
		void pop_front();
		void clear();
		void sort();//bubble sort
		void remove(const T& data);
		void remove_if(bool(*pred)(iterator&));
		void reverse();
		void swap(my_forward_list& other);
		void unique();
		void resize(size_t size, const T& data = T());
		void assign(size_t size, const T& data);
		void assign(iterator left, iterator right);
		~my_forward_list();
	};

	template<class T>
	class my_forward_list<T>::iterator
	{
	private:
		typename my_forward_list<T>::list_element* it_ptr;
		typename my_forward_list<T>::list_element* before_begin;
		typename my_forward_list<T>::list_element* end_of_list;
		void access_check();
		void check_iterator();
		friend my_forward_list;
		iterator(typename my_forward_list<T>::list_element* _it_ptr, typename my_forward_list<T>::list_element* _before_begin, typename my_forward_list<T>::list_element* _end_of_list);
	public:
		iterator();
		iterator(const iterator& other);
		iterator(iterator&& other)noexcept;
		T& operator*();
		iterator& operator=(const iterator& other);
		bool operator==(const iterator& other);
		bool operator!=(const iterator& other);
		iterator& operator++();
		iterator operator++(int);
		~iterator();
	};

	template<class T>
	class my_forward_list<T>::list_element
	{
	public:
		T data;
		list_element* ptr;
		list_element();
		list_element(const T& data, list_element* ptr_to_next);
		list_element(T&& data, list_element* ptr_to_next);
		list_element(list_element* ptr_to_next);
		~list_element();
	};

	/////////////////////////////////////my_forward_list/////////////////////////////////
	template<class T>
	my_forward_list<T>::my_forward_list()
	{
		this->ptr_to_end = new list_element();
		this->ptr_before_begin = new list_element(T(), this->ptr_to_end);
		this->ptr_to_front = nullptr;
		this->size = 0;
	}

	template<class T>
	my_forward_list<T>::my_forward_list(unsigned int size, const T& data) : my_forward_list()
	{
		for (size_t i = 1; i <= size; i++)
		{
			this->push_front(data);
		}
	}

	template<class T>
	my_forward_list<T>::my_forward_list(unsigned int size, T&& data) : my_forward_list()
	{
		for (size_t i = 1; i <= size; i++)
		{
			this->emplace_front(std::move(data));
		}
	}

	template<class T>
	my_forward_list<T>::my_forward_list(const my_forward_list<T>& other) : my_forward_list()
	{
		*this = other;
	}

	template<class T>
	my_forward_list<T>::my_forward_list(my_forward_list<T>&& other) noexcept
	{
		*this = std::move(other);
	}

	template<class T>
	my_forward_list<T>& my_forward_list<T>::operator=(const my_forward_list<T>& other)
	{
		if (this->ptr_to_front != nullptr)
			this->clear();

		if (other.ptr_to_front == nullptr)
			return *this;

		iterator iterator_for_this(this->before_begin());
		iterator iterator_for_other(other.begin());

		for (; iterator_for_other.it_ptr != other.ptr_to_end;)
		{
			iterator_for_this.it_ptr->ptr = new list_element(*iterator_for_other, this->ptr_to_end);
			++iterator_for_other;
			++iterator_for_this;
			++this->size;
		}
		this->ptr_to_front = this->ptr_before_begin->ptr;
		this->check_boundary();
		return *this;
	}

	template<class T>
	my_forward_list<T>& my_forward_list<T>::operator=(my_forward_list<T>&& other)noexcept
	{
		if (this->ptr_to_front != nullptr)
			this->clear();

		if (other.ptr_to_front == nullptr)
			return *this;

		this->size = other.size;
		this->ptr_before_begin = other.ptr_before_begin;
		this->ptr_to_end = other.ptr_to_end;
		this->ptr_to_front = other.ptr_to_front;
		other.size = 0;
		other.ptr_before_begin = other.ptr_to_end = other.ptr_to_front = nullptr;
		return *this;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::begin() const
	{
		iterator New_it(this->ptr_to_front, this->ptr_before_begin, this->ptr_to_end);
		return New_it;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::before_begin() const
	{
		iterator New_it(this->ptr_before_begin, this->ptr_before_begin, this->ptr_to_end);
		return New_it;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::end() const
	{
		iterator New_it(this->ptr_to_end, this->ptr_before_begin, this->ptr_to_end);
		return New_it;
	}

	template<class T>
	void my_forward_list<T>::clear()
	{
		if (this->ptr_to_front != nullptr)
		{
			list_element* ptr_for_del = this->ptr_to_front;
			for (size_t i = 1; i <= this->size; i++)
			{
				this->ptr_to_front = this->ptr_to_front->ptr;
				if (ptr_for_del != nullptr)
				{
					delete ptr_for_del;
					ptr_for_del = this->ptr_to_front;
				}
			}
		}
		this->size = 0;
		if (this->ptr_before_begin != nullptr && this->ptr_to_end != nullptr)
			this->check_boundary();
	}

	template<class T>
	bool my_forward_list<T>::empty()
	{
		return this->size == 0;
	}

	template<class T>
	T& my_forward_list<T>::front()
	{
		if (this->size == 0)
		{
			std::cout << "front() called on empty forward_list";
			abort();
		}
		return this->ptr_to_front->data;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::erase_after(iterator where)
	{
		if (where.it_ptr == this->ptr_to_end || where.it_ptr->ptr == this->ptr_to_end)
		{
			std::cout << "can't erase out of range element";
			abort();
		}

		list_element* ptr_to_delete = where.it_ptr->ptr;
		where.it_ptr->ptr = ptr_to_delete->ptr;

		if (ptr_to_delete == this->ptr_to_front)
			this->ptr_to_front = this->ptr_before_begin->ptr;

		delete ptr_to_delete;
		--this->size;
		this->check_boundary();
		++where;
		return where;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::erase_after(iterator first, iterator last)
	{
		if (first.it_ptr == this->ptr_to_end)
		{
			std::cout << "can't erase out of range element";
			abort();
		}

		for (;;)
		{
			if (first.it_ptr->ptr == last.it_ptr)
			{
				break;
			}
			if (first.it_ptr == this->ptr_to_end)
			{
				std::cout << "Incorrect setting of parameters in erase_after(). \"first\" element comes after \"last\"" << std::endl;
				abort();
			}
			this->erase_after(first);
		}
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::insert_after(iterator where, const T& data)
	{
		if (where.it_ptr == this->ptr_to_end)
		{
			std::cout << "can't add element after end of my_forward_list";
			abort();
		}
		where.it_ptr->ptr = new list_element(data, where.it_ptr->ptr);
		if (where.it_ptr == this->ptr_before_begin)
			this->ptr_to_front = this->ptr_before_begin->ptr;
		++where;
		++this->size;
		return where;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::emplace_after(iterator where, T&& data)
	{
		if (where.it_ptr == this->ptr_to_end)
		{
			std::cout << "can't emplace element after end of my_forward_list";
			abort();
		}
		where.it_ptr->ptr = new list_element(std::move(data), where.it_ptr->ptr);
		if (where.it_ptr == this->ptr_before_begin)
			this->ptr_to_front = this->ptr_before_begin->ptr;
		++where;
		++this->size;
		return where;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::emplace_front(T&& data)
	{
		this->emplace_after(this->before_begin(), std::move(data));
		return this->begin();
	}

	template<class T>
	inline void my_forward_list<T>::check_boundary()
	{
		if (this->size == 0)
		{
			this->ptr_to_front = nullptr;
			this->ptr_before_begin->ptr = this->ptr_to_end;
		}
		else this->ptr_before_begin->ptr = this->ptr_to_front;
	}

	template<class T>
	void my_forward_list<T>::pop_front()
	{
		this->erase_after(this->before_begin());
	}

	template<class T>
	inline void my_forward_list<T>::push_front(const T& data)
	{
		this->insert_after(this->before_begin(), data);
	}

	template<class T>
	my_forward_list<T>::~my_forward_list()
	{
		if (this->ptr_before_begin != nullptr)
		{
			delete this->ptr_before_begin;
			this->ptr_before_begin = nullptr;
		}

		if (this->ptr_to_end != nullptr)
		{
			delete this->ptr_to_end;
			this->ptr_to_end = nullptr;
		}

		this->clear();
	}

	template<class T>
	void my_forward_list<T>::sort()
	{
		size_t i = 1;
		while (i < this->size)
		{
			iterator it_before_current = this->before_begin();
			iterator it_current = this->begin();
			iterator it_next = ++this->begin();
			for (size_t j = 1; j <= this->size - i; j++)
			{
				if (*it_current > *it_next)
				{
					list_element* temp = it_current.it_ptr;
					it_before_current.it_ptr->ptr = it_next.it_ptr;
					it_current.it_ptr->ptr = it_next.it_ptr->ptr;
					it_next.it_ptr->ptr = temp;

					it_next.it_ptr = it_current.it_ptr->ptr;
					it_before_current++;
				}
				else {
					it_before_current++;
					it_current++;
					it_next++;
				}
				if (j == 1)
					this->ptr_to_front = it_before_current.it_ptr;
			}
			i++;
		}
	}

	template<class T>
	void my_forward_list<T>::remove(const T& data)
	{
		iterator it_before_current = this->before_begin();
		iterator it_current = this->begin();
		while (it_current.it_ptr != this->ptr_to_end)
		{
			if (*it_current == data)
				it_current = this->erase_after(it_before_current);
			else
			{
				it_before_current++;
				it_current++;
			}
		}
	}

	template<class T>
	void my_forward_list<T>::remove_if(bool(*pred)(iterator&))
	{
		iterator it_before_current = this->before_begin();
		iterator it_current = this->begin();
		while (it_current.it_ptr != this->ptr_to_end)
		{
			if (pred(it_current))
				it_current = this->erase_after(it_before_current);
			else
			{
				it_before_current++;
				it_current++;
			}
		}
	}

	template<class T>
	void my_forward_list<T>::reverse()
	{
		iterator it_current = this->begin();
		iterator it_before_begin = this->before_begin();
		iterator it_for_test = this->begin();
		this->ptr_to_front = nullptr;
		while (it_current.it_ptr != this->ptr_to_end)
		{
			this->insert_after(it_before_begin, *it_current);
			if (it_current == it_for_test)
				this->ptr_to_front->ptr = this->ptr_to_end;
			it_current++;
			this->size--;
		}
		this->check_boundary();
	}

	template<class T>
	void my_forward_list<T>::swap(my_forward_list<T>& other)
	{
		my_forward_list<T> temp = std::move(other);
		other = std::move(*this);
		*this = std::move(temp);
	}

	template<class T>
	void my_forward_list<T>::unique()
	{
		iterator it_current = this->begin();
		iterator it_after_current = ++this->begin();
		while (it_after_current.it_ptr != this->ptr_to_end)
		{
			if (*it_current == *it_after_current)
			{
				this->erase_after(it_current);
				iterator temp = it_current;
				it_after_current = ++temp;
			}
			else
			{
				it_after_current++;
				it_current++;
			}
		}
	}

	template<class T>
	void my_forward_list<T>::resize(size_t size, const T& data)
	{
		if (size == 0)
		{
			this->clear();
			return;
		}
		if (size < this->size)
		{
			iterator temp = this->before_begin();
			iterator after_temp = ++this->before_begin();
			for (size_t i = 0; i <= size - 1; i++)
			{
				temp++;
				after_temp++;
				if (i == size - 1)
				{
					while (after_temp.it_ptr != this->ptr_to_end)
					{
						after_temp++;
						this->erase_after(temp);
					}
				}
			}
		}
		if (size > this->size)
		{
			iterator temp = this->begin();
			size_t this_size = this->size;
			for (size_t i = 0; i < this_size -1; i++)
			{
				temp++;
			}
			for (size_t i = 1; i <= size - this_size; i++)
			{
				temp = insert_after(temp, data);
			}
		}
	}

	template<class T>
	void my_forward_list<T>::assign(size_t size, const T& data)
	{
		if (this->size != 0)
			this->clear();
		for (size_t i = 0; i < size; i++)
		{
			push_front(data);
		}
	}

	template<class T>
	void my_forward_list<T>::assign(iterator left, iterator right)
	{
		if (this->size != 0)
			this->clear();
		iterator temp = this->before_begin();
		if (left.it_ptr != nullptr)
		{
			for (; left != right; )
			{
				temp = insert_after(temp, *left);
				left++;
			}
		}
	}

	/////////////////////////////////////my_forward_list/////////////////////////////////
	//////////////////////////////////////list_element//////////////////////////////////
	template<class T>
	my_forward_list<T>::list_element::list_element()
	{
		this->data = T();
		this->ptr = nullptr;
	}

	template<class T>
	my_forward_list<T>::list_element::list_element(const T& data, list_element* ptr_to_next)
	{
		this->data = data;
		this->ptr = ptr_to_next;
	}

	template<class T>
	my_forward_list<T>::list_element::list_element(T&& data, list_element* ptr_to_next)
	{
		this->data = std::move(data);
		this->ptr = ptr_to_next;
	}

	template<class T>
	my_forward_list<T>::list_element::list_element(list_element* ptr_to_next)
	{
		this->data = T();
		this->ptr = ptr_to_next;
	}

	template<class T>
	my_forward_list<T>::list_element::~list_element()
	{
		this->ptr = nullptr;
	}
	//////////////////////////////////////list_element//////////////////////////////////
	/////////////////////////////////////iterator/////////////////////////////////
	template<class T>
	void my_forward_list<T>::iterator::access_check()
	{
		if (this->it_ptr == this->before_begin || this->it_ptr == this->end_of_list)
		{
			std::cout << std::endl << "can't dereference out of range my_forward_list iterator";
			abort();
		}
	}

	template<class T>
	void my_forward_list<T>::iterator::check_iterator()
	{
		if (this->it_ptr == nullptr)
		{
			std::cout << std::endl << "you can't increment an iterator beyond a range";
			abort();
		}
	}

	template<class T>
	my_forward_list<T>::iterator::iterator()
	{
		this->it_ptr = this->end_of_list = this->before_begin = nullptr;
	}

	template<class T>
	my_forward_list<T>::iterator::iterator(const iterator& other)
	{
		*this = other;
	}

	template<class T>
	my_forward_list<T>::iterator::iterator(list_element* _it_ptr, list_element* _before_begin, list_element* _end_of_list)
	{
		this->it_ptr = _it_ptr;
		this->before_begin = _before_begin;
		this->end_of_list = _end_of_list;
	}

	template<class T>
	my_forward_list<T>::iterator::iterator(iterator&& other) noexcept
	{
		*this = other;
	}

	template<class T>
	my_forward_list<T>::iterator::~iterator() {}

	template<class T>
	T& my_forward_list<T>::iterator::operator*()
	{
		this->access_check();
		return this->it_ptr->data;
	}

	template<class T>
	typename my_forward_list<T>::iterator& my_forward_list<T>::iterator::operator=(const iterator& other)
	{
		this->it_ptr = other.it_ptr;
		this->end_of_list = other.end_of_list;
		this->before_begin = other.before_begin;
		return *this;
	}

	template<class T>
	bool my_forward_list<T>::iterator::operator==(const iterator& other)
	{
		return this->it_ptr == other.it_ptr;
	}

	template<class T>
	bool my_forward_list<T>::iterator::operator!=(const iterator& other)
	{
		return this->it_ptr != other.it_ptr;
	}

	template<class T>
	typename my_forward_list<T>::iterator& my_forward_list<T>::iterator::operator++()
	{
		this->it_ptr = this->it_ptr->ptr;
		this->check_iterator();
		return *this;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::iterator::operator++(int)
	{
		iterator old_it(*this);
		this->it_ptr = this->it_ptr->ptr;
		this->check_iterator();
		return old_it;
	}
	/////////////////////////////////////iterator/////////////////////////////////
};

int main()
{
	return 0;
}
