# Generic Programming in C++ — Workshop Plan

**Title:** *Is it a Duck? — Intro to C++ Templates & Type Erasure*

## Prerequisites

Participants should be comfortable with:
- C with classes (structs with methods, constructors/destructors, inheritance basics)
- Basic compilation concepts (preprocessor, compilation units, object files)
- Compile-time vs runtime distinction
- How linking works (symbol tables, relocation tables)
- ODR (One Definition Rule) and `inline`

## Explicitly Out of Scope
- Move semantics and forwarding references
- Small buffer optimization (SBO)
- CRTP (Curiously Recurring Template Pattern)

---

## Part 1: Introduction to Templates

### 1.1 Motivation — Why Templates?

Present a codebase that uses `std::optional`-style return types for multiple unrelated types (e.g. `maybe_int`, `maybe_string`, `maybe_connection`). Demonstrate how a naive approach without templates results in near-identical code repeated for each type.

- **Code example:** A set of `maybe_T` wrapper structs that all share the same logic (`has()`, `get()`, constructor overloads) but differ only in the wrapped type.
- Show how this violates DRY and becomes unmaintainable as the number of types grows.

### 1.2 Templates — Core Mental Model

Key ideas to convey:
- **Template instantiation happens at compile time.** The compiler generates concrete code for each set of template arguments used.
- **Templates are generated on demand** — only the specializations actually used get compiled.
- Use [cppinsights.io](https://cppinsights.io) to show the "desugared" output of templated code.
- Briefly introduce SFINAE (Substitution Failure Is Not An Error) as a mechanism — full treatment in Part 3.

Template forms:
- Function templates
- Class templates
- Variable templates

- **Code example:** Refactor the repeated `maybe_T` structs into a single `maybe<T>` class template. View the instantiated code in cppinsights.

### 1.3 How the Compiler Thinks About Templates

- **Template argument deduction:** How the compiler infers `T` from function arguments.
- **Why templates must live in headers:** The compiler needs the full definition at each instantiation site, since instantiation happens per translation unit.
- **Why ODR isn't violated:** All template instantiations are implicitly `inline` — the linker deduplicates identical instantiations across translation units.

### 1.4 Exercises — Basic Templates

**Template writing exercises** (using cppinsights to verify):
- Write simple function and class templates to solve given problems.
- Manually predict which instantiations the compiler will generate, then verify with cppinsights.

**"Does it compile?" exercises:**
- Given templated code with intentional errors, predict whether each snippet compiles or fails.
- Builds intuition for when substitution produces malformed code vs. when unused code is silently ignored (motivating SFINAE).

**Templated data structure exercise:**
- Implement a generic `Queue<T>` backed by a singly linked list.

---

## Part 2: Type Erasure

### 2.1 Motivation — Why Type Erasure?

Present a scenario where you need to store or pass objects of unrelated types that share a common interface, but without a shared base class (i.e. no inheritance relationship).

- **Code example:** Several unrelated types that all support a common operation (e.g. `draw()`, `serialize()`), but modifying their class hierarchies isn't possible or desirable.

### 2.2 Type Erasure Without Templates

Show that type erasure can be done manually without any template machinery:
- Create an abstract base class (the "concept" interface).
- For each concrete type, write a wrapper class ("model") that inherits from the base and delegates to the wrapped object.
- Store a pointer-to-base to achieve polymorphism over unrelated types.

This works, but requires writing a new wrapper for every concrete type — tedious and repetitive.

**Exercise — Polymorphic wrapper (easy):**
- Implement the concept/model pattern by hand for a small set of types.

### 2.3 Detour: Variadic Templates

Before tackling the hard exercise, introduce variadic templates:
- **Parameter packs** — `template<typename... Args>`
- **Pack expansion** — `f(args...)`, `std::forward<Args>(args)...`
- Keep it practical: show how `std::make_unique` uses variadic templates to forward constructor arguments.

### 2.4 Type Erasure With Templates

Combine templates with the concept/model pattern to eliminate the per-type boilerplate:
- The model class becomes a class template `model<T>` that wraps any `T` satisfying the interface.
- Now adding a new type requires zero new wrapper code.

**Exercise — Implement `std::function` (hard):**
- Implement a simplified `function<R(Args...)>` using type erasure.
- Simplifications: take callables by value (no forwarding references), no SBO.
- Must handle: function pointers, lambdas, and callable objects with the correct signature.

---

## Part 3: Named Requirements, Concepts & Constraints

### 3.1 Named Requirements

C++ has a long history of "named requirements" documented on cppreference — informal contracts that types must satisfy to be used with standard library components. Examples:
- `CopyConstructible`, `DefaultConstructible`, `MoveAssignable`
- `Container`, `Allocator`, `Iterator`

Before C++20, these were enforced only by documentation and cryptic template error messages.

### 3.2 Concepts — The Theory

- A **concept** defines a set of constraints on a type (or set of types).
- A type either *models* a concept or it doesn't — it's a compile-time predicate.
- Concepts let you constrain template parameters so that:
  - Error messages are clear and immediate (at the call site, not deep in instantiation).
  - Overload resolution can select between templates based on what a type supports.

### 3.3 Concepts — The C++ Syntax

- **`requires` clause** — constrains a template: `template<typename T> requires Constraint<T>`
- **`requires` expression** — an expression that checks if a set of operations are valid for a type.
- **`requires requires`** — an inline requires expression inside a requires clause (yes, really).
- **`concept`** — a named, reusable requires clause: `template<typename T> concept Sortable = requires(T a) { ... };`

### 3.4 Exercises — Writing Concepts

**Implement concepts from specification:**
- Given verbal/written descriptions of type requirements, write matching `concept` definitions.
- Include constraints on: nested types, member functions, expressions, and associated operations.
- Use cppinsights to see how concepts desugar.

**Apply concepts to earlier exercises:**
- Revisit templated code from Parts 1 and 2.
- Add `requires` clauses to restrict template parameters appropriately.

### 3.5 Advanced: N-ary Concepts

Concepts are not limited to constraining a single type:
- A concept can express a relationship between multiple types (e.g. `Convertible<From, To>`, `SameAs<T, U>`).
- Conceptually: concepts are compile-time boolean functions — unary, binary, or n-ary.

