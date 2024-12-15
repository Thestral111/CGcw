#include <Windows.h>
#include "Window.h"
#include "Device.h"
#include "Shaders.h"
#include "Mesh.h"
#include "geometry.h"
#include "GamesEngineeringBase.h"
#include "GEMLoader.h"
#include "Texture.h"
#include "Anim.h"
#include "Camera.h"
#include "Light.h"


// control which animation to play based on distance and collision
string animationController(Camera& cam, AnimationInstance& inst,Vec3 pos) {
	string anim;
	Vec3 offSet = pos - cam.position;
	float dist = offSet.length();
	if (dist < 20.0f) {
		anim = "roar";
	}
	else
	{
		anim = "Run";
	}
	if (cam.box.checkAABBCollision(inst.box)) {
		anim = "attack";
	}
	return anim;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	DXCore dxcore;
	Shader shader;
	Shader planeShader;
	Shader animShader;
	//Triangle tri;
	plane plane;
	cube cube;

	Matrix treeWorld;
	Matrix v1;
	Matrix p1;
	TextureManager textureManager;
	TextureManager textureManager1;
	AnimationInstance instance;
	Camera camera;
	Skybox skybox;
	Light light;
	AABB playerBox;
	
	
	treeWorld = treeWorld.scaling(Vec3(0.01f, 0.01f, 0.01f));
	Matrix w1;
	w1 = Matrix::translation(Vec3(2, 0, 0));
	Matrix w2;
	w2 = w2.scaling(Vec3(10.f, 10.f, 10.f));
	
	GamesEngineeringBase :: Timer timer;
	win.init(1024, 1024, "My Window");
	dxcore.init(1024, 1024, win.hwnd, 0);
	shader.initWithNormal("3D_vertexShader.txt", "newPixelShader.txt", dxcore);

	planeShader.init("3D_vertexShader.txt", "pixelShader.txt", dxcore); 
	animShader.initAnim("Animation_vertexShader.txt", "newPixelShader.txt", dxcore);

	// init the Dinosaur (has animation)
	instance.init("TRex.gem", dxcore, textureManager1);
	// update the bounding box of the dinosaur as there was a translation of the world matrix
	instance.box.updatePos(Vec3(2, 0, 0));

	// init the plane
	plane.init(dxcore);

	Matrix x;

	// init the tree
	StaticModel tree;
	tree.load(dxcore, "Resource/pine.gem", textureManager);
	// update the bounding box of the tree
	tree.boundingBox = tree.boundingBox.transformAABB(treeWorld);

	wstring str = L"\n";
	OutputDebugString(to_wstring(tree.boundingBox.max.x).c_str());
	OutputDebugString(str.c_str());
	OutputDebugString(to_wstring(tree.boundingBox.max.y).c_str());
	OutputDebugString(str.c_str());
	OutputDebugString(to_wstring(tree.boundingBox.max.z).c_str());

	// init the skybox based on six side texture
	std::vector<std::string> cubemapFaces = {
		"Standard-Cube-Map/px.png",  "Standard-Cube-Map/nx.png",
		"Standard-Cube-Map/py.png",    "Standard-Cube-Map/ny.png",
		"Standard-Cube-Map/pz.png",  "Standard-Cube-Map/nz.png"
	};
	skybox.init(dxcore, cubemapFaces);

	float t = 0;
	
	int lastMouseX = win.mousex;
	int lastMouseY = win.mousey;

	// for FPS calculation
	float time = 0;
	int counter = 0;

		// main loop (sorry the whole while function is indented)
		while (true)
		{
			// the delta time
			float dt = timer.dt();
			t += dt;

			// FPS counter
			time += dt;
			counter++;
			if (time >= 1.0f) {
				OutputDebugString(to_wstring(counter).c_str());
				OutputDebugString(str.c_str());
				counter = 0;
				time = 0;
			}
			
			
			// get direction of camera
			Vec3 forward = camera.getForwardDirection();
			Vec3 right = camera.getRightDirection();

			// keyboard for camera/player move
			if (win.keys['W']) camera.move(forward, dt);
			if (win.keys['S']) camera.move(-forward, dt);
			if (win.keys['D']) camera.move(right, dt);
			if (win.keys['A']) camera.move(-right, dt);

			// Mouse input for rotation
			if (win.mouseButtons[0]) { // Left mouse button is pressed
				int currentMouseX = win.mousex;
				int currentMouseY = win.mousey;

				float xOffset = static_cast<float>(currentMouseX - lastMouseX);
				float yOffset = static_cast<float>(currentMouseY - lastMouseY);

				camera.rotate(xOffset, yOffset);

				lastMouseX = currentMouseX;
				lastMouseY = currentMouseY;
				

			}
			else {
				lastMouseX = win.mousex;
				lastMouseY = win.mousey;
			}

			// compute the vp matrix
			// world is the rotation, translation and scale
			// vp is lookat and projection
			Matrix p = p1.Perspective(20.f, 1.f, 0.1f, 100.f);
			Matrix view = camera.getLookat();
			Matrix vp = view * p;
			
			// update camera bounding box
			camera.updateBoundingBox();
			// check the collision between camera and tree
			if (camera.box.checkAABBCollision(tree.boundingBox)) {
				std::cout << "Player collided with the tree!" << std::endl;
				wstring msg = L"Player collided with the tree!\n";
				OutputDebugString(msg.c_str());
			}

			dxcore.clear();
			win.processMessages();

			// render the sky box
			skybox.render(dxcore, camera, p);
			
			// draw the plane
			planeShader.updateConstantVS("staticMeshBuffer", "W", &w2); // planeWorld
			planeShader.updateConstantVS("staticMeshBuffer", "VP", &vp);
			planeShader.apply(dxcore);
			plane.draw(dxcore);
			// update constant buffer for the tree
			shader.updateConstantVS("staticMeshBuffer", "W", &treeWorld); 
			shader.updateConstantVS("staticMeshBuffer", "VP", &vp); // StaticModel
			shader.updateConstantVS("staticMeshBuffer", "lightPos", &light.position);
			shader.updateConstantPS("staticMeshBuffer", "lightColour", &light.color);
			shader.apply(dxcore);
			//draw the tree
			tree.draw(dxcore, textureManager);

			// decide which animation to play
			string animName = animationController(camera, instance,Vec3(2, 0, 0));
			instance.update(animName, dt);
			animShader.updateConstantVS("animatedMeshBuffer", "W", &w1); 
			animShader.updateConstantVS("animatedMeshBuffer", "VP", &vp);
			animShader.updateConstantVS("animatedMeshBuffer", "bones", instance.matrices); //anim
		
			animShader.apply(dxcore);
			// draw the dinosaur
			instance.draw(dxcore, animShader, textureManager1);
			
			
			
			dxcore.present();
		}

}
