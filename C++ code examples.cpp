(1)
bool operator<(const person_t& left, const person_t& right)
{ 
   return std::tie(left.m_surname, left.m_name)  <  
          std::tie(right.m_surname, right.m_name);
}

(2) std::tie may be used to unpack a std::pair because std::tuple has a converting assignment from pairs
bool result; std::tie(std::ignore, result) = set.insert(value);

(3)     for (; __n; --__n)
      emplace_back();
      }

(4) int first = 5;
auto lambda = [=](int x, int z) {
    return x + z + first;
};


int(decltype(lambda)::*ptr)(int, int)const = &decltype(lambda)::operator();
std::cout << "test = " << (lambda.*ptr)(2, 3) << std::endl;


(5) 


std::map<int, int, decltype([](const auto& v1, const auto& v2){ return v1 < v2;})> m;



(6)
variable length arrays
// since C++20
int* p2 = new int[]{1, 2, 3};
