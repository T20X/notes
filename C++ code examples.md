# split string

```
  for (const auto &val_str :
       std::views::split(std::string_view(line), std::string_view(","))) {
    std::cout << " " << std::string_view{val_str.begin(), val_str.end()};
  }

```

# implementing compare using std::tie

```
 bool operator<(const person_t &left, const person_t &right) {
  return std::tie(left.m_surname, left.m_name) <
         std::tie(right.m_surname, right.m_name);
}
```

# unpacking std::pair

 std::tie may be used to unpack a std::pair because std::tuple has a
    converting assignment from pairs bool result;

```
std::tie(std::ignore, result) = set.insert(value);
```


(4) int first = 5;
auto lambda = [=](int x, int z) { return x + z + first; };

int (decltype(lambda)::*ptr)(int, int) const = &decltype(lambda)::operator();
std::cout << "test = " << (lambda.*ptr)(2, 3) << std::endl;

# quick comparison in std::map

```
    std::map<int, int, decltype([](const auto &v1, const auto &v2) {
               return v1 < v2;
             })> m;
```

# variable length arrays since C++20

 int *p2 = new int[]{1, 2, 3};

 # reading file

 In case we try to read a value, but fail, std::istream's handle these cases by setting the approriate error flags, but it will not discard any data.
 After clearing the error flags you can use std::istream::ignore to discard any data from the stream.
The function's 1st argument is the max number of potential chars to ignore, and the 2nd is the "if you hit this char, don't ignore any more*.
Let's ignore the maximum amount of characters, or until we hit ' ' (space), in which case space would also be ignored!:

#include <limits> // std::numeric_limits
in.ignore (std::numeric_limits<std::streamsize>::max()/**NO LIMIT!!!*/, ' '); //also ignores space

## std::cin stop criteria

console:
linux -> CTRL-D or newline

note that space is used to separate inputs as well


## using ranges

still should be quite efficient as only once instance of Order should be re-used by istream!

```
struct Order {
  uint64_t order_id;
  uint64_t quantity;
  double price;
  Side side;

  friend std::istream &operator>>(std::istream &s, Order &o) {
     thread_local std::string line;   
    std::getline(s, line);    
    return s;
  } 
};

int main() {

  std::ifstream file("orders.txt");
  for (const auto &o : std::views::istream<Order>(file)) {    
  }
  return 0;
}
```

## without rangess (more error-checking)
```
    errno = 0;
    ifstream f (argv[1]);
    // After this attempt to open a file, we can safely use perror() only  
    // in case f.is_open() returns False.
    if (!f.is_open())
        perror(("error while opening file " + filename).c_str());
    // Read the file via std::getline(). Rules obeyed:
    //   - first the I/O operation, then error check, then data processing
    //   - failbit and badbit prevent data processing, eofbit does not
    while(getline(f, line)) {
        process(&line);
        }
    errno_val=errno
    // Only in case of set badbit we are sure that errno has been set in
    // the current context. Use perror() to print error details.
    if (f.bad())
        perror(("error while reading file " + filename).c_str());
    else if (errno) {
        //errono may also be trigger on Windows with f.bad() returning false
    }

    f.close();//f's destructor would call it anyway
```    

## without ranges and faster (less error checking)

```
    ifstream f (argv[1]);
    // After this attempt to open a file, we can safely use perror() only  
    // in case f.is_open() returns False.
    if (!f.is_open())
        perror(("error while opening file " + filename).c_str());
    // Read the file via std::getline(). Rules obeyed:
    //   - first the I/O operation, then error check, then data processing
    //   - failbit and badbit prevent data processing, eofbit does not
    while(getline(f, line)) {
        process(&line);
        }
    // Only in case of set badbit we are sure that errno has been set in
    // the current context. Use perror() to print error details.
    if (f.bad())
        perror(("error while reading file " + filename).c_str());
    f.close();
    return 0;

```    

