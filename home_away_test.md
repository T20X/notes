
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
- can book be crossed?
- handle bad input?
- can use int instead of unsgined one for easier error checking?
- our own orders from the book are not removed?

# potentiall improvements

# how to compile manually

 g++ -std=c++20  -Wall -Werror -Wextra -Og -std=c++20 -fsanitize=address   ./app.cpp

