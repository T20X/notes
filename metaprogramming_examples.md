# counter

```

template<auto Id>
struct counter {
    using tag = counter;

    struct generator {
        friend consteval auto is_defined(tag)
        { return true; }
    };
    friend consteval auto is_defined(tag);

    template<typename Tag = tag, auto = is_defined(Tag{})>
    static consteval auto exists(auto)
    { return true; }

    static consteval auto exists(...)
    { return generator(), false; }
};

template<auto Id = int{}, auto = []{}>
consteval auto unique_id() {
    if constexpr (counter<Id>::exists(Id)) {
        return unique_id<Id + 1>();
    } else {
        return Id;
    }
}

static_assert(unique_id() == 0);
static_assert(unique_id() == 1);
static_assert(unique_id() == 2);
static_assert(unique_id() == 3);

```