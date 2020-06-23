#include <cstdint>
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
    j["imsi"] = imsi;

    std::string name = get_tag("name");
    soracom_log("name = %s\n", name.c_str());
    j["name"] = name;

    j["location"]["lat"] = orbit_get_location_lat();
    j["location"]["lon"] = orbit_get_location_lon();

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
