#include "YahooAPI.h"

size_t _writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

string YahooAPI::performRequest(string url)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    auto curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

        std::string response_string;

        struct curl_slist* header = NULL;
        header = curl_slist_append(header, string("X-API-KEY:" + _apiKey).c_str());
        header = curl_slist_append(header, "Content-Type:application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        curl = NULL;
        
        return response_string;
    }
    return "CURL_NOT_INTILIASED";
}

YahooAPIMonthData YahooAPI::getMonthData(string requestSymbol, string symbol)
{    
    return YahooAPIMonthData(symbol, performRequest("https://yfapi.net/v8/finance/spark?interval=1h&range=1mo&symbols=" + requestSymbol));
}
