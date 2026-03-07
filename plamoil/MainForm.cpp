#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

#include <windows.h>

[STAThreadAttribute]
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    try { 
        plamoil::MainForm form;
        Application::Run(% form);
    }
    catch (System::Exception^ ex) {
        MessageBox::Show("Error initializing UI:\n" + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }

    return 0;
}