#include <vector>
#include <array>
#include "../../geometry.h"
#include "geom_test_core.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main()
{
	// Examples
	/*ex_single_square();
	ex_single_circle();
	ex_step();
	*/

	//ex_reg_squares();
  	//ex_stag_rectancles();
	
	//ex_stag_circ();
	//ex_reg_ellipses();

	//ex_read_write();

	// Various geom testing with vis
	//single_rectangle_test();
	//single_ellipse_test();
	
	// Arrays
	//rectangle_array_test();	
	//square_array_test();
	//ellipse_array_test();
	//circle_array_test();

	// Can also check validity between tested and new solid node fractions
	// Walls
	//wall_test();

	// Staggered arrays
	//staggered_square_array_test();
	//staggered_circular_array_test();

	// Test reading geometry from file
	read_test();

	// Test bound checking - not idea right now
	/*std::string s("rectangle");
	bool bounds = test_object_bounds(s);
	cout << bounds << endl;*/
}
