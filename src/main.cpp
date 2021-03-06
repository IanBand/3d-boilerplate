#include "../lib/gca-plus/src/GCAdapter.h"
#include <irrlicht.h>
#include <math.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


enum CameraMode {
 Menu,
 FreeDebug,
 InBattle,
 TrackingStage,
 TrackingFighter
};

/*
struct FighterState {
    int animationID;
    // ect
}

// shared via ggpo
struct BattleState {
    FighterState fighterState[4];
    ProjectileState[16];
}
*/

int main( int argc, char *argv[] ){


    ControllerStatus adapter_buffer[4];

    if(!gca::Setup()){
        std::cout << "Failed to load gca" << std::endl;
        return 1;
    }
    
    
	/*
	The most important function of the engine is the createDevice()
	function. The IrrlichtDevice is created by it, which is the root
	object for doing anything with the engine. createDevice() has 7
	parameters:

	- deviceType: Type of the device. This can currently be the Null-device,
	   one of the two software renderers, D3D8, D3D9, or OpenGL. In this
	   example we use EDT_SOFTWARE, but to try out, you might want to
	   change it to EDT_BURNINGSVIDEO, EDT_NULL, EDT_DIRECT3D8,
	   EDT_DIRECT3D9, or EDT_OPENGL.

	- windowSize: Size of the Window or screen in FullScreenMode to be
	   created. In this example we use 640x480.

	- bits: Amount of color bits per pixel. This should be 16 or 32. The
	   parameter is often ignored when running in windowed mode.

	- fullscreen: Specifies if we want the device to run in fullscreen mode
	   or not.

	- stencilbuffer: Specifies if we want to use the stencil buffer (for
	   drawing shadows).

	- vsync: Specifies if we want to have vsync enabled, this is only useful
	   in fullscreen mode.

	- eventReceiver: An object to receive events. We do not want to use this
	   parameter here, and set it to 0.

	Always check the return value to cope with unsupported drivers,
	dimensions, etc.
	*/
	IrrlichtDevice *device =
		createDevice( video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	/*
	Set the caption of the window to some nice text. Note that there is an
	'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	/*
	Get a pointer to the VideoDriver, the SceneManager and the graphical
	user interface environment, so that we do not always have to write
	device->getVideoDriver(), device->getSceneManager(), or
	device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* sceneManager = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	/*
	We add a hello world label to the window, using the GUI environment.
	The text is placed at the position (10,10) as top left corner and
	(260,22) as lower right corner.
	*/
	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);

	/*
	To show something interesting, we load a Quake 2 model and display it.
	We only have to get the Mesh from the Scene Manager with getMesh() and add
	a SceneNode to display the mesh with addAnimatedMeshSceneNode(). We
	check the return value of getMesh() to become aware of loading problems
	and other errors.

	Instead of writing the filename sydney.md2, it would also be possible
	to load a Maya object file (.obj), a complete Quake3 map (.bsp) or any
	other supported file format. By the way, that cool Quake 2 model
	called sydney was modelled by Brian Collins.
	*/
	IAnimatedMesh* mesh = sceneManager->getMesh("../lib/irrlicht-1.8.5/media/sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = sceneManager->addAnimatedMeshSceneNode( mesh );

	/*
	To let the mesh look a little bit nicer, we change its material. We
	disable lighting because we do not have a dynamic light in here, and
	the mesh would be totally black otherwise. Then we set the frame loop,
	such that the predefined STAND animation is used. And last, we apply a
	texture to the mesh. Without it the mesh would be drawn using only a
	color.
	*/
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("../lib/irrlicht-1.8.5/media/sydney.bmp") );
	}

	/*
	To look at the mesh, we place a camera into 3d space at the position
	(0, 30, -40). The camera looks from there to (0,5,0), which is
	approximately the place where our md2 model is.
	*/
	sceneManager->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

	/*
	Ok, now we have set up the scene, lets draw everything: We run the
	device in a while() loop, until the device does not want to run any
	more. This would be when the user closes the window or presses ALT+F4
	(or whatever keycode closes a window).
	*/
	
	u32 then = device->getTimer()->getTime();
	const u32 frameLength = 16;
	unsigned int frameNumber = 0;
	
	/* TEMP */
	irr::core::vector3df cameraPosition;
    const auto /*irr::scene::ICameraSceneNode*/ camera = sceneManager->getActiveCamera();
    irr::core::vector3df cameraPositionState = irr::core::vector3df(camera->getPosition());
	
	while(device->run())
	{
	
	    const u32 now = device->getTimer()->getTime();
	
	    if( (now - then) < frameLength )
	    {
	        // not enough time has passed yet to execute the next frame
	        continue;
	    }
	    
	    then = now;
	    frameNumber++;
        
        
        // TODO: abstract into inputs 
	    gca::Process(adapter_buffer);
	    std::cout << adapter_buffer[0] << std::endl;
	    std::cout << now << std::endl;
	    
	    
	    // updateGameState()
	    
	    /*
	    Anything can be drawn between a beginScene() and an endScene()
	    call. The beginScene() call clears the screen with a color and
	    the depth buffer, if desired. Then we let the Scene Manager and
	    the GUI Environment draw their content. With the endScene()
	    call everything is presented on the screen.
	    */
	    driver->beginScene(true, true, SColor(255,100,101,140));
	    
	    cameraPositionState.Y += 0.4f * (f32)sin((double)frameNumber / 50.0f);
	    camera->setPosition(cameraPositionState);
        
	    
	    
	    // call updateSceneState()
	    sceneManager->drawAll();
	    
	    
	    // need to call updateGuiState()
	    guienv->drawAll();

	    driver->endScene();
	}


	/*
	After we are done with the render loop, we have to delete the Irrlicht
	Device created before with createDevice(). In the Irrlicht Engine, you
	have to delete all objects you created with a method or function which
	starts with 'create'. The object is simply deleted by calling ->drop().
	See the documentation at irr::IReferenceCounted::drop() for more
	information.
	*/
	gca::Stop();
	device->drop();
	
    return 0;
}
