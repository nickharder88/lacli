# Synopsis

Linear Algebra CLI that computes functions based off of matrices input by the user.

### Functions
If a function has the last parameter optional:newname, then it will give the resulting matrix the name supplied in the parameter list if given, otherwise it will use the FIRST matrix in the argument list's name.  

* rref(matrix, optional:newname) - transforms to reduced row echelon form
* inverse(matrix, optional:newname) - inverses the matrix
* I(n, name) - produces an identity matrix with dimension nxn
* aug(matrix1, matrix2, optional:newname) - creates an augmented matrix by combining matrix1 and matrix 2 

### Coming soon
rank(e)
det(e)
transpose(e)
null(e)

### Valgrind
```bash
valgrind --leak-check=yes debug
```
