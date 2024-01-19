#  potentially overlapping suboject cannot be copied to non-overlapping one!

For any object (other than a potentially-overlapping subobject) of trivially copyable type T, whether or not the object holds a valid value of type T, the underlying bytes ([intro.memory]) making up the object can be copied into an array of char, unsigned char, or std​::​byte ([cstddef.syn]).28 If the content of that array is copied back into the object, the object shall subsequently hold its original value.
[Example 1: 
constexpr std::size_t N = sizeof(T);
char buf[N];
T obj;                          // obj initialized to its original value
std::memcpy(buf, &obj, N);      // between these two calls to std​::​memcpy, obj might be modified
std::memcpy(&obj, buf, N);      // at this point, each subobject of obj of scalar type holds its original value


This is very restrictive as the following code is basically UB from above

```
    class Base { int i; };
    class Derived : public Base { int d; };
    
   void copy_base(const Base &src, Base &dest) {
      memcpy((void *)&dest, (const void *)&src, sizeof(Base));
   }

    
    int main() {
       Derived d;
       Base copy;
       copy_base(static_cast<Base&>(d), copy);
       return 0;
    }
```

# object creation point in time is not clear!
