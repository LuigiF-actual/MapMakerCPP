#include "Core/Application.h"

#include "AppLayer.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "MapMakerC++";
	appSpec.WindowSpec.Width = 1920;
	appSpec.WindowSpec.Height = 1080;

	Core::Application application(appSpec);
	

	auto menuLayer = std::make_unique<MenuLayer>();
	auto appLayer = std::make_unique<AppLayer>();
	appLayer->SetActive(false);

	application.PushLayer(std::move(menuLayer));
	application.PushLayer(std::move(appLayer));

	application.Run();
}

