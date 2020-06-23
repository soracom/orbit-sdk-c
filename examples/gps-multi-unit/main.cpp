#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <string>

#include <emscripten.h>
#include "soracom/orbit.h"

#include "nlohmann/json.hpp"
using nlohmann::json;

int32_t uplink_body();

extern "C" {

EMSCRIPTEN_KEEPALIVE
int32_t uplink() {
    soracom_log("hello, orbit!\n");

    return uplink_body();
}

}

std::string get_tag(const std::string& tag_name) {
    const char* tag_value = NULL;
    size_t tag_value_len = 0;
    int32_t err = soracom_get_tag_value(tag_name.c_str(), tag_name.size(), &tag_value, &tag_value_len);
    if (err < 0) {
        return "";
    }

    return std::string(tag_value);
}

std::string get_source_item(const std::string& name) {
    const char* value = NULL;
    size_t value_len = 0;
    int32_t err = soracom_get_source_value(name.c_str(), name.size(), &value, &value_len);
    if (err < 0) {
        return "";
    }

    return std::string(value);
}

double deg2rad(double deg) {
    return deg * (M_PI / 180);
}

double calc_distance_in_km(double lat1, double lon1, double lat2, double lon2) {
    double r = 6371; // Radius of the earth in km
	double d_lat = deg2rad(lat2 - lat1);
	double d_lon = deg2rad(lon2 - lon1);
	double a = sin(d_lat/2)*sin(d_lat/2) + cos(deg2rad(lat1))*cos(deg2rad(lat2))*sin(d_lon/2)*sin(d_lon/2);

	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	return  r * c;
}

double parse_double(const std::string& s, double default_value) {
    char *p_end;
    double d = strtod(s.c_str(), &p_end);
    if (*p_end != NULL) {
        return default_value;
    }
    return d;
}

int32_t uplink_body() {
    const char* buf = NULL;
    size_t siz = 0;
    int32_t err = soracom_get_input_buffer_as_string(&buf, &siz);
    if (err < 0) {
        return err;
    }

    soracom_log("received data: %s\n", buf);

    json j = json::parse(buf);
    soracom_release_input_buffer(buf);

    std::string imsi = get_source_item("resourceId");

    soracom_log("imsi = %s\n", imsi.c_str());
    std::string name = get_tag("name");
    std::string center_lat_str = get_tag("center_lat");
    std::string center_lon_str = get_tag("center_lon");
    std::string radius_str = get_tag("radius");

    soracom_log("name = %s\n", name.c_str());
    soracom_log("center_lat_str = %s\n", center_lat_str.c_str());
    soracom_log("center_lon_str = %s\n", center_lon_str.c_str());
    soracom_log("radius = %s\n", radius_str.c_str());

    double center_lat = parse_double(center_lat_str, -999);
    double center_lon = parse_double(center_lon_str, -999);
    double radius = parse_double(radius_str, -1);

    j["name"] = name;
    j["imsi"] = imsi;
    j["inside_area"] = 0;

    double lat = j.value("lat", -999.0);
    double lon = j.value("lon", -999.0);
    if ((center_lat >= -90) && (center_lon >= -180) && (lat >= -90) && (lon >= -180) && (radius > 0)) {
        soracom_log("center: %f, %f\npos: %f, %f\n", center_lat, center_lon, lat, lon);
        double distance = calc_distance_in_km(center_lat, center_lon, lat, lon);
        j["distance_in_km"] = distance;
        if (distance <= radius) {
            j["inside_area"] = 1;
        }
    }

    const char* userdata = NULL;
    size_t ud_siz = 0;
    err = soracom_get_userdata_as_string(&userdata, &ud_siz);
    if (err < 0) {
        return err;
    }
    soracom_log("userdata: %s\n", userdata);
    j["userdata"] = std::string(userdata);
    soracom_release_userdata(userdata);

    std::string output = j.dump();
    soracom_set_json_output(output.c_str(), output.size());

    return 0;
}
