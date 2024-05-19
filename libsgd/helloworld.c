//
// This can be built with something like:
//
// gcc -I../include -L. -DSGD_DYNAMIC=1 -o helloworld helloworld.c -lsgd_dynamic

#include <sgd/sgd.h>

int main() {

	sgd_Init();
    
    sgd_CreateWindow(640, 480, "Hello World!", 0);
    sgd_CreateScene();
    sgd_SetSceneClearColor(1,.5,0,1);
    
    while(!sgd_PollEvents()) {
		if(sgd_KeyHit(SGD_KEY_ESCAPE)) break;
        sgd_RenderScene();
        sgd_Present();
    }

	sgd_Terminate();
}
