#include "RmlUi_Backend_Allegro5.h"
#include <RmlUi/Core.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_opengl.h>

void RenderInterface_Allegro5::RenderGeometry(Rml::Vertex* vertices,
	int num_vertices,
	int* indices,
	int num_indices,
	Rml::TextureHandle texture,
	const Rml::Vector2f& translation
) {
	ALLEGRO_VERTEX_BUFFER* vbuff;
	ALLEGRO_INDEX_BUFFER* ibuff;
	int ii;
	ALLEGRO_COLOR color;
	ALLEGRO_VERTEX* vtx;
	int flags = ALLEGRO_PRIM_BUFFER_READWRITE;

	vbuff = (ALLEGRO_VERTEX_BUFFER*)al_create_vertex_buffer(NULL, NULL, num_vertices, ALLEGRO_PRIM_BUFFER_READWRITE);
	if (vbuff == NULL) {
		vbuff = (ALLEGRO_VERTEX_BUFFER*)al_create_vertex_buffer(NULL, NULL, num_vertices, 0);
		flags = 0;
	}

	ibuff = al_create_index_buffer(sizeof(short), NULL, num_indices, flags);

	if (vbuff) {
		vtx = (ALLEGRO_VERTEX*)al_lock_vertex_buffer(vbuff, 0, num_vertices, ALLEGRO_LOCK_WRITEONLY);
		
		float tw = 1;
		float th = 1;
		if(texture != 0) {
			tw = al_get_bitmap_width(textures[texture]);
			th = al_get_bitmap_height(textures[texture]);
		}
		for (ii = 0; ii < num_vertices; ii++) {
			float x, y;
			x = vertices[ii].position.x + translation.x;
			y = vertices[ii].position.y + translation.y;

			Rml::Colour<unsigned char, 255> rc = vertices[ii].colour;
			color = al_map_rgba(rc.red, rc.green, rc.blue, rc.alpha);

			vtx[ii].x = x;
			vtx[ii].y = y;
			vtx[ii].z = 0;
			vtx[ii].u = vertices[ii].tex_coord.x*tw;
			vtx[ii].v = vertices[ii].tex_coord.y*th;
			vtx[ii].color = color;
		}

		al_unlock_vertex_buffer(vbuff);
	}

	if (ibuff) {
		int ii;
		int t = (int)al_get_time();
		short* indicesb = (short*)al_lock_index_buffer(ibuff, 0, num_indices, ALLEGRO_LOCK_WRITEONLY);

		for (ii = 0; ii < num_indices; ii++) {
			indicesb[ii] = indices[ii];
		}
		al_unlock_index_buffer(ibuff);
	}

	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

	if(vbuff && ibuff) {
		al_draw_indexed_buffer(vbuff, textures[texture], ibuff, 0, num_indices, ALLEGRO_PRIM_TRIANGLE_LIST);
	}

	al_destroy_vertex_buffer(vbuff);
	al_destroy_index_buffer(ibuff);
}

void RenderInterface_Allegro5::EnableScissorRegion(bool enable) {
	if (enable)
		al_set_clipping_rectangle(clipx, clipy, clipw, cliph);
	else
		al_reset_clipping_rectangle();

	scissor_region_enabled = enable;
}

void RenderInterface_Allegro5::SetScissorRegion(int x, int y, int width, int height) {
	clipx = x;
	clipy = y;
	clipw = width;
	cliph = height;

	if (scissor_region_enabled)
		al_set_clipping_rectangle(clipx, clipy, clipw, cliph);
}

bool RenderInterface_Allegro5::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source) {
	ALLEGRO_BITMAP* t = al_load_bitmap(source.c_str());
	texture_dimensions.x = al_get_bitmap_width(t);
	texture_dimensions.y = al_get_bitmap_height(t);
	texture_handle = (Rml::TextureHandle)t;
	texture_handle = (Rml::TextureHandle)al_get_opengl_texture(t);
	textures[texture_handle] = t;
	return true;
}

bool RenderInterface_Allegro5::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) {
	al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ABGR_8888);
	ALLEGRO_BITMAP* t = al_create_bitmap(source_dimensions.x, source_dimensions.y);
	ALLEGRO_LOCKED_REGION* region = al_lock_bitmap(t, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_WRITEONLY);
	for(int y=0; y < source_dimensions.y; y++ ) {
		std::memcpy((char*)region->data+y*region->pitch, (char*)source+y*source_dimensions.x*4, source_dimensions.x*4*sizeof(char));
	}
	al_unlock_bitmap(t);
	texture_handle = (Rml::TextureHandle)al_get_opengl_texture(t);
	textures[texture_handle] = t;
	return true;
}

void RenderInterface_Allegro5::ReleaseTexture(Rml::TextureHandle texture_handle) {
	al_destroy_bitmap(textures[texture_handle]);
	textures[texture_handle] = NULL;
}

double SystemInterface_Allegro5::GetElapsedTime() {
	return al_get_time();
}

void SystemInterface_Allegro5::SetMouseCursor(const Rml::String& cursor_name) {
	if (cursor_name.empty() || cursor_name == "arrow")
		al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
	else if (cursor_name == "move")
		al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
	else if (cursor_name == "pointer")
		al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW);
	else if (cursor_name == "resize")
		al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NE);
	else if (cursor_name == "cross")
		al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_PRECISION);
	else if (cursor_name == "text")
		al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT);
	else if (cursor_name == "unavailable")
		al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_UNAVAILABLE);
	else if (Rml::StringUtilities::StartsWith(cursor_name, "rmlui-scroll"))
		al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
}

void SystemInterface_Allegro5::SetClipboardText(const Rml::String& text_utf8) {
	al_set_clipboard_text(al_get_current_display(), text_utf8.c_str());
}

void SystemInterface_Allegro5::GetClipboardText(Rml::String& text) {
	char* raw_text = al_get_clipboard_text(al_get_current_display());
	text = Rml::String(raw_text);
	al_free(raw_text);
}

Rml::Input::KeyIdentifier ConvertKey(int al_key) {
	switch (al_key) {
		case ALLEGRO_KEY_UNKNOWN:      return Rml::Input::KI_UNKNOWN;
		case ALLEGRO_KEY_ESCAPE:       return Rml::Input::KI_ESCAPE;
		case ALLEGRO_KEY_SPACE:        return Rml::Input::KI_SPACE;
		case ALLEGRO_KEY_0:            return Rml::Input::KI_0;
		case ALLEGRO_KEY_1:            return Rml::Input::KI_1;
		case ALLEGRO_KEY_2:            return Rml::Input::KI_2;
		case ALLEGRO_KEY_3:            return Rml::Input::KI_3;
		case ALLEGRO_KEY_4:            return Rml::Input::KI_4;
		case ALLEGRO_KEY_5:            return Rml::Input::KI_5;
		case ALLEGRO_KEY_6:            return Rml::Input::KI_6;
		case ALLEGRO_KEY_7:            return Rml::Input::KI_7;
		case ALLEGRO_KEY_8:            return Rml::Input::KI_8;
		case ALLEGRO_KEY_9:            return Rml::Input::KI_9;
		case ALLEGRO_KEY_A:            return Rml::Input::KI_A;
		case ALLEGRO_KEY_B:            return Rml::Input::KI_B;
		case ALLEGRO_KEY_C:            return Rml::Input::KI_C;
		case ALLEGRO_KEY_D:            return Rml::Input::KI_D;
		case ALLEGRO_KEY_E:            return Rml::Input::KI_E;
		case ALLEGRO_KEY_F:            return Rml::Input::KI_F;
		case ALLEGRO_KEY_G:            return Rml::Input::KI_G;
		case ALLEGRO_KEY_H:            return Rml::Input::KI_H;
		case ALLEGRO_KEY_I:            return Rml::Input::KI_I;
		case ALLEGRO_KEY_J:            return Rml::Input::KI_J;
		case ALLEGRO_KEY_K:            return Rml::Input::KI_K;
		case ALLEGRO_KEY_L:            return Rml::Input::KI_L;
		case ALLEGRO_KEY_M:            return Rml::Input::KI_M;
		case ALLEGRO_KEY_N:            return Rml::Input::KI_N;
		case ALLEGRO_KEY_O:            return Rml::Input::KI_O;
		case ALLEGRO_KEY_P:            return Rml::Input::KI_P;
		case ALLEGRO_KEY_Q:            return Rml::Input::KI_Q;
		case ALLEGRO_KEY_R:            return Rml::Input::KI_R;
		case ALLEGRO_KEY_S:            return Rml::Input::KI_S;
		case ALLEGRO_KEY_T:            return Rml::Input::KI_T;
		case ALLEGRO_KEY_U:            return Rml::Input::KI_U;
		case ALLEGRO_KEY_V:            return Rml::Input::KI_V;
		case ALLEGRO_KEY_W:            return Rml::Input::KI_W;
		case ALLEGRO_KEY_X:            return Rml::Input::KI_X;
		case ALLEGRO_KEY_Y:            return Rml::Input::KI_Y;
		case ALLEGRO_KEY_Z:            return Rml::Input::KI_Z;
		case ALLEGRO_KEY_SEMICOLON:    return Rml::Input::KI_OEM_1;
		//case ALLEGRO_KEY_PLUS:         return Rml::Input::KI_OEM_PLUS;
		case ALLEGRO_KEY_COMMA:        return Rml::Input::KI_OEM_COMMA;
		case ALLEGRO_KEY_MINUS:        return Rml::Input::KI_OEM_MINUS;
		case ALLEGRO_KEY_FULLSTOP:     return Rml::Input::KI_OEM_PERIOD; //?
		case ALLEGRO_KEY_SLASH:        return Rml::Input::KI_OEM_2;
		case ALLEGRO_KEY_BACKQUOTE:    return Rml::Input::KI_OEM_3;
		case ALLEGRO_KEY_OPENBRACE:    return Rml::Input::KI_OEM_4; //?
		case ALLEGRO_KEY_BACKSLASH:    return Rml::Input::KI_OEM_5;
		case ALLEGRO_KEY_CLOSEBRACE:   return Rml::Input::KI_OEM_6; //?
		//case ALLEGRO_KEY_QUOTEDBL:     return Rml::Input::KI_OEM_7;
		case ALLEGRO_KEY_PAD_0:        return Rml::Input::KI_NUMPAD0;
		case ALLEGRO_KEY_PAD_1:        return Rml::Input::KI_NUMPAD1;
		case ALLEGRO_KEY_PAD_2:        return Rml::Input::KI_NUMPAD2;
		case ALLEGRO_KEY_PAD_3:        return Rml::Input::KI_NUMPAD3;
		case ALLEGRO_KEY_PAD_4:        return Rml::Input::KI_NUMPAD4;
		case ALLEGRO_KEY_PAD_5:        return Rml::Input::KI_NUMPAD5;
		case ALLEGRO_KEY_PAD_6:        return Rml::Input::KI_NUMPAD6;
		case ALLEGRO_KEY_PAD_7:        return Rml::Input::KI_NUMPAD7;
		case ALLEGRO_KEY_PAD_8:        return Rml::Input::KI_NUMPAD8;
		case ALLEGRO_KEY_PAD_9:        return Rml::Input::KI_NUMPAD9;
		case ALLEGRO_KEY_PAD_ENTER:    return Rml::Input::KI_NUMPADENTER;
		case ALLEGRO_KEY_PAD_ASTERISK: return Rml::Input::KI_MULTIPLY;
		case ALLEGRO_KEY_PAD_PLUS:     return Rml::Input::KI_ADD;
		case ALLEGRO_KEY_PAD_MINUS:    return Rml::Input::KI_SUBTRACT;
		case ALLEGRO_KEY_PAD_DELETE:   return Rml::Input::KI_DECIMAL; //?
		case ALLEGRO_KEY_PAD_SLASH:    return Rml::Input::KI_DIVIDE;
		case ALLEGRO_KEY_PAD_EQUALS:   return Rml::Input::KI_OEM_NEC_EQUAL;
		case ALLEGRO_KEY_BACKSPACE:    return Rml::Input::KI_BACK;
		case ALLEGRO_KEY_TAB:          return Rml::Input::KI_TAB;
		//case ALLEGRO_KEY_CLEAR:        return Rml::Input::KI_CLEAR;
		case ALLEGRO_KEY_ENTER:        return Rml::Input::KI_RETURN;
		case ALLEGRO_KEY_PAUSE:        return Rml::Input::KI_PAUSE;
		case ALLEGRO_KEY_CAPSLOCK:     return Rml::Input::KI_CAPITAL;
		case ALLEGRO_KEY_PGUP:         return Rml::Input::KI_PRIOR;
		case ALLEGRO_KEY_PGDN:         return Rml::Input::KI_NEXT;
		case ALLEGRO_KEY_END:          return Rml::Input::KI_END;
		case ALLEGRO_KEY_HOME:         return Rml::Input::KI_HOME;
		case ALLEGRO_KEY_LEFT:         return Rml::Input::KI_LEFT;
		case ALLEGRO_KEY_UP:           return Rml::Input::KI_UP;
		case ALLEGRO_KEY_RIGHT:        return Rml::Input::KI_RIGHT;
		case ALLEGRO_KEY_DOWN:         return Rml::Input::KI_DOWN;
		case ALLEGRO_KEY_INSERT:       return Rml::Input::KI_INSERT;
		case ALLEGRO_KEY_DELETE:       return Rml::Input::KI_DELETE;
		//case ALLEGRO_KEY_HELP:         return Rml::Input::KI_HELP;
		case ALLEGRO_KEY_F1:           return Rml::Input::KI_F1;
		case ALLEGRO_KEY_F2:           return Rml::Input::KI_F2;
		case ALLEGRO_KEY_F3:           return Rml::Input::KI_F3;
		case ALLEGRO_KEY_F4:           return Rml::Input::KI_F4;
		case ALLEGRO_KEY_F5:           return Rml::Input::KI_F5;
		case ALLEGRO_KEY_F6:           return Rml::Input::KI_F6;
		case ALLEGRO_KEY_F7:           return Rml::Input::KI_F7;
		case ALLEGRO_KEY_F8:           return Rml::Input::KI_F8;
		case ALLEGRO_KEY_F9:           return Rml::Input::KI_F9;
		case ALLEGRO_KEY_F10:          return Rml::Input::KI_F10;
		case ALLEGRO_KEY_F11:          return Rml::Input::KI_F11;
		case ALLEGRO_KEY_F12:          return Rml::Input::KI_F12;
		/*
		case ALLEGRO_KEY_F13:          return Rml::Input::KI_F13;
		case ALLEGRO_KEY_F14:          return Rml::Input::KI_F14;
		case ALLEGRO_KEY_F15:          return Rml::Input::KI_F15;
		*/
		case ALLEGRO_KEY_NUMLOCK:      return Rml::Input::KI_NUMLOCK;
		case ALLEGRO_KEY_SCROLLLOCK:   return Rml::Input::KI_SCROLL;
		case ALLEGRO_KEY_LSHIFT:       return Rml::Input::KI_LSHIFT;
		case ALLEGRO_KEY_RSHIFT:       return Rml::Input::KI_RSHIFT;
		case ALLEGRO_KEY_LCTRL:        return Rml::Input::KI_LCONTROL;
		case ALLEGRO_KEY_RCTRL:        return Rml::Input::KI_RCONTROL;
		case ALLEGRO_KEY_ALT:          return Rml::Input::KI_LMENU;
		case ALLEGRO_KEY_ALTGR:        return Rml::Input::KI_RMENU;
		/*
		case ALLEGRO_KEY_LGUI:         return Rml::Input::KI_LMETA;
		case ALLEGRO_KEY_RGUI:         return Rml::Input::KI_RMETA;
		*/
		case ALLEGRO_KEY_LWIN:          return Rml::Input::KI_LWIN;
		case ALLEGRO_KEY_RWIN:         return Rml::Input::KI_RWIN;
		default: break;
	}

	return Rml::Input::KI_UNKNOWN;
}

int ConvertMouseButton(int button) {
	switch (button) {
		case 1: return 0; //Left
		case 2: return 1; //Right
		case 3: return 2; //Middle
		default: return 3;
	}
}

int GetKeyModifierState() {
	ALLEGRO_KEYBOARD_STATE state;
	al_get_keyboard_state(&state);

	int retval = 0;

	if (al_key_down(&state, ALLEGRO_KEY_LCTRL) || al_key_down(&state, ALLEGRO_KEY_RCTRL))
		retval |= Rml::Input::KM_CTRL;

	if (al_key_down(&state, ALLEGRO_KEY_LSHIFT) || al_key_down(&state, ALLEGRO_KEY_RSHIFT))
		retval |= Rml::Input::KM_SHIFT;

	if (al_key_down(&state, ALLEGRO_KEY_ALT))
		retval |= Rml::Input::KM_ALT;
/*
	if (state & KMOD_NUM)
		retval |= Rml::Input::KM_NUMLOCK;

	if (state & KMOD_CAPS)
		retval |= Rml::Input::KM_CAPSLOCK;
*/
	return retval;
}

bool RmlAllegroInputEventHandler(Rml::Context* context, ALLEGRO_EVENT& ev) {
	bool result = true;

	switch (ev.type) {
		case ALLEGRO_EVENT_MOUSE_AXES: {
			if(ev.mouse.dx || ev.mouse.dy)
				result = context->ProcessMouseMove(ev.mouse.dx, ev.mouse.dy, GetKeyModifierState());
			if(ev.mouse.dz)
				result = context->ProcessMouseWheel(float(-ev.mouse.dz), GetKeyModifierState());
			}
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			result = context->ProcessMouseButtonDown(ConvertMouseButton(ev.mouse.button), GetKeyModifierState());
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			result = context->ProcessMouseButtonUp(ConvertMouseButton(ev.mouse.button), GetKeyModifierState());
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			result = context->ProcessKeyDown(ConvertKey(ev.keyboard.keycode), GetKeyModifierState());
			if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
				result &= context->ProcessTextInput('\n');
			break;
		case ALLEGRO_EVENT_KEY_UP:
			result = context->ProcessKeyUp(ConvertKey(ev.keyboard.keycode), GetKeyModifierState());
			break;
		case ALLEGRO_EVENT_KEY_CHAR: {
			ALLEGRO_USTR *str = al_ustr_new("");
			al_ustr_append_chr(str, ev.keyboard.unichar);
			result = context->ProcessTextInput(Rml::String(al_cstr(str)));
			al_ustr_free(str);
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_RESIZE: {
			Rml::Vector2i dimensions(ev.display.width, ev.display.height);
			context->SetDimensions(dimensions);
			}
			break;
		case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
			context->ProcessMouseLeave();
			break;
		default: break;
	}

	return result;
}
