// Type your code here, or load an example.
#include <iostream>
#include <string>

#define EXAMPLE 7

static auto numCopies = 0;

struct DataEngine {
    DataEngine(std::string_view s) : s(s) { std::cout<<"DataEngine "<<s<<"\n"; }

#if EXAMPLE == 1 || EXAMPLE == 2 || EXAMPLE == 3 || EXAMPLE == 4 || EXAMPLE == 5 || EXAMPLE == 6
    DataEngine& updateEngine(std::string_view newS) {
        s = s.append(newS);
        return *this;
    }

    auto doCalculation() const {
        return s.size();
    }
#else
    DataEngine& updateEngine(std::string_view newS) & {
        s = s.append(newS);
        return *this;
    }

    DataEngine&& updateEngine(std::string_view newS) && {
        s = s.append(newS);
        return std::move(*this);
    }

    auto doCalculation() const& {
        std::cout<<"Doing slow version\n";
        return s.size();
    }
    auto doCalculation() && {
        std::cout<<"Doing fast version\n";
        return s.size();
    }
#endif

    DataEngine(DataEngine const& other) : s(other.s) { ++numCopies; }
    DataEngine(DataEngine&& other) : s(std::move(other.s)) {}
    DataEngine& operator=(DataEngine const& other) { s = other.s; ++numCopies; return *this; }
    DataEngine& operator=(DataEngine&& other) { s = std::move(other.s); return *this; }

    std::string s;
};

std::ostream& operator<<(std::ostream& os, DataEngine const& f) {
    return os<<f.s;
}

template <typename T>
class Gated {
public:
    Gated(T&& t) : thing(std::forward<T>(t)) {}

#if EXAMPLE == 1 || EXAMPLE == 2
    T access()
    {
        return thing;
    }
#elif EXAMPLE == 3 || EXAMPLE == 4
    T& access()
    {
        return thing;
    }
#elif EXAMPLE == 5 || EXAMPLE == 6 || EXAMPLE == 7
    T& access()
    {
        return thing;
    }

    T const& access() const
    {
       return thing;
    }
#elif EXAMPLE == 8
    T& access()&
    {
        return thing;
    }

    T const& access() const&
    {
       return thing;
    }

    T&& access() &&
    {
        return std::move(thing);
    }

#elif EXAMPLE == 9
    template <typename Self>
    auto&& access(this Self&& self)
    {
        return std::forward<T>(self.thing);
    }
#else
    auto&& access(this auto&& self)
    {
        return std::forward<T>(self.thing);
    }
#endif 

private:
    T thing;
    bool movedFrom = 0;
};

template<typename T, typename... Args>
static auto make_gated(Args&&... args) { return Gated<T>(T(std::forward<Args>(args)...)); }

DataEngine CreateDataEngine() {
    return DataEngine("Create");
}

int main() {
#if EXAMPLE == 1

    auto a = make_gated<DataEngine>("hello");
    std::cout<<"a value = "<<a.access().doCalculation()<<"\n";

#elif EXAMPLE == 2

    auto a = make_gated<DataEngine>("hello");
    std::cout<<"a value = "<<a.access().doCalculation()<<"\n";
    a.access().updateEngine(" world");
    std::cout<<"a value = "<<a.access().doCalculation()<<"\n";

#elif EXAMPLE == 3

    auto a = make_gated<DataEngine>("hello");
    std::cout<<"a value = "<<a.access().doCalculation()<<"\n";
    a.access().updateEngine(" world");
    std::cout<<"a value = "<<a.access().doCalculation()<<"\n";

#elif EXAMPLE == 4 || EXAMPLE == 5

    auto const b = make_gated<DataEngine const>("constant");
    std::cout<<"b value = "<<b.access().doCalculation()<<"\n";

#elif EXAMPLE == 6

    auto const b = make_gated<DataEngine const>("constant");
    std::cout<<"b value = "<<b.access().doCalculation()<<"\n";
    b.access().updateEngine(" world");

#elif EXAMPLE == 7 ||  EXAMPLE == 8

    auto c = CreateDataEngine().updateEngine("Work").doCalculation();
    std::cout<<"c value = "<<c<<"\n";
    auto d = Gated(CreateDataEngine()).access().updateEngine("Work").doCalculation();
    std::cout<<"d value = "<<d<<"\n";

#elif

    auto a = make_gated<DataEngine>("hello");
    std::cout<<"a value = "<<a.access().doCalculation()<<"\n";
    a.access().updateEngine(" world");
    std::cout<<"a value = "<<a.access().doCalculation()<<"\n";

    auto b = make_gated<DataEngine const>("constant");
    std::cout<<"b value = "<<b.access().doCalculation()<<"\n";

    auto c = CreateDataEngine().updateEngine("Work").doCalculation();
    std::cout<<"c value = "<<c<<"\n";
    auto d = Gated(CreateDataEngine()).access().updateEngine("Work").doCalculation();
    std::cout<<"d value = "<<d<<"\n";

#endif 

    std::cout<<"number copies "<<numCopies<<"\n";
    return 0;
}