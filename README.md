### Synopsis

Linear Algebra CLI that computes functions based off of matrices input by the user.

## Functions
rref(matrix, optional:newname)
Takes a matrix and transforms to reduced row echelon form. If a second argument is supplied, then the rref matrix is saved under that name, conserving the original matrix.

## Coming soon
rank(e)
det(e)
inverse(e)
transpose(e)
null(e)
identity(int) -> returns identity matrix of int x int

# Valgrind
valgrind --leak-check=yes debug
