//D3D hook by n7

#include "main.h" //helper funcs here


typedef HRESULT(APIENTRY *SetStreamSource)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
SetStreamSource SetStreamSource_orig = 0;

typedef HRESULT(APIENTRY *SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
SetVertexDeclaration SetVertexDeclaration_orig = 0;

typedef HRESULT(APIENTRY *SetVertexShaderConstantF)(IDirect3DDevice9*, UINT, const float*, UINT);
SetVertexShaderConstantF SetVertexShaderConstantF_orig = 0;

typedef HRESULT(APIENTRY *SetVertexShader)(IDirect3DDevice9*, IDirect3DVertexShader9*);
SetVertexShader SetVertexShader_orig = 0;

typedef HRESULT(APIENTRY *SetPixelShader)(IDirect3DDevice9*, IDirect3DPixelShader9*);;
SetPixelShader SetPixelShader_orig = 0;


typedef HRESULT(APIENTRY *DrawIndexedPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
DrawIndexedPrimitive DrawIndexedPrimitive_orig = 0;

typedef HRESULT(APIENTRY *DrawPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, UINT, UINT);
DrawPrimitive DrawPrimitive_orig = 0;

typedef HRESULT(APIENTRY *SetTexture)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9 *);
SetTexture SetTexture_orig = 0;

typedef HRESULT(APIENTRY* Present) (IDirect3DDevice9*, const RECT *, const RECT *, HWND, const RGNDATA *);
Present Present_orig = 0;

typedef HRESULT(APIENTRY* EndScene) (IDirect3DDevice9*);
EndScene EndScene_orig = 0;

typedef HRESULT(APIENTRY *Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
Reset Reset_orig = 0;

//==========================================================================================================================

WNDPROC game_wndproc = NULL;

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		//Log("ImGui_ImplWin32_WndProcHandler");
		return true;
	}

	return CallWindowProc(game_wndproc, hWnd, msg, wParam, lParam);
}

//==========================================================================================================================

HRESULT APIENTRY DrawIndexedPrimitive_hook(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	//models
	//Stride == 40
	//bullet
	//Stride == 48 && NumVertices == 2005 && vSize == 840
	//outline
	//Stride == 48 && NumVertices == 220 && vSize == 1000
	//weapon
	//Stride == 48 && NumVertices == 433 && vSize == 1356
	//snowboard
	//Stride == 40 && NumVertices != 185 && vSize == 2740
	//26grass, 27chest

	if(aimbot|| distanceesp || circleesp || lineesp > 0)
	//if(NumVertices != 185)//snowboard
	if (NumVertices > tolerance)//1160treasure chest
	if (Stride == 40 && vSize > 2500 && numElements == 7) //compatibility
	//if (Stride == 24 && vSize == 320)//unk
	//if (vSize == 2884 || vSize == 3556) //2884near, 3556far
	{
		AddModels(pDevice, 189);
		AddModels(pDevice, 180);
	}

	if (wallhack)
	if ((Stride == 40)||(Stride == 48 && vSize == 840)||(Stride == 48 && vSize == 1000)||(Stride == 48 && vSize == 1356))
	{
		float bias = 1000.0f;
		float bias_float = static_cast<float>(-bias);
		bias_float /= 2000;//10000.0f;
		pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&bias_float);
		DrawIndexedPrimitive_orig(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		pDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
	}

	/*
	//hold down P key until a texture disappears, press END to log values of those textures
	if (GetAsyncKeyState('O') & 1) //-
		countnum--;
	if (GetAsyncKeyState('P') & 1) //+
		countnum++;
	if (GetAsyncKeyState(VK_MENU) && GetAsyncKeyState('9') & 1) //reset, set to -1
		countnum = -1;

	if (countnum == NumVertices / 10)
		if (GetAsyncKeyState(VK_END) & 1) //log
			Log("Stride == %d && NumVertices == %d && vSize == %d && pSize == %d numElements == %d && decl->Type == %d && mStartregister == %d && mVectorCount == %d",
				Stride, NumVertices, vSize, pSize, numElements, decl->Type, mStartregister, mVectorCount);

	if (countnum == NumVertices / 10)
		return D3D_OK;
	*/

	return DrawIndexedPrimitive_orig(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

//==========================================================================================================================

HRESULT APIENTRY Present_hook(IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{
	if (pDevice == nullptr) return Present_orig(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	if (!is_imgui_initialised)
	{
		//generate texture
		//GenerateTexture(pDevice, &Red, D3DCOLOR_ARGB(255, 255, 0, 0));
		//GenerateTexture(pDevice, &Green, D3DCOLOR_RGBA(0, 255, 0, 255));
		//GenerateTexture(pDevice, &Blue, D3DCOLOR_ARGB(255, 0, 0, 255));
		//GenerateTexture(pDevice, &Yellow, D3DCOLOR_ARGB(255, 255, 255, 0));

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard; //control menu with mouse
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

		// Find a handle to the first top-level window belonging to the game process.
		//EnumWindows(find_game_hwnd, GetCurrentProcessId());

		// Window handle to which focus belongs for this Direct3D device (similar to EnumWindows.. above)
		D3DDEVICE_CREATION_PARAMETERS d3dcp;
		pDevice->GetCreationParameters(&d3dcp);
		game_hwnd = d3dcp.hFocusWindow;

		if (game_hwnd != NULL)
		{
			// Swap out the window message handler for our own, allowing us to intercept input events
			game_wndproc = (WNDPROC)SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

			// Perform final ImGui setup tasks and..
			ImGui_ImplWin32_Init(game_hwnd);
			ImGui_ImplDX9_Init(pDevice);
			ImGui::GetIO().ImeWindowHandle = game_hwnd;

			//Load cfg
			LoadCfg();

			is_imgui_initialised = true;
		}
	}

	//get viewport (need to use imgui shit else coords will be wrong)
	ImGuiIO io = ImGui::GetIO();
	ViewportWidth = io.DisplaySize.x;
	ViewportHeight = io.DisplaySize.y;
	ScreenCX = ViewportWidth / 2.0f;
	ScreenCY = ViewportHeight / 2.0f;

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//info
	if (info)
	{
		ImGui::SetNextWindowSize(ImVec2(560.0f, 20.0f)); //size
		ImVec4 Bgcol = ImColor(0.0f, 0.4f, 0.28f, 0.8f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, Bgcol);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));

		ImGui::Begin("title", &info, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
		ImGui::Text("Press INSERT for menu, ESC & MOUSE or TAB + ARROWS + SPACE to navigate ");
		ImGui::End();

		static DWORD lastTime = timeGetTime();
		DWORD timePassed = timeGetTime() - lastTime;
		if (timePassed > 7000)
		{
			info = false;
			lastTime = timeGetTime();
		}
	}

	//menu key
	//GetAsyncKeyState & 1 may not work in every game
	static auto is_down = false;
	static auto is_clicked = false;
	if (GetAsyncKeyState(VK_INSERT))
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!GetAsyncKeyState(VK_INSERT) && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else {
		is_clicked = false;
		is_down = false;
	}

	//show menu
	if (is_clicked)
	{
		ShowMenu = !ShowMenu;
		//save settings
		SaveCfg();
	}

	//mouse cursor on in menu
	if (ShowMenu)
		ImGui::GetIO().MouseDrawCursor = 1;
	else
		ImGui::GetIO().MouseDrawCursor = 0;

	//draw menu
	if (ShowMenu)
	{
		//ImGui::SetNextWindowPos(ImVec2(50.0f, 400.0f)); //pos
		ImGui::SetNextWindowSize(ImVec2(410.0f, 450.0f)); //size
		ImVec4 Bgcol = ImColor(0.0f, 0.4f, 0.28f, 0.8f); //bg color
		ImGui::PushStyleColor(ImGuiCol_WindowBg, Bgcol);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 0.8f)); //frame color
		const char* lineesp_Options[] = { "Up", "Center", "Down", "Off" };
		const char* aimkey_Options[] = { "Off", "Shift", "Right Mouse", "Left Mouse", "Ctrl", "Tab", "Space", "X", "C" };

		ImGui::Begin("Hack Menu");
		ImGui::Checkbox("Wallhack", &wallhack);
		ImGui::Checkbox("Distance Esp", &distanceesp);
		ImGui::Checkbox("Circle Esp", &circleesp);

		ImGui::Text("Line Esp");
		ImGui::SameLine();
		ImGui::Combo("##LineEsp", (int*)&lineesp, lineesp_Options, IM_ARRAYSIZE(lineesp_Options));

		ImGui::Checkbox("Aimbot", &aimbot);
		//ImGui::SliderInt("Aim Key", &aimkey, 0, 8);

		ImGui::Text("Aim Key");
		ImGui::SameLine();
		ImGui::Combo("##AimKey", (int*)&aimkey, aimkey_Options, IM_ARRAYSIZE(aimkey_Options));

		ImGui::SliderInt("Aim Sensitivity", &aimsens, 0, 10);
		ImGui::SliderInt("Aim Field of View", &aimfov, 0, 8);
		ImGui::SliderInt("Aim Height", &aimheight, 0, 14);
		ImGui::SliderInt("Pre Aim", &preaim, 0, 5);
		ImGui::Checkbox("Autoshoot", &autoshoot);
		//ImGui::SliderInt("countnum", &countnum, -1, 100); //debug
		ImGui::End();

	}


	//Shift|RMouse|LMouse|Ctrl|Alt|Space|X|C
	if (aimkey == 0) Daimkey = 0;
	if (aimkey == 1) Daimkey = VK_SHIFT;
	if (aimkey == 2) Daimkey = VK_RBUTTON;
	if (aimkey == 3) Daimkey = VK_LBUTTON;
	if (aimkey == 4) Daimkey = VK_CONTROL;
	if (aimkey == 5) Daimkey = VK_MENU;
	if (aimkey == 6) Daimkey = VK_SPACE;
	if (aimkey == 7) Daimkey = 0x58; //X
	if (aimkey == 8) Daimkey = 0x43; //C

	//do esp
	if (distanceesp || circleesp || lineesp > 0)
	{
		ImGui::Begin("Transparent", reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings);
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

		if (ModelEspInfo.size() != NULL)
		{
			for (unsigned int i = 0; i < ModelEspInfo.size(); i++)
			{
				if (ModelEspInfo[i].pOutX > 1.0f && ModelEspInfo[i].pOutY > 1.0f && ModelEspInfo[i].RealDistance > 14.0f)
				{
					//draw cricle
					if (circleesp)
						ImGui::GetWindowDrawList()->AddCircle(ImVec2(ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY), 4000 / ModelEspInfo[i].RealDistance, IM_COL32(255, 255, 255, 255), 12, 2.0f); //scale with distance

						//draw line
					if (lineesp == 0)
						ImGui::GetWindowDrawList()->AddLine({ ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY }, ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2 * (0)), IM_COL32(255, 255, 255, 255), 1.0f);//0up, 10middle, 20down
					else if (lineesp == 1)
						ImGui::GetWindowDrawList()->AddLine({ ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY }, ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2 * (1)), IM_COL32(255, 255, 255, 255), 1.0f);//0up, 10middle, 20down
					else if (lineesp == 2)
						ImGui::GetWindowDrawList()->AddLine({ ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY }, ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2 * (2)), IM_COL32(255, 255, 255, 255), 1.0f);//0up, 10middle, 20down

					//draw text
					if (distanceesp)
					{
						//ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY), ImColor(255, 255, 0, 255), "Model", 0, 0.0f, 0); //draw text
						if (ModelEspInfo[i].RealDistance > 12.0f && ModelEspInfo[i].RealDistance < 200.0f)
							ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY), ImColor(255, 255, 0, 255), VariableText("%.f", ModelEspInfo[i].RealDistance), 0, 0.0f, 0); //draw variable
						else if (ModelEspInfo[i].RealDistance > 200.0f && ModelEspInfo[i].RealDistance < 1000.0f)
							ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY), ImColor(255, 255, 255, 255), VariableText("%.f", ModelEspInfo[i].RealDistance), 0, 0.0f, 0); //draw variable
						else if (ModelEspInfo[i].RealDistance > 1000.0f)
							ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY), ImColor(255, 122, 122, 122), VariableText("%.f", ModelEspInfo[i].RealDistance), 0, 0.0f, 0); //draw variable
					}
				}
			}
		}
		ImGui::End();
	}

	//do aim
	if (aimbot)
		if (ModelEspInfo.size() != NULL)
		{
			UINT BestTarget = -1;
			DOUBLE fClosestPos = 99999;

			for (unsigned int i = 0; i < ModelEspInfo.size(); i++)
			{
				//aimfov
				float radiusx = (aimfov*5.0f) * (ScreenCX / 100.0f);
				float radiusy = (aimfov*5.0f) * (ScreenCY / 100.0f);

				if (aimfov == 0)
				{
					radiusx = 5.0f * (ScreenCX / 100.0f);
					radiusy = 5.0f * (ScreenCY / 100.0f);
				}

				//get crosshairdistance
				ModelEspInfo[i].CrosshairDistance = GetDistance(ModelEspInfo[i].pOutX, ModelEspInfo[i].pOutY, ScreenCX, ScreenCY);

				//if in fov
				if (ModelEspInfo[i].pOutX >= ScreenCX - radiusx && ModelEspInfo[i].pOutX <= ScreenCX + radiusx && ModelEspInfo[i].pOutY >= ScreenCY - radiusy && ModelEspInfo[i].pOutY <= ScreenCY + radiusy)

					//get closest/nearest target to crosshair
					if (ModelEspInfo[i].CrosshairDistance < fClosestPos)
					{
						fClosestPos = ModelEspInfo[i].CrosshairDistance;
						BestTarget = i;
					}
			}


			//if nearest target to crosshair
			if (BestTarget != -1 && ModelEspInfo[BestTarget].RealDistance > 12.0f)
			{
				double DistX = ModelEspInfo[BestTarget].pOutX - ScreenCX;
				double DistY = ModelEspInfo[BestTarget].pOutY - ScreenCY;

				DistX /= (float)aimsens;
				DistY /= (float)aimsens;

				//aim
				if (GetKeyState(Daimkey) & 0x8000)
					mouse_event(MOUSEEVENTF_MOVE, (float)DistX, (float)DistY, 0, NULL);

				//autoshoot on
				if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000)))
				{
					if (autoshoot == 1 && !IsPressed)
					{
						IsPressed = true;
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					}
				}
			}
		}
	//if(esp||aimbot)
	ModelEspInfo.clear();

	//autoshoot off
	if (autoshoot == 1 && IsPressed)
	{
		if (timeGetTime() - astime >= asdelay)
		{
			IsPressed = false;
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			astime = timeGetTime();
		}
	}

	// Rendering
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Present_orig(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

//==========================================================================================================================

HRESULT APIENTRY Reset_hook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT ResetReturn = Reset_orig(pDevice, pPresentationParameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return ResetReturn;
}

//=====================================================================================================================

HRESULT APIENTRY DrawPrimitive_hook(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	return DrawPrimitive_orig(pDevice, PrimitiveType, StartVertex, PrimitiveCount);
}

//=====================================================================================================================

HRESULT APIENTRY EndScene_hook(IDirect3DDevice9* pDevice)
{
	return EndScene_orig(pDevice);
}

//==========================================================================================================================

HRESULT APIENTRY SetStreamSource_hook(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT sStride)
{
	if (StreamNumber == 0)
		Stride = sStride;

	return SetStreamSource_orig(pDevice, StreamNumber, pStreamData, OffsetInBytes, sStride);
}

//==========================================================================================================================

HRESULT APIENTRY SetVertexDeclaration_hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexDeclaration9* pdecl)
{
	if (pdecl != NULL)
	{
		pdecl->GetDeclaration(decl, &numElements);
	}

	return SetVertexDeclaration_orig(pDevice, pdecl);
}

//==========================================================================================================================

HRESULT APIENTRY SetVertexShaderConstantF_hook(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const float *pConstantData, UINT Vector4fCount)
{
	if (pConstantData != NULL)
	{
		mStartregister = StartRegister;
		mVectorCount = Vector4fCount;
	}

	return SetVertexShaderConstantF_orig(pDevice, StartRegister, pConstantData, Vector4fCount);
}

//==========================================================================================================================

HRESULT APIENTRY SetVertexShader_hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexShader9 *veShader)
{
	if (veShader != NULL)
	{
		vShader = veShader;
		vShader->GetFunction(NULL, &vSize);
	}
	return SetVertexShader_orig(pDevice, veShader);
}

//==========================================================================================================================

HRESULT APIENTRY SetPixelShader_hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DPixelShader9 *piShader)
{
	if (piShader != NULL)
	{
		pShader = piShader;
		pShader->GetFunction(NULL, &pSize);
	}
	return SetPixelShader_orig(pDevice, piShader);
}

//==========================================================================================================================

HRESULT APIENTRY SetTexture_hook(LPDIRECT3DDEVICE9 pDevice, DWORD Sampler, IDirect3DBaseTexture9 *pTexture)
{
	/*
	//get texture/desc size
	if (numElements ==  && decl->Type ==  && pTexture)
	{
		texture = static_cast<IDirect3DTexture9*>(pTexture);
		if (texture)
		{
			if (FAILED(texture->GetLevelDesc(0, &sDesc)))
				goto out;

			if (sDesc.Pool == D3DPOOL_MANAGED && texture->GetType() == D3DRTYPE_TEXTURE && sDesc.Width == 64 && sDesc.Height == 64)
			{
				HRESULT hr = texture->LockRect(0, &pLockedRect, NULL, D3DLOCK_DONOTWAIT | D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK);

				if (SUCCEEDED(hr))
				{
					if (pLockedRect.pBits != NULL)
						qCRC = QuickChecksum((DWORD*)pLockedRect.pBits, pLockedRect.Pitch); //get crc
						//qCRC = QuickChecksum((DWORD*)pLockedRect.pBits, 12);
				}
				texture->UnlockRect(0);
			}
		}
	}
out:
	*/
	return SetTexture_orig(pDevice, Sampler, pTexture);
}

//==========================================================================================================================

DWORD WINAPI CDimgui(LPVOID lpParameter)
{
	while (!GetModuleHandleA("d3d9.dll")) {
		Sleep(200);
	}

	IDirect3D9* d3d = NULL;
	IDirect3DDevice9* d3ddev = NULL;

	HWND tmpWnd = CreateWindowA("BUTTON", "CD", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, Hand, NULL);
	if (tmpWnd == NULL)
	{
		//Log("[DirectX] Failed to create temp window");
		return 0;
	}

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		DestroyWindow(tmpWnd);
		//Log("[DirectX] Failed to create temp Direct3D interface");
		return 0;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = tmpWnd;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, tmpWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (result != D3D_OK)
	{
		d3d->Release();
		DestroyWindow(tmpWnd);
		//MessageBox(game_hwnd, "Run the game first and inject dll later", "Failed to create temp Direct3D device.", MB_OK);
		MessageBox(game_hwnd, L"Run the game first and inject dll later", L"Failed to create temp Direct3D device.", MB_OK);
		//Log("[DirectX] Failed to create temp Direct3D device. Run the game first and inject dll later"); 
		return 0;
	}

	// We have the device, so walk the vtable to get the address of all the dx functions in d3d9.dll
#if defined _M_X64
	DWORD64* dVtable = (DWORD64*)d3ddev;
	dVtable = (DWORD64*)dVtable[0];
#elif defined _M_IX86
	DWORD* dVtable = (DWORD*)d3ddev;
	dVtable = (DWORD*)dVtable[0]; // == *d3ddev
#endif
	//Log("[DirectX] dVtable: %x", dVtable);

	//for(int i = 0; i < 95; i++)
	//{
			//Log("[DirectX] vtable[%i]: %x, pointer at %x", i, dVtable[i], &dVtable[i]);
	//}

	// Detour functions x86 & x64
	SetStreamSource_orig = (SetStreamSource)dVtable[100];
	SetVertexDeclaration_orig = (SetVertexDeclaration)dVtable[87];
	SetVertexShaderConstantF_orig = (SetVertexShaderConstantF)dVtable[94];
	SetVertexShader_orig = (SetVertexShader)dVtable[92];
	SetPixelShader_orig = (SetPixelShader)dVtable[107];

	DrawIndexedPrimitive_orig = (DrawIndexedPrimitive)dVtable[82];
	DrawPrimitive_orig = (DrawPrimitive)dVtable[81];
	SetTexture_orig = (SetTexture)dVtable[65];
	Present_orig = (Present)dVtable[17];
	EndScene_orig = (EndScene)dVtable[42];
	Reset_orig = (Reset)dVtable[16];
	
	Sleep(2000);//required in a few games

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)SetStreamSource_orig, (PBYTE)SetStreamSource_hook);
	DetourAttach(&(LPVOID&)SetVertexDeclaration_orig, (PBYTE)SetVertexDeclaration_hook);
	//DetourAttach(&(LPVOID&)SetVertexShaderConstantF_orig, (PBYTE)SetVertexShaderConstantF_hook);
	DetourAttach(&(LPVOID&)SetVertexShader_orig, (PBYTE)SetVertexShader_hook);
	//DetourAttach(&(LPVOID&)SetPixelShader_orig, (PBYTE)SetPixelShader_hook);

	DetourAttach(&(LPVOID&)DrawIndexedPrimitive_orig, (PBYTE)DrawIndexedPrimitive_hook);
	//DetourAttach(&(LPVOID&)DrawPrimitive_orig, (PBYTE)DrawPrimitive_hook);
	//DetourAttach(&(LPVOID&)SetTexture_orig, (PBYTE)SetTexture_hook);
	DetourAttach(&(LPVOID&)Present_orig, (PBYTE)Present_hook);
	//DetourAttach(&(LPVOID&)EndScene_orig, (PBYTE)EndScene_hook);
	DetourAttach(&(LPVOID&)Reset_orig, (PBYTE)Reset_hook);
	DetourTransactionCommit();
	
	//Log("[Detours] Detours attached\n");

	d3ddev->Release();
	d3d->Release();
	DestroyWindow(tmpWnd);

	return 1;
}

//==========================================================================================================================

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Hand = hModule;
		DisableThreadLibraryCalls(hModule); //disable unwanted thread notifications to reduce overhead
		GetModuleFileNameA(hModule, dlldir, 512);
		for (int i = (int)strlen(dlldir); i > 0; i--)
		{
			if (dlldir[i] == '\\')
			{
				dlldir[i + 1] = 0;
				break;
			}
		}
		CreateThread(0, 0, CDimgui, 0, 0, 0); //init our hooks

		break;
	case DLL_PROCESS_DETACH:
		// Uninitialize wndProc
		SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG>(WndProc));
		// Uninitialize ImGui
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX9_Shutdown();
		ImGui::DestroyContext();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
