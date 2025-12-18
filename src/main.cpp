#include <iostream>
#include "queue.h"

int main()
{
    std::cout << "Демонстрация QueueArray\n";
    QueueArray<int> qa(5);
    qa.Put(10);
    qa.Put(20);
    qa.Put(30);
    std::cout << qa << "\n"; 

    std::cout << "Get: " << qa.Get() << "\n";  // 10
    std::cout << qa << "\n\n";

    std::cout << "Демонстрация QueueList\n";
    QueueList<int> ql;
    ql.Put(100);
    ql.Put(200);
    ql.Put(300);
    std::cout << ql << "\n"; 

    std::cout << "Get: " << ql.Get() << "\n";  // 100
    std::cout << ql << "\n";

    return 0;
}