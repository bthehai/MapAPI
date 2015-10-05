#pragma once

//typedef unsigned int int8u;

template<class T>
class MemoryBuffer
{
public:
	MemoryBuffer(int growby = 10);
	MemoryBuffer(void* pv, int n);
	MemoryBuffer(const MemoryBuffer&);
	virtual ~MemoryBuffer();

public:
	//	Accessors
	int getSize() const;
	int getCapacity() const;
	bool setSize(int);
	bool setCapacity(int);
	T* getBuffer() const;
	T* getBuffer();
	bool setBuffer(void* pv, int n);
	bool isEmpty() const;
	T& getAt(int i) const;
	T& elementAt(int i);
	bool setAt(const T& val, int i);
	int getUpperBound() const;
	void freeExtra();

	//	Other operations
	MemoryBuffer& operator=(const MemoryBuffer&);
	T& operator[](int i);
	const T& operator[](int i) const;
	MemoryBuffer& operator+=(const MemoryBuffer& src);
	void empty();
	void copy(const MemoryBuffer& src);
	bool deleteAt(int i, int n = 1);
	bool insertAt(const T& v, int i);
	int add(const T&);
	int append(const MemoryBuffer&);	
	int find(const T&) const;
	

public:
	// Insert functions
	int insertAt(int position, const void *buf, int bytes);
	int insertAt(int position, const MemoryBuffer &buf)
	{
		return insertAt(position, (const void *)buf.m_pData, buf.m_Size);
	}

	// Replace functions
	int replaceAt(int position, const void *buf, int bytes);
	int replaceAt(int position, const MemoryBuffer &buf)
	{
		return replaceAt(position, (const void *)buf.m_pData, buf.m_Size);
	}

	// Search functions
	int find(void *buf, int offset = 0); 
	int find(const void *buf, int bytes, int offset = 0) const;
	int find(MemoryBuffer &buf, int offset = 0)
	{
		return find(buf.m_pData, buf.m_Size, offset); 
	}
	int find(const MemoryBuffer &buf, int offset = 0) const
	{
		return find(buf.m_pData, buf.m_Size, offset); 
	}

	// Overloaded operators
	int operator!()
	{
		return ((m_pData == NULL) || (m_Size == 0));
	} 
	int operator!() const
	{
		return ((m_pData == NULL) || (m_Size == 0));
	}
	friend MemoryBuffer operator+(const MemoryBuffer &a, const MemoryBuffer &b);
	friend int operator==(const MemoryBuffer &a, const MemoryBuffer &b)
	{
		return bufferCompare(a, b) == 0;
	}
	friend int operator!=(const MemoryBuffer &a, const MemoryBuffer &b)
	{
		return bufferCompare(a, b) != 0;
	}
	friend int operator>(const MemoryBuffer &a, const MemoryBuffer &b)
	{
		return bufferCompare(a, b) > 0;
	}
	friend int operator>=(const MemoryBuffer &a, const MemoryBuffer &b)
	{
		return bufferCompare(a, b) >= 0;
	}
	friend int operator<(const MemoryBuffer &a, const MemoryBuffer &b)
	{
		return bufferCompare(a, b) < 0;
	}
	friend int operator<=(const MemoryBuffer &a, const MemoryBuffer &b)
	{
		return bufferCompare(a, b) <= 0;
	}

	friend int bufferCompare(const MemoryBuffer &a,  const MemoryBuffer &b);  

protected:
	//	Internal functions
	virtual bool grow(int);

	//	Data members
	int m_Size;		// number of elements added so far
	int m_Capacity;	// maximum number of elements we have room for
	int m_GrowBy;
	T* m_pData;		// data area for array
};

typedef MemoryBuffer<short> ShortMBuffer;
typedef MemoryBuffer<byte> ByteMBuffer;
typedef MemoryBuffer<int> IntMBuffer;
typedef MemoryBuffer<float> FloatMBuffer;

template<class T>
MemoryBuffer<T>::MemoryBuffer(int growby)
{
	m_Size = 0;				// empty to start
	m_Capacity = 0;			// remember the size
	m_GrowBy = growby;
	m_pData = NULL;
}

template<class T>
MemoryBuffer<T>::MemoryBuffer(void* pv, int n)
{
	m_Capacity = n;
	m_Size = n;
	m_GrowBy = 10;
	m_pData = new T[m_Capacity];
	memcpy(m_pData, pv, sizeof(T)*m_Size);
}

template<class T>
MemoryBuffer<T>::MemoryBuffer(const MemoryBuffer& a)
{
	if(this == &a)
		return;

	m_Size = 0;		// empty to start
	m_Capacity = 0;	// remember the size
	m_GrowBy = 10;
	m_pData = NULL;
	append(a);		// copy each element from the given array
}

template<class T>
MemoryBuffer<T>::~MemoryBuffer()
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

template<class T>
bool MemoryBuffer<T>::grow(int growto)
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

template<class T>
T* MemoryBuffer<T>::getBuffer() const
{
	return m_pData;
}

template<class T>
T* MemoryBuffer<T>::getBuffer()
{
	return m_pData;
}

template<class T>
bool MemoryBuffer<T>::setBuffer(void* pv, int n)
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

template<class T>
bool MemoryBuffer<T>::setCapacity(int s)
{
	if (s > m_Capacity)			// enlarge array
		return grow(s);			// if we can
	return true;
}

template<class T>
int MemoryBuffer<T>::getCapacity() const
{
	return m_Capacity;
}

template<class T>
bool MemoryBuffer<T>::setSize(int s)
{
	if (s > m_Capacity)
		if (!grow(s))
			return false;
	m_Size = s;
	return true;
}

template<class T>
int MemoryBuffer<T>::getSize() const
{
	return m_Size;
}

template<class T>
bool MemoryBuffer<T>::setAt(const T& val, int i)
{
	if (i >= m_Capacity)			// need to extend array?
	{
		int n = m_Capacity;
		n += m_GrowBy;			// grow to 1 1/2 times current size
		if (n <= i)
			n = i+1;	// unless user wants more
		if (!grow(n))			// extend failure
			return false;
	}
	m_pData[i] = val;
	if (i >= m_Size)			// enlarge array size if at end
		m_Size = i+1;
	return true;
}

template<class T>
T& MemoryBuffer<T>::getAt(int i) const
{
	return m_pData[i];
}

template<class T>
T& MemoryBuffer<T>::elementAt(int i)
{
	return m_pData[i];
}

template<class T>
const T& MemoryBuffer<T>::operator[](int i) const
{
	return m_pData[i];
}

template<class T>
T& MemoryBuffer<T>::operator[](int i)
{
	return m_pData[i];
}

template<class T>
int MemoryBuffer<T>::find(const T& elem) const
{
	const T* p = m_pData;

	for (int i = 0; i < m_Size; ++i)	// look for matching element
		if (*p++ == elem)
			return i;					// found it
	return -1;
}

template<class T>
int MemoryBuffer<T>::add(const T& v)
{
	int	index = m_Size;

	if (!setAt(v, index))
		return -1;
	return index;
}

template<class T>
bool MemoryBuffer<T>::deleteAt(int i, int n)
{
	T* elem;
	int	shift;

	if ((i < 0) || (i >= m_Size))		// element out of range?
		return false;
	if (n == 0)
		n = 1;					// default is one element
	shift = m_Size-(i+n);			// number to shuffle up
	elem = m_pData+i;
	memcpy(elem, elem+n, sizeof(T)*shift);
	m_Size -= n;
	return true;
}

template<class T>
bool MemoryBuffer<T>::insertAt(const T& v, int i)
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

template<class T>
int MemoryBuffer<T>::append(const MemoryBuffer<T>& src)
{
	int	n = m_Size+src.m_Size;

	if (!grow(n))
		return -1;
	for (int i = 0; i < src.m_Size; ++i)
		m_pData[m_Size+i] = src.m_pData[i];
	m_Size += src.m_Size;
	return n - 1;
}

template<class T>
MemoryBuffer<T>& MemoryBuffer<T>::operator +=(const MemoryBuffer<T>& src)
{
	append(src);
	return *this;
}

template<class T>
void MemoryBuffer<T>::copy(const MemoryBuffer<T>& src)
{
	if (this == &src)
		return;
	m_Size = 0;
	m_Capacity = 0;
	if (m_pData)
		delete []m_pData;
	m_pData = NULL;
	append(src);
}

template<class T>
MemoryBuffer<T>& MemoryBuffer<T>::operator =(const MemoryBuffer<T>& src)
{
	copy(src);
	return *this;
}

template<class T>
void MemoryBuffer<T>::empty()
{
	m_Size = 0;
}

template<class T>
bool MemoryBuffer<T>::isEmpty() const
{
	return m_Size == 0;
}

template<class T>
int MemoryBuffer<T>::getUpperBound() const
{
	return m_Size-1;
}

template<class T>
void MemoryBuffer<T>::freeExtra()
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
MemoryBuffer<T> operator+(const MemoryBuffer<T> &a, const MemoryBuffer<T> &b)
{
	MemoryBuffer buf(a.getSize()+b.getSize());
	buf.append(a);
	buf.append(b);
	return buf;
}

template<class T>
int bufferCompare(const MemoryBuffer<T> &a,  const MemoryBuffer<T> &b)
{
	int an = a.m_Size;
	int bn = b.m_Size;
	int sn = (an < bn) ? an : bn;
	T *ap = (T*)a.m_pData;
	T *bp = (T*)b.m_pData;
	for(int i = 0; i < sn; i++)
	{
		if(*ap < *bp)
			return -1;
		if(*ap > *bp)
			return 1;
		ap++;
		bp++;
	}
	if(an == bn)
		return 0;
	if(an < bn)
		return -1;
	return 1;
}

template<class T>
int MemoryBuffer<T>::find(void *buf, int offset)
// Returns index of first occurrence of pattern void *buf.
// Returns -1 if the pattern is not found.
{
	if(!m_pData)
		return -1;

	T *start = m_pData+offset;          // Start of buffer data
	T *next = start;                   // Next buffer element
	T *pattern = (T*)buf; // Next pattern element
	int i = offset;                     // Next buffer element index

	while(i < m_Size && *pattern)
	{
		if (*next == *pattern)
		{
			pattern++;
			if(*pattern == 0)
				return i; // Pattern was found
			next++;
		}
		else
		{
			i++;
			start++;
			next = start;
			pattern = (T *)buf;
		}
	}
	return -1; // No match was found
}

template<class T>
int MemoryBuffer<T>::find(const void *buf, int bytes, int offset) const
// Returns index of first occurrence of pattern void *buf.
// Returns -1 if the pattern is not found.
{
	if(!m_pData)
		return -1;

	T *start = (T*)m_pData+offset; // Start of buf data
	T *next = start;                   // Next buffer element
	T *pattern = (T*)buf; // Next pattern element
	int i = offset, j = 1;              // Buffer and pattern indexes

	while(i < m_Size && j <= bytes)
	{
		if (*next == *pattern)
		{   // Matching character
			if(j == bytes)
				return i; // Entire match was found
			next++; pattern++; j++;
		}
		else
		{ // Try next spot in buffer
			i++;
			start++;
			next = start;
			pattern = (T*)buf;
			j = 1;
		}
	}
	return -1; // No match was found
}

template<class T>
int MemoryBuffer<T>::insertAt(int position, const void *buf, int bytes)
// Insert a specified number of bytes a the current position, keeping
// the current buffer intact. Returns the number of bytes inserted or
// zero if an error occurs.
{
	int old_length = m_Size; // Record the current buffer length

	// Ensure that there are enough bytes to hold the insertion
	if(!grow(bytes+m_Size))
		return 0;

	m_Size = bytes+m_Size;

	if(!m_pData)
		return (int)0;

	if(position < old_length)
	{ // Move the data in the middle of the buffer
		memmove(m_pData+position+bytes, m_pData+position, old_length-position);
	}

	if(position > m_Size)
		position = m_Size; // Stay in bounds
	memmove(m_pData+position, buf, bytes);
	return bytes;
}

template<class T>
int MemoryBuffer<T>::replaceAt(int position, const void *buf, int bytes)
// Replace a specified number of bytes at the specified position. Returns
// the number of bytes replaced or zero if an error occurs.
{
	if(position > m_Size)
		position = m_Size; // Stay in bounds
	int end = m_Size-position;
	int needed;
	if(bytes > end)
	{ // There are not enough bytes to hold the replacement
		needed = bytes-end;
		if(!grow(m_Size+needed))
			return 0;
		m_Size = m_Size+needed;
	}
	memmove(m_pData+position, buf, bytes);
	return bytes;
}