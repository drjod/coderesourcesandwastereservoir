#ifndef SBX_SOLVER_H_
#define SBX_SOLVER_H_



#include "sbx_stream.h"
#include "sbx_equation.h"
#include "sbx_matrix.h"

struct Solver_error {
    std::string name;
    Solver_error( const char* q ) :name(q) { }
    Solver_error( std::string n ) :name(n) { }
};

//-----------------------------------------------------------------------------

inline void solver_error( const char* p )
{
    throw Solver_error(p);
}


namespace sbx {


    class Solver
    {
    protected:

    	Matrix* A;
        Vector* b;
        Vector* x;

        double error;
        int maxIterations;
        double(*norm)(const Vector&);

        void set_matrix_pointer( Equation* eqn ) {
        	A = eqn->A; b = eqn->b; x = eqn->x;
        }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Class Bcgs
    // Task:
    //         biconjugate gradient method

    class Bcgs : public Solver
    {
        double r_rs, A_p_rs;
        double alpha, beta, omega;
        Vector r, s, p, rs;
        Vector A_s, A_p;

    public:
        Bcgs ( double _error, int _maxIterations, double(*_norm)(const Vector&) )
        {
            error = _error;
            maxIterations = _maxIterations;
            norm = _norm;
        }

        void execute( Equation* );

        template <typename T> void writeIterationValues( int iterationNDX, double runningError, T& outstream )
            {
                //show( "    Iterations", (double*)iterationNDX, outstream, 1 );
                show( "        Error", &runningError, outstream, 1 );

                show( "x",x, outstream, 2 );
                show( "p",&p, outstream, 2 );
                show( "r",&r, outstream, 2 );
                show( "s",&s, outstream, 2 );
                show( "alpha",&alpha, outstream, 2 );
                show( "beta",&beta, outstream, 2 );
                show( "omega",&omega, outstream, 2 );
            }
    };

    void Bcgs::execute( Equation* eqn )
    {
        inform( "Solver bcgs" , CONSOLE, 0 );
        set_matrix_pointer( eqn );
        int i=0;
        double runningError = 0., norm_b;

        r.allocate( eqn->get_size() );
        s.allocate( eqn->get_size() );
        p.allocate( eqn->get_size() );
        rs.allocate( eqn->get_size() );
        A_p.allocate( eqn->get_size() );
        A_s.allocate( eqn->get_size() );

        alpha = beta = omega = 0.;
        s = 0.;
        *x = 1.;

        multiply(*A,*x, r);
        r = *b - r;
        p = r;
        rs = r + *x;
        writeIterationValues( i, runningError, CONSOLE );

        for( ; i < maxIterations; i++ )
        {
        	multiply(*A,p, A_p);
            r_rs = dot_product(r,rs);
            A_p_rs = dot_product(A_p,rs);
            alpha = r_rs/(A_p_rs);
            s = r - A_p * alpha;
            multiply(*A,s,A_s);
            omega = dot_product(A_s,s)/dot_product(A_s,A_s);
            *x = *x + p * alpha  + s * omega;
            r = s - A_s * omega;
            beta = dot_product(r,rs) * alpha / (r_rs * omega);
            p = r + (p - A_p * omega) * beta;
            norm_b = norm(*b);
            runningError = norm(r)/ norm_b;

            if (A_p_rs == 0 || r_rs == 0 || omega == 0 || norm_b == 0)
            	solver_error("Division by zero in Bcgs::execute");

            writeIterationValues( i, runningError, CONSOLE );
            if ( fabs(runningError) < error)
                break;
        } // end for

        show( "Iteration", &i, CONSOLE, 0 );
        show( "Error", &runningError, CONSOLE, 0);
    }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Class Gauss
        // Task:
        //         Gauss elimination

        class Gauss : public Solver
        {
             SIZE_T size;

        public:
            Gauss () {}

            void execute( Equation* );

            template <typename T> void writeIterationValues( int iterationNDX, T& outstream )
            {
                double* ptr = new double;  // to cast int
                *ptr = iterationNDX;
                show( "    Iterations", ptr, outstream, 1 );

                show( "A", A, outstream, 2 );
                show( "b", b, outstream, 2 );

                delete ptr;
            }
        };

        void Gauss::execute( Equation* eqn )
        { // TODO: go to raw data on matrix_base

            inform( "Solver Gauss" , CONSOLE, 0 );
            set_matrix_pointer( eqn );
            size = A->dim2(); // n x n
            double factor, temp;
            double __A;
            // elimination - mode 0
            for ( SIZE_T p = 0; p < size-1; p++) // pivot
            {
                for ( SIZE_T i = p+1; i < size; i++)
                {
                    if( (*A)(i, p ) != 0 )
                    {
                    	__A = (*A)(p, p);
                    	if ( __A == 0 )
                    	    solver_error("Division by zero in Gauss elemination");
                        factor = (*A)(i, p) / __A;
                        for ( SIZE_T j = p+1; j < size; j++)
                        {
                        	(*A)(i, j) = (*A)(i, j) - factor * (*A)(p,j);
                        }
                        (*b)(i) =  (*b)(i) - factor * (*b)(p) ;
                    }
                }

                writeIterationValues( p, CONSOLE );
            }
            // substitution - mode 1
            for ( SIZE_T i = size-1; i >=0; i--)
            {
                temp = 0;
                for ( SIZE_T j = i+1; j < size; j++)
                    temp += (*A)(i,j)* (*x)(j);
                __A = (*A)(i, i);
                if ( __A == 0 )
                    solver_error("Division by zero in Gauss substitution");
                (*x)(i) = ( (*b)(i) - temp  ) /  __A ;
            }

        }
}


#endif /* SBX_SOLVER_H_ */
