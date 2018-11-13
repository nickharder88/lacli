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
* rank(e) - the rank of the matrix. First reduces to rref.
* det(e) - the determinant of the 2x2 matrix
* trace(e) - the trace of the matrix

### Coming soon
1. Constant multiplication of matrices
1. Assigning result to variables
1. don't invert 2x2 matrices with 0 determinant
1. row equivalence
1. null(e)
1. evalue(matrix)
1. evec(matrix, name)

### Valgrind
```bash
valgrind --leak-check=yes debug
```
