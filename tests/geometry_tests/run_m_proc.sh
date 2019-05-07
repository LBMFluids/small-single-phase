#!/bin/bash
/Applications/MATLAB_R2018b.app/bin/matlab -nodisplay -nojvm -nosplash -nodesktop -r \
		"try, proc_arr('$1','$2'), catch e, disp(getReport(e)), exit(1), end, exit(0);"
	  echo "matlab exit code: $?"
