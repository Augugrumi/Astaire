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

    std::function<std::vector<Address>(uint32_t , std::vector<Address>)> f =
            [this](uint32_t k, std::vector<Address> v) {
                CURLcode req_code_res;
                std::string req_data_res;
                std::string req_addr;
                req_addr.append("route/")
                        .append(std::to_string(k));

                setup_curl_for_request(req_addr, req_data_res);

                req_code_res = curl_easy_perform(curl);
                if (req_code_res != CURLE_OK) {
                    LOG(lwarn, "Error: failure to retrieve data from roulette");
                    return std::vector<Address>();
                } else {
                    LOG(ldebug, "Request performed successfully");

                    utils::JsonUtils::JsonWrapper json(req_data_res);
                    //utils::JsonUtils::JsonWrapper content(json.getObj(utils::addressFields::CONTENT).asString());
                    Json::Value content = json.getObj(utils::addressFields::CONTENT);
                    LOG(ltrace, "Response: \n" + req_data_res);
                    if (json.getField(utils::addressFields::RESULT) != utils::jsonCode::OK) {

                        LOG(lwarn, "Error while retrieving the next index");
                        return std::vector<Address>();
                    }
                    try {
                        std::vector<Address> v;
                        for (Json::Value::ArrayIndex i = 0; i != content.size(); i++)
                            if (content[i].isMember("url"))
                                v.emplace_back(Address(
                                        content[i]["url"].asString(),
                                        content[i]["port"].asUInt()));
                        return v;
                    } catch (const std::invalid_argument& e) {
                        LOG(lwarn, "The received address from roulette is not valid");
                        e.what();
                        return std::vector<Address>();
                    }
                }
    };
    // update each 5 minutes?
    updater = new refresher::map_refresher<uint32_t, std::vector<Address>>(
            &local_resolver, f, 30000);
    updater->start();
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

const Address AddressResolver::get_next(uint32_t p_id, uint32_t si,
        utils::sfc_header::SFCFixedLengthHeader header) const {
    auto it = local_resolver.find(p_id);
    if (it != local_resolver.end())
        if (it->second.size()>si)
            return it->second.at(si);
    CURLcode req_code_res;
    std::string req_data_res;
    std::string req_addr = url_builder(roulette_addr.get_URL(), p_id, si);

    setup_curl_for_request(req_addr, req_data_res);

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
            // TODO implement on roulette case of index requested is last + 1 to differentiate errors and chain end
            return get_chain_endpoint(header);
            //LOG(lwarn, "Error while retrieving the next index");
            //return Address("", 0);
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

const Address AddressResolver::get_chain_endpoint(
        utils::sfc_header::SFCFixedLengthHeader header) const {
    CURLcode req_code_res;
    std::string req_data_res;
    std::string endpoint = header.get_direction_flag()? "egress/" : "ingress/";
    std::string req_addr = req_addr.append("enpoints/")
                                   .append(endpoint)
                                   .append(header.get_source_ip_address())
                                   .append(Address::path_separator)
                                   .append(header.get_destination_ip_address())
                                   .append(Address::path_separator)
                                   .append(std::to_string(header.get_source_port()))
                                   .append(Address::path_separator)
                                   .append(std::to_string(header.get_destination_port()))
                                   .append(Address::path_separator)
                                   .append(std::to_string(header.get_service_path_id()));

    setup_curl_for_request(req_addr, req_data_res);

    req_code_res = curl_easy_perform(curl);
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error: failure to retrieve data from roulette");
        return Address("", 0);
    } else {
        LOG(ldebug, "Request performed successfully");

        utils::JsonUtils::JsonWrapper json(req_data_res);
        utils::JsonUtils::JsonWrapper content(json.getObj(utils::addressFields::CONTENT).asString());

        LOG(ltrace, "Response: \n" + req_data_res);
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

void AddressResolver::setup_curl_for_request(std::string req_addr,
                                             std::string req_data_res) const {
    LOG(ltrace, "Setting curl options");
    LOG(ltrace, "Request to: " + req_addr);
    CURLcode req_code_res = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
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
}


} // namespace address
