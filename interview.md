# pycology

- if you feel overwhelmed, just distract on something else for 10-30s
- try to read interviewer as what he wants
- learn his reactions to see if what you say he things is true
- make him actually like you and be comfrtable with you
- try to ask some questions to confirm your assumptions and see if the interviewer eats the bait
- try to bait the interviewer into your area
- Try to iterate through few solutions and see if you can bait the interviewer into to confirm if you are on the right track!

# pre-interview warup

basic coding
- gcd, string split
  
basic C++
- any, shared_ptr, optional, unique_ptr, function

# coding tips

- you keep missing the following:
    explicit
    returning from assignment operator
    checking for *this assignment in assignment operator

- disable all plugins in Firefox
- variables need to have meaning rather than some technical terms, this would help with coding and eleminate bugs as than ifyou start using some meaning in the context it does not belong, you'll find a bug
- **ALWAYS REVISE YOUR CODE BEFORE START EXECUTING IT IN DEBUGGER**
- handy compare util  
```
template <class L, class R>
bool compare(const L& l, const R& r, int i) {  
 if (l != r) {
  cout << "\n[" << i << "] -> left [" << l << "] not equal right [" << r << "]\n";
  return false;
 }

 return true;
}
```

# C++ coding specifics

## vector

- just do empty vector constructor and than emplace_back / push_back!
- remember that vector uses move if move constructor is noexcept than copy and move anyway if copy is not available
- increment back_ ptr only after constructing an element to be thread safe!

## shared_ptr

- constructor must be explicit!

# make sure you are rested, go to the gym before the interview

# problem solving tricks


## Upon receiving the question

Many candidates jump into coding the moment they hear the question. That is usually a big mistake. Take a moment to repeat the question back at the interviewer and make sure that you understand exactly what they are asking. Repeating back/rephrasing the question will reduce the chances of miscommunication.

Always seek clarification about the question upon hearing it even if it you think it is clear to you. You might discover something that you have missed out and it also sends a signal to the interviewer that you are a careful person who pays attention to details. Some interviewers deliberately omit important details to see if you ask the right questions.

Some common questions you can ask:

How big is the size of the input?
How big is the range of values?
What kind of values are there? Are there negative numbers? Floating points? Will there be empty inputs?
Are there duplicates within the input?
What are some extreme cases of the input?
Can I destroy the original array/graph/data structure?
How is the input stored? If you are given a dictionary of words, is it a list of strings or a Trie?
Try to iterate through few solutions and see if you can bait the interviewer into to confirm if you are on the right track!
Clarify 100% terminating condition!

After you have sufficiently clarified the scope and intention of the problem, explain your high-level approach to the interviewer even if it is a naive solution. If you are stuck, consider various approaches, and explain out loud why it will/will not work. Sometimes your interviewer might drop hints and lead you towards the right path.

Start with a brute force approach, communicate it to the interviewer, explain the time and space complexity and why it is bad. It is unlikely that the brute force approach will be one that you will be coding. At this point, the interviewer will usually pop the dreaded "Can we do better?" question, meaning that they are looking for a more optimal approach. In my opinion, this is usually the hardest part of the interview. In general, look for repeated work and try to optimize them by potentially caching the calculated result somewhere and reference it later, rather than having to compute it all over again. There are some tips on tackling topic-specific questions that I dive into details below.

Only start coding after you and your interviewer have agreed on an approach and they have given you the green light.

## General algorithm

- make sure problem is understood and some examples are presented
- make sure constraints are clear, that may give some intel on what aglo / data structure to use to solve that problem
- if they give an array, think if you can sort that array!
- for recursive / dp solution you must optimize one kind of value, so it is important to think what kind of value to choose
    486. Predict the Winner


## Edge cases

- always be mindfull of 0, duplicates, int max, int min and overflow use cases!
- always be carefull of the base case and the very first iteration initialization
- clarify terminating condition 100%

## BFS

Problems that use BFS usually ask to find the fewest number of steps (or the shortest path) needed to reach a certain end point (state) from the starting one. Besides this, certain ways of passing from one point to another are offered, all of them having the same cost of 1 (sometimes it may be equal to another number). Often there is given a N x M table (formed of N lines and M columns) where certain cells are passable and others are impassable, and the target of the problem is to find the shortest time/path needed to reach the end point from the start one. Such tables may represent mazes, maps, cities, and other similar things. These may be considered as classical BFS problems


### Problem hints: At first sight this may seem like dynamic programming or backtracking. But as always, take a look into the text of the statement. After a while you should observe the following things:

- A table is given.
- The knight can jump from one cell to some of its neighbors.
- The cost of passing from a cell to another is always 1 (just one jump).
- You need to find the minimum number of steps (jumps).

Given this information we can see that the problem can be easily solved by the help of BFS. Don’t get confused by the fact that connected points are represented by unconnected cells. Think of cells as points in a graph, or states (whatever you want) – and in order to pass from one point to another, the knight should be able to jump from the first to the second point


## Brute Force and Backtracking

I have placed these 2 techniques in the same category because they are very similar. Both do the same thing – try all possible cases (situations) and choose the best one, or count only those that are needed (depending on the problem). Practically, Backtracking is just more advanced and optimized than Brute Force. It usually uses recursion and is applied to problems having low constraints (for example N<=20).


### Problem hints: Well, this is one of the easiest examples. So which are the hints of this statement?

You need to find all possible situations (positions) that satisfy a certain rule (threatens all given pieces).

The limits are very low – only 8 knights are at most given.

It’s a common Brute Force problem’s statement. Note that x and y limits are not relevant, because you need only try all positions that threaten one of the knights. For each of these positions see if the knight placed at that position threatens all others too.


## Backtracking / Recursion

This technique may be used in many types of problems. Just take a look at the limits (N, M and other main parameters). They serve as the main hint of a backtrack problem. If these are very small and you haven’t found a solution that’s easier to implement – then just don’t waste your time on searching it and implement a straight-forward backtracking solution.

Usually problems of this kind ask you to find (similarly to Brute Force):

Every possible configuration (subset) of items. These configurations should respect some given rules.
The “best” configuration (subset) that respects some given rules.
There are at most 6 people.

IMPORTANT! **Note that Recursive solutions got normally space complexity of O(n) as it takes recursion N steps to reach the base case normally!**


We are given that n <= 6. Typically, problems that ask you to find all of something with low bounds can be solved with backtracking

### Problem hints

First look at the constraints – there are at most ONLY 6 people! It’s enough for generating all possible permutations, sets etc.

There are different possible ways to pass the people from one side to another and you need to find the best one.

This is of course a problem solved with a backtracking: at the beginning choose any 2 people to pass the bridge first, and after that at each step try to pass any of those that have been left on the start side. From all these passages select the one that needs the smallest amount of time. Note that among persons that have passed over the bridge, the one having the greatest speed should return (it’s better than returning one having a lower speed). This fact makes the code much easier to implement. After having realized these things – just code the solution. There may be others – but you will lose more time to find another than to code this one.

## Dynamic Programming

Quite a few problems are solved with the help of this technique. Knowing how to detect this type of problem can be very valuable. However in order to do so, one has to have some experience in dynamic programming. Usually a DP problem has some main integer variables (e.g. N) which are neither too small, nor too big – so that a usual DP complexity of N^2, N^3 etc. fits in time. Note that in the event that N is very small (for TC problems usually less than 30) – then it is likely the problem is not a DP one. Besides that there should exist states and one or more ways (rules) to reach one greater state from another lower one. In addition, greater states should depend only upon lower states. What is a so-called state? It’s just a certain configuration or situation. To better understand dynamic programming, you may want to read this article.


Let’s analyze a simple classic DP problem:

Given a list of N coins with their values (V1, V2, … ,VN), and the total sum S. Find the minimum number of coins the sum of which is S (you can use as many coins of one type as you want), or report that it’s not possible to select coins in such a way that they sum up to S.

Let N <= 1,000 and S <= 1,000.



### Problem hints

Two main integer variables are given (N and S). These are neither too small, nor are they too big (i.e. a complexity of N*S fits in time).

A state can be defined as the minimum number of coins needed to reach a certain sum.

A sum (state) i depends only on lower sums (states) j (j<i).

By adding a coin to a certain sum – another greater sum is reached. This is the way to pass from one state to another.

**you need to find max of something**

### Common method for solving it

Typically, dynamic programming problems can be solved with three main components. If you're new to dynamic programming, this might be hard to understand but is extremely valuable to learn since most dynamic programming problems can be solved this way.

First, we need some function or array that represents the answer to the problem from a given state. For many solutions on LeetCode, you will see this function/array named "dp". For this problem, let's say that we have an array dp. As just stated, this array needs to represent the answer to the problem for a given state, so let's say that dp[i] represents the length of the longest increasing subsequence that ends with the ithi^{th}ith  element. The "state" is one-dimensional since it can be represented with only one variable - the index i.

Second, we need a way to transition between states, such as dp[5] and dp[7]. This is called a recurrence relation and can sometimes be tricky to figure out. Let's say we know dp[0], dp[1], and dp[2]. How can we find dp[3] given this information? Well, since dp[2] represents the length of the longest increasing subsequence that ends with nums[2], if nums[3] > nums[2], then we can simply take the subsequence ending at i = 2 and append nums[3] to it, increasing the length by 1. The same can be said for nums[0] and nums[1] if nums[3] is larger. Of course, we should try to maximize dp[3], so we need to check all 3. Formally, the recurrence relation is: dp[i] = max(dp[j] + 1) for all j where nums[j] < nums[i] and j < i