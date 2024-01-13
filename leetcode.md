
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


```
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        size_t n = text1.size();
        size_t m = text2.size();

        vector<vector<int>> s(n+1, vector<int>(m+1));
        for (int i = 0; i <= n; ++i) {
            for (int j = 0; j <=m; ++j) {
                if (i == 0 || j == 0) {
                    continue;
                } else if (text1[i-1] == text2[j-1]) {
                    s[i][j] = s[i-1][j-1]+1;
                } else {
                    s[i][j] = std::max(s[i-1][j],s[i][j-1]);
                }
            }
        }

        return s[n][m];        
    }
};

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







    

  