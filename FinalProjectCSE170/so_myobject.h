
// Ensure the header file is included only once in multi-file projects
# pragma once

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <cmath> 
# include "visualc14/gsim/gs_image.h"
# include <gsim/gs_light.h>
# include <gsim/gs_material.h>
# include <cstring>
// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoMyObject : public GlObjects
 { private :
    GlShader _vsh, _fsh;
    GlProgram _proggouraud;
	GsMaterial _mtl;

    GsArray<GsVec>   P; // coordinates
    GsArray<GsVec>   N; // normals
	GsArray <GsVec2>  T; // Texture Coordinates
    int _numelements;
	const float PI = 3.14159265359;
	const float theta = 2 * PI; 
 public:
	const int nfaces = 50;
	float len = 0.9f;
	float topRadius = 0.5f; 
	float bottomRadius = 0.5f; 
	float length = 0.5f; 
    // etc

   public :
    SoMyObject ();
	SoMyObject(float faces, float leng, float rt, float rb);
    void init (const char* tex_name);
    void build ( );
    void draw ( GsMat& tr, GsMat& pr, const GsLight& l, float changer );


	GsImage I; 
	gsuint id; 
 };

