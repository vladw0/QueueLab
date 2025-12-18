#pragma once
#include <iostream>

template <class T>
class TArrList {
protected:
    T* dataArray;
    int* nextIndexArray;
    int* prevIndexArray;

    int allocatedSize;
    int elementCount;

    int headIndex;
    int tailIndex;
    int freeListHeadIndex;

public:
    //Конструкторы и деструктор
    TArrList();
    TArrList(int initialSize);

    TArrList(const TArrList& other);
    TArrList(TArrList&& other);
    ~TArrList();

    int GetFreeNodeIndex();
    void FreeNodeIndex(int index);

    //Операции за O(1)
    void PushStart(const T& value);
    void PushEnd(const T& value);

    void PopStart();
    void PopEnd();

    void InsertAfter(int index, const T& value);
    void Remove(int index);

    bool IsEmpty() const;
    bool IsFull() const;

    //Операции за O(n)
    void Print() const;
    int  operator[](int position) const;
    void Find(const T& value) const;
    int  Count(const T& value) const;
    void FindAll(const T& value) const;

    //Resize
    void Resize(int newSize);

    //Операторы
    TArrList& operator=(const TArrList& other);
    TArrList& operator=(TArrList&& other);

    bool operator==(const TArrList& other) const;
    bool operator!=(const TArrList& other) const;

    TArrList operator+(const TArrList& other) const;

    template <class U>
    friend std::ostream& operator<<(std::ostream& out, const TArrList<U>& list);

    template <class U>
    friend std::istream& operator>>(std::istream& in, TArrList<U>& list);

    //Итератор
    class Iterator {
        int currentIndex;
        int* nextArrayPointer;
        T* dataArrayPointer;

    public:
        Iterator(int startIndex, int* nextPtr, T* dataPtr)
        {
            currentIndex = startIndex;
            nextArrayPointer = nextPtr;
            dataArrayPointer = dataPtr;
        }

        bool operator!=(const Iterator& other) const
        {
            return currentIndex != other.currentIndex;
        }

        T& operator*()
        {
            return dataArrayPointer[currentIndex];
        }

        T* operator->()
        {
            return &dataArrayPointer[currentIndex];
        }

        Iterator& operator++()
        {
            currentIndex = nextArrayPointer[currentIndex];
            return *this;
        }
    };

    Iterator begin()
    {
        return Iterator(headIndex, nextIndexArray, dataArray);
    }

    Iterator end()
    {
        return Iterator(-1, nextIndexArray, dataArray);
    }
};

//реализация
template <class T>
TArrList<T>::TArrList()
{
    allocatedSize = 16;
    elementCount = 0;

    dataArray = new T[allocatedSize];
    nextIndexArray = new int[allocatedSize];
    prevIndexArray = new int[allocatedSize];

    for (int i = 0; i < allocatedSize - 1; i++)
    {
        nextIndexArray[i] = i + 1;
    }
    nextIndexArray[allocatedSize - 1] = -1;

    headIndex = -1;
    tailIndex = -1;
    freeListHeadIndex = 0;
}

template <class T>
TArrList<T>::TArrList(int initialSize)
{
    if (initialSize <= 0)
    {
        initialSize = 16;
    }

    allocatedSize = initialSize;
    elementCount = 0;

    dataArray = new T[allocatedSize];
    nextIndexArray = new int[allocatedSize];
    prevIndexArray = new int[allocatedSize];

    for (int i = 0; i < allocatedSize - 1; i++)
    {
        nextIndexArray[i] = i + 1;
    }
    nextIndexArray[allocatedSize - 1] = -1;

    headIndex = -1;
    tailIndex = -1;
    freeListHeadIndex = 0;
}

template <class T>
TArrList<T>::TArrList(const TArrList& other)
{
    allocatedSize = other.allocatedSize;
    elementCount = other.elementCount;

    dataArray = new T[allocatedSize];
    nextIndexArray = new int[allocatedSize];
    prevIndexArray = new int[allocatedSize];

    for (int i = 0; i < allocatedSize; i++)
    {
        dataArray[i] = other.dataArray[i];
        nextIndexArray[i] = other.nextIndexArray[i];
        prevIndexArray[i] = other.prevIndexArray[i];
    }

    headIndex = other.headIndex;
    tailIndex = other.tailIndex;
    freeListHeadIndex = other.freeListHeadIndex;
}

template <class T>
TArrList<T>::TArrList(TArrList&& other)
{
    dataArray = other.dataArray;
    nextIndexArray = other.nextIndexArray;
    prevIndexArray = other.prevIndexArray;

    allocatedSize = other.allocatedSize;
    elementCount = other.elementCount;

    headIndex = other.headIndex;
    tailIndex = other.tailIndex;
    freeListHeadIndex = other.freeListHeadIndex;

    other.dataArray = nullptr;
    other.nextIndexArray = nullptr;
    other.prevIndexArray = nullptr;
    other.allocatedSize = 0;
    other.elementCount = 0;
    other.headIndex = -1;
    other.tailIndex = -1;
    other.freeListHeadIndex = -1;
}

template <class T>
TArrList<T>::~TArrList()
{
    delete[] dataArray;
    delete[] nextIndexArray;
    delete[] prevIndexArray;
}


template <class T>
int TArrList<T>::GetFreeNodeIndex()
{
    if (freeListHeadIndex == -1)
    {
        int newSize = allocatedSize * 2;
        Resize(newSize);
    }

    int index = freeListHeadIndex;
    freeListHeadIndex = nextIndexArray[index];
    return index;
}

template <class T>
void TArrList<T>::FreeNodeIndex(int index)
{
    nextIndexArray[index] = freeListHeadIndex;
    prevIndexArray[index] = -1;
    freeListHeadIndex = index;
}

//вставка/удаление
template <class T>
void TArrList<T>::PushStart(const T& value)
{
    int newIndex = GetFreeNodeIndex();

    dataArray[newIndex] = value;
    prevIndexArray[newIndex] = -1;
    nextIndexArray[newIndex] = headIndex;

    if (headIndex != -1)
    {
        prevIndexArray[headIndex] = newIndex;
    }
    else
    {
        tailIndex = newIndex;
    }

    headIndex = newIndex;
    elementCount++;
}

template <class T>
void TArrList<T>::PushEnd(const T& value)
{
    int newIndex = GetFreeNodeIndex();

    dataArray[newIndex] = value;
    nextIndexArray[newIndex] = -1;
    prevIndexArray[newIndex] = tailIndex;

    if (tailIndex != -1)
    {
        nextIndexArray[tailIndex] = newIndex;
    }
    else
    {
        headIndex = newIndex;
    }

    tailIndex = newIndex;
    elementCount++;
}

template <class T>
void TArrList<T>::PopStart()
{
    if (headIndex == -1)
    {
        return;
    }

    int removeIndex = headIndex;
    headIndex = nextIndexArray[removeIndex];

    if (headIndex != -1)
    {
        prevIndexArray[headIndex] = -1;
    }
    else
    {
        tailIndex = -1;
    }

    FreeNodeIndex(removeIndex);
    elementCount--;
}

template <class T>
void TArrList<T>::PopEnd()
{
    if (tailIndex == -1)
    {
        return;
    }

    int removeIndex = tailIndex;
    tailIndex = prevIndexArray[removeIndex];

    if (tailIndex != -1)
    {
        nextIndexArray[tailIndex] = -1;
    }
    else
    {
        headIndex = -1;
    }

    FreeNodeIndex(removeIndex);
    elementCount--;
}

template <class T>
void TArrList<T>::InsertAfter(int index, const T& value)
{
    if (index < 0 || index >= allocatedSize)
    {
        return;
    }

    int newIndex = GetFreeNodeIndex();

    dataArray[newIndex] = value;
    nextIndexArray[newIndex] = nextIndexArray[index];
    prevIndexArray[newIndex] = index;

    if (nextIndexArray[index] != -1)
    {
        prevIndexArray[nextIndexArray[index]] = newIndex;
    }
    else
    {
        tailIndex = newIndex;
    }

    nextIndexArray[index] = newIndex;
    elementCount++;
}

template <class T>
void TArrList<T>::Remove(int index)
{
    if (index < 0 || index >= allocatedSize)
    {
        return;
    }

    int leftIndex = prevIndexArray[index];
    int rightIndex = nextIndexArray[index];

    if (leftIndex != -1)
    {
        nextIndexArray[leftIndex] = rightIndex;
    }
    else
    {
        headIndex = rightIndex;
    }

    if (rightIndex != -1)
    {
        prevIndexArray[rightIndex] = leftIndex;
    }
    else
    {
        tailIndex = leftIndex;
    }

    FreeNodeIndex(index);
    elementCount--;
}

//состояния 
template <class T>
bool TArrList<T>::IsEmpty() const
{
    return elementCount == 0;
}

template <class T>
bool TArrList<T>::IsFull() const
{
    return freeListHeadIndex == -1;
}

//операции за O(n)
template <class T>
void TArrList<T>::Print() const
{
    std::cout << "[ ";
    int currentIndex = headIndex;
    while (currentIndex != -1)
    {
        std::cout << dataArray[currentIndex] << " ";
        currentIndex = nextIndexArray[currentIndex];
    }
    std::cout << "]\n";
}

template <class T>
int TArrList<T>::operator[](int position) const
{
    if (position < 0 || position >= elementCount)
    {
        throw -1;
    }

    int currentIndex = headIndex;
    for (int i = 0; i < position; i++)
    {
        currentIndex = nextIndexArray[currentIndex];
    }

    return dataArray[currentIndex];
}

template <class T>
void TArrList<T>::Find(const T& value) const
{
    int currentIndex = headIndex;
    int position = 0;
    while (currentIndex != -1)
    {
        if (dataArray[currentIndex] == value)
        {
            std::cout << "Found at position " << position << "\n";
            return;
        }
        currentIndex = nextIndexArray[currentIndex];
        position++;
    }
    std::cout << "Not found\n";
}

template <class T>
int TArrList<T>::Count(const T& value) const
{
    int currentIndex = headIndex;
    int counter = 0;
    while (currentIndex != -1)
    {
        if (dataArray[currentIndex] == value)
        {
            counter++;
        }
        currentIndex = nextIndexArray[currentIndex];
    }
    return counter;
}

template <class T>
void TArrList<T>::FindAll(const T& value) const
{
    int currentIndex = headIndex;
    int position = 0;
    std::cout << "Positions: ";
    while (currentIndex != -1)
    {
        if (dataArray[currentIndex] == value)
        {
            std::cout << position << " ";
        }
        currentIndex = nextIndexArray[currentIndex];
        position++;
    }
    std::cout << "\n";
}

template <class T>
void TArrList<T>::Resize(int newSize)
{
    if (newSize <= allocatedSize)
    {
        return;
    }

    T* newDataArray = new T[newSize];
    int* newNextArray = new int[newSize];
    int* newPrevArray = new int[newSize];

    for (int i = 0; i < allocatedSize; i++)
    {
        newDataArray[i] = dataArray[i];
        newNextArray[i] = nextIndexArray[i];
        newPrevArray[i] = prevIndexArray[i];
    }

    for (int i = allocatedSize; i < newSize - 1; i++)
    {
        newNextArray[i] = i + 1;
    }
    newNextArray[newSize - 1] = freeListHeadIndex;

    freeListHeadIndex = allocatedSize;

    delete[] dataArray;
    delete[] nextIndexArray;
    delete[] prevIndexArray;

    dataArray = newDataArray;
    nextIndexArray = newNextArray;
    prevIndexArray = newPrevArray;
    allocatedSize = newSize;
}

template <class T>
TArrList<T>& TArrList<T>::operator=(const TArrList& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->~TArrList();
    new(this) TArrList(other);
    return *this;
}

template <class T>
TArrList<T>& TArrList<T>::operator=(TArrList&& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->~TArrList();
    new(this) TArrList(std::move(other));
    return *this;
}

template <class T>
bool TArrList<T>::operator==(const TArrList& other) const
{
    if (elementCount != other.elementCount)
    {
        return false;
    }

    int aIndex = headIndex;
    int bIndex = other.headIndex;

    while (aIndex != -1 && bIndex != -1)
    {
        if (dataArray[aIndex] != other.dataArray[bIndex])
        {
            return false;
        }
        aIndex = nextIndexArray[aIndex];
        bIndex = other.nextIndexArray[bIndex];
    }

    return true;
}

template <class T>
bool TArrList<T>::operator!=(const TArrList& other) const
{
    return !(*this == other);
}

template <class T>
TArrList<T> TArrList<T>::operator+(const TArrList& other) const
{
    TArrList<T> result(elementCount + other.elementCount + 4);

    int currentIndex = headIndex;
    while (currentIndex != -1)
    {
        result.PushEnd(dataArray[currentIndex]);
        currentIndex = nextIndexArray[currentIndex];
    }

    currentIndex = other.headIndex;
    while (currentIndex != -1)
    {
        result.PushEnd(other.dataArray[currentIndex]);
        currentIndex = other.nextIndexArray[currentIndex];
    }

    return result;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const TArrList<T>& list)
{
    out << "[ ";
    int currentIndex = list.headIndex;
    while (currentIndex != -1)
    {
        out << list.dataArray[currentIndex] << " ";
        currentIndex = list.nextIndexArray[currentIndex];
    }
    out << "]";
    return out;
}

template <class T>
std::istream& operator>>(std::istream& in, TArrList<T>& list)
{
    int n;
    in >> n;

    list = TArrList<T>(n + 4);

    for (int i = 0; i < n; i++)
    {
        T value;
        in >> value;
        list.PushEnd(value);
    }

    return in;
}