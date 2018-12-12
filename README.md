# Synopsis
Linear Algebra CLI is an interpreter, written in C, that manipulates matrices to calculate properties such as eigenvalues and determinants, which are then used to solve differential equations. The project was inspired as a way to study for two classes at Ohio State, CSE 2421 and MATH 2568. It also provided an opportunity to learn about interpreters as well as to architect a small program.

### How to install
1. Clone this repository
1. Run make on the root directory(may only work on linux)
1. If there are any errors, report it in the issues section
    1. Frequent build error involves ld not linking math.h. Try editing the Makefile so that -lm is in a different location.

### General Usage
#### Variables
To define a constant  
```
var a = 1
```  
To define a matrix
```
var b = [[1,2], [1,2]]
```  

To print the value of a variable
```
var a = 1
print a
```

##### Arithmetic
The program can do most simple arithmetic such as
```
var a = 1
var b = a + 2
var c = (b+a)/b
```

##### Functions
To call a function  
```
var a = [[1,2],[1,2]]
var arref = rref(a)
```

##### Matrix functions
* rref(matrix) - transforms to reduced row echelon form
* inverse(matrix) - inverses the matrix
* I(n) - produces an identity matrix with dimension nxn
* aug(matrix...) - creates an augmented matrix by combining a set of matrices
* transpose(matrix1) - the transpose of a matrix
* evec(matrix) - calculates eigenvectors of the matrix
* basis(v1,...,v2) - calculates a basis for a set of vectors
* null(matrix) - calculates the null space of a matrix
* jnform(matrix) - calculates the jordan normal form of a matrix

##### Value functions
* rank(matrix) - the rank of the matrix. First reduces to rref
* det(matrix) - the determinant of the 2x2 matrix
* trace(matrix) - the trace of the matrix
* eval(matrix) - finds the eigenvalues of the matrix
* dim(cols, ncols) - the dimension of a set of column vectors
* disc(matrix) - the discriminant of a matrix
* linind(cols) - determines whether a set of column vectors are linearly independent
* nullity(matrix) - the nullity of a matrix
* sss(matrix) - determines whether a 2x2 matrix is a Sink, Saddle, or Source
* isstable(matrix) - determines whether a 2x2 matrix is linearly stable
* diffsolve(matrix) - solves a 2x2 differential equation
* equal(val, val) - determines whether two values are equal


### Coming soon
1. diffsolve(matrix) - solves the matrix of form dx/dt = CX
1. Chapter 6
    1. issimilar
    1. classifyjnf
1. Chapter 7
    1. nxn eigenvalues
1. Chapter 8
    1. linearmap(a, b)
    1. nullspace(lmap)
    1. range(lmap)
    1. coordinates(matrix)
1. Help menu
1. Autotab functions
1. History

### Dev notes
The REPL was inspired and mostly designed off of this tutorial
http://www.craftinginterpreters.com/

##### Bugs
1. Currently cannot have a [[x1, x2, x3]] matrix since 1 row will not read the rows property of a matrix, instead it will read the literals property of the matrix
