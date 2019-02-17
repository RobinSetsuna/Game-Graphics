#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		// The application's handle
		"DirectX Game",	   	// Text for the window's title bar
		1280,			// Width of the window's client area
		720,			// Height of the window's client area
		true)			// Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	/*if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }*/

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
	delete cam;
	delete mat;
	delete bench;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	LoadShaders();
	mouseDown = false;
	cam = new Camera(width, height);
	bench = new Mesh("helix.obj", device);
	mat = new Material(vertexShader, pixelShader);

	DiretionalLight dirLight1;
	dirLight1.Init(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.2470f, 0.f, 0.4f, 1), XMFLOAT3(1, 0, 0));
	DiretionalLight dirLight2;
	dirLight2.Init(XMFLOAT4(0.2f, 0.3f, 0.35f, 1.0f), XMFLOAT4(0.2f, 0.2f, 1.f, 1), XMFLOAT3(0, 0, 1));

	pixelShader->SetData("DirLight1", &dirLight1, sizeof(DiretionalLight));
	pixelShader->SetData("DirLight2", &dirLight2, sizeof(DiretionalLight));
	
	entities[0] = Entity(bench, mat);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	entities[0].Rotate(0, deltaTime, 0);
	//entities[1].Move(0, deltaTime, 0);
	//entities[2].Scale(deltaTime*0.1f, deltaTime*0.1f, deltaTime*0.1f);
	//entities[2].Rotate(0, 0, deltaTime);
	//entities[3].Move(-deltaTime, -deltaTime, 0);
	//entities[4].Move(deltaTime, -deltaTime, 0);
	//entities[4].Rotate(0, 0, deltaTime);

	XMVECTOR dir = XMVectorScale(XMLoadFloat3(&cam->GetDirection()), deltaTime * 10);
	XMVECTOR up = XMVectorSet(0, deltaTime * 10, 0, 0);
	XMVECTOR left = XMVectorScale(XMVector3Normalize(XMVector3Cross(dir, up)), deltaTime * 10);
	XMVECTOR incrementVector = XMVectorSet(0,0,0,0);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		incrementVector = XMVectorAdd(incrementVector, dir);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		incrementVector = XMVectorSubtract(incrementVector, dir);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		incrementVector = XMVectorAdd(incrementVector, left);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		incrementVector = XMVectorSubtract(incrementVector, left);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		incrementVector = XMVectorAdd(incrementVector, up);
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		incrementVector = XMVectorSubtract(incrementVector, up);
	}

	cam->Update(incrementVector);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	//obj1.SetTranslation(0, 0, 0);
	for (int i = 0; i < 1; i++)
	{
		entities[i].PrepareMaterial(cam->GetViewMatrix(), cam->GetProjectionMatrix());

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		//square
		ID3D11Buffer* vbuffer = entities[i].mesh->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset);
		entities[i].mesh->GetIndexBuffer();
		context->IASetIndexBuffer(entities[i].mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(entities[i].mesh->GetIndexNumber(), 0, 0);
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	mouseDown = true;
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	mouseDown = false;
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (mouseDown == true)
	{
		int offsetX = x - prevMousePos.x;
		int offsetY = y - prevMousePos.y;
		cam->SetRotation(offsetY * 0.005f, offsetX * 0.005f);
		cam->Rotate();
	}
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion