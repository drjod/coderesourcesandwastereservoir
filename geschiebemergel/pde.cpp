#include <iostream>

typedef std::size_t SIZE_T;

#include "sbx_clock.h"
#include "sbx_stream.h"
#include "sbx_solver.h"
#include "sbx_equation.h"
#include "sbx_matrix.h"
#include "matrix_class.h"
#include <string>
using namespace std;
using namespace sbx;




int main(int argc, char** argv)
{
	SIZE_T size =  1000;
    sbx::Clock time;
    //Output result( "result.txt" );
    Bcgs solver( 1.e-5, 10000, norm0 ); // error, max iterations
    //Gauss solver;
    Equation* groundwater = new Equation( size ); // number of finite difference nodes

    time.start("Assembly");
    groundwater->assemble( diffusion_1D );
    groundwater->incorporate_boundary_conditions( fixed_heads );
    time.measure( CONSOLE );
    show( "A",groundwater->get_matrix(), CONSOLE, 2 );
    show( "b",groundwater->get_RHS(), CONSOLE, 2 );


    time.start("Solver");
    solver.execute( groundwater );
    time.measure( CONSOLE );

    show( "Result x",groundwater->get_result(), CONSOLE, 0 );
    //result.write( "x", groundwater.get_result() );

    delete groundwater;
    return 0;
}
