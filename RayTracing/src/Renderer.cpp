#include "Renderer.h"
#include "Walnut/Random.h"

void Renderer::Render() {
	//horizontal to get more cache hits
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {

			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f; //remaping from 0 -> 1 to -1 -> 1
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

void Renderer::OnResize(uint32_t width, uint32_t height) {

	if (m_FinalImage) {
		// No change
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height); // internal check for change in width and/or height
	} else {
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];		
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.5f);
	float radius = 0.5f;
	//rayDirection = glm::normalize(rayDirection);

	// (b.x^2 + b.y^2 + b.z^2)t^2 + 2(a.yb.y + a.xb.x + a.zb.z)t + (a.y^2 + a.x^2 + a.z^2 - r^2) = 0
	// a = ray origin (starting point)
	// b = ray direction 
	// r = radius
	// t = ray magnitude (where we hit)

	// terms a b c come from the quadradic equation ax^2 + bx + c = 0
	// float a = (rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z); // this is happens to be the dot product with itself
	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	// Quadratic formula discriminant
	// b^2 - 4ac
	float discriminant = b * b - 4.0f * a * c;
	if (discriminant >= 0) 
		return 0xffff00ff;
	
	return 0xff000000;
}
