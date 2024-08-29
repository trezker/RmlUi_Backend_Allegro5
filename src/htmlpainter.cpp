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
	//TODO: https://liballeg.org/a5docs/trunk/primitives.html#al_draw_indexed_buffer
	if(texture != 0) {
		al_draw_bitmap(textures[texture], texture*256, 0, 0);
	}
	//printf("Geo %i\n", num_vertices);
	/*
	for(int t=0; t<num_indices/3; ++t) {
		float x1 = vertices[indices[t]].position.x + translation.x;
		float y1 = vertices[indices[t]].position.y + translation.y;
		float x2 = vertices[indices[t+1]].position.x + translation.x;
		float y2 = vertices[indices[t+1]].position.y + translation.y;
		float x3 = vertices[indices[t+2]].position.x + translation.x;
		float y3 = vertices[indices[t+2]].position.y + translation.y;
		Rml::Colour<unsigned char, 255> rc = vertices[indices[t]].colour;
		ALLEGRO_COLOR color = al_map_rgba(rc.red, rc.green, rc.blue, rc.alpha);
		al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, color);
	}
	*/
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

	//Theta += Speed;
	//al_build_transform(&MainTrans, ScreenW / 2, ScreenH / 2, 1, 1, Theta);

//	if (Blend)
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
//	else
//		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);

	//al_use_transform(&MainTrans);

	if(vbuff && ibuff) {
		al_draw_indexed_buffer(vbuff, textures[texture], ibuff, 0, num_indices, ALLEGRO_PRIM_TRIANGLE_LIST);
	}

	//al_use_transform(&Identity);

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
	printf("LoadTexture: %s\n", source.c_str());
	ALLEGRO_BITMAP* t = al_load_bitmap(source.c_str());
	texture_dimensions.x = al_get_bitmap_width(t);
	texture_dimensions.y = al_get_bitmap_height(t);
	texture_handle = (Rml::TextureHandle)t;
	texture_handle = (Rml::TextureHandle)al_get_opengl_texture(t);
	textures[texture_handle] = t;
	return true;
}

bool AlRenderInterface::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) {
	printf("GenerateTexture: %i x %i\n", source_dimensions.x, source_dimensions.y);
	ALLEGRO_BITMAP* t = al_create_bitmap(source_dimensions.x, source_dimensions.y);
	ALLEGRO_LOCKED_REGION* region = al_lock_bitmap(t, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_WRITEONLY);

	for(int y=0; y < source_dimensions.y; y++ ) {
		std::memcpy((char*)region->data+y*region->pitch, (char*)source+y*source_dimensions.x*4, source_dimensions.x*4*sizeof(char));
	}
	al_unlock_bitmap(t);
	texture_handle = (Rml::TextureHandle)al_get_opengl_texture(t);
	textures[texture_handle] = t;
	return true;
}

void AlRenderInterface::ReleaseTexture(Rml::TextureHandle texture_handle) {
	printf("ReleaseTexture\n");
	al_destroy_bitmap(textures[texture_handle]);
	textures[texture_handle] = NULL;
}
