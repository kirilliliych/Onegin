# Quadratic-equation

The programme is free-to-use for all kinds of developers.
### What does the project do
***
Solves Ax^2 + Bx + C = 0 

### Why is the project useful
***
Working with physical or mathematical models, we often need to deal with quadratic equations.
It seemed to me that there should be a programme solving them as using it is more efficient and convenient rather than calculating by hands. 
Now you can get right answer without any troubles.
### How to work with
***
This programme accepts three variables:  A, B and C from Ax^2 + Bx + C = 0, so you should enter A, B and C.
Just follow the instructions appearing on the screen and you will benefit.  
**Arguments' type is double! Do not try to initialize them with symbol, string, etc. It will not work!**
If you are mistaken, a message will remember you to enter number.

Programme shows how many roots does the equation have and shows them if there are any / not an infinity of them.

### Brief structure
***
1. _void ReadCoeffs (double *first_coef, double *second_coef, double *third_coef)_ - accepts A, B and C.
2. _void EquationCoefGetRightInput (double *coefficient)_ - repeats input if there is a mistake until it is correct.
3. _int SolveQuadraticEquation (double first_coef, double second_coef, double third_coef, double *ptr_first_root, double *ptr_second_root)_ - solves the quadratic equation __(calls SolveLinearEquation if needs)__, returns number of roots.
4. _int SolveLinearEquation (double linear_coef, double constant_of_free_term, double *ptr_first_root)_ - solves the linear equation, returns number of roots. 
5. _bool IsEqualTo0 (double value)_ - decides whether double variable is equal to 0 with 0.001 precision.
6. _void PrintAnswer (int num_roots, double first_root, double second_root)_ - shows an answer.
7. _bool IsEqual (double value_1, double value_2)_ - decides whether two double variables are close enough to each other to consider they are equal with 0.001 precision.     
8. _void Swap (double *value_1, double *value_2)_ - swaps two variables (numbers).
### How can I help
***
I explicitly welcome any outside contributors. Use my mail: _ilichev.kv@phystech.edu_.
__A strong request__:
* Ask before making significant changes
* Do not break the codestyle
* Do not destruct previous functionality with additions

### Where to ask questions
***
Use the mail mentioned earlier.
