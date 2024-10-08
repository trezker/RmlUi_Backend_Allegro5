#include <RmlUi/Core.h>
#include <RmlUi/Core/RenderInterface.h>
#include <RmlUi/Core/Types.h>

#include <map>
#include <allegro5/allegro.h>

class RenderInterface_Allegro5 : public Rml::RenderInterface {
private:
	bool scissor_region_enabled = false;
	int clipx, clipy, clipw, cliph;
	struct GeometryView {
		Rml::Span<const Rml::Vertex> vertices;
		Rml::Span<const int> indices;
	};
public:
	Rml::CompiledGeometryHandle CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) override;
	void ReleaseGeometry(Rml::CompiledGeometryHandle geometry) override;
	void RenderGeometry(Rml::CompiledGeometryHandle handle, Rml::Vector2f translation, Rml::TextureHandle texture) override;

	void EnableScissorRegion(bool enable);
	void SetScissorRegion(Rml::Rectanglei region) override;

	Rml::TextureHandle LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) override;
	Rml::TextureHandle GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions) override;
	void ReleaseTexture(Rml::TextureHandle texture_handle) override;
};

class SystemInterface_Allegro5 : public Rml::SystemInterface {
public:
	double GetElapsedTime();
	void SetMouseCursor(const Rml::String& cursor_name) override;
	void SetClipboardText(const Rml::String& text) override;
	void GetClipboardText(Rml::String& text) override;
};

// Applies input on the context based on the given Allegro event.
// @return True if the event is still propagating, false if it was handled by the context.
bool RmlAllegroInputEventHandler(Rml::Context* context, ALLEGRO_EVENT& ev);
