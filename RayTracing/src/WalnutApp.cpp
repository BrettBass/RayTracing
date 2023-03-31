#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Renderer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Setting");
		ImGui::Text("Last render %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render")) {
			Render();
		}
		ImGui::End();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_renderer.GetFinalImage();
		if(image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight()}, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render(); //"real time" renderer
	}

	void Render() {
		Timer timer;

		m_renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_renderer.Render();

		m_LastRenderTime = timer.ElapsedMillis();
	}





	int descriminant(uint32_t i, uint32_t j, uint32_t k) {

	}

	std::vector<uint32_t> intersection(uint32_t i, uint32_t j, uint32_t k) {

	}

	void sphere() { // do later, (add z component)
		if (descriminant(1, 2, 3) >= 0) { 
			points.insert(points.end(), intersection(1, 2, 3).begin(), intersection(1, 2, 3).end());
		}
		else {
			points.insert(points.end(), 0);
		}
	}

	void RayPath() {

	}

private:
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0.0f;
	Renderer m_renderer;

	std::vector<uint32_t> points;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}