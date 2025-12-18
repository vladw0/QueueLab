#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include "queue.h"

// Вспомогательная функция для захвата исключений const char*
TEST(QueueExceptions, ThrowConstChar)
{
    //Проверка что throw работает
}

//QueueArray

TEST(QueueArrayTest, DefaultConstructor)
{
    QueueArray<int> q;
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_FALSE(q.IsFull());
    //EXPECT_EQ(q.getCurrentSize(), 0);
    //EXPECT_EQ(q.getCapacity(), 10); ?
}

TEST(QueueArrayTest, ConstructorWithCapacity)
{
    QueueArray<int> q(7);
    EXPECT_TRUE(q.IsEmpty());
    //EXPECT_EQ(q.getCapacity(), 7); ?
}

TEST(QueueArrayTest, PutAndGet)
{
    QueueArray<int> q(4);
    q.Put(1);
    q.Put(2);
    q.Put(3);

    EXPECT_EQ(q.Get(), 1);
    EXPECT_EQ(q.Get(), 2);
    EXPECT_EQ(q.Get(), 3);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(QueueArrayTest, CircularBehavior)
{
    QueueArray<int> q(3);
    q.Put(10);
    q.Put(20);
    q.Get();  //удаляем 10
    q.Put(30);
    q.Put(40);  //теперь rear должен обойти круг

    EXPECT_EQ(q.Get(), 20);
    EXPECT_EQ(q.Get(), 30);
    EXPECT_EQ(q.Get(), 40);
}

TEST(QueueArrayTest, IsFullAndPutException)
{
    QueueArray<int> q(2);
    q.Put(1);
    q.Put(2);
    EXPECT_TRUE(q.IsFull());

    EXPECT_THROW(q.Put(3), const char*);
}

TEST(QueueArrayTest, GetFromEmptyException)
{
    QueueArray<int> q;
    EXPECT_THROW(q.Get(), const char*);
}

TEST(QueueArrayTest, CopyConstructor)
{
    QueueArray<int> original(4);
    original.Put(5);
    original.Put(10);

    QueueArray<int> copy(original);
    EXPECT_EQ(copy, original);
    EXPECT_EQ(copy.Get(), 5);
    EXPECT_EQ(copy.Get(), 10);
}

TEST(QueueArrayTest, MoveConstructor)
{
    QueueArray<int> original(4);
    original.Put(7);
    original.Put(14);

    QueueArray<int> moved(std::move(original));
    EXPECT_TRUE(original.IsEmpty());  //оригинал пустой после перемещения
    EXPECT_EQ(moved.Get(), 7);
    EXPECT_EQ(moved.Get(), 14);
}

TEST(QueueArrayTest, AssignmentOperatorCopy)
{
    QueueArray<int> a(3);
    a.Put(100);
    QueueArray<int> b;
    b = a;

    EXPECT_EQ(b.Get(), 100);
}

TEST(QueueArrayTest, AssignmentOperatorMove)
{
    QueueArray<int> a(3);
    a.Put(200);
    QueueArray<int> b;
    b = std::move(a);

    EXPECT_TRUE(a.IsEmpty());
    EXPECT_EQ(b.Get(), 200);
}

TEST(QueueArrayTest, EqualityAndInequality)
{
    QueueArray<int> q1(3);
    q1.Put(1); q1.Put(2);
    QueueArray<int> q2(3);
    q2.Put(1); q2.Put(2);

    EXPECT_TRUE(q1 == q2);
    EXPECT_FALSE(q1 != q2);

    q2.Put(3);
    EXPECT_FALSE(q1 == q2);
}

TEST(QueueArrayTest, StreamOutput)
{
    QueueArray<int> q(4);
    q.Put(11); q.Put(22);

    std::ostringstream oss;
    oss << q;
    EXPECT_EQ(oss.str(), "QueueArray: 11 22 ");
}

/*TEST(QueueArrayTest, StreamInput)
{
    QueueArray<int> q(5);
    std::istringstream iss("3 6 9");
    iss >> q;

    EXPECT_EQ(q.Get(), 3);
    EXPECT_EQ(q.Get(), 6);
    EXPECT_EQ(q.Get(), 9);
}*/

//QueueList

TEST(QueueListTest, DefaultConstructor)
{
    QueueList<int> q;
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_FALSE(q.IsFull());
    EXPECT_EQ(q.getCurrentSize(), 0);
}

TEST(QueueListTest, PutAndGet)
{
    QueueList<int> q;
    q.Put(100);
    q.Put(200);
    q.Put(300);

    EXPECT_EQ(q.Get(), 100);
    EXPECT_EQ(q.Get(), 200);
    EXPECT_EQ(q.Get(), 300);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(QueueListTest, NeverFull)
{
    QueueList<int> q;
    for (int i = 0; i < 1000; ++i)
    {
        q.Put(i);
    }
    EXPECT_FALSE(q.IsFull());
}

TEST(QueueListTest, GetFromEmptyException)
{
    QueueList<int> q;
    EXPECT_THROW(q.Get(), const char*);
}

TEST(QueueListTest, CopyConstructor)
{
    QueueList<int> original;
    original.Put(5);
    original.Put(10);

    QueueList<int> copy(original);
    EXPECT_EQ(copy, original);
    EXPECT_EQ(copy.Get(), 5);
    EXPECT_EQ(copy.Get(), 10);
}

TEST(QueueListTest, MoveConstructor)
{
    QueueList<int> original;
    original.Put(7);
    original.Put(14);

    QueueList<int> moved(std::move(original));
    EXPECT_TRUE(original.IsEmpty());
    EXPECT_EQ(moved.Get(), 7);
    EXPECT_EQ(moved.Get(), 14);
}

TEST(QueueListTest, AssignmentOperatorCopy)
{
    QueueList<int> a;
    a.Put(100);
    QueueList<int> b;
    b = a;

    EXPECT_EQ(b.Get(), 100);
}

TEST(QueueListTest, AssignmentOperatorMove)
{
    QueueList<int> a;
    a.Put(200);
    QueueList<int> b;
    b = std::move(a);

    EXPECT_TRUE(a.IsEmpty());
    EXPECT_EQ(b.Get(), 200);
}

TEST(QueueListTest, EqualityAndInequality)
{
    QueueList<int> q1;
    q1.Put(1); q1.Put(2);
    QueueList<int> q2;
    q2.Put(1); q2.Put(2);

    EXPECT_TRUE(q1 == q2);
    EXPECT_FALSE(q1 != q2);

    q2.Put(3);
    EXPECT_FALSE(q1 == q2);
}

TEST(QueueListTest, StreamOutput)
{
    QueueList<int> q;
    q.Put(11); q.Put(22);

    std::ostringstream oss;
    oss << q;
    std::string out = oss.str();
    EXPECT_NE(out.find("11"), std::string::npos);
    EXPECT_NE(out.find("22"), std::string::npos);
}

TEST(QueueListTest, StreamInput)
{
    QueueList<int> q;
    std::istringstream iss("30 60 90");
    iss >> q;

    EXPECT_EQ(q.Get(), 30);
    EXPECT_EQ(q.Get(), 60);
    EXPECT_EQ(q.Get(), 90);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}