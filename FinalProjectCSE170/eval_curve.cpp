# include "eval_curve.h"

So_Curve::So_Curve()
{

}

float So_Curve:: factorial (float n)
{
	if (n < 1.0f) { return 1.0f; }
	float fac = n * factorial(n - 1);
	return fac;
}
float So_Curve::calc_func (float size, float index)
{
	return factorial (size) / ( factorial (index) * factorial (size - index) );
}

float So_Curve::N(int i, int k, float u)
{
	float ui = float(i);
	if (k == 1) {  return ui <= u && u < ui + 1 ? 1.0f : 0;  }

	else
	{
		return (float(u - ui) / float(k - 1)) * N(i, k - 1, u) + (float(ui + k - u) / float(k - 1)) * N(i + 1, k - 1, u);
	}
}


void So_Curve::eval_bspline(GsArray<GsVec>& evo_curve, const GsArray<GsVec>& ctrl_pts, int amount, float _t)
{
	evo_curve.size(0);
	int size = ctrl_pts.size(); 
	float delta = float(size) / float(amount);
	puts("Got to eval_spline");
	for (float t = (3.5); t <= size; t += delta)
	{
		GsVec q(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < size; i++)
		{
			q += ctrl_pts[i] * N(i, 3, t);
		}
		evo_curve.push() = q;
	}
}
void So_Curve::eval_lagrange(GsArray<GsVec>& evo_curve, const GsArray<GsVec>& ctrl_pts, int amount)
{
	evo_curve.size(0);	//:: Clear vector of previous formulations

	float delta = ((float)ctrl_pts.size() - 1) / float(amount); //:: Rate of Change Delta (Parameter)
	
	//:: Lagrange Formulation
	for (float t = 1; t <= ctrl_pts.size(); t += delta)
	{
		GsVec y (0.0f,0.0f,0.0f); 	

		for (int i = 1; i <= int(ctrl_pts.size()); i++)
		{
			float b1, b2; b1 = 1.0f; b2 = 1.0f; 

			for (int j = 1; j <= int(ctrl_pts.size()); j++)
			{
	
				if (j == i) { continue; }

				b1 *= (t - j); 
				b2 *= (i - j); 
			}

			y += ctrl_pts[i - 1] * (b1 / b2);
		
		}
		evo_curve.push() = y;
	}

}

void So_Curve::eval_bezier(GsArray<GsVec>& evo_curve, const GsArray<GsVec>& ctrl_pts, int amount)
{
	evo_curve.size(0);	//:: Clear vector of previous formulations

	for (int t = 0; t < amount; t++)
	{
		GsVec y (0.0f, 0.0f, 0.0f);					//:: GsVec y to hold summation of the Bezier Equation
		float x = (float(t) / float(amount - 1));	//:: Parameter x representing t in the Bezier Equation
		for (int i = 0; i < ctrl_pts.size(); i++)
		{
			y += ctrl_pts[i] *  (float(calc_func(float(ctrl_pts.size() - 1), float(i))) * powf(x, i) * powf(1 - x, ctrl_pts.size() - i - 1));	//:: Bezier Formulation	
		}
		evo_curve.push() = y;
	}

}


void So_Curve::eval_catmull(GsArray<GsVec> &evo_curve, const GsArray<GsVec> &ctrl_pts, int amount)
{

	int size = ctrl_pts.size();
	evo_curve.size(0);
	//evo_curve.size(size);

	GsVec p_pos, p_neg, I;

	GsArray<GsVec> newArray; 


	newArray.push() = ctrl_pts[0];
		for (int i = 1; i < size - 1; i++)
		{
			I = (ctrl_pts[i + 1] - ctrl_pts[i - 1]) / 2.0f;
			p_neg = ctrl_pts[i] - (I / 3.0f);
			p_pos = ctrl_pts[i] + (I / 3.0f);
			//evo_curve[i - 1] = p_neg;
			//evo_curve[i] = I;
			//evo_curve[i + 1] = p_pos;
			newArray.push() = p_neg;
			newArray.push() = ctrl_pts[i];
			newArray.push() = p_pos;
		}
		newArray.push() = ctrl_pts[size-1];
		//eval_bspline(evo_curve, newArray, amount);
}

void So_Curve::eval_bessel(GsArray<GsVec> &evo_curve, const GsArray<GsVec> &ctrl_pts, int amount)
{

		evo_curve.size(0); 
		int size = ctrl_pts.size(); 
		evo_curve.size(size);

		GsArray<GsVec> newArray; 
		newArray.push() = ctrl_pts[0];

		for (int i = 1; i < size - 1; i++)
		{

			float neg_dis, pos_dis;

			neg_dis = std::sqrt(std::powf((float(ctrl_pts[i].x) - float(ctrl_pts[i - 1].x)), 2.0f) + std::powf((float(ctrl_pts[i].y) - float(ctrl_pts[i - 1].y)), 2.0f));
			pos_dis = std::sqrt(std::powf((float(ctrl_pts[i + 1].x) - float(ctrl_pts[i].x)), 2.0f) + std::powf((float(ctrl_pts[i + 1].y) - float(ctrl_pts[i].y)), 2.0f));

			GsVec v_hatP, v_hatN;

			v_hatP = (ctrl_pts[i + 1] - ctrl_pts[i]) / pos_dis;
			v_hatN = (ctrl_pts[i] - ctrl_pts[i - 1]) / neg_dis;
			GsVec V;
			V = ((pos_dis * v_hatN) + (neg_dis * v_hatP)) / (pos_dis + neg_dis);

			//evo_curve[i - 1] = ctrl_pts[i] - ((1 / 3) * neg_dis * V);
			//evo_curve[i] = ctrl_pts[i] + ((1 / 3) * pos_dis * V);
			newArray.push() = ctrl_pts[i] - ((1.0f / 3.0f) * neg_dis * V);
			newArray.push() = ctrl_pts[i];
			newArray.push() = ctrl_pts[i] + ((1.0f / 3.0f) * pos_dis * V);
		}

		newArray.push() = ctrl_pts[size - 1];

		//eval_bspline(evo_curve, newArray, amount);
}