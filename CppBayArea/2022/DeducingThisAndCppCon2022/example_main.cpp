#include <iostream>
#include <string>

#define EXAMPLE 10

static auto numCopies = 0;

struct DataEngine {
    DataEngine(std::string_view s) : s(s) { std::cout<<"DataEngine "<<s<<"\n"; }

#if EXAMPLE == 1 || EXAMPLE == 2 || EXAMPLE == 3 || EXAMPLE == 4 || EXAMPLE == 5
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

    DataEngine(DataEngine const& other) : s(other.s) { std::cout<<"copy\n"; ++numCopies; }
    DataEngine(DataEngine&& other) : s(std::move(other.s)) {}
    DataEngine& operator=(DataEngine const& other) { s = other.s; ++numCopies; std::cout<<"copy2\n"; return *this; }
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

    template<typename... Args>
    Gated(Args&&... args) : thing(std::forward<Args>(args)...) {}

#if EXAMPLE == 1 || EXAMPLE == 2
    T access()
    {
        // Do access check
        return thing;
    }
#elif EXAMPLE == 3
    T& access()
    {
        // Do access check
        return thing;
    }
#elif EXAMPLE == 4 || EXAMPLE == 5 || EXAMPLE == 6
    T& access()
    {
        // Do access check
        return thing;
    }

    T const& access() const
    {
        // Do access check
        return thing;
    }
#elif EXAMPLE == 7 || EXAMPLE == 8
    T& access()&
    {
        std::cout<<"access &\n";
        // Do access check
        return thing;
    }

    T const& access() const&
    {
        std::cout<<"access const &\n";
        // Do access check
        return thing;
    }

    T&& access() &&
    {
        std::cout<<"access const &&\n";
        // Do access check
        return std::move(thing);
    }

#else
    template <typename Self>
    auto&& access(this Self&& self)
    {
        // Do access check
        return std::forward<Self>(self).thing;
    }
#endif 

private:
    T thing;
    bool movedFrom = 0;
};

template<typename T, typename... Args>
static auto make_gated(Args&&... args) { return Gated<T>(std::forward<Args>(args)...); }

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

#elif EXAMPLE == 4

    auto b = make_gated<DataEngine const>("constant");
    std::cout<<"b value = "<<b.access().doCalculation()<<"\n";

#elif EXAMPLE == 5

    auto b = make_gated<DataEngine const>("constant");
    std::cout<<"b value = "<<b.access().doCalculation()<<"\n";
    b.access().updateEngine(" world");

#elif EXAMPLE == 6 ||  EXAMPLE == 7

    auto c = CreateDataEngine().updateEngine("Work").doCalculation();
    std::cout<<"c value = "<<c<<"\n";
    auto d = Gated(CreateDataEngine()).access().updateEngine("Work").doCalculation();
    std::cout<<"d value = "<<d<<"\n";

#else

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