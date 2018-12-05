# Synopsis

Linear Algebra CLI that computes functions based off of matrices input by the user.

### Functions
If a function has the last parameter optional:newname, then it will give the resulting matrix the name supplied in the parameter list if given, otherwise it will use the FIRST matrix in the argument list's name.  

#####Matrix functions
* rref(matrix, optional:newname) - transforms to reduced row echelon form
* inverse(matrix, optional:newname) - inverses the matrix
* I(n, name) - produces an identity matrix with dimension nxn
* aug(matrix1, matrix2, optional:newname) - creates an augmented matrix by combining matrix1 and matrix 2 
* transpose(matrix1, optional:newname) - flips values around diagonal to get transpose. must be a square matrix.

#####Value functions
* rank(matrix) - the rank of the matrix. First reduces to rref.
* det(matrix) - the determinant of the 2x2 matrix
* trace(matrix) - the trace of the matrix
* eval(matrix) - finds the eigenvalues of the matrix
* evec(matrix) - finds the eigenvectors of the matrix

### Coming soon
1. span(m)
1. dim(m)
1. basis(m)

1. diffsolve(matrix) - solves the matrix of form dx/dt = CX

### Dev notes
http://www.craftinginterpreters.com/

Currently cannot have a [[x1, x2, x3]] matrix since 1 row will not read the rows property of a matrix, instead it will read the literals property of the matrix

span doesnt work because it row reduces the | augmented side of the matrix
span fails, work on proble from page 145

### Valgrind
```bash
valgrind --leak-check=yes debug
```
