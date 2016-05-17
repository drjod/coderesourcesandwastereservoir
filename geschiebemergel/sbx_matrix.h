#ifndef SBX_MATRIX_H_
#define SBX_MATRIX_H_

#include <iostream>
#include <stdlib.h>
#include "sbx_double.h"

namespace sbx {

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
	//    class Matrix
	//
	//
	//

    class Matrix
    {
	    long rows, columns;
	    Double** elems;

    public:
	    // con-/destructors
	    Matrix() { rows = 0; columns = 0; }
   	    Matrix( long _rows ) : rows( _rows ), columns( _rows ), elems( new Double*[_rows] ) // symmetric
        {
		    for(long i=0; i< _rows; i++)
    	        elems[i] = new Double[_rows];
        }


   	    Matrix( long _rows, long _columns ) : rows( _rows ), columns( _columns ), elems( new Double*[_rows] )
        {
		    for(long i=0; i< _rows; i++)
    	        elems[i] = new Double[_columns];
        }

	    Matrix( long _rows, long _columns, Double value ) : rows( _rows ), columns( _columns ), elems( new Double*[_rows] ) // with initialization
        {
		    for( long i=0; i< _rows; i++ )
		   	    elems[i] = new Double[_columns];

            for( long i=0;  i < _rows; i++ )
        	    for( long j=0; j< _columns; j++ )
        	  	    elems[i][j] = value;
        }

	    Matrix( const Matrix& other ) : rows( other.get_rows() ), columns( other.get_columns() ), elems( new Double*[other.get_rows()] ) // copy Matrix
	    {
		    for( long i=0; i< other.get_rows(); i++ )
			    elems[i] = new Double[other.get_columns()];

		    for( long i=0;  i < other.get_rows(); i++ )
			    for( long j=0; j< other.get_columns(); j++ )
			        elems[i][j] = other.at( i,j );
	    }

	    Matrix( Double value ) : rows( 1 ), columns( 1 ), elems( new Double*[1] ) // for cast
	    {
			elems[0] = new Double[1];
			elems[0][0] = value;
	    }

	    ~Matrix( void ) { delete [] elems; }
	    /////
	    Double at( long row, long column ) const { return elems[row][column]; }
	    void set( long row, long column, Double value ) { elems[row][column] = value; }
	    Double** get_elems( void ) { return elems; }
	    long get_rows( void ) const { return rows; }
	    long get_columns( void ) const { return columns; }
	    Matrix unity( void ) {
	    	 for( long i=0; i<rows; i++ )
	    	        	for( long j=0; j<columns; j++ )
	    	        	    if( i == j )
	    	        		    elems[i][j] = 1.;
	    	        	    else
	    	        		    elems[i][j] = 0.;
             return *this;
	    }

	    Matrix& operator=( const Matrix&);
        Matrix operator-();
        Matrix operator!(); // transpose

        friend Matrix operator+( const Matrix&, const Matrix& );
        friend Matrix operator-( const Matrix&, const Matrix& );
        friend Matrix operator*( const Matrix&, const Matrix& );
        friend Double operator|( const Matrix&, const Matrix& ); // inner product

        friend std::ostream& operator<<( std::ostream&, const Matrix& );
        friend std::istream& operator>>( std::istream&, Matrix& );

        void assemble( long row, long locationOfFirstColumn, long numberOfColumns, Double* values )
		{
			for(long i = 0; i< numberOfColumns; i++ )
	   			elems[row][locationOfFirstColumn+i]  = values[i];
		}
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Assignment

    Matrix& Matrix::operator=( const Matrix& other )
	{
		if (this == &other)
			return *this;

		Double **ptr= new Double*[other.get_rows()];
		for( long i=0; i< other.get_rows(); i++ )
			ptr[i] = new Double[other.get_columns()];

		for( long i=0;  i < other.get_rows(); i++ )
			for( long j=0; j< other.get_columns(); j++ )
				ptr[i][j] = other.at( i,j );

		for( long i=0;  i < rows; i++ )
			delete [] elems[i];

		rows =  other.get_rows();
		columns =  other.get_columns();

		elems = new Double*[rows];
		for( long i=0; i< rows; i++ )
			elems[i] = new Double[columns];

		for( long i=0; i< rows; i++ )
				elems[i] = ptr[i];

		return *this;
	}

    Matrix Matrix::operator-()
	{
		 Matrix C(rows, columns);

		 for( long i=0; i<rows; i++ )
			 for( long j=0; j<columns; j++ )
				 C.set(i,j,-elems[i][j]);

		 return C;
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Transpose


    Matrix Matrix::operator!()
   	{
   		 Matrix C(columns, rows);

   		 for( long i=0; i<columns; i++ )
   			 for( long j=0; j<rows; j++ )
   				 C.set(i,j,elems[j][i]);

   		 return C;
   	}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Matrix operator+( const Matrix& A, const Matrix& B )
    {
        Matrix C( A.get_rows(),A.get_columns() );

        for( long i=0;  i < A.get_rows(); i++ )
            for( long j=0; j< A.get_columns(); j++ )
	            C.set( i,j, A.at(i,j) + B.at(i,j) );

	    return C;
     }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Matrix operator-( const Matrix& A, const Matrix& B )
    {
        Matrix C( A.get_rows(),A.get_columns() );

        for( long i=0;  i < A.get_rows(); i++ )
            for( long j=0; j< A.get_columns(); j++ )
	            C.set( i,j, A.at(i,j) - B.at(i,j) );

	    return C;
     }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Matrix operator*( const Matrix& A, const Matrix& B )
    {

    	if (A.get_rows() == 1 && A.get_columns() == 1)
    	{  // Double * Matrix
    		Matrix C(B.get_rows(), B.get_columns() );
    		Double value = A.at(0,0);

    		for( long i = 0;  i < C.get_rows(); i++ )
    			for( long j=0; j< C.get_columns(); j++ )
    				C.set( i,j, value * B.at(i,j) );

    		return C;
    	}
    	else
    	{ // matrix multiplication
			Matrix C(A.get_rows(), B.get_columns() );
			Double value;

			for( long i = 0;  i < C.get_rows(); i++ )
				for( long j=0; j< C.get_columns(); j++ )
				{
					value = 0;
					for( long k=0; k < A.get_columns(); k++ )
						value +=  A.at(i,k) * B.at(k,j);
					C.set( i,j, value );
				}
			return C;
			}

    }

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //
   // Inner product


   Double operator|( const Matrix& A, const Matrix& B)
   {
       if (A.get_rows() != B.get_rows() ||  A.get_columns() != B.get_columns())
		   return -1.;

	   Double innerProduct = 0;
	   for( long i=0; i<A.get_rows(); i++ )
		   for( long j=0; j<A.get_columns(); j++ )
	 	       innerProduct += A.at(i,j) *  B.at(i,j);

 	   return innerProduct;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::ostream& operator<<( std::ostream& stream, const Matrix& A)
    {
        for( long i=0;  i < A.get_rows(); i++ )
        {
    	    for( long j=0; j< A.get_columns(); j++ )
    	    	stream << "[" << i <<  "," << j << "]: " << A.at(i,j) << "  ";
    	    stream << std::endl;
    	}
    	return stream;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::istream& operator>>( std::istream& stream, Matrix& A)
    {
    	Double value;
        for( long i=0;  i < A.get_rows(); i++ )
        {
      		for( long j=0; j< A.get_columns(); j++ )
    	    {
      			std::cout << "[" << i<< "," << j << "]: ";
    	    	stream >> value;
    	    	A.set(i,j, value);
    	   	}
    	}
        return stream;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // NON-MEMBER FUNCTIONS

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Unit matrix

    Matrix unity(long rows, long columns=-1)
    {
    	if (columns == -1)
    		columns = rows; // symmetric

        Matrix C(rows, columns);
        for( long i=0; i<rows; i++ )
        	for( long j=0; j<columns; j++ )
        	    if( i == j )
        		    C.set(i,i,1.);
        	    else
           		    C.set(i,i,0.);
        return C;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Norms

    Double norm_0( Matrix* A ) // max norm
	{
	    Double value = 0;
	    Double ** elems = A->get_elems();

		for( long i=0; i<A->get_rows(); i++ )
    	    for( long j=0; j<A->get_columns(); j++ )
    	    {
    	    	if( fabs(elems[i][j]) > value )
    	    		value = fabs(elems[i][j]);
			}
		return value;
	}

	Double norm_1( Matrix* A )
	{
		Double value = 0;
		Double ** elems = A->get_elems();

		for( long i=0; i<A->get_rows(); i++ )
    	    for( long j=0; j<A->get_columns(); j++ )
    	    {
    	    	if( elems[i][j] > 0)
    	    		value += elems[i][j];
    	    	else
    	    		value -= elems[i][j];
			}
		return value;
	}

}
#endif /* SBX_MATRIX_H_ */
