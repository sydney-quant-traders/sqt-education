/*
There are a bunch of CONVENTIONS to follow when implementing a class.
The compiler enforces none of this.
Most C++ programmers will assume these conventions hold when reading your code.
Most templated libraries (e.g. std::vector<T>) will assume that the type T
you supply it obey some of these conventions.

Class / design by contract conventions:
- classes have invariants that hold after construction and after every public method call

Ownership Semantics
- an object (instance of a class) `owns` resources / other objects (the resources owned by change over time)
- what does it mean to "own" a resource? abstractly it means that 
    - has a handle to the resource
    - may potentially have to release the resource (or transfer this responsibilty to another object)
- non-owners may access the resource, but never have to worry about the lifetime of the resource 

RAII Semantics Conventions:
- constructor acquires [resources that the object owns]
- destructor  releases [resources that the object owns at the time of destruction]

Move Semantics Conventions:
- Using a const lvalue reference to construct an object of the same type (if valid)
  results in the resources being cloned (via resource-specific logic)
  e.g. in a vector, everything within is copy constructed over
- Using a rvalue reference to construct an object of the same type (if valid)
  results in the constructed object taking ownership of the resources of the original
  and leaves the original in a valid but unspecified state.

Inheritance Conventions:
- Liskov Substitution Principle
- Can be destroyed via pointer to base class, i.e. base class has virtual destructor

The compiler allows you to make code that violates these conventions.
But it makes things harder to reason about by yourself, and others reading your code.
It also might make your type incompatible with libraries.
*/
