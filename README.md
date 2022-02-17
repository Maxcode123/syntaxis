# syntaxis
Syntax analyzer that recognizes a language with only two tokens (a left one and a right one). A valid expression must contain the same number of the two tokens. A valid expression as read from the left must not contain more right tokens than left ones.

## Compile & run
Run and tested using g++ compiler, version 9.3.0\
Running `g++ syntax.cpp -o syntax && ./syntax` outputs the below:\
![image](https://user-images.githubusercontent.com/64724733/154538573-990695b3-d437-4485-8e02-4369d613bf81.png)\
Then the user can enter a token stream and see the stack content, state and remaining stream in each step.\
![image](https://user-images.githubusercontent.com/64724733/154539006-b1bbf558-69c7-491d-b65e-91124bb9dd31.png)

