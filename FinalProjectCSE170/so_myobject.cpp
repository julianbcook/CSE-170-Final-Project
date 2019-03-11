
# include "so_myobject.h"

SoMyObject::SoMyObject()
 {
   _numelements=0;
 }

SoMyObject::SoMyObject(float faces, float leng, float rt, float rb)
	:nfaces(faces), len(leng), topRadius(rt), bottomRadius(rb){
	_numelements = 0;
}

// init programs here, this will be done only once:
void SoMyObject::init (const char* tex_name)
 {
   // Build program:
   _vsh.load_and_compile ( GL_VERTEX_SHADER,"../vsh_mcol_gouraud.glsl" );
   _fsh.load_and_compile ( GL_FRAGMENT_SHADER, "../fsh_gouraud.glsl");
   _proggouraud.init_and_link ( _vsh, _fsh );

   // Define buffers needed:
   gen_vertex_arrays ( 1 ); // will use at least 1 vertex array
   gen_buffers ( 3 );       // will use at least 1 buffer
   //_prog.uniform_locations ( .. ); // declare here uniforms
   //_prog.uniform_location ( 0, "vTransf" ); // each name must appear in the shader
   //_prog.uniform_location ( 1, "vProj" );

   glGenTextures(1, &id); // generated ids start at 1
   _proggouraud.uniform_locations(11); // will send 9 variables
   _proggouraud.uniform_location(0, "vTransf");
   _proggouraud.uniform_location(1, "vProj");
   _proggouraud.uniform_location(2, "lPos");
   _proggouraud.uniform_location(3, "la");
   _proggouraud.uniform_location(4, "ld");
   _proggouraud.uniform_location(5, "ls");
   _proggouraud.uniform_location(6, "ka");
   _proggouraud.uniform_location(7, "kd");
   _proggouraud.uniform_location(8, "ks");
   _proggouraud.uniform_location(9, "sh");
   _proggouraud.uniform_location(10, "Tex1");
   _proggouraud.uniform_location(11, "changer");

   if (!I.load(tex_name)) { std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1); }


   glBindTexture(GL_TEXTURE_2D, id);
   glTexImage2D(GL_TEXTURE_2D, 0, 4, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data());
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   I.init(0, 0);

   glBindVertexArray(0);
   //...
 }

// fill your arrays here, may be called each time the object has to change:
void SoMyObject::build (  )
 {

	P.size(0); T.size(0); N.size(0);
   for (int i = 0; i <= nfaces; i++)
   {
	   float x_coord = topRadius * cos(i * (theta / nfaces));	float x_coordTopRight = topRadius * cos((i + 1) * (theta / nfaces));
	   float y_coord = topRadius * sin(i * (theta / nfaces));	float y_coordTopRight = topRadius * sin((i + 1) * (theta / nfaces));

	   float x_coordBtm = bottomRadius * cos(i * (theta / nfaces));	float x_coordBtmRight = bottomRadius * cos((i + 1) * (theta / nfaces));
	   float y_coordBtm = bottomRadius * sin(i * (theta / nfaces));	float y_coordBtmRight = bottomRadius * sin((i + 1) * (theta / nfaces));

	   GsPnt a = GsVec(x_coord, len, y_coord);
	   GsPnt b = GsVec(x_coordTopRight, len, y_coordTopRight);
	   GsPnt c = GsVec(x_coordBtmRight, -len, y_coordBtmRight);

	   GsPnt d = GsVec(x_coordBtm, -len, y_coordBtm);
	   GsPnt e = GsVec(x_coord, len, y_coord);
	   GsPnt f = GsVec(x_coordBtmRight, -len, y_coordBtmRight);

	   //:: Build Sides
	   P.push() = a; 
	   P.push() = b;
	   P.push() = c;

	   P.push() = d; 
	   P.push() = e; 
	   P.push() = f; 


	   //:: Texture Coordinates 
	   float texPnt = float(i) / float(nfaces); 
	   float texPntNxt = float(i + 1) / float(nfaces);
	   
	   T.push() = GsVec2(1 - texPnt, 0);		
	   T.push() = GsVec2(1 - texPntNxt, 0);		 
	   T.push() = GsVec2(1 - texPntNxt, 1);		

	   T.push() = GsVec2(1 - texPnt, 1);
	   T.push() = GsVec2(1 - texPnt, 0);
	   T.push() = GsVec2(1 - texPntNxt, 1);


	   //:: Normals (Smooth)
	   GsVec normal1, normal2, normal3, normal4, normal5, normal6;

	   //:: Compute Cylinder Side Normals : 
	   normal1 = a - GsVec(0.0f, len, 0.0f);
	   normal2 = b - GsVec(0.0f, len, 0.0f);
	   normal3 = c - GsVec(0.0f, len, 0.0f);
	   normal4 = d - GsVec(0.0f, len, 0.0f);
	   normal5 = e - GsVec(0.0f, len, 0.0f);
	   normal6 = f - GsVec(0.0f, len, 0.0f);

	   normal1.normalize();
	   normal2.normalize();
	   normal3.normalize();
	   normal4.normalize();
	   normal5.normalize();
	   normal6.normalize();

	   N.push() = normal1;
	   N.push() = normal2;
	   N.push() = normal3;
	   N.push() = normal4;
	   N.push() = normal5;
	   N.push() = normal6;
		  
   }
	   
 
   _mtl.specular.set(255, 255, 255);
   _mtl.shininess = 75; // increase specular effect
   glBindVertexArray(va[0]); // break the existing vertex array object binding.

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);

   glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
   glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*P.size(), P.pt(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
   glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float)*N.size(), N.pt(), GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
   glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // false means no normalization 
   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numelements = P.size();

   // free non-needed memory:
   P.capacity(0); T.capacity(0); N.capacity(0);

 }

// call this everytome you need to draw:
void SoMyObject::draw ( GsMat& tr, GsMat& pr, const GsLight& l, float changer )
 {

   float f[4];
   float sh = (float)_mtl.shininess;
   if (sh < 0.001f) sh = 75;
   // Draw:

   glUseProgram(_proggouraud.id);
   glUniformMatrix4fv(_proggouraud.uniloc[0], 1, GL_FALSE, tr.e);
   glUniformMatrix4fv(_proggouraud.uniloc[1], 1, GL_FALSE, pr.e);
   glUniform3fv(_proggouraud.uniloc[2], 1, l.pos.e);
   glUniform4fv(_proggouraud.uniloc[3], 1, l.amb.get(f));
   glUniform4fv(_proggouraud.uniloc[4], 1, l.dif.get(f));
   glUniform4fv(_proggouraud.uniloc[5], 1, l.spe.get(f));
   glUniform4fv(_proggouraud.uniloc[6], 1, _mtl.ambient.get(f));
   glUniform4fv(_proggouraud.uniloc[7], 1, _mtl.specular.get(f));
   glUniform4fv(_proggouraud.uniloc[8], 1, _mtl.diffuse.get(f));
   glUniform1fv(_proggouraud.uniloc[9], 1, &sh);
   glUniform1ui(_proggouraud.uniloc[10], 1u);
   glUniform1f(_proggouraud.uniloc[11], changer);

   glBindVertexArray ( va[0] );
   // call a draw function for the arrays here
   glBindTexture(GL_TEXTURE_2D, id);
   glDrawArrays(GL_TRIANGLES, 0, _numelements);
   glBindVertexArray(0); // break the existing vertex array object binding.
 
 }

