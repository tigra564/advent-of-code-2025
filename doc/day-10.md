# Day 10: Factory

## Part 1

Since the Part 2 of Day 9 was a challenge, I decided to create a structure called `Machine` to hold the input data. This time it was a bit overkill, but I left it as is.

It looks like all first parts of each day are algorithmically easy, and brute force proved to be a decent way to deal with them. Day 10 was not an exception. However, from now on I (rightfully) decided to assume that not only are the inputs correct, but also that each line of any input has a unique valid solution (in terms of the resulting number). The algorithm relies on this—if a line had no solution or multiple ones, it could break. Thus, there are minimal checks on the input data.

For clarity, we can reformulate the puzzle as follows. There is a 10-bit integer (the indicators array) *A*, and a number of other 10-bit integers (the buttons) *B1, ..., Bn*. We need to find a combination *Bi ^ Bj ^ ... ^ Bk* (where *^* denotes bitwise XOR) such, that it equals *A*. It is enough to try all *2^(N)* combinations, where *N* is the number of buttons, since each button can be either pressed or not. At least one of them will work, and we must choose the one with the minimum number of toggles—i.e., minimize the size of the set *{i, j, ..., k}*.

This simple command calculates the maximum number of buttons in the main input:

```sh
cat ../10.txt | sed -e 's/[^(]//g' | sort | tail -1 | wc -c
```

In my case, it gave 14: 13 opening brackets and a newline character. So for each line of input, we have to check at most 2¹³ = 8192 combinations of buttons. That's not too much for 162 lines.

## Part 2

### Estimating brute force solution

Though it looks similar to Part 1, Part 2 is way too complicated to be solved by brute force. Let's take the third line of the sample:

```
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}
```

Part 1 requires checking 2⁴ = 16 combinations of buttons. Part 2 is a different story. A single button press contributes as a '+1' to some of the required joltage levels. The first joltage level can be achieved by pressing some combination of the first three buttons (those that have 0 in their wiring schematics). The [general formula](https://en.wikipedia.org/wiki/Combination#Number_of_combinations_with_repetition) *C*(*n* + *k* − 1, *k*), where *n* is the number of buttons and *k* is the target joltage level, gives us the following numbers.

1. For the first joltage level: *C*(3 + 10 − 1, 10) = *C*(12, 10) = *C*(12, 2) = 12 · 11 / 2 = 66.
2. For the second joltage level: *C*(3 + 11 − 1, 11) = 78.
3. For the third joltage level: *C*(3 + 11 − 1, 11) = 78.
4. For the fourth joltage level: *C*(2 + 5 − 1, 5) = 6.
5. For the fifth joltage level: *C*(3 + 10 − 1, 10) = 66.
6. For the sixth joltage level: *C*(1 + 5 − 1, 5) = 1.

The whole solution would require checking a total of 66 × 78 × 78 × 6 × 66 × 1 = 159,011,424 combinations.

### ILP to the rescue

The good news is that there is a method for solving this class of problems with moderate complexity. It is called linear programming — or, in our case, [integer linear programming (ILP)](https://en.wikipedia.org/wiki/Integer_programming)—since we are solving the puzzle in integers.

The bad news is that we deliberately banned ourselves from using third‑party tools and must implement the approach by hand.

Before reading further, I'd recommend looking through at least a couple of chapters on the subject. A good linear programming textbook should contain some examples of formalizing a problem statement.

For this puzzle, we have the following formulation. For illustration, let's take the third line of the sample input.

* Our variables *x₀, x₁, ..., xₙ* are the numbers of presses of each button. In the example, we have 4 buttons, so the variables are: *x₀, x₁, x₂*, and *x₃*.
* Our [constraints](https://en.wikipedia.org/wiki/Constraint_(mathematics)) are:
  * The sum of the variables for buttons that affect a given joltage level must equal that level. For the first joltage level, this takes the form: 1·*x₀* + 1·*x₁* + 1·*x₂* + 0·*x₃* = 10. Here, pressing the fourth button does not affect the first joltage level, while pressing any of the other buttons increases it by one.
  * The values of the variables must be non-negative integers.
* Our objective function is the minimization of the total number of presses: *x₀* + *x₁* + *x₂* + *x₄* → min.

Let's write this as a complete system of equations and inequalities:

```
1·x₀ + 1·x₁ + 1·x₂ + 0·x₃ = 10
1·x₀ + 0·x₁ + 1·x₂ + 1·x₃ = 11
1·x₀ + 0·x₁ + 1·x₂ + 1·x₃ = 11
1·x₀ + 1·x₁ + 0·x₂ + 0·x₃ =  5
1·x₀ + 1·x₁ + 1·x₂ + 0·x₃ = 10
0·x₀ + 0·x₁ + 1·x₂ + 0·x₃ =  5
x₀ ≥ 0
x₁ ≥ 0
x₂ ≥ 0
x₃ ≥ 0

F = x₀ + x₁ + x₂ + x₃ → min
```

We build a custom ILP solver where the objective function and constraints are always in the same form. Let's put the constraints and the objective function aside for a moment and focus on the equations. Also, let's drop the variable names and work with the corresponding [augmented matrix](https://en.wikipedia.org/wiki/Augmented_matrix).

```
  1  1  1  0 10
  1  0  1  1 11
  1  0  1  1 11
  1  1  0  0  5
  1  1  1  0 10
  0  0  1  0  5
```

Our first goal is to transform it into diagonal form, like this:

```
 1  0  0 a₁ b₁ c₁
 0  1  0 a₂ b₂ c₂
 0  0  1 a₃ b₃ c₃
```

I'll explain the meaning of this transformation later.

The allowed transformations are:

1. Multiply or divide an entire row by some non‑zero integer. All values in the row must remain integers.
2. Add to a row a linear combination of other rows. A linear combination of rows is a sum of the results of their multiplication or division, as described in item 1.
3. If a row becomes all zeros, we remove it, because it is a linear combination of other rows and does not provide any additional information for the solution.

This approach is called [Gaussian elimination](https://en.wikipedia.org/wiki/Gaussian_elimination).

Let's return to our example.

Subtract row 0 from the other rows if they have a 1 in column 0. Here, only row 1 is affected.

```
  1  1  1  0 10
  0  1  0 -1 -1
  0  1  0 -1 -1
  0  0  1  0  5
  0  0  1  0  5
```

Subtract row 1 from the other rows so that they have 0 in column 1. Row 2 becomes all zeros and is removed.

```
  1  0  1  1 11
  0  1  0 -1 -1
  0  0  1  0  5
  0  0  1  0  5
```

Finally, subtract row 2 (formerly row 3) from the other rows so that they have 0 in column 2. Row 3 becomes all zeros and disappears.

```
  1  0  0  1  6
  0  1  0 -1 -1
  0  0  1  0  5
```

Or, in equation form:

```
1·x₀ + 1·x₃ = 6     (1)
1·x₁ - 1·x₃ = -1    (2)
1·x₂ = 5            (3)
```

Here, the variables *x₀, x₁*, and *x₂* are called **basic variables**, since they form a basis: a system where each basic variable is independent of the others. However, they may depend on the remaining variables, called **free variables**. Here, x₃ is a free variable. A variable is called 'free' because it can take any value, and the solution will still be valid as long as both it and the dependent basic variables also satisfy the constraints (see the inequalities above).

From now on, our goal is to choose a value for the free variable such that all variables are non‑negative integers and the objective function is minimized.

From equation (1), we conclude that *x₁* = 6 − *x₃*. Since both *x₁* and *x₃* must be non‑negative integers, we have 0 ≤ *x₃* ≤ 6.

From equation (2), we get *x₂* = −1 + *x₃*, which implies *x₃* ≥ 1.

Equation (3) imposes no constraints on *x₃*.

Thus, 1 ≤ *x₃* ≤ 6.

Now let's see how *x₃* affects the objective function.

*F* = *x₀* + *x₁* + *x₂* + *x₃* = (6 − *x₃*) + (−1 + *x₃*) + 5 + *x₃* = *x₃*.

*F* reaches its minimum when *x₃* is minimal, i.e., *x₃* = 1.

The solution is:

* *x₀* = 6 − *x₃* = 5
* *x₁* = −1 + *x₃* = 0
* *x₂* = 5
* x₃ = 1

In terms of presses: we press button `(0,1,2,3,4)` 5 times, button `(0,1,2,4,5)` 5 times, and button `(1,2)` once. Nice!

### Theory is good, but what about automation?

We will address the following four topics later in this section:

1. Diagonalizing the problem matrix.
2. Calculating free variable bounds.
3. Fixing free variable values.
4. Keeping it integer.

### Diagonalizing the problem matrix

The explanation will be easier if we put aside the integer constraint for a while. We'll fix that later.

We iterate over the matrix rows, keeping track of the current column in which we search for a non‑zero cell. We start at row 0, column 0. We search down the column for any non‑zero cell. If the row found is below the current row, we swap these rows.

If no such row is found, this means that the column contains a free variable. That's because a variable cannot be basic if it has zero coefficients in all rows from the current one down—its value is unconstrained by those equations, so it is free. In this case, we increment the column number and start over.

Eventually, we will either run out of rows or find a row containing a basic variable. In the latter case, we first divide all the values in the row by the value of the found cell, so that the cell becomes 1. After that, we null out all the cells in the other rows for the current column. It's easy: we multiply the values of the current row by the coefficient in the target cell of the other row, and subtract the result from that other row.

We end up with some number *N* of rows, which is usually (but not always) less than the initial number of rows. This number *N* is called the [rank](https://en.wikipedia.org/wiki/Rank_(linear_algebra)) of the matrix, and it equals the number of basic variables.

Finally, we scan the resulting matrix again. In each row, we find the first non‑zero coefficient; the column it occupies is the position of a basic variable. All variables not in the list of basic variables are free.

### Calculating free variable bounds

We end up with equations of the following form:

1 × (some basic variable) + (a linear combination of free variables) = right‑hand side value.

Or, in a form more convenient for our purposes:

some basic variable = right‑hand side value − (a linear combination of free variables).

Using these equations, we can substitute all basic variables in the objective function with combinations of free variables and constants. This means that the objective function now depends only on the free variables and the right‑hand side constants. Thus, we can eliminate the basic variables when finding bounds for the free variables.

We copy the diagonalized matrix and keep only the free variables and right‑hand sides in the copy. Now we have a system of inequalities for the free variables, because we silently assume that for each equation,

(a linear combination of free variables) = right‑hand side value − (the basic variable)

the value of (right‑hand side + basic variable) is maximized when that basic variable is zero. There must be at least one inequality that contains a free variable with a positive coefficient (the others being non‑negative, i.e., at least zero). Otherwise, we could take arbitrarily large values for the free variables and still satisfy all equations. That would mean there is no optimal solution, but we assume there is one by the definition of the puzzle.

Here comes the hardest part: in the example above, we used some mathematical reasoning on the inequalities to derive bounds for the free variables. Translating that reasoning into an algorithm is far from trivial. So instead of juggling inequalities, we use a coarse approach.

1. For each inequality, we find a variable with a positive coefficient. If any other variable has a negative coefficient, we skip to the next inequality. Otherwise—and there must be at least one such inequality—the left‑hand side is at least (coefficient × variable). Therefore, this variable cannot exceed the right‑hand side divided by its coefficient. Hence our bound estimate: free variable ≤ right‑hand side / (coefficient of the free variable). We don't bother with the lower bound and simply keep the initial constraint that all variables (including free ones) are non‑negative.
2. If, after examining all inequalities, we still have some unbounded free variables, we iterate over the inequalities again. This time, for each bounded free variable that has a negative coefficient, we substitute the variable with its upper bound and recalculate the right‑hand side. There is no need to handle the case of non‑negative coefficients, since in that case we would use the lower bound, which is already zero and is not taken into account anyway. Moreover, if we don't substitute free variables with non‑negative coefficients, we don't even need to remember their bounds. They will be recalculated correctly in the next pass.
3. Rinse and repeat with the new inequalities until all free variables have some upper bound.

### Example: Deriving Bounds for Free Variables

Consider the first line from the sample input:
```
[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
```

After diagonalization, we obtain a system with two free variables. The augmented matrix (excluding the basic variables) is:

```
  1  -1   2
  0   1   5
  1  -1   1
  0   1   3
```

Each row corresponds to an inequality of the form `(linear combination of free variables) ≤ right‑hand side`.

---

**Iteration 1.** For each inequality, we look for a variable with a positive coefficient, provided all other coefficients are non‑negative. If any other coefficient is negative, we skip that inequality (it cannot give a valid upper bound at this step).

| Row  | Coefficients | RHS  | Positive coeff. | All other coeffs non‑negative? | Bound    |
| ---- | ------------ | ---- | --------------- | ------------------------------ | -------- |
| 1    | 1, -1        | 2    | *x₀*            | No (*x₁* has -1)               | —        |
| 2    | 0, 1         | 5    | *x₁*            | Yes (0 is non‑negative)        | *x₁* ≤ 5 |
| 3    | 1, -1        | 1    | *x₀*            | No (*x₁* has -1)               | —        |
| 4    | 0, 1         | 3    | *x₁*            | Yes                            | *x₁* ≤ 3 |

After this pass, we have a preliminary bound: *x₁* ≤ 3 (the tighter one).

We still have no bound for *x₀*, so we proceed to the next iteration.

---

**Iteration 2.** Now we substitute the known bound for *x₁* into the inequalities that we skipped earlier (those with a negative coefficient for *x₁*).

- Row 1: *x₀* − *x₁* ≤ 2. Substitute *x₁* = 3 (its upper bound): *x₀* − 3 ≤ 2 → *x₀* ≤ 5.
- Row 3: *x₀* − *x₁* ≤ 1. Substitute *x₁* = 3: *x₀* − 3 ≤ 1 → *x₀* ≤ 4.

Now we have a bound for *x₀*: *x₀* ≤ 4. Since *x₁* ≤ 3 is already tighter than the bound from row 2 (*x₁* ≤ 5), we keep *x₁* ≤ 3.

---

**Iteration 3.** We check if any inequality can give a tighter bound for *x₀* or *x₁* using the newly found bounds.

- Row 1 with *x₀* ≤ 4 gives no new bound for *x₁*.
- Row 3 with *x₀* ≤ 4 gives no new bound for *x₁*.

No further improvements are possible. The algorithm terminates with the final bounds:

> **Bounds: [4, 3]**

This means *x₀* can range from 0 to 4, and *x₁* from 0 to 3. These bounds are then used to enumerate all valid combinations of the free variables.

### Fixing free variable values

The idea is now straightforward: we evaluate the objective function for every possible combination of values of the free variables, each bounded between 0 and its upper limit. We skip any combination that makes the objective function negative (which shouldn't happen, since it's a sum of non‑negative terms). Otherwise, we keep the minimum value found.

However, we don't know in advance how many free variables there are, so we don't know how many nested loops we would need to iterate over all possible combinations. So we use recursion. The recursion builds a full vector of values for all free variables, iterating only over the possible values of the most recently added variable at each step. If the vector's size equals the number of free variables, it evaluates the objective function. Otherwise, it recurses, pushing the values of the next free variable onto the vector.

### Keeping it integer

All of the above works if we implement it using floating‑point numbers and hope we don't run into rounding errors. Another option is to use rational numbers—i.e., pairs of integers: a numerator and a denominator. However, this would mean either using [boost::rational](https://www.boost.org/doc/libs/latest/libs/rational/rational.html) (which is forbidden) or implementing rational numbers from scratch. I decided that reimplementing rational numbers for a limited ILP solver would be overkill. So I stuck with pure integers. And here is how that decision changes things.

1. If we just use integers and always multiply numbers, we may end up with quite large right‑hand sides. So the algorithm uses division by the [greatest common divisor (GCD)](https://en.wikipedia.org/wiki/Greatest_common_divisor) hither and yon.
2. Diagonalizing the problem matrix:
   * When searching for a row with a non‑zero cell in the current column, I gave priority to cells with the smallest absolute values, ideally 1 or -1.
   * After each transformation, the modified row is checked to see if it can be divided entirely by some integer. This is done by computing the GCD of all its cells. If the GCD is greater than 1, the row is divided by it.
   * When nulling out a value in column *C* of row *R1* by subtracting row *R2*, I compute:
   
      *R1* · (*R2* / *GCD*(*R1, R2*)) - *R2* · (*R1* / *GCD*(*R1, R2*)). This reduces the final multiplier for *R1*.
   * At the end of the Gaussian elimination, the coefficients of the basic variables may differ. I equalize them to their [least common multiple (LCM)](https://en.wikipedia.org/wiki/Least_common_multiple) using GCD again.
3. Fixing free variable values:
   * Since the coefficients of the basic variables are no longer guaranteed to be 1, I initialize the objective function coefficients with the common basis factor instead of 1.
   * When evaluating the objective function for a given set of free variable values, I also check that the result is divisible by the common basis factor. Otherwise, the solution does not provide a valid integer result.

With all these tricks applied, I ended up with a maximum common basis factor of 36 and a maximum right‑hand side value of 815. Small enough to fit in 32‑bit integers.

[<< To Day 9](day-09.md)   [To Day 11 >>](day-11.md)
