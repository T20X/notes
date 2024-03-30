
# Sliding window

```
int lengthOfLongestSubstringTwoDistinct(string s) {
        int res{}, start{};
        unordered_map<char, int> m;

        for(int i = 0; i < s.size(); i++) {
            m[s[i]]++;
            if(m.size() < 3) res = std::max(res, i - start + 1);
            while(m.size() > 2) if(!--m[s[start++]]) m.erase(s[start-1]);
        } return res;
    }
```

# DP

## number of steps to visiit all nodes in connected undirected graph
https://leetcode.com/problems/shortest-path-visiting-all-nodes

## house robber

```
class Solution {
public:
     int rob(const vector<int>& nums) {
        vector<int> dp(nums.size()+1);
        dp[0]=0;
        dp[1]=nums[0];
        for (size_t i = 1; i < nums.size(); ++i) {          
          dp[i+1] = max(nums[i]+dp[i-1],dp[i]);
        }
        return dp.back();        
    }
};
```

### DFS

Now that we have our recurrence relation, we can solve this problem using DFS. But first, we must decide what our base case will be. A base case is any state where we can calculate the result without making recursive calls. In this problem, the base case could be when we have visited all nodes, return 0 because we have visited all nodes and no more steps need to be made, or the base case could be when there is only 1 unvisited node, return 0 because we have visited all other nodes and we must be standing on the final node.

Here, since we are thinking in the backward direction, we will choose the latter option. Thus, we will start our DFS at the "top", i.e. all nodes already being visited

Because the optimal path may end at any of the nodes (we don't know for sure unless we check), we will need to perform a DFS for each node. Subsequent searches will not be too expensive as we will maintain our cache throughout

You may notice that this recurrence will cause infinite cycles, because the graph is undirected. For example, if node A and node B share an edge, then (A, mask) and (B, mask) would bounce between each other infinitely. When we implement the algorithm, we can get around these cycles by caching a state's value as infinity (since the problem is asking for a minimum) the first time we visit it, and before we traverse to any other states. This ensures that if we go from (A, mask) to (B, mask) we cannot return to A until we have visited at least one new node (thus changing mask)


### BFS

However, instead of using top-down dynamic programming in the form of DFS + memoization, we will perform BFS by implementing a queue. Instead of starting at endingMask, we will start at the base case masks - only having one bit set to 1, and then work our way towards endingMask


If we are at node A and move to node B, it doesn't matter if we go B -> A -> B or A -> B - in both cases, upon arriving at B, we want our mask to have the bit corresponding to node B set as 1. This is a nice improvement on the previous approach as for each neighbor, we only need to consider 1 possibility instead of 2. Since we always want the bit to be set to 1, we will use an OR operation with 1 << neighbor to make sure the bit is set to 1


# sequences

## printing all sub-sequences

```
class Solution {
public:
  int numSubseq(std::vector<int> &nums, int target) {
    std::sort(nums.begin(), nums.end());
    return nums.empty() ? 0 : donumSubseq(nums, {}, target, 0, nums.size() - 1);
  }

  int donumSubseq(const std::vector<int> &origNums,
                  std::vector<int> numsCurrent, int target, size_t currentLevel,
                  size_t origNumsLevel) {
    if (currentLevel == origNumsLevel + 1) {
      if (!numsCurrent.empty()) {
        int min = *numsCurrent.begin();
        int max = min;
        if (numsCurrent.size() > 1) {
          max = *std::prev(numsCurrent.end());
        }
        return min + max <= target ? 1 : 0;
      } else {
        return 0;
      }
    }

    auto newNums = numsCurrent;
    newNums.emplace_back(origNums[currentLevel]);

    return donumSubseq(origNums, newNums, target, currentLevel + 1,
                       origNumsLevel) +
           donumSubseq(origNums, numsCurrent, target, currentLevel + 1,
                       origNumsLevel);
  }
};
```

## number of all sub-sequences whose sum of min + max is less or equal target

[5, 3, 1] target 12

- it has to be sorted as it would give the same result regardless (proved by induction)
   [1, 3, 5]

- now let's try broute force and construct all possible combinations
  
        [1]   
   /                                                                   |
   [1] (got [3,5 ] to consider further, which adds 2^3 to result)    [] (this path is not giving any solution now)
   /                        |
   [3]                      []
 
   /                          | 
   [5]                       []

   [1,3,5] [1,3] [3]  ...  - ALL SOLUTIONS   

   the optimal soution comes from a simple recursive solution!


   [- find the beginiing and the end of the biggest range which would work
   - assume the smallest element in this range is part of the solution! Than obviously the rest of all sub-sequences for N-1 elements would also work which is 2^N. ]
   - repeat the same inducative hypotesis for another longest range


## longest sub-sequence between two strings

### First property

LCS(X^A,Y^A) = LCS(X,Y)^A, for all strings X, Y and all symbols A, where ^ denotes string concatenation. This allows one to simplify the LCS computation for two sequences ending in the same symbol. For example, LCS("BANANA","ATANA") = LCS("BANAN","ATAN")^"A", Continuing for the remaining common symbols, LCS("BANANA","ATANA") = LCS("BAN","AT")^"ANA".

### Second property

If A and B are distinct symbols (A≠B), then LCS(X^A,Y^B) is one of the maximal-length strings in the set { LCS(X^A,Y), LCS(X,Y^B) }, for all strings X, Y.

For example, LCS("ABCDEFG","BCDGK") is the longest string among LCS("ABCDEFG","BCDG") and LCS("ABCDEF","BCDGK"); if both happened to be of equal length, one of them could be chosen arbitrarily.

To realize the property, distinguish two cases:

If LCS("ABCDEFG","BCDGK") ends with a "G", then the final "K" cannot be in the LCS, hence LCS("ABCDEFG","BCDGK") = LCS("ABCDEFG","BCDG").
If LCS("ABCDEFG","BCDGK") does not end with a "G", then the final "G" cannot be in the LCS, hence LCS("ABCDEFG","BCDGK") = LCS("ABCDEF","BCDGK").

#### recursive solution

```
int longestCommonSubsequence(string text1, string text2) {  
        vector<vector<int>> dp;
        size_t n = text1.size();
        size_t m = text2.size();
        dp.resize(n, vector<int>(m,-1));  
        return solve(0, 0, text1, text2, dp);                 
     }
    
    int solve(int n, int m, string text1, string text2, auto& dp) {
        if (text1.empty() || text2.empty())
            return 0;        
        
        int result = dp[n][m] ;
        if (result != -1)
            return result;
        
        //assume first latter in text1 is not part of optimal solution
        int first_not_included = solve(n+1,m,text1.substr(1,text1.size()-1), text2, dp);
        int first_included = 0;
        
        auto first_char_pos = text2.find_first_of(text1.front());
        if (first_char_pos != string::npos)
        {
            size_t text2_from_pos = first_char_pos + 1;
            first_included = 1 + solve(
                n+1,m+text2_from_pos,
                text1.substr(1,text1.size()-1), 
                text2.substr(text2_from_pos, text2.size()-text2_from_pos),
                dp); 
        } 
        
        result = max(first_not_included, first_included);
        dp[n][m]=result;
        return result;
    }
```

```
    int longestCommonSubsequence(string text1, string text2) {  
        size_t n = text1.size();
        size_t m = text2.size(); 
        
        vector<vector<int>> dp;
        dp.resize(n+1, vector<int>(m+1,0));          
        for (int i = n - 1; i >= 0; i--)
        {
            for (int j = m - 1; j >= 0; j--)
            {
                if (text1[i] == text2[j])
                {
                    dp[i][j] += dp[i+1][j+1] + 1;                    
                }
                else
                {
                    dp[i][j]=max(dp[i][j+1], dp[i+1][j]);
                }
            }
        }        
        
        return dp[0][0];                 
     }


```

## longest increasing sub-sequence

The idea is to track longest increasing sub-sequence . what about if later you get sub-sequence with smaller items but longer, well, just replace previous sub-sequence items...crazy clever trick which can be improved with BS


```
class Solution {
    public int lengthOfLIS(int[] nums) {
        ArrayList<Integer> sub = new ArrayList<>();
        sub.add(nums[0]);
        
        for (int i = 1; i < nums.length; i++) {
            int num = nums[i];
            if (num > sub.get(sub.size() - 1)) {
                sub.add(num);
            } else {
                // Find the first element in sub that is greater than or equal to num
                int j = 0;
                while (num > sub.get(j)) {
                    j += 1;
                }
                
                sub.set(j, num);
            }
        }        
        return sub.size();
    }
}
```







    

  