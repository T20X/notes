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
- gcd,lcm
- string split
- dfs
- bfs
- topologic sorting
- counting sorting
- connected components in graph
- djkestra
- min spam tree
- selection
- heap
- segment tree
- binary sort, lower bound, upper bound
- tree walks
- tree insert
- lru/lfu
- longest  maximum subsequence
- dynamic sets
  
basic C++
- any, shared_ptr, optional, unique_ptr, function, double buffer, triple buffer, list

# coding tips

- check array boundaries and loop terminate conditions
- if stuck, use examples to help you navigate through the challange and ask interviewer to help if you need one
- always be mindfull of 0, duplicates, int max, int min and overflow use cases!
- always be carefull of the base case and the very first iteration initialization
- clarify terminating condition 100%
- ***you keep missing the following***:
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

* Many candidates jump into coding the moment they hear the question. That is usually a big mistake. Take a moment to repeat the question back at the interviewer and make sure that you understand exactly what they are asking. Repeating back/rephrasing the question will reduce the chances of miscommunication.

* Always seek clarification about the question upon hearing it even if it you think it is clear to you. You might discover something that you have missed out and it also sends a signal to the interviewer that you are a careful person who pays attention to details. Some interviewers deliberately omit important details to see if you ask the right questions.

* Some common questions you can ask:

- How big is the size of the input?
- How big is the range of values?
- What kind of values are there? Are there negative numbers? Floating points? Will there be empty inputs?
- Are there duplicates within the input?
- What are some extreme cases of the input?
- Can I destroy the original array/graph/data structure?
- How is the input stored? If you are given a dictionary of words, is it a list of strings or a Trie?
- Try to iterate through few solutions and see if you can bait the interviewer into to confirm if you are on the right track!
- Clarify 100% terminating condition!

* IMPORTANT! ***when solving the problem, try taking the notes about what discussed!***

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

# Figures

## 3D Plane

    (z)  |
         |      
         |     
         |
         |   
         |
         |_ _ _ _ _ _ _ _ _ _
        /                    (x)         
       /     
      /   
     /
    /
   /  (y)
   

## Tree


           [15]
         /      \
     [10]         [20]
    /   \        /   \
[8]     [12]  [18]    [25]

##  Matrix
            0   1   2    3   4    5 (j,c)
            ________________________
  (i,r) 0   |___|___|___|___|___|___|
        1   |___|___|___|___|___|___|
        2   |___|___|___|___|___|___|
        3   |___|___|___|___|___|___|
        4   |___|___|___|___|___|___|
        5   |___|___|___|___|___|___|




