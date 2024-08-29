#include "htmlpainter.h"
#include <RmlUi/Core.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_opengl.h>

void AlRenderInterface::RenderGeometry(Rml::Vertex* vertices,
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

void AlRenderInterface::EnableScissorRegion(bool enable) {
	//TODO: See SDL backend for example
//	printf("EnableScissorRegion\n");
}

void AlRenderInterface::SetScissorRegion(int x, int y, int width, int height) {
//	printf("SetScissorRegion\n");
}

double AlSystemInterface::GetElapsedTime() {
	return 0;
}

bool AlRenderInterface::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source) {
	ALLEGRO_BITMAP* t = al_load_bitmap(source.c_str());
	texture_dimensions.x = al_get_bitmap_width(t);
	texture_dimensions.y = al_get_bitmap_height(t);
	texture_handle = (Rml::TextureHandle)t;
	texture_handle = (Rml::TextureHandle)al_get_opengl_texture(t);
	textures[texture_handle] = t;
	return true;
}

bool AlRenderInterface::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) {
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

void AlRenderInterface::ReleaseTexture(Rml::TextureHandle texture_handle) {
	al_destroy_bitmap(textures[texture_handle]);
	textures[texture_handle] = NULL;
}
