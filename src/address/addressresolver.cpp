#include "addressresolver.h"

namespace address {

AddressResolver::AddressResolver(const Address& r_a) : roulette_addr(r_a) {
    //curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl == nullptr) {
        LOG(lfatal, "Impossible to properly init curl");
        exit(EXIT_FAILURE);
    }
}

AddressResolver::AddressResolver(const std::string& address, int port)
    : AddressResolver(*new Address(address, port)) {
}

AddressResolver::~AddressResolver() {
    if (curl != nullptr) {
        curl_easy_cleanup(curl);
    }
    //curl_global_cleanup();
}

const Address AddressResolver::get_next(uint32_t p_id, uint32_t si) const {

    CURLcode req_code_res;
    std::string req_data_res;
    std::string req_addr = url_builder(roulette_addr.get_URL(), p_id, si);

    //auto curl_opt_url = CURLOPT_URL;
    //auto curl_opt_follow_location = CURLOPT_FOLLOWLOCATION;

    auto cb = [] (void* ptr, size_t size, size_t nmemb, std::string* data) -> size_t {
        data->append(reinterpret_cast<char*>(ptr), size * nmemb);
        return size * nmemb;
    };

    curl_easy_setopt(curl, CURLOPT_URL, roulette_addr.get_URL().c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &req_data_res);

    req_code_res = curl_easy_perform(curl);
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error while retrieving address from roulette");
        return Address(nullptr, 0);
    } else {
        LOG(ldebug, "Request perform successfully");

        utils::JsonUtils::JsonWrapper json(req_data_res);

        return Address(json.getField(utils::addressFields::ADDRESS),
                       std::stol(json.getField(utils::addressFields::PORT)));
    }
}

const std::string AddressResolver::url_builder(const std::string& original_url, uint32_t p_id, uint32_t si) const {
    std::string req_addr = original_url;

    req_addr.append(Address::path_separator)
            .append(std::to_string(p_id))
            .append(Address::path_separator)
            .append(std::to_string(si))
            .append(Address::path_separator);

    return req_addr;
}
} // namespace address
