
# Sliding window

int lengthOfLongestSubstringTwoDistinct(string s) {
        int res{}, start{};
        unordered_map<char, int> m;

        for(int i = 0; i < s.size(); i++) {
            m[s[i]]++;
            if(m.size() < 3) res = std::max(res, i - start + 1);
            while(m.size() > 2) if(!--m[s[start++]]) m.erase(s[start-1]);
        } return res;
    }


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







    

  