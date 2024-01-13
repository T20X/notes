
# Immediatly executed expressions

const Foo f = [&]]() {
  if (create) {
    return Foo();
  } else {
    return Foo();
  }
}

# CRTP 

The main idea is to pass Derived class to the base class
it can help in few ways:

No need to override dispatch in every event class! event_crtp gives it for free!

```
struct state
{
  StatePtr on_event(const event_connecting &e)... StatePtr
      on_event(const event_connected &e)...
}

template <State>
struct event {
  virtual StatePtr dispatch(state &s) = 0
}

template <class Derived>
class event_crtp : public event<state> {
  StatePtr dispatch(state &s) override {
    s.on_event(*static_cast<const Derived *>(this));
  }
}

class event_connection : event_crtp<event_connection> {
} class event_connected : event_crtp<event_connected> {
}

// BE CAREFULL TO AVOID INFINITE LOOPS IF BASE AND DERIVED FUNCTIINS HAVE THE
// SAME NAME

template <typename D>
class B {
public:
  ... void f(int i) { static_cast<D *>(this)->f(i); }
};

class D : public B<D> {
  // no f() here!
};
... B<D> *b = ...;
b->f(5); // goes into infinite loop!

It provides a good way to access derived calss members !

    template <typename P>
    class WithRelease {
public:
  void release() { static_cast<P *>(this)->p_ = NULL; }
};

// CTRP with templates paramters
template <typename T, template <typename, typename> class... Policies>
class Value : public Policies<T, Value<T, Policies...>>... {
```

# overloaded 

```
 template <typename... Ts>
                 struct overloaded : Ts... {
    using Ts::operator()...;
  };

  template <typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

  std::visit(overloaded{[&](const normal_scoring &state) {},
                        [&](const forty_scoring &state) {}},
             m_state);
```

# Swap

      in modern C++ swap can can give an edge over move swap,
      as normal swap based of move assignemnt would take 3 moves !

      Normally good libaries in c++ would use swap call

      template <class ForwardIt1, class ForwardIt2>
      constexpr void iter_swap(ForwardIt1 a,
                               ForwardIt2 b) // constexpr since C++20
  {
    using std::swap; // here we bounce back to std::swap call if we cannot
                     // perform swap ADL on a and b!
    swap(*a, *b);
  }

  typical patter namespace stuff {
    struct foo {
      void swap(foo &other);
    };
    swap(foo & lhs, foo & rhs) { lhs.swap(rhs); }
  }

  int main() {
    foo a, b;
    int i = 0;
    int j = 42;

    using std::swap;
    swap(i, j); // calls std::swap
    swap(a, b); // calls stuff::swap(stuff::foo&, stuff::foo&) via ADL
  }

  # runtime concept polymorphism
  
  ```
  tempalte<typename T> void
        draw(const T &x, ostream out) {
    out << x << endl;
  }
  template <> void draw(const mytype &x, osteram out) {
    out << x.to_string() << endl;
  }

  struct package {
    friend void draw(package &p, ostream) {
      p._c->draw();
    } // defines a non-member friend function!
    struct conceptbase {
      virtual draw(ostream out) = 0;
      virtual concept *copy() {}
    };

    template <typname T> struct model : conceptbase {
      virtual draw(ostream out) { draw(data_, out); }
      concept *copy() {}
      T data_
    };

    unique_ptr<conceptbase> c_;
  }
```

# accessing global variables 

## global variables in header files is
good !at least ordering can be enforced,
not like with translation units !

```
inline Informer & str() {
    static Informer str1{};
    return str1;
  }
  static auto &g_str = detail::str();

  template <class = void> struct Temp { static Informer other; };
  template <> inline Informer Temp<>::other{};
  static auto &g_str2 = Temp<>::other;

  inline Informer g_str3; // the best! C++17
```

# Type Erasure

```
template <typename T>
           class smartptr_te_lb {
    struct deleter_base {
      virtual void apply(void *) = 0;
      virtual ~deleter_base() {}
    };
    template <typename Deleter> struct deleter : public deleter_base {
      deleter(Deleter d) : d_(d) {}
      virtual void apply(void *p) { d_(static_cast<T *>(p)); }
      Deleter d_;
    };

  public:
    template <typename Deleter>
    smartptr_te_lb(T *p, Deleter d)
        : p_(p),
          d_(sizeof(Deleter) > sizeof(buf_) ? new deleter<Deleter>(d)
                                            : new (buf_) deleter<Deleter>(d)) {}
    ~smartptr_te_lb() {
      d_->apply(p_);
      if (static_cast<void *>(d_) == static_cast<void *>(buf_)) {
        d_->~deleter_base();
      } else {
        delete d_;
      }
    }
    T *operator->() { return p_; }
    const T *operator->() const { return p_; }

  private:
    T *p_;
    deleter_base *d_;
    char buf_[16];
  };
```

# Named Idiom
```
class City {

 class DayEvents {
    public:
      DayEvents() : flood(false), fire(false), , , {}
      DayEvents &SetFlood() {
        flood = true;
        return *this;
      }
      DayEvents &SetFire() {
        fire = true;
        return *this;
      }
      ... private : friend City;
      bool flood;
      bool fire;
      ...
    };
    void day(DayEvents events);
    ...
  };

  City capital(...);
  capital.day(City::DayEvents().SetFire().setFlood());



 template <typename T>
  class Collection {
  public:
    Collection() {}
    T filter(); // "*this" is really a T, not an A
    T sort() {
      T sc;
      ... return sc;
    } // Create new sorted collection
  };

  class SortedCollection : public Collection<SortedCollection> {
  public:
    SortedCollection search();
    SortedCollection median();
  };

  Collection<SortedCollection> c;
  auto c1 = c.sort()       // Now SortedCollection
                .search()  // Needs SortedCollection, gets it
                .filter()  // Invokes, preserves SortedCollection
                .median(); // SortedCollection::median is called
}
```

# FRIEND FACTORY

```
template <typename T>
class C {
  int x_;

public:
  C(int x) : x_(x) {}
  friend C operator+(const C &lhs, const C &rhs) { return C(lhs.x_ + rhs.x_); }
}

template <typename D>
class B {
public:
  friend bool operator!=(const D &lhs, const D &rhs) { return !(lhs == rhs); }
};
template <typename T> class C : public B<C<T>> {
  T x_;

public:
  C(T x) : x_(x) {}
  friend bool operator==(const C &lhs, const C &rhs) {
    return lhs.x_ == rhs.x_;
  }
};

```


# template method

the main problem with this pattern is composibility as duplicated code is all the way around.Policy pattern battles that.

```
class CountingFileWriter : public FileWriter {
  size_t count_;
  void Preamble(const char *data) { count_ += strlen(data); }
};

class CountingLoggingFileWriter : public LoggingFileWriter {
  size_t count_;
  void Preamble(const char *data) {
    count_ += strlen(data);
    LoggingFileWriter::Preamble(data);
  }
};

```

The Fragile Base Class problem is not limited to the Template Method, but is, to some degree, inherent in all object-oriented languages. The problem arises when changes to the base class break the derived class

While there is no general solution to the Fragile Base Class problem as long as inheritance is used at all, the guideline that helps to avoid it when using the Template Method is straightforward—when changing the base class and the structure of the algorithms, or the framework, avoid changing which customization points are invoked. Specifically, do not skip any customization options that were already invoked and do not add new calls to already existing ones (it's OK to add new customization points, as long as the default implementation is sensible). If such a change cannot be avoided, you will need to review every derived class to see whether it relied on the implementation override that is now removed or replaced, and what the consequences of such a change are


# Singleton

You should instead consider whether the design should enforce that a certain object is unique

An alternative implementation that should be preferred, when possible, is to explicitly trigger the release of all resources by the static objects at the end of the program. For example, our ErrorLogger object may have a clear() method—when called, it will complete the error logging tasks and release all memory back to the manager; the destructor is then left with nothing to do but destroy the static instance itself. This design relies on the goodwill of the programmer, and so is not enforced by the compiler. Nonetheless, it is a better alternative in many cases


# Stragegy Pattern

template <typename T> void delete_T(T* p) {
  delete p; }
SmartPtr<int, delete_int_t> p(new int(42), delete_T<int>)

Another way to address some of the weaknesses of policy-based design is to try and accomplish the same goal by different means. There is no substitute for the entirety of the

capabilities offered by the policies—the Strategy pattern is there for a reason. However, there are alternative patterns that offer somewhat superficial similarities, and may be used to solve some of the same problems as the policy-based design addresses. We will see one such alternative in Chapter 17, Adapters and Decorators, when we talk about decorators. Another solution that may look very policy-like in a limited domain will be shown in the next sectio


# Policy

policies, which are building blocks for classes—they let the programmer customize the implementation for a particular behavior

template <typename T, template <typename, typename> class ... Policies>
class Value : public Policies<T, Value<T, Policies ... >> ...
{ ..... };
using V = Value<int, Addable, Multipliable, Ordered>; // Works in any order


The techniques we have introduced in the previous section form the foundation of policy-based design—policies can be classes, template instantiations, or templates (used by template template parameters). The policy classes can be composed, inherited, or used statically at compile time. If a policy needs to know the type of the primary policy-based class, the CRTP can be used. The rest is largely variations on the same theme, as well as tricky ways to combine several techniques to accomplish something new. We will now consider some of these more advanced techniques

Note that a policy that needs to disable or enable constructors does not automatically have to be used as a base class—move assignment or construction also moves all data members, and, therefore, a non-movable data member will disable the move operations just as well. The more important reason to use inheritance here is the empty base class optimization


# Decorator

```
public:
  template <typename P>
  VeteranUnit(P && p, double strength_bonus, double armor_bonus)
      : U(std::move(*move_cast<U>(p))), strength_bonus_(strength_bonus),
        armor_bonus_(armor_bonus) {}
  double attack() { return U::attack() + strength_bonus_; }
  double defense() { return U::defense() + armor_bonus_; }

private:
  double strength_bonus_;
  double armor_bonus_;
};
```

# Composable decorator!

```
template <typename Callable> class LockDecorator {
public:
  LockDecorator(const Callable &c, std::mutex &m) : c_(c), m_(m) {}
  template <typename... Args> auto operator()(Args &&...args) const {
    std::lock_guard<std::mutex> l(m_);
    return c_(std::forward<Args>(args)...);
  }

private:
  const Callable &c_;
  std::mutex &m_;
}

auto safe_f = decorate_debug(
                  decorate_lock(
                      [](int x) {
  return unsafe_f(x); }, 
                      m
                  ), 
                  "f(x)");
```

This is the limitation of composable adapters—they work great until the interface they add has to return an adapted type. We had no problems with comparison operators returning bool, but once we have to return the adapted type itself, the composability breaks down


# Visitor

Using the Visitor pattern, we can add a new operation to the class hierarchy without modifying the classes themselves

```
class Cat;
class Dog;
class PetVisitor {
public:
  virtual void visit(Cat * c) = 0;
  virtual void visit(Dog * d) = 0;
};

class Pet {
public:
  virtual void accept(PetVisitor & v) = 0; // **ACCEPT IS REQUIRED SO THAT THE RIGHT DYNAMI TYPE IS PASSED TO VISITOR**
  .....
};

class Cat : public Pet {
    public:
    void accept(PetVisitor& v) override { v.visit(this);
}

}
;

void dispatch(Pet *p, PetVisitor *v) { p->accept(*v); }
Pet *p = .....;
PetVisitor *v = .....;
dispatch(p, v); // Double dispatch

```

Once we add the new visit() overload to the PetVisitor base class, we have to
    add it to all derived classes as well;
otherwise,  the compiler will let us know that we have a pure virtual function without    an override.The latter is also one of the main disadvantages of the Visitor pattern—if a new class is added to the hierarchy, all visitors must be updated, whether the new classes actually need to support these operations or not .For this reason, it is sometimes recommended to use the visitor only on relatively stable hierarchies that do not have new classes added often.There is also an alternative visitor implementation that somewhat mitigates this problem; we will see it later in this chapte

# Named variable

``` struct argument {
  template <typename UnderlyingType>
  NamedType operator=(UnderlyingType &&value) const {
    return NamedType(std::forward<UnderlyingType>(value));
  }
  argument() = default;
  argument(argument const &) = delete;
  argument(argument &&) = delete;
  argument &operator=(argument const &) = delete;
  argument &operator=(argument &&) = delete;
};

displayCoolName(firstName = "James", lastName = "Bond");
```
