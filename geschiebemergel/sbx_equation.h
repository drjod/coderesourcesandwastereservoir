#ifndef SBX_EQUATION_H_
#define SBX_EQUATION_H_

#include "sbx_matrix.h"

namespace sbx {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // class: Equation
    // Task:
    //         Assembly (only 1D diffusion implemented)
    //         incorporate BCs

    class Equation
    {
    	friend class Solver; // that solver has access to A, b, x
        long size;

    protected:
        Matrix* A;
        Matrix* b;
        Matrix* x;

    public:
        Equation ( long _size ) : size(_size)
        {
        	// config
        	A = new Matrix(size,size,0.);
        	b = new Matrix(size,1,0.);
        	x = new Matrix(size,1,1.);
        }

        ~Equation() { delete A; delete b; delete x; }

        Matrix* get_result() { return x; }

        void assemble ( void(*func)( long, Matrix*) )
        {
        	func(size, A); // currently only diffusion_1D( long size, Matrix* A )
        	incorporate_boundary_conditions();
        }

        void incorporate_boundary_conditions( void )
        {
    		b->set(0,0,2.);       // inlet, h(0) = 2
    		b->set(size-1,0,1.);  // outlet, h(size-1) = 1
        }
    };

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Finite difference method - 1D diffusion equation

    void diffusion_1D( long size, Matrix* A )
    {
    	Double* values = new Double[5];
		values[0]=-1.; values[1]=2; values[2]= -1; values[3]= 1.;

		A->assemble( 0, 0, 1, values+3 );
		A->assemble( size-1, size-1, 1, values+3 );

		for(long i = 1; i < size-1; i++ )
			A->assemble( i, i-1, 3, values);
    }

}

#endif /* SBX_EQUATIONS_H_ */
