#include <iostream> 

namespace MySTL
{
	template<class T>
	class my_vector
	{
	private:
		T* array;
		unsigned int Size;//количество элементов в векторе
		unsigned int Capacity;//количество выделенных ячеек в векторе
		static double CAPACITY_INCREASE_FACTOR; // опеределяет количество дополнительных ячеек в векторе (30%)
		bool increase_capacity();
		void check(int subscript);
	public:
		class iterator;
		my_vector();
		my_vector(const unsigned int size);
		my_vector(const unsigned int size, T element);
		my_vector(const my_vector& other);
		my_vector(my_vector&& other)noexcept; 
		~my_vector();
		T& operator[](const int& i);
		int size();
		int capacity();
		void push_back(const T& New_Element);
		void pop_back();
		void reserve(unsigned int capacity);
		T& at(const unsigned int position);
		void clear();
		bool empty();
		iterator begin();
		iterator end();
		void insert(iterator Iterator, const T& element);
		void erase(iterator Iterator);
		void swap(my_vector& right);
		my_vector& operator=(const my_vector& right);
		my_vector& operator=(my_vector&& right)noexcept;
	};

	template<class T>
	class my_vector<T>::iterator
	{
	private:
		T* ptr;
		T* vector_begin;
		T* vector_end;
		void access_check();
		void check_iterator();
		void check_erase_iterator();
	public:
		friend my_vector;
		iterator();
		iterator(const iterator& other);
		iterator(iterator&& other)noexcept;
		~iterator();
		T& operator*();
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		iterator& operator=(const iterator& other);
		iterator operator+(const int shift);
		iterator operator-(const int shift);
		iterator operator+=(const int shift);
		iterator operator-=(const int shift);
		bool operator!=(const iterator& other);
		bool operator<(const iterator& other);
		bool operator>(const iterator& other);
		bool operator<=(const iterator& other);
		bool operator>=(const iterator& other);
		bool operator==(const iterator& other);
	};

	//////////////////////////iterator/////////////////////////
	template<class T>
	void my_vector<T>::iterator::access_check()
	{
		if (this->ptr < this->vector_begin || this->ptr >= (this->vector_end))
		{
			std::cout << std::endl << "can't dereference out of range my_vector iterator";
			abort();
		}
	}

	template<class T>
	void my_vector<T>::iterator::check_iterator()
	{
		if (this->ptr < this->vector_begin || this->ptr >(this->vector_end))
		{
			std::cout << std::endl << "you can't increment (or decrement) an iterator beyond a range";
			abort();
		}
	}

	template<class T>
	void my_vector<T>::iterator::check_erase_iterator()
	{
		if (this->ptr == this->vector_end)
		{
			std::cout << std::endl << "my_vector erase iterator outside range";
			abort();
		}
	}

	template<class T>
	bool my_vector<T>::iterator::operator!=(const iterator& other)
	{
		return this->ptr != other.ptr;
	}

	template<class T>
	bool my_vector<T>::iterator::operator<(const iterator& other)
	{
		return this->ptr < other.ptr;
	}

	template<class T>
	bool my_vector<T>::iterator::operator>(const iterator& other)
	{
		return this->ptr > other.ptr;
	}

	template<class T>
	bool my_vector<T>::iterator::operator<=(const iterator& other)
	{
		return this->ptr <= other.ptr;
	}

	template<class T>
	bool my_vector<T>::iterator::operator>=(const iterator& other)
	{
		return this->ptr >= other.ptr;
	}

	template<class T>
	bool my_vector<T>::iterator::operator==(const iterator& other)
	{
		return this->ptr == other.ptr;
	}

	template<class T>
	typename my_vector<T>::iterator my_vector<T>::iterator::operator+=(const int shift)
	{
		this->ptr += shift;
		this->check_iterator();
		return *this;
	}

	template<class T>
	typename my_vector<T>::iterator my_vector<T>::iterator::operator-=(const int shift)
	{
		this->ptr -= shift;
		this->check_iterator();
		return *this;
	}

	template<class T>
	typename my_vector<T>::iterator my_vector<T>::iterator::operator+(const int shift)
	{
		iterator New_iterator = *this;
		New_iterator.ptr += shift;
		New_iterator.check_iterator();
		return New_iterator;
	}

	template<class T>
	typename my_vector<T>::iterator my_vector<T>::iterator::operator-(const int shift)
	{
		iterator New_iterator = *this;
		New_iterator.ptr -= shift;
		New_iterator.check_iterator();
		return New_iterator;
	}

	template<class T>
	typename my_vector<T>::iterator& my_vector<T>::iterator::operator++()
	{
		this->ptr++;
		this->check_iterator();
		return *this;
	}

	template<class T>
	typename my_vector<T>::iterator my_vector<T>::iterator::operator++(int)
	{
		iterator New_iterator = *this;
		this->ptr++;
		this->check_iterator();
		return New_iterator;
	}

	template<class T>
	typename my_vector<T>::iterator& my_vector<T>::iterator::operator--()
	{
		this->ptr--;
		this->check_iterator();
		return *this;
	}

	template<class T>
	typename my_vector<T>::iterator my_vector<T>::iterator::operator--(int)
	{
		iterator New_iterator = *this;
		this->ptr--;
		this->check_iterator();
		return New_iterator;
	}

	template<class T>
	typename my_vector<T>::iterator& my_vector<T>::iterator::operator=(const iterator& other)
	{
		this->ptr = other.ptr;
		this->vector_begin = other.vector_begin;
		this->vector_end = other.vector_end;
		return *this;
	}

	template<class T>
	T& my_vector<T>::iterator::operator*()
	{
		this->access_check();
		return *(this->ptr);
	}

	template<class T>
	my_vector<T>::iterator::~iterator() {}

	template<class T>
	my_vector<T>::iterator::iterator()
	{
		this->ptr = this->vector_begin = this->vector_end = nullptr;
	}

	template<class T>
	my_vector<T>::iterator::iterator(const iterator& other)
	{
		*this = other;
	}

	template<class T>
	my_vector<T>::iterator::iterator(iterator&& other)noexcept
	{
		*this = other;
	}
	//////////////////////////iterator/////////////////////////
	//////////////////////////my_vector/////////////////////////
	template<class T>
	void my_vector<T>::insert(iterator Iterator, const T& element)
	{
		this->Size++;
		if (this->increase_capacity())
		{
			T* New_Array = new T[this->Capacity]();
			iterator it = this->begin();
			size_t index = 0;
			for (; it < Iterator; ++it, ++index)
			{
				New_Array[index] = this->array[index];
			}
			New_Array[index] = element;
			++index;
			for (; index < this->Size; index++)
			{
				New_Array[index] = this->array[index - 1];
			}
			delete[]this->array;
			this->array = New_Array;
			New_Array = nullptr;
		}
		else
		{
			iterator end_iterator = this->end() - 1;
			for (; end_iterator != Iterator; --end_iterator)
			{
				*(end_iterator) = *(end_iterator - 1);
			}
			*Iterator.ptr = element;
		}
	}

	template<class T>
	void my_vector<T>::erase(iterator Iterator)
	{
		Iterator.check_erase_iterator();
		this->Size--;
		iterator end_iterator = this->end();
		size_t index = 0;
		for (; Iterator < end_iterator; ++Iterator)
		{
			*Iterator = *(Iterator + 1);
		}
	}

	template<class T>
	void my_vector<T>::swap(my_vector<T>& right)
	{
		my_vector<T> new_vector = right;
		right = std::move(*this);
		*this = std::move(new_vector);
	}

	template<class T>
	my_vector<T>& my_vector<T>::operator=(const my_vector<T>& right)
	{
		if (this->array != nullptr)
			delete[]this->array;

		this->array = new T[right.Capacity]();
		this->Capacity = right.Capacity;
		this->Size = right.Size;
		for (size_t index = 0; index < this->Size; index++)
		{
			this->array[index] = right.array[index];
		}
		return *this;
	}

	template<class T>
	my_vector<T>& my_vector<T>::operator=(my_vector<T>&& right) noexcept
	{
		this->Capacity = right.Capacity;
		this->Size = right.Size;
		this->array = right.array;
		right.Capacity = right.Size = 0;
		right.array = nullptr;
		return *this;
	}

	template<class T>
	typename my_vector<T>::iterator my_vector<T>::end()
	{
		iterator New_iterator;
		New_iterator.ptr = New_iterator.vector_end = this->array + this->Size;
		New_iterator.vector_begin = this->array;
		return New_iterator;
	}

	template<class T>
	typename my_vector<T>::iterator my_vector<T>::begin()
	{
		iterator New_iterator;
		New_iterator.ptr = this->array;
		New_iterator.vector_begin = this->array;
		New_iterator.vector_end = this->array + this->Size;
		return New_iterator;
	}

	template<class T>
	bool my_vector<T>::empty()
	{
		if (this->Size == 0)
			return true;
		else return false;
	}

	template<class T>
	void my_vector<T>::pop_back()
	{
		if (this->Size != 0)
		{
			this->array[this->Size - 1] = T();
			this->Size--;
		}
		else return;
	}

	template<class T>
	void my_vector<T>::clear()
	{
		if (this->array != nullptr)
		{
			delete[]this->array;
			this->array = nullptr;
			this->Size = this->Capacity = 0;
		}
	}

	template<class T>
	void my_vector<T>::reserve(unsigned int capacity)
	{
		this->Capacity = capacity;
		this->array = new T[capacity];
	}

	template<class T>
	T& my_vector<T>::at(const unsigned int position)
	{
		this->check(position);
		return this->array[position];
	}

	template<class T>
	bool my_vector<T>::increase_capacity()//true - необходимо перезаписывать вектор в новую область памяти, false - нет необходимости перезаписывать данные вектора
	{
		const int EXTRA_CELL = 1;
		if (this->Size > this->Capacity)// если новое количество элементов в векторе больше зарезервированного
		{
			this->Capacity = this->Size + (int)(this->Size * this->CAPACITY_INCREASE_FACTOR + EXTRA_CELL);
			return true;
		}
		else return false;
	}

	template<class T>
	void my_vector<T>::check(int subscript)
	{
		if (subscript < 0 || subscript >= Size)
		{
			std::cout << std::endl << "my_vector subscript out of range";
			abort();
		}
	}

	template<class T>
	void my_vector<T>::push_back(const T& New_Element)
	{
		this->Size++;
		if (this->increase_capacity())//если пора увеличивать this->Capacity, то перезаписываем данные в новую область памяти
		{
			T* New_Array = new T[this->Capacity]();
			for (size_t index = 0; index < Size - 1; index++)
			{
				New_Array[index] = this->array[index];
			}
			New_Array[Size - 1] = New_Element;
			delete[]this->array;
			this->array = New_Array;
			New_Array = nullptr;
		}
		else this->array[Size - 1] = New_Element;
	}

	template<class T>
	int my_vector<T>::capacity()
	{
		return this->Capacity;
	}

	template<class T>
	int my_vector<T>::size()
	{
		return this->Size;
	}

	template<class T>
	my_vector<T>::my_vector()
	{
		const int EMPTY = 0;
		this->array = nullptr;
		this->Size = this->Capacity = EMPTY;
	}

	template<class T>
	my_vector<T>::my_vector(const unsigned int size) : my_vector()
	{
		if (size > 0)
		{
			this->Size = this->Capacity = size;
			this->array = new T[size];
			for (unsigned int i = 0; i < size; i++)
			{
				this->array[i] = T(); 
			}
		}
		else this->check(size);
	}

	template<class T>
	my_vector<T>::my_vector(const unsigned int size, T element) : my_vector()
	{
		if (size > 0)
		{
			this->Size = this->Capacity = size;
			this->array = new T[size];
			for (unsigned int i = 0; i < size; i++)
			{
				this->array[i] = element;
			}
		}
		else this->check(size);
	}

	template<class T>
	my_vector<T>::my_vector(const my_vector<T>& other)
	{
		*this = other;
	}

	template<class T>
	my_vector<T>::my_vector(my_vector<T>&& other)noexcept
	{
		*this = std::move(other);
	}

	template<class T>
	my_vector<T>::~my_vector()
	{
		if (this->array != nullptr)
		{
			delete[]this->array;
			this->Size = this->Capacity = 0;
		}
	}

	template<class T>
	T& my_vector<T>::operator[](const int& i)
	{
		return array[i];
	}
	template<class T>
	double my_vector<T>::CAPACITY_INCREASE_FACTOR = 0.3;
	//////////////////////////my_vector/////////////////////////
};

int main()
{
	return 0;
}
