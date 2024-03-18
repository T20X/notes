1. Code should aim to be a (broadly) repeatable pattern as you scroll through it. I.e. even if the concepts/domain change, the approach to data reading/writing, use (or avoidance) of functions, member functions vs free functions, etc should remain consistent.

2. Variable names should represent their type. E.g. "OrderBook orderBook" is good, i.e. the variable represents its class. "TradeStatistics tradeManager" is bad, because either the variable is misnamed, or even worse the class is misnamed. Either way, misnaming of variables is a code smell.

3. Reduction of nesting levels (often acheived by moving nested code to separate functions) is great. Increasing of nesting levels increases mental load, and can be a source of bugs (particularly around returns that should now be continues, and also shadowed variables).

4. Strong types are great. For parameters of functions, favor "void GetPrice(DiscountLevel)" over "void GetPrice(double)", even if the discount level's underlying type is a double. The same for boolean parameters: prefer enums for clarity (at the call site) and safety.

5. Make sure that critical utility classses are unit tested really well (and it's easy to do). But here's a contraversial one: don't go overboard unit testing/mock testing interactions between classes (it's a false sense of security, the bugs you hit in production probably won't be the ones you can think of in development, you will never make the same mistake twice, and large amounts of low value tests really slow down productivity). Instead, focus on solid integration testing/pre-production testing (or feature releases to a small subset of users).

6. Create pull requests for every feature/bugfix/refactor, write up the objective, and discuss the ways that you tested it (even if no one reviews it). This is a great way to confirm you understand your change, and have given the testing enough thought.

7. Big pull requests should be turned into a short design/code walkthrough. Developers' brains can't handle a 100 file code review.

8. Code comments are out of fashion (which is a shame, I like them). But there's nothing wrong with debug log lines and runtime assertions that double as code documentation, e.g. "assert (value >= 0, 'input can't be negative')".

9. Avoid "if order == 'buy' return true else return false". Instead: "if order == 'buy' return true else if order == 'sell' return false else error". Never trust external data.

10. Be super-consistent with naming, coding style, use of containers and library functions, etc (and no need to use all language features when a subset of it is probably good enough). It might seem overly picky in code reviews, but having an extremely consistent and simple style throughout the code is easy to achieve if you remain focused on it, and pays off massively for each new hire.
