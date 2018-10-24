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
                req_addr.append("routes/")
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
                    LOG(ltrace, "1. Response: \n" + req_data_res);
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

/*AddressResolver::AddressResolver(const std::string& address, uint16_t port)
    : AddressResolver(Address(address, port)) {
}*/

AddressResolver::~AddressResolver() {
    updater->stop();
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
                                        utils::sfc_header::SFCFixedLengthHeader header,
                                        unsigned char* pkt) const {
    auto it = local_resolver.find(p_id);
    if (it != local_resolver.end())
        if (it->second.size()>si)
            return it->second.at(si);
    CURLcode req_code_res;
    std::string req_data_res;
    std::string req_addr = url_builder(roulette_addr.get_URL(), p_id, si);

    //setup_curl_for_request(req_addr, req_data_res);
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
        LOG(ldebug, req_data_res);
        //utils::JsonUtils::JsonWrapper json(req_data_res);

        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        Json::Value response;

        std::string errors;
        bool parsingSuccessful = reader->parse(req_data_res.c_str(),
                      req_data_res.c_str() + req_data_res.size(),
                      &response,
                      &errors);

        if (!parsingSuccessful) {
            LOG(ldebug, errors);
        }

        LOG(ldebug, response[utils::addressFields::RESULT].asString());

        for( Json::Value::const_iterator outer = response.begin() ; outer != response.end() ; outer++ ) {
            for( Json::Value::const_iterator inner = (*outer).begin() ; inner!= (*outer).end() ; inner++ ) {
                std::string s(inner.key().asString());
                s.append(" : ").append((*inner).asString());
                LOG(ldebug, s);
            }
        }

        LOG(ltrace, "2. Response: \n" + req_data_res);
        if (response[utils::addressFields::RESULT].asString() != utils::jsonCode::OK) {

            // TODO implement on roulette case of index requested is last + 1 to differentiate errors and chain end
            if (response[utils::addressFields::ERR_CODE].asInt() == -1) {
                LOG(ltrace, "get endpoint chain");
                return get_chain_endpoint(header, pkt);
            }
            //LOG(lwarn, "Error while retrieving the next index");
            //return Address("", 0);
        }
        try {
            return Address(response[utils::addressFields::CONTENT]
                                   [utils::addressFields::ADDRESS].asString(),
                           std::stol(response[utils::addressFields::CONTENT]
                                             [utils::addressFields::PORT].asString()));
        } catch (const std::invalid_argument& e) {
            LOG(lwarn, "The received address from roulette is not valid");
            e.what();
            return Address("", 0);
        }
    }
}

const std::string AddressResolver::url_builder(const std::string& original_url, uint32_t p_id, uint32_t si) const {
    std::string req_addr = original_url;

    req_addr.append("routes/")
            .append(std::to_string(p_id))
            .append(Address::path_separator)
            .append(std::to_string(si));

    return req_addr;
}

const Address AddressResolver::get_chain_endpoint(
        utils::sfc_header::SFCFixedLengthHeader header, unsigned char* pkt) const {
    LOG(ldebug, "1");
    CURLcode req_code_res;
    LOG(ldebug, "2");
    std::string req_data_res;
    LOG(ldebug, "3");
    std::string endpoint = header.get_direction_flag()? "ingress/" : "egress/";
    LOG(ldebug, "4");

    struct iphdr h_ip;
    memcpy(&h_ip, pkt, sizeof(iphdr));

    std::string protocol;
    h_ip.protocol == 6 ? protocol = "tcp" : protocol = "udp";

    std::string req_addr;
    req_addr.append(roulette_addr.get_URL())
            .append("endpoints/")
            .append(endpoint)
            .append(header.get_source_ip_address())
            .append(Address::path_separator)
            .append(header.get_destination_ip_address())
            .append(Address::path_separator)
            .append(std::to_string(htons(header.get_source_port())))
            .append(Address::path_separator)
            .append(std::to_string(htons(header.get_destination_port())))
            .append(Address::path_separator)
            .append(std::to_string(header.get_service_path_id()))
            .append(Address::path_separator)
            .append(protocol);

    LOG(ldebug, "**************");
    LOG(ldebug, req_addr);
    LOG(ldebug, "**************");

    setup_curl_for_request(req_addr, req_data_res);

    req_code_res = curl_easy_perform(curl);
    LOG(ltrace, req_data_res);
    if (req_code_res != CURLE_OK) {
        LOG(lwarn, "Error: failure to retrieve data from roulette");
        return Address("", 0);
    } else {
        LOG(ldebug, "Request performed successfully " + req_data_res);

        try {
            Json::CharReaderBuilder builder;
            std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
            Json::Value response;

            std::string errors;
            bool parsingSuccessful = reader->parse(req_data_res.c_str(),
                                                   req_data_res.c_str() +
                                                        req_data_res.size(),
                                                   &response,
                                                   &errors);

            if (!parsingSuccessful) {
                LOG(ldebug, errors);
            }

            std::string endpoint = header.get_direction_flag() ?
                                       utils::addressFields::INGRESS :
                                       utils::addressFields::EGRESS;

            return Address(response[utils::addressFields::CONTENT]
                                   [endpoint].asString());
        } catch (const std::invalid_argument& e) {
            LOG(lwarn, "The received address from roulette is not valid");
            e.what();
            return Address("", 0);
        }
    }
}

void AddressResolver::setup_curl_for_request(std::string req_addr,
                                             std::string& req_data_res) const {
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
