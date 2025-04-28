#include "HoshinoCore.h"
using namespace std;

int main(int argc, char** argv)
{
    cout<<"Hoshino Engine Start" << endl;
    Hoshino::Application* app = Hoshino::CreateApplication();
    app->Run();
    delete app;
    return 0;
}