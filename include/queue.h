#pragma once

#include <iostream>
#include "ArrList.h" 

//Шаблонный класс QueueArray - очередь на фиксированном круговом массиве
template <typename T>
class QueueArray
{
private:
    T* dataArray; //Указатель на динамический массив, где хранятся элементы очереди
    int frontIndex; //Индекс начала очереди — отсюда берём элементы методом Get()
    int rearIndex; //Индекс конца очереди — сюда добавляем элементы методом Put()
    int capacity; //Максимальная ёмкость очереди
    int currentSize; //Текущее количество элементов в очереди

public:
    QueueArray() //Конструктор по умолчанию
    {
        capacity = 10;
        dataArray = new T[capacity];
        frontIndex = 0;
        rearIndex = 0;
        currentSize = 0;
    }

    explicit QueueArray(int initialCapacity)
    {
        if (initialCapacity <= 0)
        {
            throw "Capacity must be positive";
        }
        capacity = initialCapacity;
        dataArray = new T[capacity];
        frontIndex = 0;
        rearIndex = 0;
        currentSize = 0;
    }

    QueueArray(const QueueArray& other) //Конструктор копирования
    {
        capacity = other.capacity;
        dataArray = new T[capacity];
        frontIndex = other.frontIndex;
        rearIndex = other.rearIndex;
        currentSize = other.currentSize;

        int index = other.frontIndex;
        int count = 0;
        while (count < currentSize)
        {
            dataArray[index % capacity] = other.dataArray[index % capacity];
            index = index + 1;
            count = count + 1;
        }
    }

    QueueArray(QueueArray&& other) //Конструктор перемещения
    {
        dataArray = other.dataArray;
        frontIndex = other.frontIndex;
        rearIndex = other.rearIndex;
        capacity = other.capacity;
        currentSize = other.currentSize;

        other.dataArray = nullptr;
        other.frontIndex = 0;
        other.rearIndex = 0;
        other.capacity = 0;
        other.currentSize = 0;
    }

    ~QueueArray() //Деструктор
    {
        if (dataArray != nullptr)
        {
            delete[] dataArray;
        }
    }

    QueueArray& operator=(const QueueArray& other) //Оператор присваивания для копирование
    {
        if (this == &other)
        {
            return *this;
        }

        if (dataArray != nullptr)
        {
            delete[] dataArray;
        }

        capacity = other.capacity;
        dataArray = new T[capacity];
        frontIndex = other.frontIndex;
        rearIndex = other.rearIndex;
        currentSize = other.currentSize;

        int index = other.frontIndex;
        int count = 0;
        while (count < currentSize)
        {
            dataArray[index % capacity] = other.dataArray[index % capacity];
            index = index + 1;
            count = count + 1;
        }

        return *this;
    }

    QueueArray& operator=(QueueArray&& other) //Оператор присваивания для перемещения
    {
        if (this == &other)
        {
            return *this;
        }

        if (dataArray != nullptr)
        {
            delete[] dataArray;
        }

        dataArray = other.dataArray;
        frontIndex = other.frontIndex;
        rearIndex = other.rearIndex;
        capacity = other.capacity;
        currentSize = other.currentSize;

        other.dataArray = nullptr;
        other.frontIndex = 0;
        other.rearIndex = 0;
        other.capacity = 0;
        other.currentSize = 0;

        return *this;
    }

    bool operator==(const QueueArray& other) const //Оператор сравнения
    {
        if (currentSize != other.currentSize)
        {
            return false;
        }

        int thisIndex = frontIndex;
        int otherIndex = other.frontIndex;
        int count = 0;
        while (count < currentSize)
        {
            if (dataArray[thisIndex % capacity] != other.dataArray[otherIndex % other.capacity])
            {
                return false;
            }
            thisIndex = thisIndex + 1;
            otherIndex = otherIndex + 1;
            count = count + 1;
        }
        return true;
    }

    bool operator!=(const QueueArray& other) const //Оператор неравенства
    {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const QueueArray& q) //Оператор вывода
    {
        os << "QueueArray: ";
        if (q.currentSize == 0)
        {
            os << "empty";
            return os;
        }

        int index = q.frontIndex;
        int count = 0;
        while (count < q.currentSize)
        {
            os << q.dataArray[index % q.capacity] << " ";
            index = index + 1;
            count = count + 1;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, QueueArray& q) //Оператор ввода
    {
        while (q.currentSize > 0)
        {
            q.Get();
        }

        T value;
        while (is >> value)
        {
            q.Put(value);
        }
        return is;
    }

    void Put(const T& value) //Метод добавления элемента в конец очереди (enqueue)
    {
        if (currentSize == capacity)
        {
            throw "Queue is full";
        }

        dataArray[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity;
        currentSize = currentSize + 1;
    }

    T Get() //Метод извлечения элемента из начала очереди (dequeue)
    {
        if (currentSize == 0)
        {
            throw "Queue is empty";
        }

        T value = dataArray[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        currentSize = currentSize - 1;
        return value;
    }

    bool IsEmpty() const 
    {
        return currentSize == 0;
    }

    bool IsFull() const
    {
        return currentSize == capacity;
    }

    int getCurrentSize() const { return currentSize; }  //Геттер для получения текущего размера
};

// QueueList — очередь на базе TArrList<T>
template <typename T>
class QueueList
{
private:
    TArrList<T> storageList;  //Внутреннее хранилище

public:
    QueueList(){} //Конструктор по умолчанию

    QueueList(const QueueList& other) //Конструктор копирования
    {
        storageList = other.storageList;
    }

    QueueList(QueueList&& other) //Конструктор перемещения
    {
        storageList = std::move(other.storageList);
    }

    ~QueueList(){} //Деструктор

    QueueList& operator=(const QueueList& other)
    {
        if (this == &other)
        {
            return *this;
        }
        storageList = other.storageList;
        return *this;
    }

    QueueList& operator=(QueueList&& other)
    {
        if (this == &other)
        {
            return *this;
        }
        storageList = std::move(other.storageList);
        return *this;
    }

    bool operator==(const QueueList& other) const
    {
        return storageList == other.storageList;
    }

    bool operator!=(const QueueList& other) const
    {
        return !(storageList == other.storageList);
    }

    friend std::ostream& operator<<(std::ostream& os, const QueueList& q)
    {
        os << "QueueList: ";
        if (q.storageList.IsEmpty())
        {
            os << "empty";
        }
        else
        {
            os << q.storageList;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, QueueList& q)
    {
        while (!q.storageList.IsEmpty())
        {
            q.storageList.PopStart(); //Очищение очереди
        }

        is >> q.storageList;

        return is;
    }

    void Put(const T& value)
    {
        storageList.PushEnd(value);  //Добавление в конец
    }

    T Get()
    {
        if (storageList.IsEmpty())
        {
            throw "Queue is empty";
        }

        T value = storageList[0];
        storageList.PopStart(); //Удаление первого элемента
        return value;
    }

    bool IsEmpty() const
    {
        return storageList.IsEmpty();
    }

    bool IsFull() const
    {
        return storageList.IsFull();  
    }

    int getCurrentSize() const //Геттер размера
    {
        return -1;
    }
};
