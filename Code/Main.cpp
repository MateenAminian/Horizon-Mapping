#include "World.h"


using namespace Framework;


float	deltaTime;


namespace
{
	HWND			frameworkWindow;
	LARGE_INTEGER	counterFrequency;

	int32 previousTime = 0;
	bool quitFlag = false;					// Program quits when this flag is set.

	const float nearDepth = 0.1F;			// View frustum parameters.
	const float fovy = 60.0F;

	Point3D cameraPosition;					// View state.
	float viewAzimuth;
	float viewAltitude;

	bool leftButtonFlag = false;			// Input state.
	bool rightButtonFlag = false;
	bool forwardFlag = false;
	bool backwardFlag = false;
	bool leftFlag = false;
	bool rightFlag = false;
	bool upFlag = false;
	bool downFlag = false;

	float lastMouseX = 0.0F;
	float lastMouseY = 0.0F;

	CameraNode *cameraNode;
	TextGeometry *ambientDrawCountText;
	TextGeometry *lightDrawCountText;
	TextGeometry *lightSourceCountText;
}


void CalculateView(void)
{
	Vector2D t = CosSin(viewAzimuth * Math::two_pi);
	Vector2D p = CosSin(viewAltitude * Math::two_pi);
	Vector3D view(t.x * p.x, t.y * p.x, p.y);

	float f = InverseMag(view.xy);
	Vector3D right(view.y * f, -view.x * f, 0.0F);
	Vector3D down = Cross(view, right);

	cameraNode->nodeTransform[0] = right;
	cameraNode->nodeTransform[1] = down;
	cameraNode->nodeTransform[2] = view;
	cameraNode->nodeTransform[3] = cameraPosition;

	cameraNode->SetWorldTransform(cameraNode->nodeTransform);
}

void ResetView(void)
{
	cameraPosition.Set(0.0F, 0.0F, 1.0F);
	viewAzimuth = 0.0F;
	viewAltitude = 0.0F;

	CalculateView();
}

void HandleMouseDrag(float dx, float dy)
{
	float t = viewAzimuth - dx * 0.0005F;
	if (t < 0.0F) t += 1.0F;
	else if (t >= 1.0F) t -= 1.0F;
	viewAzimuth = t;

	viewAltitude = Clamp(viewAltitude - dy * 0.0005F, -0.23F, 0.23F);
}

void MoveCamera(void)
{
	float d = deltaTime * 50.0F;

	if (forwardFlag)
	{
		cameraPosition += cameraNode->nodeTransform[2] * d;
	}

	if (backwardFlag)
	{
		cameraPosition -= cameraNode->nodeTransform[2] * d;
	}

	if (leftFlag)
	{
		cameraPosition -= cameraNode->nodeTransform[0] * d;
	}

	if (rightFlag)
	{
		cameraPosition += cameraNode->nodeTransform[0] * d;
	}

	if (upFlag)
	{
		cameraPosition.z += d;
	}

	if (downFlag)
	{
		cameraPosition.z -= d;
	}

	CalculateView();
}

bool HandleKeyboardEvent(UINT message, WPARAM wparam, LPARAM lparam)
{
	if (message == WM_KEYDOWN)
	{
		if (wparam == VK_ESCAPE)
		{
			quitFlag = true;
		}
		else if (wparam == VK_F1)
		{
			ResetView();
		}
		else if (wparam == 'W')
		{
			forwardFlag = true;
		}
		else if (wparam == 'S')
		{
			backwardFlag = true;
		}
		else if (wparam == 'A')
		{
			leftFlag = true;
		}
		else if (wparam == 'D')
		{
			rightFlag = true;
		}
		else if (wparam == VK_SPACE)
		{
			upFlag = true;
		}
		else if (wparam == 'C')
		{
			downFlag = true;
		}
	}
	else if (message == WM_KEYUP)
	{
		if (wparam == 'W')
		{
			forwardFlag = false;
		}
		else if (wparam == 'S')
		{
			backwardFlag = false;
		}
		else if (wparam == 'A')
		{
			leftFlag = false;
		}
		else if (wparam == 'D')
		{
			rightFlag = false;
		}
		else if (wparam == VK_SPACE)
		{
			upFlag = false;
		}
		else if (wparam == 'C')
		{
			downFlag = false;
		}
	}

	return (true);
}

bool HandleMouseEvent(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (message == WM_LBUTTONDOWN)
	{
		leftButtonFlag = true;
		SetCapture(window);
	}
	else if ((message == WM_LBUTTONUP) || (message == WM_NCLBUTTONUP))
	{
		leftButtonFlag = false;
		if (!rightButtonFlag)
		{
			ReleaseCapture();
		}
	}
	else if (message == WM_RBUTTONDOWN)
	{
		rightButtonFlag = true;
		SetCapture(window);
	}
	else if ((message == WM_RBUTTONUP) || (message == WM_NCRBUTTONUP))
	{
		rightButtonFlag = false;
		if (!leftButtonFlag)
		{
			ReleaseCapture();
		}
	}
	else if ((message == WM_MOUSEMOVE) || (message == WM_NCMOUSEMOVE))
	{
		float x = float(LOWORD(lparam));
		float y = float(HIWORD(lparam));

		static bool firstTime = true;
		if (!firstTime)
		{
			float dx = x - lastMouseX;
			float dy = y - lastMouseY;

			HandleMouseDrag(dx, dy);
		}

		firstTime = false;
		lastMouseX = x;
		lastMouseY = y;
	}

	#if USE_RAW_INPUT

		else if ((message == WM_INPUT) && (GET_RAWINPUT_CODE_WPARAM(wparam) == RIM_INPUT))
		{
			UINT	size;

			GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
			char *data = new char[size];

			GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, data, &size, sizeof(RAWINPUTHEADER));
			const RAWINPUT *rawInput = reinterpret_cast<RAWINPUT *>(data);

			if (rawInput->header.dwType == RIM_TYPEMOUSE)
			{
				float dx = float(rawInput->data.mouse.lLastX);
				float dy = float(rawInput->data.mouse.lLastY);

				HandleMouseDrag(dx, dy);
			}

			delete[] data;
			return (false);		// According to API docs, DefWindowProc() needs to be called.
		}

	#endif

	return (true);
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
		case WM_CLOSE:

			quitFlag = true;
			return (0);

		case WM_KEYDOWN:
		case WM_KEYUP:

			if (HandleKeyboardEvent(message, wparam, lparam))
			{
				return (0);
			}

			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_NCRBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
		case WM_INPUT:
			
			if (HandleMouseEvent(window, message, wparam, lparam))
			{
				return (0);
			}

			break;
	}

	return (DefWindowProc(window, message, wparam, lparam));
}


int32 GetTimeValue(void)
{
	// This function returns an absolute time value in milliseconds.

	LARGE_INTEGER	counter;

	QueryPerformanceCounter(&counter);
	return (int32(counter.QuadPart * 1000U / counterFrequency.QuadPart));
}

void InitializeFramework(void)
{
	QueryPerformanceFrequency(&counterFrequency);
	previousTime = GetTimeValue();

	float width = graphicsManager->GetViewportWidth();
	float height = graphicsManager->GetViewportHeight();

	const float aspectRatio = width / height;
	float projectionDistance = 1.0F / Tan(fovy * (Math::pi / 180.0F) * 0.5F);

	cameraNode = new FrustumCamera(projectionDistance, aspectRatio, nearDepth);
	worldManager->GetRootNode()->AppendSubnode(cameraNode);

	OrthoCamera *overlayCamera = new OrthoCamera(width, height);
	worldManager->SetOverlayCameraNode(overlayCamera);

	ResetView();

	Font *font = new Font("Fonts/Arial.slug");
	float fontSize = graphicsManager->GetViewportHeight() * 0.015F;

	TextGeometry *titleText = new TextGeometry(font, fontSize, "CMPM 163", Color4U(255, 255, 255, 255));
	titleText->nodeTransform.SetTranslation(Point3D(fontSize, fontSize * 1.5F, 0.0F));
	worldManager->AddOverlayGeometryNode(titleText);

	ambientDrawCountText = new TextGeometry(font, fontSize, nullptr, Color4U(255, 255, 255, 255));
	ambientDrawCountText->nodeTransform.SetTranslation(Point3D(fontSize, fontSize * 3.0F, 0.0F));
	worldManager->AddOverlayGeometryNode(ambientDrawCountText);

	lightDrawCountText = new TextGeometry(font, fontSize, nullptr, Color4U(255, 255, 255, 255));
	lightDrawCountText->nodeTransform.SetTranslation(Point3D(fontSize, fontSize * 4.5F, 0.0F));
	worldManager->AddOverlayGeometryNode(lightDrawCountText);

	lightSourceCountText = new TextGeometry(font, fontSize, nullptr, Color4U(255, 255, 255, 255));
	lightSourceCountText->nodeTransform.SetTranslation(Point3D(fontSize, fontSize * 6.0F, 0.0F));
	worldManager->AddOverlayGeometryNode(lightSourceCountText);

	font->Release();

	worldManager->BuildWorld();
	worldManager->PreprocessWorld();
}

void TerminateFramework(void)
{
}

void RunFramework(void)
{
	for (;;)
	{
		MSG		message;
		int32	ambientDrawCount, lightDrawCount, lightSourceCount;

		int32 time = GetTimeValue();
		deltaTime = float(time - previousTime) * 0.001F;
		previousTime = time;

		while (PeekMessageW(&message, frameworkWindow, 0, 0, PM_REMOVE))
		{
			DispatchMessageW(&message);
		}

		if (quitFlag)
		{
			break;
		}

		MoveCamera();

		graphicsManager->BeginRendering();
		worldManager->RenderWorld(&ambientDrawCount, &lightDrawCount, &lightSourceCount);

		ambientDrawCountText->SetText(String<127>("Ambient draw count: ") + ambientDrawCount);
		lightDrawCountText->SetText(String<127>("Light draw count: ") + lightDrawCount);
		lightSourceCountText->SetText(String<127>("Light source count: ") + lightSourceCount);

		worldManager->RenderOverlay();
		graphicsManager->EndRendering();
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int cmdShow)
{
	WNDCLASSEXW		windowClass;

	static const wchar_t applicationName[] = L"CMPM 163 Framework";

	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = &WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = instance;
	windowClass.hIcon = nullptr;
	windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	windowClass.hbrBackground = nullptr;
	windowClass.lpszMenuName = nullptr;
	windowClass.lpszClassName = applicationName;
	windowClass.hIconSm = nullptr;
	RegisterClassExW(&windowClass);
	
	int32 screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int32 screenHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT rect = {0, 0, screenWidth, screenHeight};

	#if USE_FULL_SCREEN

		DWORD style = WS_VISIBLE | WS_POPUP;
		AdjustWindowRectEx(&rect, style, false, 0);
		frameworkWindow = CreateWindowExW(0, applicationName, applicationName, style, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, instance, nullptr);

	#else

		screenWidth -= 256;
		screenHeight -= 256;

		rect.left += 128;
		rect.top += 128;
		rect.right -= 128;
		rect.bottom -= 128;

		DWORD style = WS_VISIBLE | WS_BORDER | WS_CAPTION;
		AdjustWindowRectEx(&rect, style, false, 0);
		frameworkWindow = CreateWindowExW(0, applicationName, applicationName, style, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, instance, nullptr);

	#endif

	SetFocus(frameworkWindow);
	SetCursor(windowClass.hCursor);

	#if USE_RAW_INPUT

		RAWINPUTDEVICE		rawInputDevice;

		rawInputDevice.usUsagePage = 1;
		rawInputDevice.usUsage = 2;
		rawInputDevice.dwFlags = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE;
		rawInputDevice.hwndTarget = frameworkWindow;

		RegisterRawInputDevices(&rawInputDevice, 1, sizeof(RAWINPUTDEVICE));

	#endif

	graphicsManager = new GraphicsManager(screenWidth, screenHeight);
	if (graphicsManager->Initialize(instance, frameworkWindow))
	{
		worldManager = new WorldManager;

		InitializeFramework();
		RunFramework();
		TerminateFramework();

		delete worldManager;
		graphicsManager->Terminate(frameworkWindow);
	}

	delete graphicsManager;

	DestroyWindow(frameworkWindow);
	UnregisterClassW(applicationName, instance);
	return (0);
}
