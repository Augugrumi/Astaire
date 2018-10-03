#include "addressresolver.h"

namespace address {

AddressResolver::AddressResolver(const Address& r_a) : roulette_addr(r_a) {

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    LOG(ltrace, "Curl initialized");
    if (curl == nullptr) {
        LOG(lfatal, "Impossible to properly init curl");
        exit(EXIT_FAILURE);
    }
}

AddressResolver::AddressResolver(const std::string& address, uint16_t port)
    : AddressResolver(*new Address(address, port)) {
}

AddressResolver::~AddressResolver() {
    if (curl != nullptr) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

size_t AddressResolver::curl_callback(void* ptr, size_t size, size_t nmemb, std::string* data) {
    LOG(ltrace, "In query lambda");
    data->append(reinterpret_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

const Address AddressResolver::get_next(uint32_t p_id, uint32_t si) const {

    CURLcode req_code_res;
    std::string req_data_res;
    std::string req_addr = url_builder(roulette_addr.get_URL(), p_id, si);

    LOG(ltrace, "Setting curl options");
    LOG(ltrace, "Request to: " + req_addr);
    req_code_res = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error while setting the request to GET");
    }
    req_code_res = curl_easy_setopt(curl, CURLOPT_URL, req_addr.c_str());
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error while setting the CURLOPT_URL flag");
    }
    req_code_res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error while setting the CURLOPT_FOLLOWLOCATION flag");
    }
    req_code_res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, AddressResolver::curl_callback);
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error while setting the CURLOPT_WRITEFUNCTION flag");
    }
    req_code_res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &req_data_res);
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error while setting the CURLOPT_WRITEDATA flag");
    }
    req_code_res = curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error while setting the CURLOPT_USERAGENT flag");
    }

    req_code_res = curl_easy_perform(curl);
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error: failure to retrieve data from roulette");
        return Address("", 0);
    } else {
        LOG(ldebug, "Request performed successfully");

        utils::JsonUtils::JsonWrapper json(req_data_res);
        utils::JsonUtils::JsonWrapper content(json.getObj(utils::addressFields::CONTENT).toStyledString());

        LOG(ltrace, "Response: \n" + req_data_res);
        if (json.getField(utils::addressFields::RESULT) != utils::jsonCode::OK) {
            LOG(lwarn, "Error while retrieving the next index");
            return Address("", 0);
        }
        try {

            return Address(content.getField(utils::addressFields::ADDRESS),
                           std::stol(content.getField(utils::addressFields::PORT)));
        } catch (const std::invalid_argument& e) {
            LOG(lwarn, "The received address from roulette is not valid");
            e.what();
            return Address("", 0);
        }
    }
}

const std::string AddressResolver::url_builder(const std::string& original_url, uint32_t p_id, uint32_t si) const {
    std::string req_addr = original_url;

    req_addr.append("route/")
            .append(std::to_string(p_id))
            .append(Address::path_separator)
            .append(std::to_string(si));

    return req_addr;
}
} // namespace address
