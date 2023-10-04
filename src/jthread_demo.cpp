#include <format>
#include <iostream>
#include <thread>
#include <string>
#include <string_view>

using namespace std;

void func1(int n)
{

//    cout << std::format("[{}] Pass by values\n", std::this_thread::get_id());
    cout << std::format("[{}] Pass by values\n", n);
//    cout <<  std::this_thread::get_id()<<endl;

}

void func2(int &n)
{
//    cout <<  std::this_thread::get_id()<<endl;
    cout << "Pass by reference\n";
    ++n;
}

class foo
{
public:

    void bar(int n)
    {
//        cout <<  std::this_thread::get_id()<<endl;
        cout << "Call member function\n";
    }
};

class bar
{
public:

    void operator()()
    {
//        cout <<  std::this_thread::get_id()<<endl;
        cout << "Call function operator\n";
    }
};

int main()
{
    int n = 10;
    foo f;
    bar b;
    jthread t1(func1, n + 10);
    jthread t2(func2, std::ref(n));
    jthread t2a(func2, std::ref(n));
    jthread t2b(std::move(t2a));
    jthread t3(&foo::bar, &f, n);
    jthread t4(b);

    t2.join();
    cout << "n = " << n << '\n';
    return 0;
}
