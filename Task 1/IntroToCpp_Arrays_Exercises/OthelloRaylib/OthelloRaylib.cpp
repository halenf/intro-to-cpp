
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"

#include "Application.h"

int main(int argc, char* argv[])
{
    {
        Application app;
        app.Run();
    }

    return 0;
}
