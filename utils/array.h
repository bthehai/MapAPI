#pragma once

template<class T> 
class Array
{
public:
	Array(int growby = 10);
	Array(const Array<T>&);
	virtual ~Array();

	//	Accessors
	int getSize() const;
	int getCapacity() const;
	bool setSize(int);
	bool setCapacity(int);
	T* getData() const;
	T* getData();
	bool setData(T* pv, int n);
	bool isEmpty() const;
	T& getAt(int i) const;
	T* getPointerAt(int i) const;
	T& elementAt(int i);
	bool setAt(const T& val, int i);
	int getUpperBound() const;
	void freeExtra();
	void reverse();
	void sort();

	//	Other operations
	Array<T>& operator=(const Array<T>&);
	T& operator[](int i);
	const T& operator[](int i) const;
	Array<T>& operator+=(const Array<T>& src);
	void empty();
	bool removeAt(int i, int n = 1);
	bool insert(int i, const T& v);
	int add(const T&);
	bool append(const Array<T>&);
	bool copy(const Array<T>& src);
	int find(const T&) const;

protected:
	//	Internal functions
	virtual bool grow(int);
	virtual	void destructItems(int first, int last);

	//	Data members
	int m_Size;		// number of elements added so far
	int m_Capacity;	// maximum number of elements we have room for
	int m_GrowBy;
	T* m_pData;		// data area for array
};

template <class T> 
Array<T>::Array(int growby)
{
	m_Size = 0;				// empty to start
	m_Capacity = 0;			// remember the size
	if (growby <= 10)
		growby = 10;
	m_GrowBy = growby;
	m_pData = NULL;
}

template <class T> 
Array<T>::Array(const Array<T>& a)
{
	if(this == &a)
		return;

	m_Size = 0;		// empty to start
	m_Capacity = 0;	// remember the size
	m_GrowBy = 10;
	m_pData = NULL;
	append(a);		// copy each element from the given array
}

template <class T> 
inline void Array<T>::destructItems(int first, int last)
{
}

template <class T> 
inline Array<T>::~Array()
{
	empty();
	if (m_pData)
	{
		delete []m_pData;
		m_pData = NULL;
	}
	m_Size = 0;
	m_GrowBy = 10;
	m_Capacity = 0;
}

template <class T> 
bool Array<T>::grow(int growto)
{
	int	n = growto-m_Capacity;
	T* pOldData;

	if (n > 0)
	{
		while (m_Capacity < growto)
			m_Capacity += m_GrowBy;
		if (m_pData == NULL)	// make new data area?
		{
			m_pData = new T[m_Capacity];
			if (!m_pData)		// could not enlarge?
				return false;
		}
		else
		{
			pOldData = m_pData;
			m_pData = new T[m_Capacity];
			if (!m_pData)
				return false;
			memcpy(m_pData, pOldData, sizeof(T)*m_Size);
			delete []pOldData;
		}
	}
	return true;
}

template <class T> 
inline T* Array<T>::getData() const
{
	return m_pData;
}

template <class T> 
inline T* Array<T>::getData()
{
	return m_pData;
}

template <class T> 
bool Array<T>::setData(T* pv, int n)
{
	if (m_pData == NULL)
	{
		m_Capacity = n;
		m_Size = n;
		m_GrowBy = 10;
		m_pData = new T[m_Capacity];
		if (m_pData == NULL)
			return false;
		memcpy(m_pData, pv, sizeof(T)*m_Size);
	}
	else
	{
		if (m_Capacity < n)
		{
			delete []m_pData;
			m_Capacity = n;
			m_pData = new T[m_Capacity];
			if (m_pData == NULL)
				return false;
		}
		m_Size = n;
		memcpy(m_pData, pv, sizeof(T)*m_Size);
	}
	return true;
}

template <class T> 
bool Array<T>::setCapacity(int s)
{
	if (s > m_Capacity) // enlarge array
		return Grow(s); // if we can
	return true;
}

template <class T> 
inline int Array<T>::getCapacity() const
{
	return m_Capacity;
}

template <class T> 
bool inline Array<T>::setSize(int s)
{
	if (s > m_Capacity)
		if (!Grow(s))
			return false;
	m_Size = s;
	return true;
}

template <class T> 
inline int Array<T>::getSize() const
{
	return m_Size;
}

template <class T> 
bool Array<T>::setAt(const T& val, int i)
{
	if (i >= m_Capacity)		// need to extend array?
	{
		int n = m_Capacity;
		n += m_GrowBy;			// grow to 1 1/2 times current size
		if (n <= i)
			n = i+1;			// unless user wants more
		if (!grow(n))			// extend failure
			return false;
	}
	m_pData[i] = val;
	if (i >= m_Size)			// enlarge array size if at end
		m_Size = i+1;
	return true;
}

template <class T> 
inline T& Array<T>::getAt(int i) const
{
	return m_pData[i];
}

template <class T> 
inline T* Array<T>::getPointerAt(int i) const
{
	return &m_pData[i];
}

template <class T> 
inline T& Array<T>::elementAt(int i)
{
	return m_pData[i];
}

template <class T> 
inline const T& Array<T>::operator[](int i) const
{
	return m_pData[i];
}

template <class T> 
inline T& Array<T>::operator[](int i)
{
	return m_pData[i];
}

template <class T>
int Array<T>::find(const T& elem) const
{
	const T* p = m_pData;

	for (int i = 0; i < m_Size; ++i) {	// look for matching element
		if (*p++ == elem)
			return i;					// found it
	}
	return -1;
}

template <class T>int inline Array<T>::add(const T& v)
{
	int	index = m_Size;

	if (!setAt(v, index))
		return -1;
	return index;
}

template <class T> 
bool Array<T>::removeAt(int i, int n)
{
	T* p;
	int	shift;

	if ((i < 0) || (i >= m_Size))
		return false;
	if (n == 0)
		n = 1;
	shift = m_Size-(i+n);
	p = m_pData+i;
	memcpy(p, p+n, sizeof(T)*shift);
	m_Size -= n;
	return true;
}

template <class T> 
bool Array<T>::insert(int i, const T& v)
{
	if (i >= m_Capacity)
	{
		if (!grow(i+1))
			return false;
	}
	else
	{
		if (m_Size == m_Capacity)
			if (!grow(m_Capacity+m_GrowBy))
				return false;
	}

	int nMoveCount = m_Size-i;
	if (nMoveCount > 0)
		memmove(&m_pData[i+1], &m_pData[i], nMoveCount*sizeof(T));
	m_pData[i] = v;
	m_Size++;
	return true;
}

template <class T> 
bool Array<T>::append(const Array<T>& src)
{
	int	n = m_Size + src.m_Size;

	if (!grow(n))
		return false;
	for (int i = 0; i < src.m_Size; ++i)
		m_pData[m_Size+i] = src.m_pData[i];
	m_Size += src.m_Size;
	return true;
}

template <class T> 
Array<T>& Array<T>::operator +=(const Array<T>& src)
{
	append(src);
	return *this;
}

template <class T> 
bool Array<T>::copy(const Array<T>& src)
{
	if (this == &src)
		return true;
	m_Size = 0;
	m_Capacity = 0;
	if (m_pData)
		delete []m_pData;
	m_pData = NULL;
	return append(src);
}

template <class T> 
Array<T>& Array<T>::operator =(const Array<T>& src)
{
	copy(src);
	return *this;
}

template <class T> 
void Array<T>::empty()
{
	if (m_Size > 0)
		destructItems(0, m_Size-1);
	m_Size = 0;
}

template <class T>
inline bool Array<T>::isEmpty() const
{
	return m_Size == 0;
}

template<class T> 
int Array<T>::getUpperBound() const
{
	return m_Size-1;
}

template<class T> 
void Array<T>::freeExtra()
{
	if (m_Size != m_Capacity)
	{
		T *pNewData = NULL;
		if (m_Size != 0)
		{
			pNewData = new T[m_Size];
			memcpy(pNewData, m_pData, m_Size*sizeof(T));
		}
		delete []m_pData;
		m_pData = pNewData;
		m_Capacity = m_Size;
	}
}

template<class T> 
void Array<T>::reverse()
{
	int size = m_Size/2;
	T tmp;
	for (int i = 0; i < size; i++)
	{
		tmp = m_pData[i];
		m_pData[i] = m_pData[m_Size-i-1];
		m_pData[m_Size-i-1] = tmp;
	}
}

template<class T> 
void Array<T>::sort()
{
	int i, j, Inc;
	T Tmp;

	for(Inc = m_Size / 2; Inc > 0; Inc /= 2)
		for(i = Inc; i < m_Size; i++)
		{
			Tmp = m_pData[i];
			for(j = i; j >= Inc; j -= Inc)
				if(Tmp < m_pData[j-Inc])
					m_pData[j] = m_pData[j-Inc];
				else
					break;
			m_pData[j] = Tmp;
		}
}