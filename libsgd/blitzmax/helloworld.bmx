Include "sgd.bmx"

SGD.LoadSGD "../lib/sgd_dynamic.dll"

SGD.Init()

SGD.CreateWindow(640, 480, "Hello World!", 0)
SGD.CreateScene()
SGD.SetSceneClearColor(1,.5,0,1)

While (SGD.PollEvents() & sgd.EVENT_MASK_CLOSE_CLICKED) = 0
    SGD.RenderScene()
    SGD.Present()
Wend

SGD.Terminate()