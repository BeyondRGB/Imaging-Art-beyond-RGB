//
// Defines a class to manage communication between a process and the front end
//

#include <cppcodec/base64_rfc4648.hpp>
#include <cpptrace/cpptrace.hpp>
#include <server/communication_obj.hpp>

CommunicationObj::CommunicationObj(server *s, websocketpp::connection_hdl hd1,
                                   message_ptr msg) {
    server_m = s;
    connectionHandle_m = hd1;
    opcode_m = msg->get_opcode();
}

CommunicationObj::CommunicationObj(const CommunicationObj &other) {
    server_m = other.server_m;
    connectionHandle_m = other.connectionHandle_m;
    opcode_m = other.opcode_m;
}

void CommunicationObj::send_msg(std::string msg) {
    if (server_m == nullptr) {
        std::cerr << "[CommunicationObj] Cannot send message: server is null"
                  << std::endl;
        return;
    }

    try {
        server_m->send(connectionHandle_m, msg, opcode_m);
    } catch (const std::exception& e) {
        std::cerr << "[CommunicationObj] Failed to send websocket message: "
                  << e.what() << std::endl;
    }
}

void CommunicationObj::send_bin(std::vector<uchar> &v) {
    if (server_m == nullptr) {
        std::cerr << "[CommunicationObj] Cannot send binary message: server is null"
                  << std::endl;
        return;
    }

    const void *binToSend = (void *)v.data();
    // Need to find out how to send bin without this send, since it needs a
    // string for what it is sending
    try {
        server_m->send(connectionHandle_m, binToSend, v.size(),
                       websocketpp::frame::opcode::binary);
    } catch (const std::exception& e) {
        std::cerr << "[CommunicationObj] Failed to send websocket binary: "
                  << e.what() << std::endl;
    }
}

void CommunicationObj::set_id(long newID) { id = newID; }

unsigned long CommunicationObj::get_id() const { return id; }

void CommunicationObj::send_json(const jsoncons::json &body) {
    std::string msg;
    body.dump(msg);
    send_msg(msg);
}

void CommunicationObj::send_response(const std::string &responseType,
                                     const jsoncons::json &responseData) {
    jsoncons::json responseBody;
    responseBody.insert_or_assign("RequestID", id);
    responseBody.insert_or_assign("ResponseType", responseType);
    responseBody.insert_or_assign("ResponseData", responseData);
    send_json(responseBody);
}

void CommunicationObj::send_info(std::string msg, std::string sender) {
    jsoncons::json response_data;
    response_data.insert_or_assign("message", msg);
    response_data.insert_or_assign("sender", sender);
    send_response("Info", response_data);
}

void CommunicationObj::send_error(std::string msg, std::string sender,
                                  cpptrace::stacktrace trace, bool critical) {
    jsoncons::json response_data;
    response_data.insert_or_assign("message", msg);
    response_data.insert_or_assign("sender", sender);
    response_data.insert_or_assign("trace", trace.to_string());
    response_data.insert_or_assign("critical", critical);
    send_response("Error", response_data);
}

void CommunicationObj::send_progress(double val, std::string sender) {
    jsoncons::json response_data;
    response_data.insert_or_assign("value", val);
    response_data.insert_or_assign("sender", sender);
    send_response("Progress", response_data);
}

void CommunicationObj::send_base64(btrgb::Image *image,
                                   enum btrgb::image_quality qual) {
    btrgb::binary_ptr_t bin = image->getEncodedPNG(qual);
    this->send_base64(image->getPath(), bin.get(), btrgb::PNG);
}

void CommunicationObj::send_binary(btrgb::Image *image,
                                   enum btrgb::image_quality qual) {
    btrgb::binary_ptr_t bin = image->getEncodedPNG(qual);
    this->send_binary(image->getPath(), bin.get(), btrgb::PNG);
}

void CommunicationObj::send_base64(std::string name,
                                   std::vector<uchar> *direct_binary,
                                   enum btrgb::output_type type) {
    jsoncons::json response_data;
    btrgb::base64_ptr_t b64 = this->createDataURL(type, direct_binary);
    response_data.insert_or_assign("dataURL", *b64);
    response_data.insert_or_assign("name", name);
    send_response("ImageBase64", response_data);
};

void CommunicationObj::send_binary(std::string name,
                                   std::vector<uchar> *direct_binary,
                                   enum btrgb::output_type type) {
    jsoncons::json response_data;
    switch (type) {
    case btrgb::PNG:
        response_data.insert_or_assign("type", "image/png");
        break;
    case btrgb::JPEG:
        response_data.insert_or_assign("type", "image/jpeg");
        break;
    default:
        throw std::logic_error(
            "[CommunicationObj::send_binary] Invalid image type. ");
    }
    response_data.insert_or_assign("name", name);
    send_response("ImageBinary", response_data);

    /* Temporarily add binID on and send. */
    send_bin(*direct_binary);

    // this->binID++;
}

void CommunicationObj::send_reports(jsoncons::json reports,
                                    std::string report_type) {
    jsoncons::json response_data;
    response_data.insert_or_assign("reportType", report_type);
    response_data.insert_or_assign("reports", reports);
    send_response("Report", response_data);
}

void CommunicationObj::send_spectrum(float *data, int size) {
    jsoncons::json response_data;
    response_data["size"] = size;
    response_data["spectrum"] = jsoncons::json::make_array(size);
    for (int i = 0; i < size; i++)
        response_data["spectrum"][i] = data[i];
    send_response("SpectralPicker", response_data);
}

void CommunicationObj::send_spectrum_measured(float *estimated_data,
                                              float *reference_data, int size) {
    jsoncons::json response_data;
    response_data.insert_or_assign("reportType", "SpectralPickerMeasured");
    response_data["size"] = size;
    response_data["estimated_spectrum"] = jsoncons::json::make_array(size);
    for (int i = 0; i < size; i++)
        response_data["estimated_spectrum"][i] = estimated_data[i];

    response_data["referenced_spectrum"] = jsoncons::json::make_array(size);
    for (int i = 0; i < size; i++)
        response_data["referenced_spectrum"][i] = reference_data[i];

    send_response("Report", response_data);
}

void CommunicationObj::send_pipeline_components(jsoncons::json compoents_list) {
    jsoncons::json response_data;
    response_data.insert_or_assign("component_json", compoents_list);
    send_response("PipelineComponents", response_data);
}

btrgb::base64_ptr_t
CommunicationObj::createDataURL(enum btrgb::output_type type,
                                std::vector<uchar> *direct_binary) {

    std::string img_type;
    switch (type) {
    /* Supported */
    case btrgb::PNG:
        img_type = "png";
        break;
    case btrgb::JPEG:
        img_type = "jpeg";
        break;
    /* Unsupported */
    case btrgb::TIFF:
    default:
        throw std::logic_error(
            "[CommunicationObj::createDataURL] Invalid image type. ");
    }

    btrgb::base64_ptr_t result_base64(
        new std::string("data:image/" + img_type + ";base64," +
                        cppcodec::base64_rfc4648::encode(*direct_binary)));

    return result_base64;
}

void CommunicationObj::send_post_calibration_msg(std::string results_path) {
    jsoncons::json response_data;
    response_data.insert_or_assign("path", results_path);
    send_response("CalibrationComplete", response_data);
}
