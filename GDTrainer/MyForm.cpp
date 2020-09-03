#include "MyForm.h"
#include <Windows.h>


using namespace GDTrainer;

int Main()
{

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	MyForm^ NewUi = gcnew MyForm();
	Application::Run(NewUi);

	return 1;


}