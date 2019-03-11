
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_light.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_model.h"
# include "so_myobject.h"
# include "eval_curve.h"
# include <ctime>
# include <chrono> 
# include <windows.h>
# include <MMSystem.h>
//#pragma comment(lib, "irrKlang.lib")
//# include "irrKlang\irrKlang.h" //sound


	
using std:: cout;
using std::chrono::nanoseconds; 
using std::chrono::seconds; 
using std::chrono::minutes;
using Clock = std::chrono::high_resolution_clock;

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
//    GlShader _vshflat, _fshflat, _vshgou, _fshgou, _vshphong, _fshphong;
  //  GlProgram _progflat, _proggou, _progphong;




	 //:: Time Functionality
	 time_t initialize, prog_start;
	 time_t startTime, current_time, endTime; 
	 void updateTime(::nanoseconds& nano, ::seconds& sec); 
	 seconds s; 
	 nanoseconds ns;
	

	 float seconds;
    // My scene objects:
    SoAxis _axis;
    SoModel _model;

    // Scene data:
    bool  _viewaxis;
	bool _spinoDino; 
    GsModel _gsm;
	float changer = 0.75f;
	float lightSrc = 10.0f;
	bool changeView = false;
	const float PI = 3.14159265359;
	const float theta = 2 * PI;


	float z_move = 0.0f; 
	float x_move = 0.0f; 


	float high_val = 1.0f; 
	float low_val = 0.0f; 
	float right_leg_height = 0.0f;
	float left_leg_height = 0.0f;


	SoModel _spinoLeftArm; 
	SoModel _spinoLeftElbow;
	SoModel _spinoRightShoulder; 
	SoModel _spinoRightElbow; 
	SoModel _spinoLegs; 
	SoModel _spinoLeftLeg; 
	SoModel _spinoRightLeg;
	SoModel _landscape;
	SoModel _spinosaurus;
	SoModel _spinoLeftFoot; 
	SoModel _spinoRightFoot; 
	SoModel _spinoLeftThigh; 
	SoModel _spinoRightThigh;
	SoModel _spinoLowerJaw; 
	SoModel _spinoHead; 
	SoModel _spinoNeck;


	SoMyObject _jp3;
	SoMyObject _jp;
	SoMyObject _jWorld;
	SoMyObject _lostWorld;
	SoMyObject _tree1; 
	SoMyObject _tree2; 
	SoMyObject _tree3; 
	SoMyObject _tree4; 
	SoMyObject _tree5; 

	float faces = 40.0f;
	float leng = 5.0f;
	float rt = 5.0f;
	float rb = 5.0f;


	SoMyObject _tester = SoMyObject(faces, leng, rt, rb);

    GsLight _light;
	GsLight _shadow;
	GsLight _green;

	//:: Limb Rotation Variables

	//:: Arm Rotations
	enum ShoulderRots {UpShoulder, DownShoulder, RightShoulder, LeftShoulder};
	ShoulderRots shoulder_rot; 
	float rotateLeftShoulderZ = 0.0f; 
	float rotateRightShoulderZ = 0.0f;
	float rotateLeftShoulder = 0.0f;
	float rotateRightShoulder = 0.0f; 
	//::: Elbow Variables 
	enum ElbowRots { UpElbow, DownElbow, LeftElbow, RightElbow };
	ElbowRots elbow_rot; 
	float rotateLeftElbow = 0.0f;
	float rotateRightElbow = 0.0f;
	float rotateRightElbowZ = 0.0f; 
	float rotateLeftElbowZ = 0.0f; 

	//:: Torso Parts
	enum TorsoPosition  { TorsoUp, TorsoDown }; 
	TorsoPosition torso_portion;
	float move_y = 0.0f;
	float rotTorso = 0.0f;
	float rotTorsoZ = 0.0f;
	float rotTorsoY = 0.0f;
	//:: Upper Parts
	float rotNeck = 0.0f; 
	float rotHeadX = 0.0f; 
	float rotHeadY = 0.0f; 
	float rotHeadZ = 0.0f;
	float rotJaw = 0.0f; 
	enum HeadTilt  { LeftTilt, RightTilt }; 
	HeadTilt head_tilt; 
	enum Jaws {Open, Close};
	Jaws jaws; 
	enum NeckTilt {NeckDown, NeckUp};
	NeckTilt neck_tilt;

	//:: Thighs
	enum Legs {Right, Left};
	enum LegTilt {LegTiltLeft, LegTiltRight};
	Legs legs;
	LegTilt leg_tilt;
	bool top = false;
	bool bottom = false;
	float high_leg_val = 0.9f; 
	float low_leg_val = 0.1f;
	float rotRightThighX = 0.0f;
	float rotRightThighZ = 0.0f; 
	float rotLeftThighX = 0.0f; 
	float rotLeftThighZ = 0.0f; 
	//:: Knees
	float rotRightKnee = 0.0f; 
	float rotLeftKnee = 0.0f; 
	//:: Feet
	float rotRightFoot = 0.0f; 
	float rotLeftFoot = 0.0f; 
	float rotLegZ = 0.0f; 

	float rotEverything = 0.0f;
    
    // App data:
    enum MenuEv { evOption0, evOption1 };

	//:: Curves
	GsArray <GsVec> ctrl_pts; 
	GsArray <GsVec> evo_curve; 
	So_Curve _bezier;
	
	void interpolator(const GsArray<GsVec>& evo_curve);
	bool interpolation = false;
	

    float _rotx, _roty, _fovy;
    int _w, _h;

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    void loadModel ( int model );
    GsVec2 windowToScene ( const GsVec2& v );
	void moveLegsUpNDown();
	void moveLegsZ();
	void keyFrame1(); bool init_keyFrame1 = false; 
	void keyFrame2(); bool init_keyFrame2 = false; 
	void keyFrame3(); bool init_keyFrame3 = false;
	void keyFrame4(); bool init_keyFrame4 = false; 
	void keyFrame5(); bool init_keyFrame5 = false; 
	void keyFrame6(); bool init_keyFrame6 = false;
	bool movLegZ = false;
	//irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();

   private : // functions derived from the base class
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	virtual void glutIdle(); 
	bool done = false;

	GsMat shadowMatrix();
 };

#endif // APP_WINDOW_H
