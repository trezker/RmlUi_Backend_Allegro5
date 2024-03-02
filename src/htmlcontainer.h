#include <litehtml/litehtml.h>
#include <map>
#include <string>

class htmlcontainer : public litehtml::document_container {
public:
	virtual void get_language(litehtml::string& language, litehtml::string& culture) const;
	virtual void get_media_features(litehtml::media_features& media) const;
	virtual void get_client_rect(litehtml::position& client) const;
	virtual void del_clip();
	virtual void set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius);
	virtual void import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& baseurl);
	virtual void transform_text(litehtml::string& text, litehtml::text_transform tt);
	virtual void set_cursor(const char* cursor);
	virtual void on_anchor_click(const char* url, const litehtml::element::ptr& el);
	virtual void link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el);
	virtual void set_base_url(const char* base_url);
	virtual void set_caption(const char* caption);
	virtual void draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root);
	virtual void draw_background(litehtml::uint_ptr hdc, const std::vector<litehtml::background_paint>& bg);
	virtual void get_image_size(const char* src, const char* baseurl, litehtml::size& sz);
	virtual void load_image(const char* src, const char* baseurl, bool redraw_on_ready);
	virtual void draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker);
	virtual const char* get_default_font_name() const;
	virtual int get_default_font_size() const;
	virtual int pt_to_px(int pt) const;
	virtual void draw_text(litehtml::uint_ptr hdc, const char* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos);
	virtual int text_width(const char* text, litehtml::uint_ptr hFont);
	virtual void delete_font(litehtml::uint_ptr hFont);
	virtual litehtml::uint_ptr create_font(const char* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm);
	virtual litehtml::element::ptr	create_element( const char* tag_name,
														const litehtml::string_map& attributes,
														const std::shared_ptr<litehtml::document>& doc);
};
