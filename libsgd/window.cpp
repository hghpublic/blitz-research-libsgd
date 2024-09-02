#include "internal.h"

void SGD_DECL sgd_CreateWindow(int width, int height, SGD_String title, int flags) {
	if (sgdx::g_mainWindow) sgd_Error("Window has already been created");
	sgdx::started();

	auto window = new sgd::Window(sgdx::Vec2u(width, height), title, (sgdx::WindowFlags)flags);

	window->closeClicked.connect(nullptr, [] { //
		sgdx::postEvent({SGD_EVENT_MASK_CLOSE_CLICKED});
	});

	window->sizeChanged.connect(nullptr, [](sgdx::CVec2u size) { //
		if (sgdx::g_mainScene) {
			sgd_RenderScene();
			sgd_Present();
		}
		sgdx::postEvent({SGD_EVENT_MASK_SIZE_CHANGED});
	});

	window->lostFocus.connect(nullptr, [] { //
		sgdx::postEvent({SGD_EVENT_MASK_LOST_FOCUS});
	});

	window->gotFocus.connect(nullptr, [] { //
		sgdx::postEvent({SGD_EVENT_MASK_GOT_FOCUS});
	});

	sgdx::g_mainWindow = window;

	sgd::createGC(window);

	sgd_ResetScene(false);
}

void SGD_DECL sgd_DestroyWindow() {
	delete sgdx::mainWindow();
	sgdx::g_mainWindow = nullptr;
}

void SGD_DECL sgd_SetWindowTitle(SGD_String title) {
	sgdx::mainWindow()->setTitle(title);
}

SGD_API SGD_String SGD_DECL sgd_GetWindowTitle() {
	static sgd::String title;
	title = sgdx::mainWindow()->title() + '\0';
	return title.data();
}

void SGD_DECL sgd_SetWindowPosition(int x, int y) {
	sgdx::mainWindow()->setPosition({x,y});
}

int SGD_DECL sgd_GetWindowX() {
	return sgdx::mainWindow()->position().x;

}

int SGD_DECL sgd_GetWindowY() {
	return sgdx::mainWindow()->position().y;
}

void SGD_DECL sgd_SetWindowSize(int width, int height) {
	sgdx::mainWindow()->setSize(sgd::Vec2u(width,height));
}

int SGD_DECL sgd_GetWindowWidth() {
	return (int)sgdx::mainWindow()->size().x;
}

int SGD_DECL sgd_GetWindowHeight() {
	return (int)sgdx::mainWindow()->size().y;
}



void SGD_DECL sgd_SetFullscreenMode(int width, int height, int hertz) {
	sgdx::mainWindow()->setFullscreenMode(sgd::Vec2u(width, height), hertz);
}

void SGD_DECL sgd_SetWindowState(int state) {
	sgdx::mainWindow()->setState((sgd::WindowState)state);
}

int SGD_DECL sgd_GetWindowState() {
	return (int)sgdx::mainWindow()->state();
}

SGD_Bool SGD_DECL sgd_IsKeyDown(int keyCode) {
	if ((uint32_t)keyCode > SGD_KEY_LAST) sgd_Error("Key code out of range");
	return sgdx::mainWindow()->keyboard()->key((sgd::KeyCode)keyCode).down();
}

SGD_Bool SGD_DECL sgd_IsKeyHit(int keyCode) {
	if ((uint32_t)keyCode > SGD_KEY_LAST) sgd_Error("Key code out of range");
	return sgdx::mainWindow()->keyboard()->key((sgd::KeyCode)keyCode).hit();
}

int SGD_DECL sgd_GetChar() {
	auto& queue = sgdx::mainWindow()->keyboard()->charQueue();
	if (queue.empty()) return 0;
	auto chr = queue.front();
	queue.pop_front();
	return (int)chr;
}

void SGD_DECL sgd_FlushChars() {
	auto& queue = sgdx::mainWindow()->keyboard()->charQueue();
	queue.clear();
}

float SGD_DECL sgd_GetMouseX() {
	return sgdx::mainWindow()->mouse()->position().x;
}

float SGD_DECL sgd_GetMouseY() {
	return sgdx::mainWindow()->mouse()->position().y;
}

float SGD_DECL sgd_GetMouseZ() {
	return sgdx::mainWindow()->mouse()->position().z;
}

float SGD_DECL sgd_GetMouseVX() {
	return sgdx::mainWindow()->mouse()->velocity().x;
}

float SGD_DECL sgd_GetMouseVY() {
	return sgdx::mainWindow()->mouse()->velocity().y;
}

float SGD_DECL sgd_GetMouseVZ() {
	return sgdx::mainWindow()->mouse()->velocity().z;
}

void SGD_DECL sgd_SetMouseZ(float z) {
	sgdx::mainWindow()->mouse()->setPositionZ(z);
}

void SGD_DECL sgd_SetMouseCursorMode(int cursorMode) {
	sgdx::mainWindow()->mouse()->cursorMode = (sgd::CursorMode)cursorMode;
}

SGD_Bool SGD_DECL sgd_IsMouseButtonDown(int button) {
	return sgdx::mainWindow()->mouse()->button(button).down();
}

SGD_Bool SGD_DECL sgd_IsMouseButtonHit(int button) {
	return sgdx::mainWindow()->mouse()->button(button).hit();
}

SGD_Bool SGD_DECL sgd_IsGamepadConnected(int gamepad) {
	sgdx::mainWindow();

	if ((uint32_t)gamepad >= sgdx::Window::maxGamepads) sgd_Error("Gamepad index out of range");

	return sgdx::Gamepad::getGamepad(gamepad)->connected();
}

SGD_Bool SGD_DECL sgd_IsGamepadButtonDown(int gamepad, int button) {
	sgdx::mainWindow();

	if ((uint32_t)gamepad >= sgdx::Window::maxGamepads) sgd_Error("Gamepad index out of range");
	if ((uint32_t)button >= sgdx::Gamepad::numButtons) sgd_Error("Gamepad button out of range");

	return sgdx::Gamepad::getGamepad(gamepad)->button(button).down();
}

SGD_Bool SGD_DECL sgd_IsGamepadButtonHit(int gamepad, int button) {
	sgdx::mainWindow();

	if ((uint32_t)gamepad >= sgdx::Window::maxGamepads) sgd_Error("Gamepad index out of range");
	if ((uint32_t)button >= sgdx::Gamepad::numButtons) sgd_Error("Gamepad button out of range");

	return sgdx::Gamepad::getGamepad(gamepad)->button(button).hit();
}

float SGD_DECL sgd_GetGamepadAxis(int gamepad, int axis) {
	sgdx::mainWindow();

	if ((uint32_t)gamepad >= sgdx::Window::maxGamepads) sgd_Error("Gamepad index out of range");
	if ((uint32_t)axis >= sgdx::Gamepad::numAxes) sgd_Error("Gamepad index out of range");

	return sgdx::Gamepad::getGamepad(gamepad)->axis(axis);
}
