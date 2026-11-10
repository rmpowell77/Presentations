// Type your code here, or load an example.
#include <iostream>
#include <string>


struct DataEngine {
    DataEngine(std::string_view s) : s(s) { std::cout<<"DataEngine "<<s<<"\n"; }

    void updateEngine(std::string_view newS) {
        s = s.append(newS);
    }

    auto doCalculation() const {
        return s.size();
    }

/*
    ~DataEngine() { std::cout<<"~DataEngine "<<s<<", movedFrom "<<movedFrom<<"\n"; }
    DataEngine(DataEngine const& other) : s(other.s) { std::cout<<"ctor &DataEngine "<<other.s<<"\n"; }
    DataEngine(DataEngine&& other) : s(std::move(other.s)) { other.movedFrom = 1; std::cout<<"ctor &&DataEngine "<<s<<"\n"; }
    DataEngine& operator=(DataEngine const& other) { s = other.s; std::cout<<"DataEngine=& "<<other.s<<"\n"; return *this; }
    DataEngine& operator=(DataEngine&& other) { s = std::move(other.s); other.movedFrom = 1; std::cout<<"DataEngine=&& "<<s<<"\n"; return *this; }
*/

    std::string s;
    bool movedFrom = 0;
};

std::ostream& operator<<(std::ostream& os, DataEngine const& f) {
    return os<<f.s;
}

template <typename T>
class Gated {
public:
    Gated(T&& t) : thing(std::forward<T>(t)) { std::cout<<"Gated "<<thing<<"\n"; }
/*
    ~Gated() { std::cout<<"~Gated, movedFrom "<<movedFrom<<"\n"; }
    Gated(Gated const& other) : thing(other.thing) { std::cout<<"ctor &Gated "<<thing<<"\n"; }
    Gated(Gated&& other) : thing(std::move(other.thing)) { other.movedFrom = 1; std::cout<<"ctor &&Gated "<<thing<<"\n"; }
    Gated& operator=(Gated const& other) { thing = other.thing; std::cout<<"Gated=& "<<thing<<"\n"; return *this; }
    Gated& operator=(Gated&& other) { thing = std::move(other.thing); other.movedFrom = 1; std::cout<<"Gated=&& "<<thing<<"\n"; return *this; }
*/
/*
    decltype(auto) access(this auto&& Self) {
        std::cout<<"access const &\n";
       return std::forward<T>(Self.thing);
    }
    */

    T access()
    {
        return thing;
    }
/*

    T const& access() const &
    {
//        std::cout<<"access const &\n";
       return thing;
    }

    T& access() &
    {
//        std::cout<<"access &\n";
        return thing;
    }

    T&& access() &&
    {
//        std::cout<<"access &&\n";
        return std::move(thing);
    }
    
*/

private:
    T thing;
    bool movedFrom = 0;
};

template<typename T, typename... Args>
static auto make_gated(Args&&... args) { return Gated<T>(T(std::forward<Args>(args)...)); }

int main() {
    auto a = make_gated<DataEngine>("hello");
    std::cout<<"a value = "<<a.access().doCalculation()<<"\n";
    // a.access().updateEngine(" world");

    // std::cout<<"a value = "<<a.access().doCalculation()<<"\n";

//   std::cout<<"a.access() = "<<make_gated<Foo>("hello").access()<<"\n";

    // This example shows why we need a R-value access!!!
//   std::cout<<"a.access() = "<<(make_gated<Foo>("hello").access() = Foo("world"))<<":\n";
//   make_gated<DataEngine>("hello").access().doCalculation();
//    std::cout<<"a.access() = "<<make_gated<Foo>(make_gated<Foo>("hello").access() = Foo("world")).access()<<"\n";

    // std::cout<<"a.access() = "<<Gated<Foo>(Foo("hello")).access()<<"\n";
//    std::cout<<"a.access() = "<<(Gated<Foo>(Foo("hello")) = Gated<Foo>(Foo("world"))).access()<<"\n";
    // auto a = Gated<Foo>(Foo("world"));
    // Gated<Foo>(Foo("hello")) = std::move(a);

    // auto d = (Gated<Foo>(Foo("hello")) = Gated<Foo>(Foo("world"))).access();
    // std::cout<<"a.access() = "<<d<<"\n";

    return 0;
}