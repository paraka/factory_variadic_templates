#include <iostream>
#include <memory>
#include <string>

struct Base 
{
    virtual ~Base() = default;
};

struct Derived : public Base
{
    explicit Derived(int value) : value_(value) {}
    ~Derived() = default;

    int value_;
};

struct Other : public Base
{
    explicit Other(const std::string &foo, int bar = 3) : foo_(foo), bar_(bar) {}
    ~Other() = default;

    std::string foo_;
    int bar_;
};

struct Bar : public Base
{
    Bar(int a, int b) : a_(a), b_(b) {}
    ~Bar() = default;

    int a_;
    int b_;
};

/** 
 * Just a little printing metaprogramming stuff 
 * Applying CppCon 2016: Michał Dominiak “Variadic expansion in examples" 
 * https://www.youtube.com/watch?v=Os5YLB5D2BU 
 */

struct unit {};

struct swallow
{
    template <typename... Args>
    swallow(Args && ...) {}
};

template <typename... Args>
void print(Args... args)
{
    swallow{ (std::cout << args << ' ', unit{})... };
    std::cout << "\n";
}

/* End of print magic stuff */

/* The factory magic is here */
namespace Factory
{
    template <typename T, typename... Args> 
    std::unique_ptr<T> create_stuff(Args && ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

int main(int argc, char *argv[])
{
    auto ptr = Factory::create_stuff<Derived>(5);
    if (!ptr) throw std::bad_alloc();

    print("Derived: ", ptr->value_);

    auto ptr2 = Factory::create_stuff<Other>(std::string("foo"));
    if (!ptr2) throw std::bad_alloc();

    print("Other: ", ptr2->foo_, ptr2->bar_);

    auto ptr3 = Factory::create_stuff<Bar>(5, 7);
    if (!ptr3) throw std::bad_alloc();

    print("Bar: ", ptr3->a_, ptr3->b_);

    return 0;
}
