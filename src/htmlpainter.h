#include <RmlUi/Core.h>
#include <RmlUi/Core/RenderInterface.h>
#include <RmlUi/Core/Types.h>

#include <map>
#include <allegro5/allegro.h>

typedef std::map<Rml::TextureHandle, ALLEGRO_BITMAP*> Textures;

class AlRenderInterface : public Rml::RenderInterface {
private:
	bool scissor_region_enabled = false;
	int clipx, clipy, clipw, cliph;
public:
	Textures textures;

	// Called by RmlUi when it wants to render geometry that the application does not wish to optimise.
	virtual void RenderGeometry(Rml::Vertex* vertices,
		int num_vertices,
		int* indices,
		int num_indices,
		Rml::TextureHandle texture,
		const Rml::Vector2f& translation);

	// Called by RmlUi when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(bool enable);

	// Called by RmlUi when it wants to change the scissor region.
	virtual void SetScissorRegion(int x, int y, int width, int height);

	bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source) override;
	bool GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) override;
	void ReleaseTexture(Rml::TextureHandle texture_handle) override;
};

class AlSystemInterface : public Rml::SystemInterface {
public:
	virtual double GetElapsedTime();
	void SetMouseCursor(const Rml::String& cursor_name) override;
};

// Applies input on the context based on the given Allegro event.
// @return True if the event is still propagating, false if it was handled by the context.
bool InputEventHandler(Rml::Context* context, ALLEGRO_EVENT& ev);

// Converts the Allegro key to RmlUi key.
Rml::Input::KeyIdentifier ConvertKey(int al_key);

// Converts the Allegro mouse button to RmlUi mouse button.
int ConvertMouseButton(int sdl_mouse_button);

// Returns the active RmlUi key modifier state.
int GetKeyModifierState();
