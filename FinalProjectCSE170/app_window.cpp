
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = true;
   _spinoDino = true; 
   _fovy = GS_TORAD(60.0f);
   _rotx = _roty = 0;
   _w = w;
   _h = h;
 }

void AppWindow::initPrograms ()
 {
   // Init my scene objects:
   _axis.init ();
   _model.init ();
   _spinoLeftArm.init();
   _spinoLeftElbow.init();
	_spinoRightShoulder.init();
	_spinoRightElbow.init();
   _spinoLegs.init();
   _spinoLeftLeg.init();	// Left Knee
   _spinoRightLeg.init();	// Right Knee
  _spinosaurus.init();
  _spinoHead.init(); 
  _spinoLowerJaw.init();
  _spinoLeftThigh.init();
  _spinoRightThigh.init();
  _spinoRightFoot.init();
  _spinoLeftFoot.init();
  _spinoNeck.init();
  _landscape.init(); 
  //:: Enums
  legs = Left;
  head_tilt= LeftTilt;
  leg_tilt = LegTiltLeft;
  neck_tilt = NeckUp;
  jaws = Close;
  elbow_rot = UpElbow; 
  shoulder_rot = UpShoulder;
  torso_portion = TorsoDown; 

  prog_start = time(0);
  startTime = time(0);
  time(&startTime);
  //:: Input Control Points
  ctrl_pts.push() = GsVec(-0.555699f, 0.0f, 0.144338f);
  ctrl_pts.push() = GsVec(-0.371669f, 0.0f, 0.4330312f);
  ctrl_pts.push() = GsVec(-0.0793857f, 0.0f, 0.688009f);
  ctrl_pts.push() = GsVec(0.119078f, 0.0f, -0.673575f);
  ctrl_pts.push() = GsVec(0.343803f, 0.0f, 0.625462f); 
  ctrl_pts.push() = GsVec(0.494356f, 0.0f , 0.53886f);
  ctrl_pts.push() = GsVec(0.635085f, 0.0f, 0.312731f);
  ctrl_pts.push() = GsVec(0.635085f, 0.0f, 0.120281f);
  ctrl_pts.push() = GsVec(0.588175f, 0.0f,  -0.0288675f);
  ctrl_pts.push() = GsVec(0.447446f, 0.0f, -0.23094f);
  ctrl_pts.push() = GsVec(0.202072f, 0.0f, -0.245374f);
  ctrl_pts.push() = GsVec(0.0252591f, 0.0f, -0.202072f);
  ctrl_pts.push() = GsVec(-0.119078f, 0.0f, -0.0866026f);
  ctrl_pts.push() = GsVec(-0.270633f, 0.0f, 0.105847f);


  _jp3.init("../jp3.png");
  _jp.init("../jp.png");
  _lostWorld.init("../lostworld.png");
  _jWorld.init("../jworld.png");
  _tree1.init("../treetexture.png");
  _tree2.init("../treetexture.png");
  _tree3.init("../treetexture.png");
  _tree4.init("../treetexture.png");
  _tree5.init("../treetexture.png");

  _tree1.topRadius = 0;
  _tree2.topRadius = 0;
  _tree3.topRadius = 0;
  _tree4.topRadius = 0;
  _tree5.topRadius = 0;

  _tester.init("../jungle.png");

  _jp3.build();
  _jp.build(); 
  _lostWorld.build(); 
  _jWorld.build();
  _tester.build();

   // set light:
   _light.set ( GsVec(5,10,10), GsColor(90,90,90,255), GsColor::white, GsColor::white );
   _shadow.set(GsVec(0, 0, 0), GsColor(0, 0, 0, 0), GsColor::black, GsColor::black);
   _green.set(GsVec(0, 2, 0), GsColor(0, 255, 0, 255), GsColor::green, GsColor::green);
  //_light.set(GsVec(lightSrc, 0, 10.0f), GsColor(90, 90, 90, 255), GsColor::white, GsColor::white);
   // Load demo model:
   loadModel ( 1 );
 }

static void printInfo ( GsModel& m )
 {
   std::cout<<"V:  "<<m.V.size()<<"\n";
   std::cout<<"F:  "<<m.F.size()<<"\n";
   std::cout<<"N:  "<<m.N.size()<<"\n";
   std::cout<<"M:  "<<m.M.size()<<"\n";
   std::cout<<"Fn: "<<m.Fn.size()<<"\n";
   std::cout<<"Fm: "<<m.Fm.size()<<"\n";
 }

void AppWindow::loadModel ( int model )
 {
   float f;
   GsString file;
   switch ( model )
    { case 1: f=0.01f; file="../models/porsche.obj"; break;
      case 2: f=0.20f; file="../models/al.obj"; break;
      case 3: f=0.10f; file="../models/f-16.obj"; break;
	  case 4: f = 0.10f; file = "../models/closeSpinoLeftArm.obj"; break;
      default: return;
    }

   f = 0.5f;

   //:: Initialize Landscape
   file = "../models/_landscape.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
  // _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _landscape.build(_gsm);
   redraw();

   f = 0.10f;


   file = "../new_models/spinosaurusShoulderLeftOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoLeftArm.build(_gsm);
   redraw();


   file = "../new_models/spinosaurusElbowLeftOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoLeftElbow.build(_gsm);
   redraw();


  file = "../new_models/spinosaurusTorsoProtoUpperOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinosaurus.build(_gsm);
   redraw();



   file = "../new_models/spinosaurusShoulderRightOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoRightShoulder.build(_gsm);
   redraw();




   file = "../new_models/spinosaurusElbowRightOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoRightElbow.build(_gsm);
   redraw();





   file = "../new_models/spinosaurusLeftKneeOfficialStar.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoLeftLeg.build(_gsm);
   redraw();



   file = "../new_models/spinosaurusRightKneeOfficialStar.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoRightLeg.build(_gsm);
   redraw();

   // Head
   file = "../new_models/spinosaurusUpperHeadOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoHead.build(_gsm);
   redraw();

   // Lower Jaw
   file = "../new_models/spinosaurusLowerJawOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoLowerJaw.build(_gsm);
   redraw();

   // Right Foot
   file = "../new_models/spinosaurusOfficialRightFoot.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoRightFoot.build(_gsm);
   redraw();

   // Left Foot
   file = "../new_models/spinosaurusOfficialLeftFoot.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoLeftFoot.build(_gsm);
   redraw();

   // Upper Neck
   file = "../new_models/spinosaurusUpperNeckOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoNeck.build(_gsm);
   redraw();
   

   // Right Thigh
   file = "../new_models/spinosaurusRightThighOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoRightThigh.build(_gsm);
   redraw();

   // Left Thigh
   file = "../new_models/spinosaurusLeftThighOfficial.obj";
   std::cout << "Loading " << file << "...\n";
   if (!_gsm.load(file)) std::cout << "Error!\n";
   printInfo(_gsm);
   _gsm.smooth(GS_TORAD(35));
   _gsm.scale(f); // to fit our camera space
   _spinoLeftThigh.build(_gsm);
   redraw();




 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
	float incr = GS_TORAD(2.5f);
   switch ( key )
    { //case ' ': _viewaxis = !_viewaxis; redraw(); break;
	  case 27 : exit(1); // Esc was pressed
    
	  case 49:  time(&initialize); keyFrame1(); redraw(); break; // Case 1 = key 1, keyframe #1
	  case 50:  time(&initialize); init_keyFrame2 = !init_keyFrame2;   redraw(); break;	// Case 2 = key 2 keyframe #2
	  case 51:  keyFrame3(); redraw(); break; // Case 3 = key3 
	  case 52: time(&initialize); init_keyFrame4 = !init_keyFrame4;  redraw(); break;
	  case 53:  time(&initialize); init_keyFrame5 = !init_keyFrame5;  redraw(); break;
	  case 54: time(&initialize); init_keyFrame6 = !init_keyFrame6; redraw(); break;
	  case 55:  _bezier.eval_bezier(evo_curve,  ctrl_pts, 10); redraw();  break;
	  case 56:  interpolation = !interpolation; redraw(); break;//time(&initialize); interpolator(evo_curve); redraw(); break;
	//:: Shoulders ------------------------------------
	  case 'a': rotateRightShoulder += 0.05f; redraw(); break;
	  case 's': rotateRightShoulder -= 0.05f; redraw(); break;
	  case 'q': rotateLeftShoulder += 0.05f; redraw(); break;
	  case 'w': rotateLeftShoulder -= 0.05f; redraw(); break;
		  //:: ------------------------------------------

	//:: Elbows -------------------------------------------
	  case 'e':  rotateLeftElbow += 0.05f; redraw(); break;
	  case 'r':  rotateLeftElbow -= 0.05f; redraw(); break;
	  case 'd': rotateRightElbow += 0.05f; redraw(); break; 
	  case 'f': rotateRightElbow -= 0.05f; redraw(); break;
		  //:: --------------------------------------------

	//:: Torso
	  case 'z': rotTorso += 0.05f; redraw(); break; 
	  case 'x': rotTorso -= 0.05f; redraw(); break;
	//:: --------------------------------------------

	//:: Neck Rotations-------------------
	  case 48: rotNeck += 0.05f; redraw(); break;	// 0
	  case 57: rotNeck -= 0.05f; redraw(); break;	//	9
	// -------------------------------------------

	//:: Head Rotations --------------------
	  case 'c':	rotHeadX += 0.05f; redraw(); break; 
	  case 'v': rotHeadX -= 0.05f; redraw(); break;
	  case 'b': rotHeadY += 0.05f; redraw(); break;
	  case 'n': rotHeadY -= 0.05f; redraw(); break;
	  case 'm': rotHeadZ += 0.05f; redraw(); break;
	  case 44: rotHeadZ -= 0.05f; redraw(); break;  // ','
	//:: --------------------------------------------

	//:: Jaw Rotation ------------------------------
	  case 'g': rotJaw += 0.05f; redraw(); break; 
	  case 'h': rotJaw -= 0.05f; redraw(); break; 
	//:: ------------------------------------------

	//:: Thigh Rotations --------------------
	  case 't': rotLeftThighX += 0.05f; redraw(); break; 
	  //case 'y': rotLeftThighX -= 0.05f; redraw(); break; 
	  case 'o': rotLeftThighZ += 0.05f; redraw(); break; 
	  case 'p': rotLeftThighZ -= 0.05f; redraw(); break; 
	//:: -----------------------------------

		 //:: Knee Rotations
	  case 59: rotLeftKnee += 0.05f; redraw(); break;
	  case 34: rotLeftKnee -= 0.05f; redraw(); break;
	  case 91: rotRightKnee += 0.05f; redraw(); break;
	  case 93: rotRightKnee -= 0.05f; redraw(); break;
		  //:: -----------------------------------

	  case '+': changer += 0.05; redraw(); break;
	  case '-': changer -= 0.05; redraw(); break;

	  case 32 : changeView = !changeView; redraw(); break;


	  case 'l':      _roty -= incr; redraw(); break;
	  case 'j':		 _roty += incr; redraw(); break;
	  case 'i':      _rotx += incr; redraw(); break;
	  case 'k':      _rotx -= incr; redraw(); break;

	  case 'y': rotEverything -= .1f; redraw(); break;
	  case 'u': rotEverything += .1f; redraw(); break;
	


      default : loadModel ( int(key-'0') );
                break;
	}
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
   {
   case GLUT_KEY_LEFT:      x_move += 0.1f; keyFrame3(); redraw(); break; //:: Increment on the Z Axis
   case GLUT_KEY_RIGHT:     x_move -= 0.1f; keyFrame3(); redraw(); break; //:: Decrement on the Z Axis
   case GLUT_KEY_UP:        z_move += 0.1f; keyFrame3(); redraw(); break; //:: Increment on the X Axis
   case GLUT_KEY_DOWN:      z_move -= 0.1f; keyFrame3(); redraw(); break; //:: Decrement on the X Axis
      case GLUT_KEY_PAGE_UP:   _fovy-=incf; break;
      case GLUT_KEY_PAGE_DOWN: _fovy+=incf; break;
      default: return; // return without rendering
	}
   if (rd) redraw(); // ask the window to be rendered when possible
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

void AppWindow::updateTime(::nanoseconds &nano, ::seconds& sec)
{
	current_time = difftime(time(0), startTime);
	std::chrono::seconds s(current_time); 
	nano = std::chrono::duration_cast<std::chrono::nanoseconds> (s); 
	sec = std::chrono::duration_cast<std::chrono::seconds> (nano);
}
void AppWindow::interpolator(const GsArray<GsVec>& evo_curve)
{
	if (current_time % 2 == 0)
	{

			for (int i = 0; i < evo_curve.size(); i++)
			{
				cout << "X: " << evo_curve[i].x << "\n";
				cout << "Z: " << evo_curve[i].z << "\n";
				x_move = evo_curve[i].x;
				z_move = evo_curve[i].z;
				Sleep(5);
				redraw();
			}		
	}

}

void AppWindow::glutIdle()
{
	updateTime(ns, s);
	time(&endTime);
	current_time = difftime(endTime, startTime);
	
	//if (init_keyFrame3) { keyFrame3(); }
	if (init_keyFrame2) { keyFrame2(); }
	//if (init_keyFrame1) { keyFrame1(); }
	if (init_keyFrame4) { keyFrame4(); }
	if (init_keyFrame5) {  keyFrame5(); }
	if (init_keyFrame6) { keyFrame6(); }

	if (interpolation) {// interpolator(evo_curve); }

		if (current_time % 2 == 0)
		{

			for (int i = 0; i < evo_curve.size(); i++)
			{
				cout << "X: " << evo_curve[i].x << "\n";
				cout << "Z: " << evo_curve[i].z << "\n";
	
				_spinosaurus.obj_center.x = evo_curve[i].x;
				_spinosaurus.obj_center.z = evo_curve[i].z;
				//Sleep(5);
				redraw();
			}
		}
	}

	
	//time(&startTime);
	redraw();
}

void AppWindow::keyFrame1()
{

		if (head_tilt == LeftTilt)
		{
			if (rotHeadZ <= 0.4f)
			{
				rotHeadZ += 0.025f;

			}

			else { head_tilt = RightTilt; }

		}
		if (head_tilt == RightTilt)
		{
			if (rotHeadZ >= -0.4f)
			{
				rotHeadZ -= 0.025;
			}

			else { head_tilt = LeftTilt; }

		}

		if (neck_tilt == NeckUp)
		{
			if (rotNeck >= 0.08)
			{
				neck_tilt = NeckDown;
			}
			rotNeck += 0.02f;
		}

		if (neck_tilt == NeckDown)
		{
			if (rotNeck <= -0.08)
			{
				neck_tilt = NeckUp;
			}

			rotNeck -= 0.02f;
		}

		if (jaws == Close)
		{
			if (rotJaw <= -0.128f)
			{
				jaws = Open;
			}
			rotJaw -= 0.008f;
		}
		if (jaws == Open)
		{

			if (rotJaw >= 0.32f)
			{
				jaws = Close;
			}
			rotJaw += 0.008f;
		}

		if (shoulder_rot == UpShoulder)
		{
			if (rotateLeftShoulder > 0.3f)
			{
				shoulder_rot = DownShoulder;
			}

			rotateLeftShoulder += 0.02f;
			rotateRightShoulder -= 0.02f;
			rotateLeftElbowZ += 0.02f;
			rotateRightElbowZ -= 0.02f;
			rotTorsoY += 0.02f;
		}

		if (shoulder_rot == DownShoulder)
		{
			if (rotateLeftShoulder < -0.3f) { shoulder_rot = UpShoulder; }
			rotateLeftShoulder -= 0.02f;
			rotateRightShoulder += 0.02f;
			rotateLeftElbowZ -= 0.02f;
			rotateRightElbowZ += 0.02f;
			rotTorsoY -= 0.02f;
		}

		redraw();

}

void AppWindow::keyFrame2()
{
	seconds = difftime(initialize, prog_start); 
	cout << "Seconds Difference: " << current_time << "\n";
	if(current_time % 1 == 0)
	{
		if (torso_portion == TorsoDown)
		{
			if (move_y < -0.1)
			{
				torso_portion = TorsoUp;
			}
			move_y -= 0.005f;
			rotLeftThighX -= 0.005f;
			rotRightThighX -= 0.005f;
			rotRightKnee -= 0.005f;
			rotLeftKnee -= 0.005f;
			rotNeck -= 0.005f;
			rotHeadX -= 0.005f;

		}

		if (torso_portion == TorsoUp)
		{
			if (move_y > 0.2f)
			{
				torso_portion = TorsoDown;
			}
			move_y += 0.005f;
			rotLeftThighX += 0.005f;
			rotRightThighX += 0.005f;
			rotRightKnee += 0.005f;
			rotLeftKnee += 0.005f;
			rotNeck += 0.005f;
			rotHeadX += 0.005f;
		}
	}

}

void AppWindow::keyFrame3()
{
	
		if (legs == Left)
		{
			if (rotLeftThighX < 0.3f)
			{
				rotLeftThighX += 0.025f;
				rotLeftKnee += 0.025f;

				rotRightThighX -= 0.025f;
				rotRightKnee -= 0.025f;
			}

			else
			{
				legs = Right;
			}
		}

		if (legs == Right)
		{
			if (rotRightThighX < 0.3f)
			{
				rotRightThighX += 0.025f;
				rotRightKnee += 0.025f;

				rotLeftThighX -= 0.025f;
				rotLeftKnee -= 0.025f;
			}

			else
			{
				legs = Left;
			}
		}
	
		z_move += 0.025f;
		redraw();
	
	
}


void AppWindow::keyFrame4()
{
	
	cout << "Seconds Difference: " << current_time << "\n";
	if (current_time % 2 == 0)
	{
		if (leg_tilt == LegTiltLeft)
		{
			puts("Currently in LegTiltLeft");
			if (rotTorsoZ <= 0.35f)
			{
				rotTorsoZ += 0.0008f;
				rotLeftThighZ += 0.0008f;
				std::cout << "RotRightThighZ: " << rotRightThighZ << "\n";
				if (rotRightThighZ >= -0.02f)
				{
					leg_tilt = LegTiltRight;
				}

				else if (rotRightThighZ < -0.01f)
				{

					rotRightThighZ += 0.0008f;
				}

			}

			else { leg_tilt = LegTiltRight; }
		}

		if (leg_tilt == LegTiltRight)
		{
			puts("Currently in LegTiltRight");
			if (rotTorsoZ >= -0.35f)
			{
				rotTorsoZ -= 0.0008f;
				rotRightThighZ -= 0.0008f;
				//rotLeftThighZ -= 0.005f;
				std::cout << "RotLeftThighZ: " << rotLeftThighZ << "\n";
				if (rotLeftThighZ >= 0.0008f)
				{
					rotLeftThighZ -= 0.0008f;
				}

				else if (rotLeftThighZ >= 0.125f || rotLeftThighZ == -0.0008)
				{
					leg_tilt = LegTiltLeft;
				}
				if (rotRightThighZ <= -0.35f)
				{
					rotRightThighZ += 0.0008f;
				}

			}

			else { leg_tilt = LegTiltLeft; }
		}

		if (head_tilt == LeftTilt)
		{
			std::cout << "Head Tilt: " << rotHeadY << "\n";
			if (rotHeadY >= 1.32)
			{
				head_tilt = RightTilt;
			}
			rotHeadY += 0.04f;
		}

		if (head_tilt == RightTilt)
		{
			std::cout << "Head Tilt Right: " << rotHeadY << "\n";
			if (rotHeadY <= -1.32)
			{
				head_tilt = LeftTilt;
			}
			rotHeadY -= 0.04f;
		}

		keyFrame3();

		redraw();
	}
}

void AppWindow::keyFrame5()
{

	//PlaySound(TEXT("C:\\Users\julia\Desktop\LuisCode\visualc14\roar.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//SoundEngine->play2D("roar.wav");

	cout << "Seconds Difference: " << current_time << "\n";
	if (current_time % 1 == 0)
	{
		if (torso_portion == TorsoDown)
		{
			if (rotNeck < -0.7f) { torso_portion = TorsoUp; }
			rotRightThighX -= 0.01f;
			rotLeftThighX -= 0.01f;
			rotRightKnee += 0.01f;
			rotLeftKnee += 0.01f;
			move_y -= 0.0001f;
			rotNeck -= 0.01f;
			rotHeadX += 0.01f;
			rotateLeftElbowZ -= 0.01f;
			rotateRightElbowZ += 0.01f;

			if (rotJaw < 0.8f) { rotJaw += 0.01f; }
		}

		if (torso_portion == TorsoUp)
		{
			if (rotNeck > 0.7f) { rotJaw = 0.1f; torso_portion = TorsoDown; }
			rotRightThighX += 0.01f;
			rotLeftThighX += 0.01f;
			rotRightKnee -= 0.01f;
			rotLeftKnee -= 0.01f;
			rotNeck += 0.01f;
			move_y += 0.0001f;
			rotHeadX -= 0.01f;
			rotateLeftElbowZ += 0.01f;
			rotateRightElbowZ -= 0.01f;
			rotJaw += 0.01f;
			if (rotJaw <= 0.01f)
			{
				rotNeck += 0.02f;
				rotHeadX -= 0.01f;
			}

			else { rotJaw -= 0.01f; }

		}
		redraw();
	}

}

void AppWindow::keyFrame6()
{
	if (head_tilt == LeftTilt)
	{
		if (rotHeadY > 0.5f) { head_tilt = RightTilt; }
		rotHeadY += 0.008f;
	}

	if (head_tilt == RightTilt)
	{
		if (rotHeadY < -0.5) { head_tilt = LeftTilt; }
		rotHeadY -= 0.008f; 
	}
	
	if (current_time % 1 == 0 && torso_portion == TorsoDown)
	{
		rotRightThighX += 0.008f;
		rotLeftThighX += 0.008f; 
		rotLeftKnee -= 0.008f; 
		rotRightKnee -= 0.008f; 
		move_y -= 0.0004f;

	}
	if (rotRightThighX >= 1.0f)
	{
		torso_portion = TorsoUp;
	}

	if (torso_portion == TorsoUp)
	{
		if (current_time % 1 == 0)
		{
			rotRightThighX -= 0.008f;
			rotLeftThighX -= 0.008f;
			rotLeftKnee += 0.008f;
			rotRightKnee += 0.008f;
			move_y += 0.0004f;
		}

		if (move_y > 0.002f)
		{
			init_keyFrame6 = false;
			init_keyFrame5 = true;
			init_keyFrame4 = true;

		}
	}


	redraw(); 

}
void AppWindow::moveLegsUpNDown()
{
	if (legs == Right)
	{
		bottom = false;
		right_leg_height += 0.005f;
		printf("Right Leg Height: %f\n", right_leg_height);
		if (right_leg_height >= high_val)
		{
			puts("Right Leg has hit the top");
			top = true;
		}

		while (top)
		{
			right_leg_height -= 0.005f;
			printf("Decrementing Right Leg Height: %f\n", right_leg_height);
			if (right_leg_height <= low_val)
			{
				right_leg_height -= 0.005f;
				top = false;
				bottom = true;
				break;
			}


		}
		if (bottom) { puts("Switching to left leg");  legs = Left; }
	}

	if (legs == Left)
	{
		bottom = false;
		left_leg_height += 0.005f;
		printf("Left Leg Height: %f\n", left_leg_height);
		if (left_leg_height >= high_val)
		{
			puts("Left Leg has hit the top");
			top = true;
		}
		while (top)
		{
			left_leg_height -= 0.005f;
			printf("Decrementing Left Leg Height: %f\n", left_leg_height);
			if (left_leg_height <= low_val)
			{
				top = false;
				bottom = true;
				break;
			}
		}

		if (bottom) { puts("Switching to Right Leg");  legs = Right; }
	}

}

void AppWindow::moveLegsZ()
{
	if (left_leg_height <= high_val)
	{
		rotLegZ += 0.005f;
	}

	else if (left_leg_height >= high_val)
	{
		puts("Got elif");
		rotLegZ -= 0.005f;
		movLegZ = false;
	}
	 
}

//:: Constructing Shadow Matrix https://www.opengl.org/archives/resources/features/StencilTalk/tsld021.htm
GsMat AppWindow::shadowMatrix()
{
	float ground_plane[4] = {0.0, 0.5, 0.0, 0.0};
	float light_pos[4] = {0.0, 0.5, 0.5, 0.0};

	float DOT 
		= (ground_plane[0] * light_pos[0]) 
		+ (ground_plane[1] * light_pos[1]) 
		+ (ground_plane[2] * light_pos[2])
		+ (ground_plane[3] * light_pos[3]) 
		+ (ground_plane[4] * light_pos[4]);

	GsMat shadow; 

	shadow = GsMat
	(
		float (DOT - light_pos[0] * ground_plane[0]),
		float (0 - light_pos[0] * ground_plane[1]),
		float (0 - light_pos[0] * ground_plane[2]),
		float (0 - light_pos[0] * ground_plane[3]),

		float (0 - light_pos[1] * ground_plane[0]),
		float (DOT - light_pos[1] * ground_plane[1]),
		float (0 - light_pos[1] * ground_plane[2]),
		float (0 - light_pos[1] * ground_plane[3]),

		float (0 - light_pos[2] * ground_plane[0]),
		float (0 - light_pos[2] * ground_plane[1]),
		float (DOT - light_pos[2] * ground_plane[2]), 
		float (0 - light_pos[2] * ground_plane[3]),

		float (0 - light_pos[3] * ground_plane[0]),
		float (0 - light_pos[3] * ground_plane[1]),
		float (0 - light_pos[3] * ground_plane[2]),
		float (DOT - light_pos[3] * ground_plane[3])
	);

	return shadow;
}

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
	glClearColor(0.2f, 0.5f, 0.9f, 1); // change the background a little bit
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { _axis.build(1.0f); // axis has radius 1.0
    }

   // Define our scene transformation:
   GsMat rx, ry, stransf;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix

   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp, sproj, camView2;
   
 

   GsVec eye(0,0,2), center(0,0,0), up(0,1,0);
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix  


   float aspect=1.0f, znear=0.1f, zfar=50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   sproj = persp * camview; // set final scene projection




   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause our values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   // Draw:

   //:: Translation Matrices 
   GsMat spinoTorsoTranslate, spinoLegsTranslation, spinoLeftShoulderTranslate, spinoLeftElbowTranslate, spinoRightShoulderTranslate, spinoRightElbowTranslate, landscapeTranslate;
   //:: Thighs
   GsMat spinoRightThighTrans, spinoLeftThighTrans; 
   //:: Knees 
   GsMat spinoRightKneeTrans, spinoLeftKneeTrans;
   //:: Feet
   GsMat spinoRightFootTrans, spinoLeftFootTrans; 
   //:: Upper Body (Head, Jaw, Neck)
   GsMat spinoTopHeadTrans, spinoJawTrans, spinoNeckTrans; 


   GsMat movementZ, movementX, movementY, movementTransform; 
   GsMat RightLegTrans; 

   movementX.translation(x_move, 0.0f, 0.0f);
   movementY.translation(0.0f, move_y, 0.0f);
   movementZ.translation(0.0f, 0.0f, z_move);

   if (changeView)
   {
	   /*  case 'l':      _roty -= incr; redraw(); break;
	   case 'j':		 _roty += incr; redraw(); break;
	   case 'i':      _rotx += incr; redraw(); break;
	   case 'k':      _rotx -= incr; redraw(); break;*/
	   GsVec eye(_spinosaurus.obj_center.x + 0.5f, _spinosaurus.obj_center.x + x_move, _spinosaurus.obj_center.z + 0.3f), center(_spinosaurus.obj_center.z, _spinosaurus.obj_center.y, _spinosaurus.obj_center.z + z_move), up(0, 1, 0);
	   camView2.lookat(eye, center, up);
	   sproj = persp * camView2;
	   // interpolation = !interpolation;
   }



   movementTransform = movementX * movementZ * movementY;
   

   spinoTorsoTranslate.translation(_spinosaurus.obj_center.x * 0.0f, _spinosaurus.obj_center.y, _spinosaurus.obj_center.z - 0.2f);
   spinoLegsTranslation.translation(_spinoLegs.obj_center.x - 0.164f, _spinoLegs.obj_center.y + 0.08f, _spinoLegs.obj_center.z + 0.03f);
   RightLegTrans.translation(_spinoRightLeg.obj_center.x + 0.314f, _spinoRightLeg.obj_center.y + 0.08f, _spinoRightLeg.obj_center.z + 0.03f);


   //:: New Translations
   spinoTopHeadTrans.translation(_spinoHead.obj_center.x + 0.17f, _spinoHead.obj_center.y + 0.1f, _spinoHead.obj_center.z - 0.01f);						//:: Head Translation
   spinoJawTrans.translation(_spinoLowerJaw.obj_center.x + 0.0077f, _spinoLowerJaw.obj_center.y - 0.03f, _spinoLowerJaw.obj_center.z - 0.06f);				//:: Jaw Translation
   spinoNeckTrans.translation(_spinoNeck.obj_center.x - 0.32f, _spinoNeck.obj_center.y + 0.01f, _spinoNeck.obj_center.z + 0.2f);							//:: Neck Translation
   spinoRightThighTrans.translation(_spinoRightThigh.obj_center.x + 0.017f, _spinoRightThigh.obj_center.y + 0.02f, _spinoRightThigh.obj_center.z - 0.02f);	//:: Right Thigh Translation
   spinoRightKneeTrans.translation(_spinoRightLeg.obj_center.x - 0.03f, _spinoRightLeg.obj_center.y + 0.04f, _spinoRightLeg.obj_center.z + 0.04f);			//:: Right Knee Translation 
   spinoLeftThighTrans.translation(_spinoLeftThigh.obj_center.x + 0.03f, _spinoLeftThigh.obj_center.y + 0.03f, _spinoLeftThigh.obj_center.z - 0.02f);		//:: Left Thigh Translation
   spinoLeftKneeTrans.translation(_spinoLeftLeg.obj_center.x + 0.01f, _spinoLeftLeg.obj_center.y + 0.04f, _spinoLeftLeg.obj_center.z + 0.04f);				//:: Left Knee Translation
   spinoRightFootTrans.translation(_spinoRightFoot.obj_center.x - 0.01f, _spinoRightFoot.obj_center.y - 0.12f, _spinoRightFoot.obj_center.z - 0.13f);		//:: Right Foot Translation
   spinoLeftShoulderTranslate.translation(_spinoLeftArm.obj_center.x + 0.06f, -_spinoLeftArm.obj_center.y - 0.03f, -_spinoLeftArm.obj_center.z + 0.17f);	//:: Left ShoulderTranslation Updated
   spinoLeftElbowTranslate.translation(_spinoLeftElbow.obj_center.x + 0.011f, _spinoLeftElbow.obj_center.y + 0.045f, _spinoLeftElbow.obj_center.z - 0.09f);	//:: Left Elbow Translation Updated
   spinoLeftFootTrans.translation(_spinoLeftFoot.obj_center.x + 0.004f, _spinoLeftFoot.obj_center.y - 0.13f, _spinoLeftFoot.obj_center.z - 0.13f);			//:: Left Foot Translation 
   spinoRightShoulderTranslate.translation(_spinoRightShoulder.obj_center.x - 0.06f, _spinoRightShoulder.obj_center.y + 0.03f, _spinoRightShoulder.obj_center.z + 0.25f);	//:: Right Shoulder Updated 
   spinoRightElbowTranslate.translation(_spinoRightElbow.obj_center.x - 0.011f, _spinoRightElbow.obj_center.y + 0.045f, _spinoRightElbow.obj_center.z - 0.09f);		//:: Right Elbow Updated
  


   landscapeTranslate.translation(0.0f, 0.1095f, 0.0f);

   GsMat _jp3Translation, _jp3Rot, _jpTranslation, _jpRot, _lostWorldTranslation, _lostWorldRot, _jWorldTranslation, _jWorldRot;
   GsMat _t1trans, _t2trans, _t3trans, _t4trans, _t5trans;

   _jp3Translation.translation(1.5f, 0.5f, -2.0f); _jpTranslation.translation(0.0f, 0.5f, -3.0f);
   _lostWorldTranslation.translation(-1.5f, 0.5f, -2.5f); _jWorldTranslation.translation(-2.5f,0.5f, -0.8f);

   _jp3Rot.roty(PI / 3);  _jpRot.roty(theta / 3); _jWorldRot.roty(5*PI/6);

   _t1trans.translation(-2.8f, 0.0f, 0.3f);
   _t2trans.translation(-2.0f, 0.0f, 2.0f);
   _t3trans.translation(-2.0f, 0.0f, 1.0f);
   _t4trans.translation(2.0f, 0.0f, -1.0f);
   _t5trans.translation(2.0f, 0.0f, 0.2f);
 
   //:: Full Transmortation Matrices
   GsMat spinoTorsoTransformation, spinoLeftElbowTransformation, spinoLeftShoulderTransformation, spinoRigthShoulderTransformation, spinoRightElbowTransformation;

   //:: Rotation Matrices (Left) 
   GsMat  spinoLeftShoulderRotate, spinoTorso, spinoTorsoZRot, spinoTorsoYRot, spinoLeftElbowRotate;

   //:: Rotation Matrices (Right) 
   GsMat spinoRightShoulderRotate, spinoRightElbowRotate;

   //**** New ****\\
      //:: Thigh Rotations
   GsMat spinoRightThighRotX, spinoRightThighRotZ, spinoLeftThighRotX, spinoLeftThighRotZ;
   //:: Knee Rotations
   GsMat spinoRightKneeRot, spinoLeftKneeRot;
   //:: Upper Rotations
   GsMat spinoNeckRot, spinoHeadRotX, spinoHeadRotY, spinoHeadRotZ, spinoJawRot;
   GsMat spinoLeftElbowRotateZ, spinoLeftShoulderRotateZ, spinoRightElbowRotateZ, spinoRightShoulderRotateZ;

   spinoTorso.roty(rotEverything);

   spinoLeftElbowRotateZ.rotz(rotateLeftElbowZ); spinoLeftShoulderRotateZ.rotz(rotateLeftShoulderZ); 
   spinoRightElbowRotateZ.rotz(rotateRightElbowZ); spinoRightShoulderRotateZ.rotz(rotateRightShoulderZ);
   spinoTorso.rotx(rotTorso); spinoTorsoZRot.rotz(rotTorsoZ); spinoTorsoYRot.roty(rotTorsoY);
   spinoLeftShoulderRotate.rotx(rotateLeftShoulder);
   spinoLeftElbowRotate.rotx(rotateLeftElbow); 
   spinoRightShoulderRotate.rotx(rotateRightShoulder);
   spinoRightElbowRotate.rotx(rotateRightElbow);



   //:: New Rotations

   spinoHeadRotX.rotx(rotHeadX); spinoHeadRotY.roty(rotHeadY); spinoHeadRotZ.rotz(rotHeadZ); //:: Head Rotations
   spinoJawRot.rotx(rotJaw);	//:: Jaw Rotation
   spinoNeckRot.rotx(rotNeck);	//:: Neck Rotation
   spinoRightThighRotX.rotx(rotRightThighX);  spinoRightThighRotZ.rotz(rotRightThighZ);	//:: Right Thigh Rotations
   spinoLeftThighRotX.rotx(rotLeftThighX);	spinoLeftThighRotZ.rotz(rotLeftThighZ);		//:: Left Thigh Rotations
   spinoRightKneeRot.rotx(rotRightKnee); spinoLeftKneeRot.rotx(rotLeftKnee);		//:: KneeRots



	spinoTorsoTransformation = spinoTorsoTranslate *spinoTorsoZRot * spinoTorsoYRot *  spinoTorso;
   spinoLeftShoulderTransformation = spinoTorsoTransformation *  spinoLeftShoulderTranslate * spinoLeftShoulderRotate * spinoLeftShoulderRotateZ;
   spinoLeftElbowTransformation = spinoLeftShoulderTransformation * spinoLeftElbowTranslate * spinoLeftElbowRotate * spinoLeftElbowRotateZ;
   spinoRigthShoulderTransformation = spinoTorsoTransformation * spinoRightShoulderTranslate * spinoRightShoulderRotate * spinoRightShoulderRotateZ;
   spinoRightElbowTransformation = spinoRigthShoulderTransformation * spinoRightElbowTranslate * spinoRightElbowRotate * spinoRightElbowRotateZ ;

   //:: New Transformations for Limbs
   GsMat spinoNeckTransform, spinoHeadTransform, spinoJawTransform, spinoLeftThighTransform, spinoRightThighTransform, spinoLeftKneeTransform, spinoRightKneeTransform, spinoLeftFootTransform, spinoRightFootTransform;

   spinoNeckTransform = spinoTorsoTransformation * spinoNeckTrans * spinoNeckRot ;
   spinoHeadTransform = spinoNeckTransform * spinoTopHeadTrans * spinoHeadRotX * spinoHeadRotY * spinoHeadRotZ;
   spinoJawTransform = spinoHeadTransform * spinoJawTrans * spinoJawRot;
   spinoLeftThighTransform = spinoTorsoTransformation * spinoLeftThighTrans * spinoLeftThighRotX * spinoLeftThighRotZ; 
   spinoRightThighTransform = spinoTorsoTransformation * spinoRightThighTrans * spinoRightThighRotX * spinoRightThighRotZ;
   spinoRightKneeTransform = spinoRightThighTransform * spinoRightKneeTrans * spinoRightKneeRot; 
   spinoLeftKneeTransform = spinoLeftThighTransform * spinoLeftKneeTrans * spinoLeftKneeRot; 
   spinoLeftFootTransform = spinoLeftKneeTransform * spinoLeftFootTrans; 
   spinoRightFootTransform = spinoRightKneeTransform * spinoRightFootTrans; 



   //:: Rotation for ground
   GsMat _testerRot, _testerTranslation;
   _testerRot.rotx(theta);
   _testerTranslation.translation(0.0f, -0.2f, 0.0f);

      //GsMat shadow; 
   GsVec v; GsVec n; GsVec q;
   float changeVX = 0.2f;
   float changeVY = 1.0f;
   float changeVZ = 2.0f;
  // v = GsVec(12.5, 0.0, 0.0);	n = GsVec(0.2f, 0.0f, 0.0f);  q = GsVec(-0.3f, iiiii-12.6f, -0.8f);
   v = GsVec(0.2, 1.0, 1.0);	n = GsVec(0.0f, 1.0f, 0.0f);  q = GsVec(-1.0f, -0.2f, -2.0f);
   GsMat shadow;
   float vDOTn = dot( v, n ); 
   float qDOTn = dot(q, n);

   //:: Projection Matrix
		 shadow = GsMat(
			float(vDOTn - (v.x * n.x)),  float(0 - (v.x *n.y)),     float(0 - (v.x * n.z)),     float((qDOTn)* v.x),
			float(0 - (v.y * n.x)),      float(vDOTn - (v.y *n.y)), float(0 - (v.y * n.z)),     float((qDOTn)* v.y),
			float(0 - (v.z * n.x)),      float(0 - (v.z *n.y)),     float(vDOTn - (v.z * n.z)), float((qDOTn)* v.z),
			0.0f,                          0.0f,                       0.0f,                    float(vDOTn));

   
		 if (_spinoDino)
		 {
			 _spinoLeftArm.draw(stransf * shadowMatrix() * shadow *   movementTransform * spinoLeftShoulderTransformation, sproj, _shadow);
			 _spinoLeftElbow.draw(stransf * shadowMatrix() * shadow *   movementTransform * spinoLeftElbowTransformation, sproj, _shadow);
			 _spinoRightShoulder.draw(stransf * shadowMatrix() * shadow *  movementTransform * spinoRigthShoulderTransformation, sproj, _shadow);
			 _spinoRightElbow.draw(stransf * shadowMatrix() *shadow *  movementTransform * spinoRightElbowTransformation, sproj, _shadow);
			 _spinosaurus.draw(stransf *shadowMatrix()  * shadow *  movementTransform * spinoTorsoTransformation, sproj, _shadow);


			 _spinoLeftLeg.draw(stransf * shadowMatrix()  * shadow  * movementTransform *  spinoLeftKneeTransform, sproj, _shadow);			//:: Updated Left Knee
			 _spinoRightLeg.draw(stransf * shadowMatrix()  * shadow  * movementTransform * spinoRightKneeTransform, sproj, _shadow);			//:: Updated Right Knee
			 _spinoHead.draw(stransf * shadowMatrix()  * shadow  * movementTransform * spinoHeadTransform, sproj, _shadow);
			 _spinoNeck.draw(stransf * shadowMatrix()  * shadow  * movementTransform * spinoNeckTransform, sproj, _shadow);
			 _spinoRightFoot.draw(stransf * shadowMatrix()  * shadow  * movementTransform * spinoRightFootTransform, sproj, _shadow);
			 _spinoLeftFoot.draw(stransf * shadowMatrix()  * shadow  * movementTransform * spinoLeftFootTransform, sproj, _shadow);
			 _spinoRightThigh.draw(stransf * shadowMatrix()  * shadow  * movementTransform * spinoRightThighTransform, sproj, _shadow);
			 _spinoLeftThigh.draw(stransf *shadowMatrix()  * shadow  * movementTransform * spinoLeftThighTransform, sproj, _shadow);
			 _spinoLowerJaw.draw(stransf *shadowMatrix()  * shadow  * movementTransform * spinoJawTransform, sproj, _shadow);
			 
		 }

   
   _spinoLeftArm.draw(stransf * movementTransform * spinoLeftShoulderTransformation, sproj, _light);
   _spinoLeftElbow.draw(stransf * movementTransform *  spinoLeftElbowTransformation, sproj, _light);
   _spinoRightShoulder.draw(stransf * movementTransform *  spinoRigthShoulderTransformation, sproj, _light);
   _spinoRightElbow.draw(stransf *  movementTransform * spinoRightElbowTransformation, sproj, _light);
   _spinosaurus.draw(stransf * movementTransform *  spinoTorsoTransformation, sproj, _light);
   _spinoLegs.draw(stransf * movementTransform *  spinoLegsTranslation, sproj, _light);
   _spinoLeftLeg.draw(stransf * movementTransform *  spinoLeftKneeTransform , sproj, _light);			//:: Updated Left Knee
   _spinoRightLeg.draw(stransf * movementTransform * spinoRightKneeTransform , sproj, _light);			//:: Updated Right Knee



   _spinoHead.draw(stransf * movementTransform * spinoHeadTransform, sproj, _light);
   _spinoNeck.draw(stransf * movementTransform * spinoNeckTransform, sproj, _light);
   _spinoRightFoot.draw(stransf * movementTransform * spinoRightFootTransform, sproj, _light);
   _spinoLeftFoot.draw(stransf * movementTransform * spinoLeftFootTransform, sproj, _light);
   _spinoRightThigh.draw(stransf * movementTransform * spinoRightThighTransform, sproj, _light);
   _spinoLeftThigh.draw(stransf * movementTransform * spinoLeftThighTransform, sproj, _light);
   _spinoLowerJaw.draw(stransf * movementTransform * spinoJawTransform, sproj, _light);
 
   if (_jp3.changed)
   {
	   _jp3.build();
	   _jp.build();
	   _lostWorld.build();
	   _jWorld.build();
	   _tree1.build();
	   _tree2.build();
	   _tree3.build();
	   _tree4.build();
	   _tree5.build();
   }
   _landscape.draw(stransf * landscapeTranslate, sproj, _green); 
   _jp3.draw(stransf *   _jp3Translation * _jp3Rot , sproj, _light, changer);
   _jp.draw(stransf * _jpTranslation * _jpRot, sproj, _light, changer);
   _lostWorld.draw(stransf * _lostWorldTranslation  , sproj, _light, changer);
  _jWorld.draw(stransf * _jWorldTranslation * _jWorldRot  , sproj, _light, changer);
   _tree1.draw(stransf * _t1trans, sproj, _light, changer);
   _tree2.draw(stransf * _t2trans, sproj, _light, changer);
   _tree3.draw(stransf * _t3trans, sproj, _light, changer);
   _tree4.draw(stransf * _t4trans, sproj, _light, changer);
   _tree5.draw(stransf * _t5trans, sproj, _light, changer);
   _tester.draw(stransf * _testerTranslation * _testerRot, sproj, _light, changer);
   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}


