A unit of C++ design is "overload set". 
Overload set - a collection of function in the same scope (namespace, class, etc) of the same name such that if any is found by name resolution they all will be.
  Advice:
---------
    - if two things do nearly the same thing, name them the same and just use overloading to trigger polymorphism!
    - if using overloading, the user must figure out what is going on without knowing which overload is called. LIke if you call funciotn "A" it does not matter which of them is going to be called

- if user needs to know which construct is been called, then make it explicit


# inheritance

you shouldn't inherit Triangle from Rectangle. Inheritance should be used to model an "is-a" relationship, where a derived class is a specialized version of the base class. However, a triangle is not a specialized version of a rectangle, and vice versa. They are distinct geometric shapes with different properties and behaviors.

Inheritance should be used when there's a clear hierarchical relationship between classes, typically when one class is a more specialized version of another. For example, you might have a hierarchy where Shape is the base class, and Rectangle, Triangle, and Circle are derived classes.

Here's why you shouldn't inherit Triangle from Rectangle: