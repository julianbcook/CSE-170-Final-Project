#pragma once
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <cmath>

using std:: powf; 

class So_Curve
{
public:
	So_Curve(); 

	void eval_lagrange( GsArray<GsVec> &evo_curve, const GsArray<GsVec> &ctrl_pts, int amount ); 
	//:: @param - evo_curve will be the curve that will showcase the lagrange curve
	void eval_bezier( GsArray<GsVec> &evo_curve, const GsArray<GsVec> & ctrl_pts, int amount );
	//:: @param - evo_curve will be the curve that will showcase the bezier curve

	void eval_bspline( GsArray<GsVec>& evo_curve, const GsArray<GsVec>& ctrl_pts, int amount, float t);
	void eval_catmull( GsArray<GsVec> &evo_curve, const GsArray<GsVec> &ctrl_pts, int amount);
	void eval_bessel( GsArray<GsVec> &evo_curve, const GsArray<GsVec> &ctrl_pts, int amount );

	float calc_func(float index, float size); 
	float factorial(float n); 
	float N(int i, int k, float t);
	
};