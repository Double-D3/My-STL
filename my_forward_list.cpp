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
		my_forward_list(const my_forward_list& other);
		my_forward_list(my_forward_list&& other);
		inline void push_front(const T& data);
		void pop_front();
		iterator begin() const;
		iterator befor_begin() const;
		iterator end() const;
		void clear();
		bool empty();
		T& front();
		iterator erase_after(iterator _where);
		iterator erase_after(iterator _first, iterator _last);
		my_forward_list& operator=(const my_forward_list& other);
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
		iterator(my_forward_list<T>::list_element* _it_ptr, my_forward_list<T>::list_element* _before_begin, my_forward_list<T>::list_element* _end_of_list);
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
		list_element(const T& data, list_element* ptr_to_next = nullptr);
		~list_element();
	};

	/////////////////////////////////////my_forward_list/////////////////////////////////
	template<class T>
	my_forward_list<T>::my_forward_list()
	{
		this->ptr_to_end = new my_forward_list<T>::list_element();
		this->ptr_before_begin = new my_forward_list<T>::list_element(T(), this->ptr_to_end);
		this->ptr_to_front = nullptr;
		this->size = 0;
	}

	template<class T>
	my_forward_list<T>::my_forward_list(unsigned int size, const T& data) :my_forward_list<T>::my_forward_list()
	{
		for (size_t i = 1; i <= size; i++)
		{
			this->push_front(data);
		}
	}

	template<class T>
	my_forward_list<T>::my_forward_list(const my_forward_list& other) :my_forward_list<T>::my_forward_list()
	{
		*this = other;
	}

	template<class T>
	my_forward_list<T>::my_forward_list(my_forward_list&& other)
	{
		this->ptr_before_begin = other.ptr_before_begin;
		this->ptr_to_end = other.ptr_to_end;
		this->ptr_to_front = other.ptr_to_front;
		this->size = other.size;
		other.ptr_before_begin = other.ptr_to_end = other.ptr_to_front = nullptr;
	}

	template<class T>
	my_forward_list<T>& my_forward_list<T>::operator=(const my_forward_list& other)
	{
		if (this->ptr_to_front != nullptr)
			this->clear();

		if (other.begin().it_ptr == nullptr)
			return *this;

		typename my_forward_list<T>::iterator iterator_for_this(this->befor_begin());
		typename my_forward_list<T>::iterator iterator_for_other(other.begin());

		for (; iterator_for_other.it_ptr != other.ptr_to_end;)
		{
			(*iterator_for_this.it_ptr).ptr = new my_forward_list<T>::list_element(*iterator_for_other, this->ptr_to_end);
			++iterator_for_other;
			++iterator_for_this;
			++this->size;
		}
		this->ptr_to_front = this->ptr_before_begin->ptr;
		this->check_boundary();
		return *this;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::begin() const
	{
		my_forward_list<T>::iterator New_it;
		New_it.it_ptr = this->ptr_to_front;
		New_it.before_begin = this->ptr_before_begin;
		New_it.end_of_list = this->ptr_to_end;
		return New_it;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::befor_begin() const
	{
		my_forward_list<T>::iterator New_it;
		New_it.it_ptr = this->ptr_before_begin;
		New_it.before_begin = this->ptr_before_begin;
		New_it.end_of_list = this->ptr_to_end;
		return New_it;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::end() const
	{
		my_forward_list<T>::iterator New_it;
		New_it.it_ptr = New_it.end_of_list = this->ptr_to_end;
		New_it.before_begin = this->ptr_before_begin;
		return New_it;
	}

	template<class T>
	void my_forward_list<T>::clear()
	{
		if (this->ptr_to_front != nullptr)
		{
			my_forward_list<T>::list_element* ptr_for_del = this->ptr_to_front;
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
	typename my_forward_list<T>::iterator my_forward_list<T>::erase_after(iterator _where)
	{
		if (_where.it_ptr == this->ptr_to_end || _where.it_ptr->ptr == this->ptr_to_end)
		{
			std::cout << "can't erase out of range element";
			abort();
		}

		my_forward_list<T>::list_element* ptr_to_delete = _where.it_ptr->ptr;
		my_forward_list<T>::iterator _It_after_delete(ptr_to_delete->ptr, _where.before_begin, _where.end_of_list);

		_where.it_ptr->ptr = _It_after_delete.it_ptr;
		if (ptr_to_delete == this->ptr_to_front)
			this->ptr_to_front = this->ptr_before_begin->ptr;

		delete ptr_to_delete;
		--this->size;

		this->check_boundary();
		return _It_after_delete;
	}

	template<class T>
	typename my_forward_list<T>::iterator my_forward_list<T>::erase_after(iterator _first, iterator _last)
	{
		if (_first.it_ptr == this->ptr_to_end)
		{
			std::cout << "can't erase out of range element";
			abort();
		}

		for (;;) 
		{
			if (_first.it_ptr->ptr == _last.it_ptr)
			{
				break;
			}
			if (_first.it_ptr == this->ptr_to_end)//условная защита, если элемент _first стоит после _last. Но все элементы списка после _first будут удалены до срабатывания защиты
			{
				std::cout << "Incorrect setting of parameters in erase_after(). \"first\" element comes after \"last\""<<std::endl;
					abort();
			}
			this->erase_after(_first);
		}
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
		if (this->ptr_to_front != nullptr)
		{
			this->size--;
			my_forward_list<T>::list_element* old_front = this->ptr_to_front;
			this->ptr_to_front = old_front->ptr;
			delete old_front;
		}
		this->check_boundary();
	}

	template<class T>
	inline void my_forward_list<T>::push_front(const T& data)
	{
		if (this->size == 0)
			this->ptr_to_front = new my_forward_list<T>::list_element(data, this->ptr_to_end);
		else this->ptr_to_front = new my_forward_list<T>::list_element(data, this->ptr_to_front);

		this->size++;
		this->check_boundary();
	}

	template<class T>
	my_forward_list<T>::~my_forward_list()
	{
		if (this->ptr_before_begin != nullptr)
			delete this->ptr_before_begin;

		if (this->ptr_to_end != nullptr)
			delete this->ptr_to_end;

		this->clear();
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
	my_forward_list<T>::iterator::iterator(const typename my_forward_list<T>::iterator& other)
	{
		*this = other;
	}

	template<class T>
	my_forward_list<T>::iterator::iterator(my_forward_list<T>::list_element* _it_ptr, my_forward_list<T>::list_element* _before_begin, my_forward_list<T>::list_element* _end_of_list)
	{
		this->it_ptr = _it_ptr;
		this->before_begin = _before_begin;
		this->end_of_list = _end_of_list;
	}

	template<class T>
	my_forward_list<T>::iterator::iterator(typename my_forward_list<T>::iterator&& other) noexcept
	{
		*this = other;
	}

	template<class T>
	my_forward_list<T>::iterator::~iterator() {}

	template<class T>
	T& my_forward_list<T>::iterator::operator*()
	{
		this->access_check();
		return (*this->it_ptr).data;
	}

	template<class T>
	typename my_forward_list<T>::iterator& my_forward_list<T>::iterator::operator=(const typename my_forward_list<T>::iterator& other)
	{
		this->it_ptr = other.it_ptr;
		this->end_of_list = other.end_of_list;
		this->before_begin = other.before_begin;
		return *this;
	}

	template<class T>
	bool my_forward_list<T>::iterator::operator==(const typename my_forward_list<T>::iterator& other)
	{
		return this->it_ptr == other.it_ptr;
	}

	template<class T>
	bool my_forward_list<T>::iterator::operator!=(const typename my_forward_list<T>::iterator& other)
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
		my_forward_list<T>::iterator old_it(*this);
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
