
(1) reference_wrapper now can contains incomplete types !

    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --(
        2)there are concepts
    and
    constraints
        !-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

        -A concept is a named set of requirements.The definition of a
        concept must appear at namespace scope
        - concepts are like compile time predicate
    and farily the same can be said about constraints,
    but concepts emobyd semantics(meaning !) - Each concept is a predicate,
    evaluated at compile time,
    and becomes a part of the interface of a
        template where it is used as a constraint
        - Violations of constraints are detected at compile time,
    early in the template instantiation process,
    which leads to easy to follow error message
        - The intent of concepts is to model semantic
        categories(Number, Range, RegularFunction)
rather than syntactic restrictions(HasPlus, Array) - The ability to specify a
    meaningful semantics is a defining characteristic of a true concept,
    as opposed to a syntactic constrain

    Say Sorteable is concept,
    but constraint is say Subtractable,
    Incrementable

    // Concepts cannot recursively refer to themselves and cannot be constrained
    template <class T, class U>
    concept Derived = std::is_base_of<U, T>::value;
template <typename T>
concept Hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template <C1 T>
concept Error1 = true; // Error: C1 T attempts to constrain a concept definition
template <class T>
requires C1<T>
concept Error2 =
    true; // Error: the requires-clause attempts to constrain a concept

Explicit instantiations, explicit specializations,
    or partial specializations of concepts
                are not allowed(the meaning of the original definition of a
                                    constraint cannot be changed)
                    .

            ................

            The keyword requires is used to introduce a requires -
            clause,
    which specifies constraints on template arguments or
        on a function declaration

            the keyword requires must be followed by some constant
            expression(so its possible to write requires true), but the intent is that a named concept (as in the example above) or a conjunction/disjunction of named concepts or a requires-expression is used

 ................

 constraint is a sequence of logical operations and operands that specifies requirements on template arguments. They can appear within requires-expressions (see below) and directly as bodies of concepts.
There are three types of constraints:

1) conjunctions
2) disjunctions
3) atomic constraints

The keyword requires is used to introduce a requires-clause, which specifies constraints on template arguments or on a function declaration
template<class T>
void g(T) requires is_purrable<T>(); // error, is_purrable<T>() is not a primary expression
template <class T> void h(T) requires(is_purrable<T>()); // OK

The keyword requires is also used to begin a requires - expression,
    which is a prvalue expression of type bool that describes the constraints on
        some template arguments.Such an expression is true if the constraints
            are satisfied,
    and false otherwise template <typename T>
    concept Addable = requires(T x) {
  x + x;
}; // requires-expression

template <typename T>
requires Addable<T> // requires-clause, not requires-expression
    T add(T a, T b) {
  return a + b;
}

template <typename T>
requires requires(T x) { x + x; } // ad-hoc constraint, note keyword used twice
T add(T a, T b) { return a + b; }

Concepts works really well with lambdas !template <class T>
concept Number = std::is_integer<T>::value;
auto f = [](Number n)

    // infamous `requires requires`. First `requires` is requires-clause,
    // second one is requires-expression. Useful if you don't want to introduce
    // new concept.
    template <typename T>
requires requires(T a, T b) { a + b; }
auto f4(T x)


...............

 For parameter packs, MyConcept... Ts requires MyConcept to be true for each element of the pack, not for the whole pack at once, e.g. requires<T1> && requires<T2> && ... && requires<TLast>.


 ..............
 Contstrains can be used for function overload basically, but one thing is important to note
 
 It’s important to understand how the compiler decomposes constraints and when it can see that they have common atomic constraint and deduce order between them. During decomposition, the concept name is replaced with its definition but requires-expression is not further decomposed. Two atomic constraints are identical only if they are represented by the same expression at the same location. For example, concept C = C1 && C2 is decomposed to conjunction of C1 and C2 but concept C = requires{
  ...
} becomes concept C = Expression-Location-Pair and its body is not further decomposed. If two concepts have common or even the same requirements in their requires-expression, they will always be unordered because either their requires-expressions are not equal or they are equal but at different source locations. The same happens with duplicated usage of a naked type traits - they always represent different atomic constraints because of different locations, thus, cannot be used for orderin

void f(std::integral auto){}        // #1
void f(integral_or_floating auto) {} // #2
void f(std::same_as<char> auto) {}   // #3

// calls #1 because std::integral is more constrained
// than integral_or_floating(#2)
f(int{});

(4) C++ 17 didn’t allow to move rvalue references in return statements,
    function parameters in throw expressions,
    and various forms of conversions unreasonably prevented moving

    (5) std::launder is
    not requried anymore as const andr ref now can be repalced in placement new

    (5) c++ 20 now allows you to do this;
->unofficially called direct aggregate initialization

    struct A {
  int i;
  int b;
};

A a(1, 2) so also you can do vector<A> v;
v.push_back(34, 45)



(6) perfect forwarding and storing callback for later execution is now fully supported in lambas

auto prepare(Func&& func, Args&&... args) {
  auto f = [funcCopy = std::forward<Func>(func),
            ... argCopies = std::forward<Args>(args)]() mutable {
    return funcCopy(argsCopies...);
  };
  return f;
}