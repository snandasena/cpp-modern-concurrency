//
// Created by sajit on 05/11/2023.
//

#include <bits/stdc++.h>

using namespace std;

//future<int> async_algo()
//{
//    promise<int> p;
//    auto f = p.get_future();
//
//    auto t = thread([p = std::move(p)]()mutable
//                    {
//                        int ans = 10;
//                        p.set_value(ans);
//                    });
//    t.detach();
//    return f;
//}
//
//template<class Cont>
//future<int> async_algo(Cont c)
//{
//    promise<int> p;
//    auto f = p.get_future();
//
//    auto t = thread([p = std::move(p), c]()mutable
//                    {
//                        int ans = 10;
//                        p.set_value(c(ans));
//                    });
//    t.detach();
//    return f;
//}

//auto async_algo()
//{
//    return [](auto p)
//    {
//        auto t = thread{[p = std::move(p)]() mutable
//                        {
//                            int ans = 10;
//                            p.set_value(ans);
//                        }};
//
//        t.detach();
//    };
//}

auto then(auto task, auto func)
{
    return [=](auto p)
    {
        struct _promise
        {
            decltype(p) p_;
            decltype(func) fun_;

            void set_value(auto ...vs)
            {
                p_.set_value(func(vs...));
            }

            void set_exception(auto e)
            {
                p_.set_exception(e);
            }
        };

        task(_promise{p, func});
    };
}

struct sink
{
    void set_value(auto ...)
    {

    }

    void set_exception(auto)
    {
        std::terminate();
    }
};

template<class T>
struct _state
{
    mutex mtx;
    condition_variable cv;
    variant<monostate, exception_ptr, T> data;
};

template<class T>
struct _promise
{

    _state<T> *pst;

    template<int I>
    void _set(auto ...xs)
    {
        auto lk = unique_lock(pst->mtx);
        pst->data.template emplace<I>(xs ...);
        pst->cv.notify_one();
    }

    void set_value(auto... vs)
    {
        _set<2>(vs...);
    }

    void set_exception(auto e)
    {
        _set<1>(e);
    }

};


template<class T, class Task>
T sync_wait(Task task)
{
    _state<T> state;

    task(_promise<T>{&state});

    {
        auto lk = unique_lock{state.mtx};
        state.cv.wait(lk, [&state]()
        {
            return state.data.index() != 0;
        });
    }

    if (state.data.index() == 1)
    {
        rethrow_exception(get<1>(state.data));
    }

    return move(get<2>(state.data));
}

auto new_thread()
{
    return [](auto p)
    {
        auto t = thread([p = std::move(p)]()
                        {
                            p.set_value();
                        });

        t.detach();
    };
}

auto async_algo(auto task)
{
    return then(task, []()
    {
        return 10;
    })
}

int main()
{
//    auto f = async_algo();
    /*auto f2 = f.then([](int i)
                     {
                         return i + 10;
                     });*/

//    auto f = async_algo([](int i)
//                        {
//                            return i + 10;
//                        });
//    cout << f.get();
//
//    auto f = async_algo();
//    auto f2 = then(f, [](int i)
//    {
//        return i + 10;
//    });

//    auto f = async_algo();
//    auto f2 = then(f, [](int i)
//    {
//        return i + 10;
//    });
//
//    auto f3 = then(f2, [](int i)
//    {
//        cout << i << endl;
//    });
//
//    f3(sink{});

//    auto f = async_algo();
//    auto f2 = then(f, [](int i)
//    {
//        return i + 10;
//    });
//
//    cout << sync_wait<int>(f2);

    auto f = async_algo(new_thread());
    auto f2 = then(f, [](int i)
    {
        return i + 10;
    });

    auto res = sync_wait<int>(f2);

}