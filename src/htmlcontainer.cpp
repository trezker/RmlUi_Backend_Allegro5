#include "htmlcontainer.h"

void htmlcontainer::get_language(litehtml::string& language, litehtml::string& culture) const {

}

void htmlcontainer::get_media_features(litehtml::media_features& media) const {

}

void htmlcontainer::get_client_rect(litehtml::position& client) const {

}

void htmlcontainer::del_clip() {

}

void htmlcontainer::set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius) {

}

void htmlcontainer::import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& baseurl) {

}

void htmlcontainer::transform_text(litehtml::string& text, litehtml::text_transform tt) {

}

void htmlcontainer::set_cursor(const char* cursor) {

}

void htmlcontainer::on_anchor_click(const char* url, const litehtml::element::ptr& el) {

}

void htmlcontainer::link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el) {

}

void htmlcontainer::set_base_url(const char* base_url) {

}

void htmlcontainer::set_caption(const char* caption) {

}

void htmlcontainer::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) {

}

void htmlcontainer::draw_background(litehtml::uint_ptr hdc, const std::vector<litehtml::background_paint>& bg) {

}

void htmlcontainer::get_image_size(const char* src, const char* baseurl, litehtml::size& sz) {

}

void htmlcontainer::load_image(const char* src, const char* baseurl, bool redraw_on_ready) {

}

void htmlcontainer::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker) {

}

const char* htmlcontainer::get_default_font_name() const {

}

int htmlcontainer::get_default_font_size() const {

}

int htmlcontainer::pt_to_px(int pt) const {

}

void htmlcontainer::draw_text(litehtml::uint_ptr hdc, const char* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) {

}

int htmlcontainer::text_width(const char* text, litehtml::uint_ptr hFont) {

}

void htmlcontainer::delete_font(litehtml::uint_ptr hFont) {

}

litehtml::uint_ptr htmlcontainer::create_font(const char* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm) {

}

litehtml::element::ptr	htmlcontainer::create_element(
	const char* tag_name,
	const litehtml::string_map& attributes,
	const std::shared_ptr<litehtml::document>& doc)  {

}

