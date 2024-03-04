
# plan

- setup dev env
 * download project gateway from git 
 * add it to VS workspace as another folder
 * using CMake plugin generate the cmake files and *SELECT IT AS ACTIVE PROJECT*
 * build from VS Code
 * copy over compile_commands.json to the root directory for error checking to work in VS code and vim
 * refresh testmate cases!
 * get new command working for vim
- make sure the task is clear
- confirm with the other person exact requirnemtn or if something is not clear. Also make sure that outcome of the converstation is attached when you submit a task

# questions about the task

1) if we already have 5 levels and suddenly another level comes at the top of the book, then 5th gets implicitly removed, but if the top of the book gets deleted, the implicitly deleted 6th level must come back again right?
2) result output to standard output is ok rather than file?
3) in the worse case, if we only get data for the first 4 layers, we'll never have a chance to see data for 5th level. Should I print the output as soon as possible while book is not fully recovered? Or the assumption is that at at some point the book could be fully recovered from *input data* and we shall start producing output only once we are sure that the book has been fully recovered?
4) what is size & seq num max value ? should be 64 bit unsigned ? for price, double is ok?



# potentiall improvements

# how to compile manually

 g++ -std=c++20  -Wall -Werror -Wextra -Og -std=c++20 -fsanitize=address   ./app.cpp

# questions  to Matt

1) Interviews were coding heavy, though I did not get C++ questions. Any particular reason why? Throught the process the interviews could without a doubt implicitly gauge my level of C++ to some extent, but without deeper C++ questions it would be not as precisese.
2) What are the biggest issues / problems / concerns you are facing now? It could be anything, not necesseraliy in tech stack
3) How do you see your tech stack evolving in the next years?
4) Where do see the role brining value for the team?
5) how is task distribution & discovery done in the team?
 
# what they may ask me

