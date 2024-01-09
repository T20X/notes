
# probability

(0)
Probabiility sample consists of S and P. S is the set of all the events in the event space. P is the probability functions wihch maps all subsets of S into probabilites  [0;1].

(1) P(empty space) = 0; no event no probability of it happening;
(2) P(S) = 1; the probability of any event happening is 100%;
(3) P( sum of all of events in S one by one) = sum of individual proability of each event IF events are disjoint!

(1) say there are 10 people, *split* a team of 4 and a team of 6. Just pick the team of 4 and the rest is team of 6. Like first time we peak team of 4, we also pick first time time of 6, because what is left is team of 6!
(10) = (10)
(4)    (6)

# permutations

k ^ N - number of ordered permuations with repetition(any item from k set can be re-used as many times as needed to form a set with N items).
k - number of unique items which would be used to make ordered permuations
N - length of such ordered permuations

Each level’s nodes are branching less than the level above it— The first level node branches out 3 times, the second level nodes branch out 2 times, and the third level nodes branch out 1 time. This is the factorial pattern, and it’s the reason why the number of permutations can be quickly found by using n!



(2) combination of k from n numbers with repetition is 
(n - 1 + k)
(k)
because imagine the problem like boxes below:
***|***|**** <-- dots can float!
k = 7
n = 3
so now the problem is selcting k dots separated by 3 separators!
-------
 if modulo of two numbers by k are same, then their difference is going to be divisible by k
a%k = x
b%k = x
(a - b) %k = x -x = 0
here a - b = the sum between i and j.

312(numerator)/941(denominator)

log x A = y; x = base, A - fixed number, y - exponent!
log x(a * b) = log x (a) + log x (b)

sum of arithmertic progression -> n*(a+b)/2 
n - number ofelemetns to sum, a - start, b - end

sum of geometric progression ->
(bk - a) / (k -1)

rational number a/b - where a and b are integers

negatrion - ~
conjunction - &
disjunction - |
equivalance - =

polynomial time complexity - O(n^k) where k is CONSTANT!
O(n!) - permutational time complexity!

2^N - number of sets! (say {1,2,3} and {3,2,1} are the same } including empty set!
 -  If we just want to select K subelemts then, it is n!/k!(n - k)!
n! - numer of all permutations!
 -  If we just want to select K subelemts then, it is n!/(n - k)!

triangular numbers are the sum of the first n positive integers

Pythagoreans also observed that some numbers could not be made into any nontrivial rectangular shape (a shape where both sides of the rectangle are greater than 1). These are what we now call prime numbers - numbers that are not products of smaller numbers

The product of two odd numbers is an odd number

----------
Algebraic
----------
u[[ syntax ]] = mathExpression(value) - value meanining of "synatax" is math function!
u[[expression] : mathExpresion(type) - type of expression!
u[[x1 + x2]] = u[[x1]] + u[[x2]]
u[[int]] = Z
u[[3]] = 3
u[[3]]:Z

u[[connect<e>]] : u[[source<e>]] -> u[[sink<e>]] -> Action
u[[connect<e>(so,si)]] : u[[so]](u[[si]])
tempalate <typename T> void connect(source<T> so, sink<T> si) { so(si}; }
connect(so(), si());

-------------
An operation is commutative if you can change the order of the numbers involved without changing the result. Addition and multiplication are both commutative. Subtraction is not commutative: 2 - 1 is not equal to 1 - 2.
 
An operation is associative if you can group numbers in any way without changing the answer. It doesn't matter how you combine them, the answer will always be the same. Addition and multiplication are both associative

