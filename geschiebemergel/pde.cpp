#include <iostream>
#include "sbx_matrix.h"
#include "sbx_solver.h"
#include "sbx_double.h"
#include "sbx_clock.h"
#include "sbx_stream.h"
#include "sbx_equation.h"


using namespace std;
using namespace sbx;

int main(int argc, char** argv)
{
	Clock time;
	Output result( "result.txt" );
	Bcgs bcgs_solver( 0, 1.e-5, 100000, norm_0 );
	Equation groundwater( 1000 ); // number of finite difference nodes = 1000

	time.start("Assembly");
	groundwater.assemble( diffusion_1D );
	time.measure( CONSOLE );

	time.start("Solver");
	bcgs_solver.execute( &groundwater );
        time.measure( CONSOLE );

        show( "Result x",!(*groundwater.get_result()), 0, CONSOLE );
        result.write( "x", *groundwater.get_result() );

	return 0;
}
