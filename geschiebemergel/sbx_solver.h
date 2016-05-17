#ifndef SBX_SOLVER_H_
#define SBX_SOLVER_H_

#include "sbx_matrix.h"
#include "sbx_double.h"
#include "sbx_stream.h"
#include "sbx_equation.h"


namespace sbx {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // class: Solver
    // Task:
    //         Parent class for solver of Ax=b

	class Solver
	{
	protected:

        Matrix* A; // gives individual solver access to Equation member *A, *b, *x by declaring Solver as friend
        Matrix* b;
        Matrix* x;

		int type;
		Double error;
		int maxIterations;
		Double(*norm)(Matrix*);

		void set_matrix_pointer( Equation* eqn ) { A = eqn->A; b = eqn->b; x = eqn->x; }
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Class Bcgs
	// Task:
	//         biconjugate gradient method

	class Bcgs : public Solver
	{
		Double r_rs, A_p_rs;
		Double alpha, beta, omega;
		Matrix r, s, p, rs;
		Matrix A_s, A_p;

	public:
		Bcgs ( int _type, Double _error, int _maxIterations, Double(*_norm)(Matrix*) )
		{
			type = _type;
			error = _error;
			maxIterations = _maxIterations;
			norm = _norm;
		}

		void execute( Equation* );

		template <typename T> void writeIterationValues( int iterationNDX, Double runningError, Matrix* result, T& outstream )
			{
				show("    Iterations", (double)iterationNDX, 1, outstream);
				show("        Error", runningError, 1, outstream);

				show("        x",!(*result), 2, outstream);
				show("        p",!p, 2, outstream);
				show("        r",!r, 2, outstream);
				show("        s",!s, 2, outstream);
				show("        alpha",alpha, 2, outstream);
				show("        beta",beta, 2, outstream);
				show("        omega",omega, 2, outstream);
			}
	};

	void Bcgs::execute( Equation* eqn )
	{
		inform( "Solver bcgs" ,0, CONSOLE );
		set_matrix_pointer( eqn );
		int i=0;
		Double runningError = 0.;

		alpha = beta = omega = 0.;
		s=0.;  // not assignment?
		r = *b - *A * *x;
		p = r;
		rs = r + *x;
		writeIterationValues( i, runningError, x, CONSOLE );

		for( ; i < maxIterations; i++ )
		{
			A_p = *A * p;
			r_rs = (r|rs);
			A_p_rs = (A_p|rs);
			alpha = r_rs/(A_p_rs);
			s = r - alpha * A_p;
			A_s = *A * s;
			omega = (A_s|s)/(A_s|A_s);
			*x = *x + alpha * p + omega * s;
			r = s - omega * A_s;
			beta = (r|rs) * alpha / (r_rs * omega);
			p = r + beta * (p - omega * A_p);

			runningError = norm(&r)/ norm(b);
			writeIterationValues( i, runningError, x, CONSOLE );
			if ( fabs(runningError) < error)
				break;
		} // end for

		show( "Iterations", (double)i, 0, CONSOLE );
		show( "Error", runningError, 0, CONSOLE );
	}

}


#endif /* SBX_SOLVER_H_ */
